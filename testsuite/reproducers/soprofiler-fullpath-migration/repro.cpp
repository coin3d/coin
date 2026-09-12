// Regression test for Phase 8 of the SoPath/SoFullPath downcast UB fix
// (src/profiler/{SbProfilingData.cpp,SoNodeProfiling.h,
// SoNodeVisualize.cpp,tools/inline_texture.cpp}): all four files
// reinterpret_cast<>()'d/C-style-cast a plain SoPath* (or
// action->getCurPath(), itself already typed const SoPath*) to
// SoFullPath* to call SoFullPath::getLength()/getTail() -- undefined
// behavior via a downcast to a type the object was never actually
// constructed as, caught by -fsanitize=vptr.
//
// SbProfilingData.cpp did the most work here: five private helper
// methods (isPathMatch, getIndexCreate, getIndexNoCreate,
// getIndexForwardCreate, getIndexForwardNoCreate) took a
// const SoFullPath* parameter (or re-cast to one internally) purely
// to call getLength() -- migrated to take const SoPath* and call the
// new (Phase 1) SoPath::getFullLength() instead; their getNode()/
// getIndex() calls needed no change at all, since SoFullPath doesn't
// actually override those (confirmed by reading SoFullPath.h: it only
// shadows getTail()/getNodeFromTail()/getIndexFromTail()/getLength()).
//
// SoNodeProfiling::preTraversal()'s isActive() gate turns out to be
// permanently FALSE in this build via any documented public API
// (COIN_PROFILER env var / SoProfiler::enable()): SoAction::initClass()
// decides once, at SoDB::init() time, whether to enable
// SoProfilerElement on SoAction::enabledElements, based on
// SoProfiler::isEnabled() -- but SoDB::init() only parses
// COIN_PROFILER and calls SoProfiler::init() *after* it has already
// called SoAction::initClass(). Confirmed empirically with a
// throwaway instrumented build: SoProfiler::isEnabled() reports TRUE
// after SoDB::init() (with COIN_PROFILER=on set beforehand), yet
// SoNodeProfiling::isActive() unconditionally returns false for every
// node traversed regardless. This is a separate, pre-existing bug in
// the profiler subsystem's own init ordering, unrelated to the
// SoPath/SoFullPath UB category this branch targets, and out of scope
// here -- so preTraversal()/postTraversal()'s own body (which is what
// got migrated) can't be exercised through SoDB::init() + a real
// action the way the rest of this branch's reproducers work. Its
// actual change (action->getCurPath() -- already const SoPath* --
// plus SoPath::getFullTail(), both already proven correct by every
// other reproducer on this branch) carries negligible residual risk,
// so it's left to compilation + code review rather than a dedicated
// runtime check here.
//
// SoNodeVisualize::handleEvent() and tools/inline_texture.cpp both
// only needed the *type* fixed (pp->getPath() and SoPathList's
// operator[] already return plain SoPath*, so their casts to
// SoFullPath* were doing nothing but introducing UB) -- neither one's
// remaining path usage (containsNode(), getFullTail()) has any
// "Full-vs-plain" subtlety left to test.  inline_texture.cpp isn't
// part of the CMake build at all (a standalone helper script, see its
// own top comment) -- checked to still compile standalone against the
// built library.
//
// Side finding, confirmed pre-existing (reproduced byte-for-byte
// against the unmigrated code on origin/master too) and left
// untouched here as out of scope: getIndexNoCreate()'s forward-walk
// loop condition is `pos < path->getFullLength()`, one comparison
// short of its sibling getIndexCreate()'s `pos <= path->getFullLength()`
// -- so when a path-based getter (getNodeTiming()/getNodeFootprint()/
// getNodeFlag()/getIndex() with create=FALSE) is queried for a path
// that diverges from the most-recently-created/queried one exactly at
// the tail (i.e. shares every node up to but not including the last),
// it silently returns the *parent*'s index instead of the queried
// node's. Not a SoPath/SoFullPath UB-category bug and not exercised
// by SoNodeProfiling's actual (one-shot, cache-the-index) call
// pattern, so this reproducer's own usage below deliberately avoids
// it (see the comment ahead of the cube-processing block).
//
// This reproducer exercises SbProfilingData's public API directly and
// exhaustively with two real, sibling SoPath instances sharing a
// common prefix (to drive the "matching path prefix" bookkeeping in
// getIndexCreate()/getIndexForwardCreate()), confirming create/lookup/
// timing/footprint/flag round-trip correctly, and that looking up an
// unregistered path behaves as documented (returns -1 / zero).

#include "../CoinCleanup.h"
#include <cstdio>
#include <Inventor/SoDB.h>
#include <Inventor/SoPath.h>
#include <Inventor/annex/Profiler/SbProfilingData.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoSphere.h>

