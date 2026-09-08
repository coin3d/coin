// Regression test for the "stray" files found outside the original
// seven-phase inventory of the SoPath/SoFullPath downcast UB fix (see
// the Phase 1 foundation commit on this branch for the new
// SoPath::getFullTail()/getFullNodeFromTail()/getFullIndexFromTail()/
// getFullLength() methods used throughout): src/lists/SoPathList.cpp,
// src/nodes/SoPathSwitch.cpp, src/nodes/SoSurroundScale.cpp,
// src/rendering/SoOffscreenRenderer.cpp.
//
// SoSurroundScale isn't separately exercised here: it's a real,
// non-trivial part of SoTransformerDragger's own catalog
// ("surroundScale"), so every dragger pick/render/bbox action already
// run by the earlier phases' reproducers on this branch (all clean
// under ASan/UBSan) already exercises its migrated
// updateMySurroundParams() as a side effect.
//
// Three checks:
//
// 1. SoPathList::sort()/uniquify(): search a scene with nested
//    separators for cubes, confirming the found paths get sorted
//    without crashing and that duplicate/redundant paths are removed
//    as expected (only one path should remain per distinct cube after
//    uniquify(), and none should be a strict prefix of another).
//
// 2. SoPathSwitch: build one with its `path` field set to the actual
//    path leading to itself (constructed via a real SoSearchAction) --
//    confirms its children are traversed (non-empty bounding box) --
//    versus an unrelated path, confirming they are not (empty
//    bounding box). Exercises is_matching_paths()'s migrated
//    getFullLength() comparisons directly.
//
// 3. SoOffscreenRenderer: a real off-screen render of a small scene.
//    SoOffscreenRendererP::GLRenderAbortCallback() -- which uses the
//    migrated getFullTail() -- is unconditionally registered as the
//    internal SoGLRenderAction's abort callback for the whole
//    duration of every render() call, so this alone exercises it on
//    every node visited during rendering.
//
//    SoOffscreenRenderer's default GLX backend first probes for
//    *indirect* (software) context support (glxglue_context_create_
//    software() in src/glue/gl_glx.cpp), which some environments
//    (including, at the time of writing, the one this was first
//    verified in) refuse entirely -- e.g. since RHEL8, many
//    distributions disable indirect GLX by default following
//    https://www.x.org/wiki/Development/Security/Advisory-2014-12-09/.
//    Confirmed independently with a minimal direct-vs-indirect
//    glXCreateContext() probe: direct succeeds, indirect returns NULL.
//    Coin already has an escape hatch for exactly this, documented
//    right there in glxglue_context_create_software()'s own comment:
//    the COIN_GLX_PIXMAP_DIRECT_RENDERING environment variable forces
//    a direct-rendering context for the offscreen GLX pixmap instead.
//    Set it here, before SoDB::init(), so it's in effect before the
//    glue layer's first (lazily cached) context-creation attempt.

#include <cstdlib>
#include <cstdio>
#include <Inventor/SoDB.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/SoPath.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/lists/SoPathList.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoPathSwitch.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/SbViewportRegion.h>

static SbBool
test_pathlist_sort_uniquify(void)
{
  SoSeparator * root = new SoSeparator;
  root->ref();

  SoSeparator * branch1 = new SoSeparator;
  root->addChild(branch1);
  branch1->addChild(new SoCube);

  SoSeparator * branch2 = new SoSeparator;
  root->addChild(branch2);
  branch2->addChild(new SoCube);
  branch2->addChild(new SoCube);

  SoSearchAction sa;
  sa.setType(SoCube::getClassTypeId());
  sa.setInterest(SoSearchAction::ALL);
  sa.setSearchingAll(TRUE);
  sa.apply(root);

  SoPathList pl = sa.getPaths(); // take an owning copy to sort/uniquify freely
  int lenbefore = pl.getLength();
  pl.sort();
  pl.uniquify();
  int lenafter = pl.getLength();

  fprintf(stderr, "[repro] SoPathList: %d cube paths found, %d after uniquify()\n",
          lenbefore, lenafter);

  root->unref();
  // 3 distinct SoCube nodes were added -- none of these paths is a
  // prefix of another (they're all leaf shapes), so uniquify()
  // shouldn't remove anything, and sort()/uniquify() together must not
  // crash or corrupt the list.
  return lenbefore == 3 && lenafter == 3;
}

