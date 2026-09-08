// Reproducer/regression test for the SoDragger callback mechanism fix.
//
// SoDragger::add{Start,Motion,Finish,ValueChanged,OtherEvent}Callback()
// used to reinterpret_cast<>() the caller's SoDraggerCB*
// (void(*)(void*,SoDragger*)) to the type-erased SoCallbackListCB*
// (void(*)(void*,void*)) to store it in an SoCallbackList, which then
// called it back through that generic type -- calling a function
// through a function pointer of a type other than the one it was
// declared with is undefined behavior (caught by
// -fsanitize=function), even though the ABI-level call has always
// worked correctly in practice.
//
// SoDragger now stores/invokes these callbacks through a dedicated
// SoDraggerCBList helper that never erases the type, so there is
// nothing to reinterpret_cast. This drives a real, ordinary drag
// interaction sequence (mouse down, move, an event during an active
// drag, mouse up) via SoHandleEventAction (pure ray picking, no GL
// context needed) with all five callback types registered, confirming
// each fires exactly as expected -- both that the fix didn't change
// functional behavior, and (when built with
// -fsanitize=function/undefined) that the type-punned-function-pointer
// UB is gone.

#include <cstdio>
#include <Inventor/SoDB.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/draggers/SoTransformerDragger.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/SbViewportRegion.h>

static int startcount = 0;
static int motioncount = 0;
static int finishcount = 0;
static int valuechangedcount = 0;
static int othereventcount = 0;

static void startCB(void *, SoDragger *) { startcount++; }
static void motionCB(void *, SoDragger *) { motioncount++; }
static void finishCB(void *, SoDragger *) { finishcount++; }
static void valueChangedCB(void *, SoDragger *) { valuechangedcount++; }
static void otherEventCB(void *, SoDragger *) { othereventcount++; }

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

  dragger->addStartCallback(startCB);
  dragger->addMotionCallback(motionCB);
  dragger->addFinishCallback(finishCB);
  dragger->addValueChangedCallback(valueChangedCB);
  dragger->addOtherEventCallback(otherEventCB);

  SbViewportRegion vp(400, 400);
  camera->viewAll(root, vp);

  SoHandleEventAction action(vp);

  // Center of the viewport hits the dragger's translator1 (top) face
  // straight-on from the default viewAll() camera angle.
  SbVec2s center(200, 200);

  SoMouseButtonEvent press;
  press.setButton(SoMouseButtonEvent::BUTTON1);
  press.setState(SoButtonEvent::DOWN);
  press.setPosition(center);
  action.setEvent(&press);
  action.apply(root);
  fprintf(stderr, "[repro] after mouse-down: isActive=%d\n", (int)dragger->isActive.getValue());

  SoLocation2Event move;
  move.setPosition(SbVec2s(short(center[0] + 20), short(center[1] + 20)));
  action.setEvent(&move);
  action.apply(root);

  SoKeyboardEvent key;
  key.setKey(SoKeyboardEvent::A);
  key.setState(SoButtonEvent::DOWN);
  key.setPosition(SbVec2s(short(center[0] + 20), short(center[1] + 20)));
  action.setEvent(&key);
  action.apply(root);

  SoMouseButtonEvent release;
  release.setButton(SoMouseButtonEvent::BUTTON1);
  release.setState(SoButtonEvent::UP);
  release.setPosition(SbVec2s(short(center[0] + 20), short(center[1] + 20)));
  action.setEvent(&release);
  action.apply(root);
  fprintf(stderr, "[repro] after mouse-up: isActive=%d\n", (int)dragger->isActive.getValue());

  fprintf(stderr, "[repro] startcount=%d motioncount=%d finishcount=%d "
                  "valuechangedcount=%d othereventcount=%d\n",
          startcount, motioncount, finishcount, valuechangedcount, othereventcount);

  dragger->removeStartCallback(startCB);
  dragger->removeMotionCallback(motionCB);
  dragger->removeFinishCallback(finishCB);
  dragger->removeValueChangedCallback(valueChangedCB);
  dragger->removeOtherEventCallback(otherEventCB);

  root->unref();

  int failures = 0;
  if (startcount != 1) { fprintf(stderr, "[repro] FAIL: expected startcount == 1\n"); failures++; }
  if (motioncount != 1) { fprintf(stderr, "[repro] FAIL: expected motioncount == 1\n"); failures++; }
  if (finishcount != 1) { fprintf(stderr, "[repro] FAIL: expected finishcount == 1\n"); failures++; }
  if (valuechangedcount < 1) { fprintf(stderr, "[repro] FAIL: expected valuechangedcount >= 1\n"); failures++; }
  if (othereventcount < 1) { fprintf(stderr, "[repro] FAIL: expected othereventcount >= 1\n"); failures++; }

  if (failures > 0) {
    fprintf(stderr, "[repro] FAIL: %d check(s) failed\n", failures);
    return 1;
  }
  fprintf(stderr, "[repro] PASS\n");
  return 0;
}