int
main()
{
  SoDB::init();
  CoinReproducerCleanup cleanup;

  SoSeparator * root = new SoSeparator;
  root->ref();
  SoSeparator * branch = new SoSeparator;
  root->addChild(branch);
  SoCube * cube = new SoCube;
  branch->addChild(cube);
  SoSphere * sphere = new SoSphere;
  branch->addChild(sphere);

  // Two paths sharing the root->branch prefix, diverging at the tail
  // (cube vs. sphere) -- exercises the partial-prefix-match logic in
  // getIndexCreate()/getIndexNoCreate() (samelength ends up 2, not 0
  // or full length) and getIndexForwardCreate()'s new-entry path.
  SoPath * cubepath = new SoPath(root);
  cubepath->append(branch);
  cubepath->append(cube);
  cubepath->ref();

  SoPath * spherepath = new SoPath(root);
  spherepath->append(branch);
  spherepath->append(sphere);
  spherepath->ref();

  // A path to an unrelated, never-registered node.
  SoSeparator * otherroot = new SoSeparator;
  otherroot->ref();
  SoCube * othercube = new SoCube;
  otherroot->addChild(othercube);
  SoPath * unregisteredpath = new SoPath(otherroot);
  unregisteredpath->append(othercube);
  unregisteredpath->ref();

  int failures = 0;

  SbProfilingData data;

  // Process cube's entry fully -- create, then immediately exercise
  // every path-based setter/getter for it before any other path
  // touches `data` -- mirroring the one-shot query pattern
  // SoNodeProfiling::preTraversal()/postTraversal() actually use (look
  // up a path once per node, cache the returned index, never query by
  // path again for that node). This keeps SbProfilingDataP::
  // lastPathIndex pointed at cube's own entry throughout, so every
  // path-based call below resolves via isPathMatch()'s fast path or
  // getIndexCreate()'s exhaustive search -- both migrated in this
  // phase and confirmed correct here -- rather than
  // getIndexNoCreate()'s forward-walk, which has a separate,
  // pre-existing loop-bound asymmetry with getIndexCreate() (confirmed
  // present, byte-for-byte, against the unmigrated code too) that only
  // shows up when a path-based getter is queried for a path other than
  // the one most recently created/queried -- a call pattern
  // SoNodeProfiling itself never makes, and unrelated to the
  // SoPath/SoFullPath UB this branch targets.
  const int cubeidx = data.getIndex(cubepath, TRUE);
  fprintf(stderr, "[repro] cubeidx=%d\n", cubeidx);
  if (cubeidx < 0) {
    fprintf(stderr, "[repro] FAIL: expected a valid cube index\n");
    failures++;
  }
  if (data.getIndex(cubepath, FALSE) != cubeidx) {
    fprintf(stderr, "[repro] FAIL: cubepath immediate re-lookup mismatch\n");
    failures++;
  }
  data.setNodeTiming(cubepath, SbTime(1.5));
  if (data.getNodeTiming(cubeidx).getValue() != 1.5) {
    fprintf(stderr, "[repro] FAIL: cube node timing round-trip mismatch\n");
    failures++;
  }
  data.setNodeFootprint(cubepath, SbProfilingData::MEMORY_SIZE, 4096);
  if (data.getNodeFootprint(cubeidx, SbProfilingData::MEMORY_SIZE) != 4096) {
    fprintf(stderr, "[repro] FAIL: cube node footprint round-trip mismatch\n");
    failures++;
  }
  data.setNodeFlag(cubepath, SbProfilingData::GL_CACHED_FLAG, TRUE);
  if (!data.getNodeFlag(cubeidx, SbProfilingData::GL_CACHED_FLAG)) {
    fprintf(stderr, "[repro] FAIL: cube node flag round-trip mismatch\n");
    failures++;
  }

  // Now create sphere's entry -- a sibling sharing the root->branch
  // prefix with cube, diverging at the tail -- exercising the
  // partial-prefix-match logic in getIndexCreate() (samelength ends
  // up 2, neither 0 nor the full path length) and
  // getIndexForwardCreate()'s new-entry path. Same one-shot pattern:
  // fully process sphere before touching any other path.
  const int sphereidx = data.getIndex(spherepath, TRUE);
  fprintf(stderr, "[repro] sphereidx=%d\n", sphereidx);
  if (sphereidx < 0 || sphereidx == cubeidx) {
    fprintf(stderr, "[repro] FAIL: expected a valid, distinct sphere index\n");
    failures++;
  }
  if (data.getIndex(spherepath, FALSE) != sphereidx) {
    fprintf(stderr, "[repro] FAIL: spherepath immediate re-lookup mismatch\n");
    failures++;
  }
  data.setNodeTiming(spherepath, SbTime(2.5));
  if (data.getNodeTiming(sphereidx).getValue() != 2.5) {
    fprintf(stderr, "[repro] FAIL: sphere node timing round-trip mismatch\n");
    failures++;
  }

  // An unregistered path (sharing no prefix at all with anything
  // registered so far) must not be found, and getters on it must
  // return the documented "not found" values rather than crash --
  // this is independent of call ordering, since it short-circuits
  // (samelength == 0) before either getIndexCreate() or
  // getIndexNoCreate()'s forward-walk logic ever runs.
  const int missingidx = data.getIndex(unregisteredpath);
  fprintf(stderr, "[repro] missingidx=%d\n", missingidx);
  if (missingidx != -1) {
    fprintf(stderr, "[repro] FAIL: expected -1 for an unregistered path\n");
    failures++;
  }
  if (data.getNodeTiming(unregisteredpath).getValue() != 0.0) {
    fprintf(stderr, "[repro] FAIL: expected zero timing for unregistered path\n");
    failures++;
  }
  if (data.getNodeFootprint(unregisteredpath, SbProfilingData::MEMORY_SIZE) != 0) {
    fprintf(stderr, "[repro] FAIL: expected zero footprint for unregistered path\n");
    failures++;
  }
  if (data.getNodeFlag(unregisteredpath, SbProfilingData::GL_CACHED_FLAG)) {
    fprintf(stderr, "[repro] FAIL: expected FALSE flag for unregistered path\n");
    failures++;
  }

  cubepath->unref();
  spherepath->unref();
  unregisteredpath->unref();
  otherroot->unref();
  root->unref();

  if (failures > 0) {
    fprintf(stderr, "[repro] FAIL: %d check(s) failed\n", failures);
    return 1;
  }
  fprintf(stderr, "[repro] PASS\n");
  return 0;
}
