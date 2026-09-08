// Regression test for the second wave of SoCallbackList
// function-pointer-type UB fixes, found while verifying the
// somisc-fullpath-migration reproducer's SoOffscreenRenderer check on
// the fix/sopath-fullpath-downcast-ub branch: exercising a real,
// complete SoGLRenderAction pass for the first time (once that
// reproducer's environment-specific GLX blocker was worked around)
// surfaced a *separate*, pre-existing instance of the same UB
// category already fixed for SoDragger on this branch
// (fix/socallbacklist-function-pointer-ub, commit 8d55620e4a) --
// SoGLRenderAction's precblist (SoGLPreRenderCB, registered via
// addPreRenderCallback()) was reinterpret_cast<>ing the caller's
// function pointer to the type-erased SoCallbackListCB before storing
// it in an SoCallbackList and invoking it back through that generic
// type, exactly like the SoDragger case.
//
// A full audit of every remaining SoCallbackList consumer in the tree
// (grep -rn "SoCallbackListCB" src/ include/) found one more instance
// beyond SoGLRenderAction: SoSelection's five callback lists
// (selCBList, deselCBList, startCBList, finishCBList, changeCBList).
// Unlike SoDragger/SoGLRenderAction (private impl only), these are
// `protected` members of SoSelection's public header, and Coin's own
// SoExtSelection subclass reaches into startCBList/finishCBList
// directly -- so the fix (SoSelectionPathCBList/SoSelectionClassCBList,
// analogous to SoDraggerCBList) lives in a new shared private header,
// src/nodes/SoSelectionP.h, included by both SoSelection.cpp and
// SoExtSelection.cpp, rather than being local to one .cpp file.
//
// Two checks:
//
// 1. SoGLRenderAction::addPreRenderCallback(): a real SoOffscreenRenderer
//    render (see somisc-fullpath-migration/repro.cpp for why
//    COIN_GLX_PIXMAP_DIRECT_RENDERING is needed in some environments)
//    with a pre-render callback registered, confirming it fires.
//
// 2. SoSelection: a real pick (mouse down/up via SoHandleEventAction)
//    on a cube below an SoSelection node, with all five callback
//    types registered, confirming select/start/change fire on pick,
//    and deselect/start/change fire on a second pick of empty space
//    (SINGLE policy deselects the previous pick). finishCB fires once
//    per handled mouse-release regardless of selection change.

#include <cstdlib>
#include <cstdio>
#include <Inventor/SoDB.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/SoPath.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/SbViewportRegion.h>

static int prerendercount = 0;
static void
preRenderCB(void *, SoGLRenderAction *)
{
  prerendercount++;
}

static SbBool
test_glrenderaction_prerender(void)
{
  // See the comment in somisc-fullpath-migration/repro.cpp: some
  // environments (e.g. many post-RHEL8 distributions) refuse indirect
  // GLX contexts outright, and SoOffscreenRenderer's default backend
  // probes one first. Coin's own escape hatch,
  // COIN_GLX_PIXMAP_DIRECT_RENDERING, forces a direct-rendering
  // context for the offscreen pixmap instead. Must be set before the
  // glue layer's first (lazily cached) context-creation attempt.
  setenv("COIN_GLX_PIXMAP_DIRECT_RENDERING", "1", 0);

  SbViewportRegion vp(64, 64);
  SoOffscreenRenderer renderer(vp);
  renderer.getGLRenderAction()->addPreRenderCallback(preRenderCB, NULL);

  SoSeparator * root = new SoSeparator;
  root->ref();
  SoPerspectiveCamera * camera = new SoPerspectiveCamera;
  root->addChild(camera);
  root->addChild(new SoDirectionalLight);
  SoCube * cube = new SoCube;
  root->addChild(cube);
  camera->viewAll(root, vp);

  SbBool ok = renderer.render(root);
  fprintf(stderr, "[repro] SoOffscreenRenderer::render() = %d, prerendercount=%d\n",
          (int)ok, prerendercount);

  renderer.getGLRenderAction()->removePreRenderCallback(preRenderCB, NULL);
  root->unref();
  return ok && prerendercount >= 1;
}

static int selectcount = 0;
static int deselectcount = 0;
static int startcount = 0;
static int finishcount = 0;
static int changecount = 0;

static void selectCB(void *, SoPath *) { selectcount++; }
static void deselectCB(void *, SoPath *) { deselectcount++; }
static void startCB(void *, SoSelection *) { startcount++; }
static void finishCB(void *, SoSelection *) { finishcount++; }
static void changeCB(void *, SoSelection *) { changecount++; }

static void
click(SoHandleEventAction & action, SoNode * root, const SbVec2s & pos)
{
  SoMouseButtonEvent press;
  press.setButton(SoMouseButtonEvent::BUTTON1);
  press.setState(SoButtonEvent::DOWN);
  press.setPosition(pos);
  action.setEvent(&press);
  action.apply(root);

  SoMouseButtonEvent release;
  release.setButton(SoMouseButtonEvent::BUTTON1);
  release.setState(SoButtonEvent::UP);
  release.setPosition(pos);
  action.setEvent(&release);
  action.apply(root);
}

static SbBool
test_soselection_callbacks(void)
{
  SoSeparator * root = new SoSeparator;
  root->ref();

  SoPerspectiveCamera * camera = new SoPerspectiveCamera;
  root->addChild(camera);

  SoSelection * selection = new SoSelection;
  selection->policy = SoSelection::SINGLE;
  root->addChild(selection);
  selection->addChild(new SoCube);

  selection->addSelectionCallback(selectCB);
  selection->addDeselectionCallback(deselectCB);
  selection->addStartCallback(startCB);
  selection->addFinishCallback(finishCB);
  selection->addChangeCallback(changeCB);

  SbViewportRegion vp(400, 400);
  camera->viewAll(root, vp);

  SoHandleEventAction action(vp);

  // Center of the viewport hits the cube straight-on from the default
  // viewAll() camera angle -- selects it.
  click(action, root, SbVec2s(200, 200));

  // A corner of the viewport, well outside the cube -- SINGLE policy
  // deselects the previous pick.
  click(action, root, SbVec2s(10, 10));

  fprintf(stderr, "[repro] SoSelection: selectcount=%d deselectcount=%d "
                  "startcount=%d finishcount=%d changecount=%d\n",
          selectcount, deselectcount, startcount, finishcount, changecount);

  selection->removeSelectionCallback(selectCB);
  selection->removeDeselectionCallback(deselectCB);
  selection->removeStartCallback(startCB);
  selection->removeFinishCallback(finishCB);
  selection->removeChangeCallback(changeCB);

  root->unref();

  return selectcount == 1 && deselectcount == 1 &&
         startcount == 2 && finishcount == 2 && changecount == 2;
}

int
main()
{
  SoDB::init();
  SoInteraction::init();

  int failures = 0;
  if (!test_glrenderaction_prerender()) { fprintf(stderr, "[repro] FAIL: SoGLRenderAction pre-render callback\n"); failures++; }
  if (!test_soselection_callbacks()) { fprintf(stderr, "[repro] FAIL: SoSelection callbacks\n"); failures++; }

  if (failures > 0) {
    fprintf(stderr, "[repro] FAIL: %d check(s) failed\n", failures);
    return 1;
  }
  fprintf(stderr, "[repro] PASS\n");
  return 0;
}
