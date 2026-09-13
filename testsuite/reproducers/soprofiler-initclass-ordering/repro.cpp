// Public-API startup regression. The caller sets COIN_PROFILER before this
// fresh process starts; argv[1] gives the independently expected state.
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <Inventor/SoDB.h>
#include <Inventor/annex/Profiler/SoProfiler.h>
#include <Inventor/annex/Profiler/elements/SoProfilerElement.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/SbViewportRegion.h>

struct Observation {
  int callbacks;
  SbBool elementenabled;
  int entries;
};

static void
inspect(void * userdata, SoAction * action)
{
  Observation * observation = static_cast<Observation *>(userdata);
  ++observation->callbacks;
  SoState * state = action->getState();
  observation->elementenabled =
    state->isElementEnabled(SoProfilerElement::getClassStackIndex());
  if (observation->elementenabled) {
    observation->entries = SoProfilerElement::get(state)->getProfilingData().getNumNodeEntries();
  }
}

int
main(int argc, char ** argv)
{
  if (argc != 2 || (std::strcmp(argv[1], "0") && std::strcmp(argv[1], "1"))) {
    std::fprintf(stderr, "usage: %s expected-enabled(0|1)\n", argv[0]);
    return 2;
  }
  const SbBool expected = std::strcmp(argv[1], "1") == 0 ? TRUE : FALSE;
  SoDB::init();
  SoDB::init(); // Repeated initialization must remain a no-op.
  const SbBool enabled = SoProfiler::isEnabled();
  Observation observation = { 0, FALSE, 0 };
  bool boundsok = false;
  {
    SoSeparator * root = new SoSeparator;
    root->ref();
    root->addChild(new SoCube);
    root->addChild(new SoCube);
    SoCallback * inspector = new SoCallback;
    inspector->setCallback(inspect, &observation);
    root->addChild(inspector);
    SoGetBoundingBoxAction action(SbViewportRegion(64, 64));
    action.apply(root);
    boundsok = action.getBoundingBox().getMin() == SbVec3f(-1, -1, -1) &&
      action.getBoundingBox().getMax() == SbVec3f(1, 1, 1);
    root->unref();
  }
  const bool valid = enabled == expected && observation.callbacks == 1 &&
    observation.elementenabled == expected &&
    observation.entries == (expected ? 4 : 0) && boundsok;
  std::fprintf(stderr, "enabled=%d element=%d callbacks=%d entries=%d bounds=%d: %s\n",
               enabled, observation.elementenabled, observation.callbacks,
               observation.entries, boundsok, valid ? "PASS" : "FAIL");
  SoDB::finish();
  return valid ? 0 : 1;
}
