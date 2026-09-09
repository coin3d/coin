// Regression test for Phase 3 of the SoPath/SoFullPath downcast UB fix
// (src/draggers/SoDragger.cpp, SoTransformerDragger.cpp,
// src/manips/commoncode.cpp + the five *Manip.cpp replaceNode()
// implementations).
//
// Two independent things are exercised:
//
// 1. SoTransformManip::replaceNode()/replaceManip() (commoncode.cpp's
//    macro-generated body for the sibling classes has the same shape):
//    build a plain SoTransform under an SoGroup parent, replace it with
//    a manip (exercises getFullTail()/getFullLength()/
//    getFullNodeFromTail() against a real, non-nodekit SoPath), check
//    the manip actually replaced the transform in the scene graph, then
//    replace the manip back with a plain SoTransform (the
//    commoncode.cpp macro body) and check that round-trips correctly.
//
// 2. SoDragger::isPicked()/shouldGrabBasedOnSurrogate()/
//    createPathToThis() and SoDraggerCache::update() (now storing a
//    plain SoPath* instead of a reinterpret_cast<SoFullPath*>'d one):
//    drive a real pick-drag-release sequence against an
//    SoTransformerDragger via SoHandleEventAction, same technique used
//    for issue #174's reproducers on the neighboring branch, confirming
//    the whole start/motion/finish cycle still works.

#include "../CoinCleanup.h"
#include <cstdio>
#include <Inventor/SoDB.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/SoPath.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/manips/SoTransformManip.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/draggers/SoTransformerDragger.h>
#include <Inventor/SbViewportRegion.h>

static SbBool
test_manip_replace(void)
{
  SoSeparator * root = new SoSeparator;
  root->ref();

  SoGroup * group = new SoGroup;
  root->addChild(group);

  SoTransform * xf = new SoTransform;
  xf->translation.setValue(1.0f, 2.0f, 3.0f);
  group->addChild(xf);

  SoSearchAction sa;
  sa.setNode(xf);
  sa.apply(root);
  SoPath * pathtoxf = sa.getPath();
  if (!pathtoxf) {
    fprintf(stderr, "[repro] FAIL: could not find path to SoTransform\n");
    root->unref();
    return FALSE;
  }
  pathtoxf->ref();

  SoTransformManip * manip = new SoTransformManip;
  SbBool replaced = manip->replaceNode(pathtoxf);
  pathtoxf->unref();

  fprintf(stderr, "[repro] SoTransformManip::replaceNode() = %d\n", (int)replaced);
  if (!replaced) {
    fprintf(stderr, "[repro] FAIL: replaceNode() failed\n");
    root->unref();
    return FALSE;
  }
  if (group->getChild(0) != manip) {
    fprintf(stderr, "[repro] FAIL: manip did not replace the transform in the scene graph\n");
    root->unref();
    return FALSE;
  }
  // field values should have been copied over
  if (manip->translation.getValue() != SbVec3f(1.0f, 2.0f, 3.0f)) {
    fprintf(stderr, "[repro] FAIL: manip did not inherit the transform's field values\n");
    root->unref();
    return FALSE;
  }

  SoSearchAction sa2;
  sa2.setNode(manip);
  sa2.apply(root);
  SoPath * pathtomanip = sa2.getPath();
  if (!pathtomanip) {
    fprintf(stderr, "[repro] FAIL: could not find path to the manip\n");
    root->unref();
    return FALSE;
  }
  pathtomanip->ref();

  SbBool replacedback = manip->replaceManip(pathtomanip, NULL);
  pathtomanip->unref();

  fprintf(stderr, "[repro] SoTransformManip::replaceManip() = %d\n", (int)replacedback);
  if (!replacedback) {
    fprintf(stderr, "[repro] FAIL: replaceManip() failed\n");
    root->unref();
    return FALSE;
  }
  SoNode * back = group->getChild(0);
  if (!back->isOfType(SoTransform::getClassTypeId()) || back == manip) {
    fprintf(stderr, "[repro] FAIL: replaceManip() did not restore a plain SoTransform\n");
    root->unref();
    return FALSE;
  }

  root->unref();
  return TRUE;
}

static SbBool
test_dragger_pick_drag(void)
{
  SoSeparator * root = new SoSeparator;
  root->ref();

  SoPerspectiveCamera * camera = new SoPerspectiveCamera;
  root->addChild(camera);

  SoTransformerDragger * dragger = new SoTransformerDragger;
  root->addChild(dragger);

  SbViewportRegion vp(400, 400);
  camera->viewAll(root, vp);

  SoHandleEventAction action(vp);
  SbVec2s center(200, 200);

  SoMouseButtonEvent press;
  press.setButton(SoMouseButtonEvent::BUTTON1);
  press.setState(SoButtonEvent::DOWN);
  press.setPosition(center);
  action.setEvent(&press);
  action.apply(root);
  SbBool startedok = dragger->isActive.getValue();

  SoLocation2Event move;
  move.setPosition(SbVec2s(short(center[0] + 15), short(center[1] + 15)));
  action.setEvent(&move);
  action.apply(root);

  SoMouseButtonEvent release;
  release.setButton(SoMouseButtonEvent::BUTTON1);
  release.setState(SoButtonEvent::UP);
  release.setPosition(SbVec2s(short(center[0] + 15), short(center[1] + 15)));
  action.setEvent(&release);
  action.apply(root);
  SbBool finishedok = !dragger->isActive.getValue();

  fprintf(stderr, "[repro] dragger pick/drag/release: started=%d finished=%d\n",
          (int)startedok, (int)finishedok);

  root->unref();
  return startedok && finishedok;
}

int
main()
{
  SoDB::init();
  CoinReproducerCleanup cleanup;
  SoInteraction::init();

  int failures = 0;
  if (!test_manip_replace()) failures++;
  if (!test_dragger_pick_drag()) failures++;

  if (failures > 0) {
    fprintf(stderr, "[repro] FAIL: %d check(s) failed\n", failures);
    return 1;
  }
  fprintf(stderr, "[repro] PASS\n");
  return 0;
}
