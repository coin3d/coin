// Regression test for the SoPickedPoint/SoCompactPathList/SoProto/
// SoSceneManagerP migration off reinterpret_cast<SoFullPath*> onto the
// new SoPath::getFullXxx() methods (Phase 2 of the SoPath/SoFullPath
// downcast UB fix, following the Phase 1 foundation commit on this
// branch).
//
// SoPickedPoint in particular is exercised through a real pick against
// a nodekit (SoTransformerDragger), so the picked path actually
// contains hidden nodes -- exactly the case where getFullLength()/
// getFullTail() (now used internally by SoPickedPoint) must return
// something different from getLength()/getTail(), proving the
// migration preserved behavior rather than accidentally collapsing
// onto the "wrong" (hidden-node-truncated) variant.

#include <cstdio>
#include <Inventor/SoDB.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/draggers/SoTransformerDragger.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/SbLine.h>

int
main()
{
  SoDB::init();
  SoInteraction::init();

  SoSeparator * root = new SoSeparator;
  root->ref();

  SoPerspectiveCamera * camera = new SoPerspectiveCamera;
  root->addChild(camera);

  SoTransformerDragger * dragger = new SoTransformerDragger;
  root->addChild(dragger);

  SbViewportRegion vp(400, 400);
  camera->viewAll(root, vp);

  SoRayPickAction pick(vp);
  pick.setPoint(SbVec2s(200, 200)); // center -- hits the translator1 face
  pick.apply(root);

  const SoPickedPoint * pp = pick.getPickedPoint();
  if (!pp) {
    fprintf(stderr, "[repro] FAIL: expected a hit at viewport center\n");
    root->unref();
    return 1;
  }

  const SoPath * path = pp->getPath();
  int plainlen = path->getLength();
  int fulllen = path->getFullLength();
  SoNode * tail = path->getFullTail();

  fprintf(stderr, "[repro] pick path: getLength()=%d getFullLength()=%d tail=%p (%s)\n",
          plainlen, fulllen, (void *)tail,
          tail ? tail->getTypeId().getName().getString() : "(null)");

  // getDetail(NULL) internally uses getFullLength()-1 as the tail
  // index -- confirm it doesn't crash and returns a sane (possibly
  // NULL, that's fine) result.
  const SoDetail * det = pp->getDetail();
  fprintf(stderr, "[repro] getDetail(NULL) = %p\n", (const void *)det);

  // getObjectPoint()/getObjectNormal()/getObjectTextureCoords() all
  // internally compare against getFullTail() and, for a non-NULL node
  // that differs from the tail, exercise applyMatrixAction()'s
  // findNode()/getNode() loop that used to go through the
  // SoFullPath cast too.
  SbVec3f objpt = pp->getObjectPoint(dragger);
  fprintf(stderr, "[repro] getObjectPoint(dragger) = (%g, %g, %g)\n",
          objpt[0], objpt[1], objpt[2]);

  root->unref();

  int failures = 0;
  if (fulllen <= plainlen) {
    fprintf(stderr, "[repro] FAIL: expected getFullLength() > getLength() for a nodekit pick path\n");
    failures++;
  }
  if (tail == NULL) {
    fprintf(stderr, "[repro] FAIL: expected a non-NULL full-path tail\n");
    failures++;
  }

  if (failures > 0) {
    fprintf(stderr, "[repro] FAIL: %d check(s) failed\n", failures);
    return 1;
  }
  fprintf(stderr, "[repro] PASS\n");
  return 0;
}
