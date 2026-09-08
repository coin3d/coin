// Fuller, end-to-end reproducer for Coin issue #174, exercising
// SoTransformerDragger::dragStart()'s own new early-return directly
// (as opposed to repro.cpp, which drives drag()/dragFinish() through a
// protected-access subclass on whatkind == WHATKIND_NONE without going
// through a real pick).
//
// dragStart() considers a pick "found" if the pick path runs through
// one of its 20 hardcoded named parts (translator1-6, rotator1-6,
// scale1-8), OR if getSurrogatePartPickedName() equals one of those 20
// strings. The surrogate mechanism (SoInteractionKit::setPartAsPath(),
// documented public API) lets an application register some *other*
// node's path as standing in for any public, leaf catalog part of the
// kit -- not just the 20 interactive ones. Registering a surrogate for
// any *other* public leaf part (here: "surroundScale", a real,
// unrelated catalog entry that has nothing to do with the 20 checked
// names) and then picking that surrogate node reproduces exactly the
// "found == FALSE" condition dragStart() used to abort on: a
// completely valid, non-null pick path that goes through neither the
// dragger's own named parts nor a matching surrogate name.
//
// This drives the real SoDragger::handleEvent() pick/surrogate-pick
// machinery with synthetic mouse events (SoHandleEventAction, pure ray
// picking -- no GL context needed), through mouse-down, mouse-move and
// mouse-up, confirming none of dragStart()/drag()/dragFinish() abort.

#include <cstdio>
#include <Inventor/SoDB.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/draggers/SoTransformerDragger.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/SoPath.h>
#include <Inventor/SbViewportRegion.h>

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

  // A node with nothing to do with any of dragStart()'s 20 recognized
  // part names, placed well away from the dragger's own [-1,1]^3
  // default geometry so it can be picked unambiguously.
  SoSeparator * surrogateSep = new SoSeparator;
  SoTranslation * t = new SoTranslation;
  t->translation.setValue(5.0f, 0.0f, 0.0f);
  surrogateSep->addChild(t);
  SoCube * surrogateCube = new SoCube;
  surrogateSep->addChild(surrogateCube);
  root->addChild(surrogateSep);

  SbViewportRegion vp(400, 400);
  camera->viewAll(root, vp);

  SoSearchAction sa;
  sa.setNode(surrogateCube);
  sa.apply(root);
  SoPath * surrogatepath = sa.getPath();
  if (!surrogatepath) {
    fprintf(stderr, "[repro] FAIL: could not find a path to the surrogate cube\n");
    root->unref();
    return 1;
  }
  surrogatepath->ref();

  // "surroundScale" is a real, public, leaf catalog part of
  // SoTransformerDragger that has nothing to do with any of the 20
  // names dragStart() checks -- exactly the point.
  SbBool regok = dragger->setPartAsPath("surroundScale", surrogatepath);
  fprintf(stderr, "[repro] setPartAsPath(\"surroundScale\", <surrogate cube path>) = %d\n", (int)regok);
  if (!regok) {
    fprintf(stderr, "[repro] FAIL: could not register the surrogate part\n");
    surrogatepath->unref();
    root->unref();
    return 1;
  }

  SbViewVolume vv = camera->getViewVolume(vp.getViewportAspectRatio());
  SbVec3f screenpt;
  vv.projectToScreen(SbVec3f(5.0f, 0.0f, 0.0f), screenpt);
  SbVec2s vpsize = vp.getViewportSizePixels();
  short x = static_cast<short>(screenpt[0] * float(vpsize[0]));
  short y = static_cast<short>(screenpt[1] * float(vpsize[1]));
  fprintf(stderr, "[repro] surrogate cube screen position: (%d, %d) of (%d, %d)\n",
          x, y, vpsize[0], vpsize[1]);

  SoHandleEventAction action(vp);

  SoMouseButtonEvent press;
  press.setButton(SoMouseButtonEvent::BUTTON1);
  press.setState(SoButtonEvent::DOWN);
  press.setPosition(SbVec2s(x, y));
  action.setEvent(&press);
  fprintf(stderr, "[repro] applying BUTTON1 DOWN on the surrogate cube (should route to "
                  "SoTransformerDragger::dragStart() with found == FALSE)...\n");
  action.apply(root);
  fprintf(stderr, "[repro] mouse-down handled, no abort\n");

  SoLocation2Event move;
  move.setPosition(SbVec2s(short(x + 5), short(y + 5)));
  action.setEvent(&move);
  fprintf(stderr, "[repro] applying mouse move (should route to SoTransformerDragger::drag())...\n");
  action.apply(root);
  fprintf(stderr, "[repro] mouse move handled, no abort\n");

  SoMouseButtonEvent release;
  release.setButton(SoMouseButtonEvent::BUTTON1);
  release.setState(SoButtonEvent::UP);
  release.setPosition(SbVec2s(short(x + 5), short(y + 5)));
  action.setEvent(&release);
  fprintf(stderr, "[repro] applying BUTTON1 UP (should route to SoTransformerDragger::dragFinish())...\n");
  action.apply(root);
  fprintf(stderr, "[repro] mouse-up handled, no abort\n");

  surrogatepath->unref();
  root->unref();

  fprintf(stderr, "[repro] PASS\n");
  return 0;
}
