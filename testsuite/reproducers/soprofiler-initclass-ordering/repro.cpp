// Reproducer/regression test for a profiler-subsystem initialization
// ordering bug in SoDB::init() (src/misc/SoDB.cpp).
//
// SoAction::initClass() decides once, based on SoProfiler::isEnabled(),
// whether to enable SoProfilerElement on the static
// SoAction::enabledElements list -- a decision baked into every
// SoState built for the rest of the process' lifetime (SoState's
// stack array is only as large/as populated as that list says).
// SoDB::init() used to call SoAction::initClass() long before it ever
// parsed the COIN_PROFILER environment variable (that only happened
// via SoProfilerP::parseCoinProfilerVariable(), near the very end of
// SoDB::init(), followed by a conditional SoProfiler::init() call) --
// so setting COIN_PROFILER before SoDB::init() had no effect on
// whether SoProfilerElement ever actually got enabled on any state.
//
// Confirmed empirically (before the fix) with a throwaway build
// instrumented to print SoNodeProfiling::isActive()'s return value:
// SoProfiler::isEnabled() reported TRUE after SoDB::init() (the env
// var *was* parsed, eventually), yet isActive() returned false for
// every single node visited by a real action traversal, regardless.
//
// Fixed by moving just the (self-contained, dependency-free)
// SoProfilerP::parseCoinProfilerVariable() call to right after
// SoProfilerElement::initClass() and before SoAction::initClass(),
// leaving the original, later call (which still guards the heavier
// SoProfiler::init() -- SoNodeKit::init() etc., which itself has a
// hard dependency on SoDB being far enough along not to recurse back
// into SoDB::init()) exactly where it was.
//
// This drives a real SoGetBoundingBoxAction traversal (chosen because
// it needs no GL context) over a small scene with an SoCallback node,
// whose callback function -- invoked mid-traversal, with a live
// SoState -- checks SoProfilerElement::get(state)->getProfilingData().
// getNumNodeEntries() to directly observe whether profiling data
// actually got recorded for the nodes visited before the callback
// (root, camera, and the callback node's own preceding siblings),
// rather than relying on any indirect proxy.

#include <cstdlib>
#include <cstdio>
#include <Inventor/SoDB.h>
#include <Inventor/annex/Profiler/SoProfiler.h>
#include <Inventor/annex/Profiler/elements/SoProfilerElement.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/SbViewportRegion.h>

static int numentriesseen = -1;

static void
inspectProfilingDataCB(void *, SoAction * action)
{
  SoState * state = action->getState();
  if (!state->isElementEnabled(SoProfilerElement::getClassStackIndex())) {
    numentriesseen = -1; // profiler element not even enabled on this state
    return;
  }
  SoProfilerElement * elt = SoProfilerElement::get(state);
  numentriesseen = elt->getProfilingData().getNumNodeEntries();
}

int
main()
{
  setenv("COIN_PROFILER", "on", 1);
  SoDB::init();
  fprintf(stderr, "[repro] SoProfiler::isEnabled() after SoDB::init() = %d\n",
          (int)SoProfiler::isEnabled());

  SoSeparator * root = new SoSeparator;
  root->ref();
  root->addChild(new SoCube);
  root->addChild(new SoCube);

  SoCallback * inspector = new SoCallback;
  inspector->setCallback(inspectProfilingDataCB);
  root->addChild(inspector);

  SbViewportRegion vp(64, 64);
  SoGetBoundingBoxAction action(vp);
  action.apply(root);

  fprintf(stderr, "[repro] node entries recorded before the callback ran = %d\n",
          numentriesseen);

  root->unref();

  if (numentriesseen <= 0) {
    fprintf(stderr, "[repro] FAIL: expected profiling data for at least the "
                    "already-traversed nodes (root, 2 cubes)\n");
    return 1;
  }
  fprintf(stderr, "[repro] PASS\n");
  return 0;
}