static SbBool
test_pathswitch(void)
{
  // Only the pathswitch itself in the scene -- no other geometry --
  // so the bounding box result depends *only* on whether its own
  // children got traversed or not, unambiguously.
  SoSeparator * root = new SoSeparator;
  root->ref();

  SoPathSwitch * psw = new SoPathSwitch;
  root->addChild(psw);
  psw->addChild(new SoCube);

  // An entirely separate scene graph, never attached to root, to
  // build a genuinely unrelated (mismatching) path from.
  SoSeparator * otherroot = new SoSeparator;
  otherroot->ref();
  SoCube * othercube = new SoCube;
  otherroot->addChild(othercube);

  SbViewportRegion vp(100, 100);

  // Case 1: path field pointing into a disjoint scene graph --
  // children should not be traversed, bbox should be empty.
  SoSearchAction sa1;
  sa1.setNode(othercube);
  sa1.apply(otherroot);
  SoPath * unrelatedpath = sa1.getPath();
  psw->path.setValue(unrelatedpath);

  SoGetBoundingBoxAction bbox1(vp);
  bbox1.apply(root);
  SbBool emptywhenmismatched = bbox1.getBoundingBox().isEmpty();
  fprintf(stderr, "[repro] SoPathSwitch with mismatched path: bbox empty=%d\n",
          (int)emptywhenmismatched);

  // Case 2: path field matching the *parent* path leading to the
  // pathswitch (is_matching_paths() compares the stored path's tail
  // against the current traversal path's second-to-last node -- i.e.
  // the pathswitch's own parent, not the pathswitch itself; see the
  // "last node is this node, skip it" comment in is_matching_paths())
  // -- children should be traversed, bbox should include the cube.
  SoPath * selfpath = new SoPath(root);
  psw->path.setValue(selfpath);

  SoGetBoundingBoxAction bbox2(vp);
  bbox2.apply(root);
  SbBool nonemptywhenmatched = !bbox2.getBoundingBox().isEmpty();
  fprintf(stderr, "[repro] SoPathSwitch with matching path: bbox empty=%d\n",
          (int)bbox2.getBoundingBox().isEmpty());

  otherroot->unref();
  root->unref();
  return emptywhenmismatched && nonemptywhenmatched;
}

static SbBool
test_offscreen_render(void)
{
  SbViewportRegion vp(64, 64);
  SoOffscreenRenderer renderer(vp);

  SoSeparator * root = new SoSeparator;
  root->ref();
  SoPerspectiveCamera * camera = new SoPerspectiveCamera;
  root->addChild(camera);
  root->addChild(new SoDirectionalLight);
  SoCube * cube = new SoCube;
  root->addChild(cube);
  camera->viewAll(root, vp);

  SbBool ok = renderer.render(root);
  fprintf(stderr, "[repro] SoOffscreenRenderer::render() = %d\n", (int)ok);

  root->unref();
  return ok;
}

int
main()
{
  // See the comment on test_offscreen_render() above: forces a direct
  // (rather than indirect/software) GLX context for the offscreen
  // pixmap, since some environments refuse indirect contexts outright.
  // Must be set before SoDB::init() / any GL context creation.
  setenv("COIN_GLX_PIXMAP_DIRECT_RENDERING", "1", 0); // don't override if the caller already set it

  SoDB::init();

  int failures = 0;
  if (!test_pathlist_sort_uniquify()) { fprintf(stderr, "[repro] FAIL: pathlist sort/uniquify\n"); failures++; }
  if (!test_pathswitch()) { fprintf(stderr, "[repro] FAIL: pathswitch\n"); failures++; }
  if (!test_offscreen_render()) { fprintf(stderr, "[repro] FAIL: offscreen render\n"); failures++; }

  if (failures > 0) {
    fprintf(stderr, "[repro] FAIL: %d check(s) failed\n", failures);
    return 1;
  }
  fprintf(stderr, "[repro] PASS\n");
  return 0;
}
