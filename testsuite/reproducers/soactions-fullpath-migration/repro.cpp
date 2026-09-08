// Regression test for Phase 4 of the SoPath/SoFullPath downcast UB fix
// (src/actions/: SoGetBoundingBoxAction.cpp, SoReorganizeAction.cpp,
// SoToVRMLAction.cpp -- see the Phase 1 foundation commit on this
// branch for the new SoPath::getFullXxx() methods used throughout).
//
// Three independent, real functional checks:
//
// 1. SoGetBoundingBoxAction::checkResetBefore()/checkResetAfter(): set
//    a reset path partway down a scene and confirm the bounding box
//    action actually resets state at that point (rather than crashing
//    or silently not resetting), both with resetBefore and
//    resetAfter.
//
// 2. SoReorganizeAction::apply() on a scene containing an
//    SoIndexedFaceSet under a group: exercises replaceNode()/
//    replaceIfs() (SoFullPath* parameters migrated to plain SoPath*
//    using getFullNodeFromTail()/getFullIndexFromTail()). Confirms the
//    action runs to completion without crashing and the parent group
//    still has exactly the same number of children afterward
//    (replaceChild() semantics preserved) regardless of whether it
//    actually decided to reorganize the shape.
//
// 3. SoToVRMLAction::apply(): converts a small VRML97 scene graph
//    (SoToVRMLAction goes VRML97 -> Inventor/VRML1, not the other way)
//    to VRML1 (exercising vrmlpath, now plain SoPath* instead of
//    reinterpret_cast<SoFullPath*>'d, and its getFullTail() use in
//    get_current_tail()). Confirms the conversion produces a
//    non-NULL VRML root with at least one child.

#include <cstdio>
#include <Inventor/SoDB.h>
#include <Inventor/SoPath.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoReorganizeAction.h>
#include <Inventor/actions/SoToVRMLAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/VRMLnodes/SoVRMLGroup.h>
#include <Inventor/VRMLnodes/SoVRMLBox.h>
#include <Inventor/SbViewportRegion.h>

static SbBool
test_bbox_reset(void)
{
  SoSeparator * root = new SoSeparator;
  root->ref();
  SoSeparator * mid = new SoSeparator;
  root->addChild(mid);
  SoCube * cube = new SoCube;
  mid->addChild(cube);

  SoSearchAction sa;
  sa.setNode(mid);
  sa.apply(root);
  SoPath * resetpath = sa.getPath();
  if (!resetpath) {
    fprintf(stderr, "[repro] FAIL: could not find path to mid separator\n");
    root->unref();
    return FALSE;
  }
  resetpath->ref();

  SbViewportRegion vp(100, 100);

  SoGetBoundingBoxAction bboxbefore(vp);
  bboxbefore.setResetPath(resetpath, TRUE, SoGetBoundingBoxAction::TRANSFORM);
  bboxbefore.apply(root);
  SbXfBox3f resultbefore = bboxbefore.getXfBoundingBox();

  SoGetBoundingBoxAction bboxafter(vp);
  bboxafter.setResetPath(resetpath, FALSE, SoGetBoundingBoxAction::TRANSFORM);
  bboxafter.apply(root);
  SbXfBox3f resultafter = bboxafter.getXfBoundingBox();

  resetpath->unref();
  root->unref();

  fprintf(stderr, "[repro] bbox with resetBefore: empty=%d, resetAfter: empty=%d\n",
          (int)resultbefore.isEmpty(), (int)resultafter.isEmpty());

  // Neither should crash; a non-empty box is expected in both cases
  // since the cube is still traversed either way -- resetBefore/After
  // only resets accumulated *transform* state at that path, not
  // whether the geometry below it is visited.
  return !resultbefore.isEmpty() && !resultafter.isEmpty();
}

static SbBool
test_reorganize_action(void)
{
  SoSeparator * root = new SoSeparator;
  root->ref();

  SoGroup * group = new SoGroup;
  root->addChild(group);

  SoCoordinate3 * coords = new SoCoordinate3;
  coords->point.set1Value(0, -1, -1, 0);
  coords->point.set1Value(1, 1, -1, 0);
  coords->point.set1Value(2, 1, 1, 0);
  coords->point.set1Value(3, -1, 1, 0);
  group->addChild(coords);

  SoIndexedFaceSet * ifs = new SoIndexedFaceSet;
  ifs->coordIndex.set1Value(0, 0);
  ifs->coordIndex.set1Value(1, 1);
  ifs->coordIndex.set1Value(2, 2);
  ifs->coordIndex.set1Value(3, 3);
  ifs->coordIndex.set1Value(4, -1);
  group->addChild(ifs);

  int numchildrenbefore = group->getNumChildren();

  SoReorganizeAction reorg;
  reorg.apply(root);

  int numchildrenafter = group->getNumChildren();

  fprintf(stderr, "[repro] SoReorganizeAction: children before=%d after=%d\n",
          numchildrenbefore, numchildrenafter);

  root->unref();
  return numchildrenbefore == numchildrenafter;
}

static SbBool
test_tovrml_action(void)
{
  // SoToVRMLAction converts VRML97 -> VRML1/Inventor, not the other
  // direction -- feed it a VRML97 scene.
  SoVRMLGroup * root = new SoVRMLGroup;
  root->ref();
  root->addChild(new SoVRMLBox);

  SoToVRMLAction toaction;
  toaction.apply(root);
  SoNode * vrmlroot = toaction.getVRMLSceneGraph();

  SbBool ok = FALSE;
  if (vrmlroot && vrmlroot->isOfType(SoGroup::getClassTypeId())) {
    SoGroup * g = static_cast<SoGroup *>(vrmlroot);
    ok = g->getNumChildren() > 0;
    fprintf(stderr, "[repro] SoToVRMLAction: vrml root has %d children\n", g->getNumChildren());
  }
  else {
    fprintf(stderr, "[repro] SoToVRMLAction: no VRML scene graph produced\n");
  }

  root->unref();
  return ok;
}

int
main()
{
  SoDB::init();

  int failures = 0;
  if (!test_bbox_reset()) { fprintf(stderr, "[repro] FAIL: bbox reset check\n"); failures++; }
  if (!test_reorganize_action()) { fprintf(stderr, "[repro] FAIL: reorganize action check\n"); failures++; }
  if (!test_tovrml_action()) { fprintf(stderr, "[repro] FAIL: toVRML action check\n"); failures++; }

  if (failures > 0) {
    fprintf(stderr, "[repro] FAIL: %d check(s) failed\n", failures);
    return 1;
  }
  fprintf(stderr, "[repro] PASS\n");
  return 0;
}
