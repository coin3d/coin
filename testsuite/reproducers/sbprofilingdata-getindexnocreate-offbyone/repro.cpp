// Regression for SbProfilingData::getIndexNoCreate() stopping before the
// requested tail. After registering sibling paths, a lookup of the first
// path must return its entry and metrics, not those of its parent.
//
// Explicit values are assigned through index-based setters to test each
// path-based getter. CoinTests additionally covers missing paths, repeated
// child indices and nodes shared by different parents; see README.md.
//
// Use real SoTempPath instances to avoid the separate plain-SoPath downcast
// problem addressed by PR #714. This test checks lookup results, not whether
// the complete profiler is free of undefined behavior.

#include <cstdio>
#include <Inventor/SoDB.h>
#include <Inventor/misc/SoTempPath.h>
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

  // Real SoFullPath subclasses isolate this lookup regression from the
  // separate SoPath downcast issue addressed by PR #714.
  SoTempPath * cubepath = new SoTempPath(3);
  cubepath->setHead(root);
  cubepath->append(branch);
  cubepath->append(cube);
  cubepath->ref();

  SoTempPath * spherepath = new SoTempPath(3);
  spherepath->setHead(root);
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
  SoDB::finish();

  if (failures > 0) {
    fprintf(stderr, "[repro] FAIL: %d check(s) failed\n", failures);
    return 1;
  }
  fprintf(stderr, "[repro] PASS\n");
  return 0;
}
