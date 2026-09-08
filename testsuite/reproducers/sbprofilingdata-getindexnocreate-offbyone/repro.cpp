// Reproducer/regression test for an off-by-one in
// SbProfilingData::getIndexNoCreate() (src/profiler/SbProfilingData.cpp).
//
// getIndexNoCreate()'s forward-walk loop condition was
// `pos < path->getFullLength()`, one comparison short of its sibling
// getIndexCreate()'s `pos <= path->getFullLength()`. Both functions
// build a "lastentrypathindexes" chain from the most-recently-added
// profiling entry's own ancestry, find how many leading path elements
// match that chain (samelength), and then walk *forward* from there to
// resolve/create the remaining entries down to the queried path's
// tail. getIndexCreate()'s "<=" walks all the way to and including the
// tail (pos going from samelength+1 up to and including getFullLength());
// getIndexNoCreate()'s "<" stopped one short, at the tail's *parent*,
// so the function returned the parent's index instead of the actual
// queried node's whenever the query path diverges from the
// last-entry's ancestry exactly at the tail (i.e. shares every node up
// to but not including the last one).
//
// This affects every path-based *getter* that goes through
// getIndexNoCreate(): getIndex() with create=FALSE, getNodeTiming(),
// getNodeFootprint(), getNodeFlag(). The corresponding *setters*
// (which use getIndexCreate(), already correct) were never affected.
//
// Found while verifying the Phase 8 SoPath/SoFullPath downcast UB fix
// on the sibling branch fix/sopath-fullpath-downcast-ub: reproduced
// byte-for-byte against the unmigrated code too, confirming it's a
// separate, pre-existing bug unrelated to that UB category.
//
// This test creates two sibling paths (cube, sphere) sharing a
// root->branch prefix and diverging at the tail -- exactly the
// scenario above -- then queries *cube*'s index again via the
// path-based getter (create=FALSE) right after creating *sphere*'s
// entry (so the "last entry" chain is sphere's, and cube's query
// diverges from it exactly at the tail). Before the fix this returned
// branch's index (cube's parent) instead of cube's own; after the fix
// it returns cube's own index, matching what was returned at creation
// time.

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

  SoSeparator * root = new SoSeparator;
  root->ref();
  SoSeparator * branch = new SoSeparator;
  root->addChild(branch);
  SoCube * cube = new SoCube;
  branch->addChild(cube);
  SoSphere * sphere = new SoSphere;
  branch->addChild(sphere);

  SoPath * cubepath = new SoPath(root);
  cubepath->append(branch);
  cubepath->append(cube);
  cubepath->ref();

  SoPath * spherepath = new SoPath(root);
  spherepath->append(branch);
  spherepath->append(sphere);
  spherepath->ref();

  int failures = 0;

  SbProfilingData data;

  const int cubeidx = data.getIndex(cubepath, TRUE);
  const int sphereidx = data.getIndex(spherepath, TRUE);
  fprintf(stderr, "[repro] cubeidx=%d sphereidx=%d\n", cubeidx, sphereidx);
  if (cubeidx < 0 || sphereidx < 0 || cubeidx == sphereidx) {
    fprintf(stderr, "[repro] FAIL: expected two distinct, valid indices\n");
    failures++;
  }

  // At this point the "last entry" is sphere's -- querying cube's
  // path again (create=FALSE, so via getIndexNoCreate()) diverges
  // from sphere's ancestry exactly at the tail (both share
  // root->branch, differ only in the final node). This is exactly the
  // pattern the off-by-one mishandled.
  const int cubeidx2 = data.getIndex(cubepath, FALSE);
  fprintf(stderr, "[repro] cubeidx2 (re-lookup after sphere) = %d\n", cubeidx2);
  if (cubeidx2 != cubeidx) {
    fprintf(stderr, "[repro] FAIL: expected getIndex(cubepath, FALSE) == %d, got %d "
                    "(likely returned the parent/branch index instead)\n",
            cubeidx, cubeidx2);
    failures++;
  }

  // Same check via the other path-based getters, which all go through
  // getIndexNoCreate() the same way.
  data.setNodeTiming(cubeidx, SbTime(1.5));
  const SbTime timing = data.getNodeTiming(cubepath);
  fprintf(stderr, "[repro] cube timing (looked up by path) = %g\n", timing.getValue());
  if (timing.getValue() != 1.5) {
    fprintf(stderr, "[repro] FAIL: getNodeTiming(cubepath) didn't resolve to cube's own entry\n");
    failures++;
  }

  data.setNodeFootprint(cubeidx, SbProfilingData::MEMORY_SIZE, 4096);
  const size_t footprint = data.getNodeFootprint(cubepath, SbProfilingData::MEMORY_SIZE);
  fprintf(stderr, "[repro] cube footprint (looked up by path) = %zu\n", footprint);
  if (footprint != 4096) {
    fprintf(stderr, "[repro] FAIL: getNodeFootprint(cubepath) didn't resolve to cube's own entry\n");
    failures++;
  }

  data.setNodeFlag(cubeidx, SbProfilingData::GL_CACHED_FLAG, TRUE);
  const SbBool flag = data.getNodeFlag(cubepath, SbProfilingData::GL_CACHED_FLAG);
  fprintf(stderr, "[repro] cube GL_CACHED_FLAG (looked up by path) = %d\n", (int)flag);
  if (!flag) {
    fprintf(stderr, "[repro] FAIL: getNodeFlag(cubepath) didn't resolve to cube's own entry\n");
    failures++;
  }

  cubepath->unref();
  spherepath->unref();
  root->unref();

  if (failures > 0) {
    fprintf(stderr, "[repro] FAIL: %d check(s) failed\n", failures);
    return 1;
  }
  fprintf(stderr, "[repro] PASS\n");
  return 0;
}
