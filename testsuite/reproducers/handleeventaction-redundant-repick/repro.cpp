// Reproducer for Coin issue #106 ("getPickedPoint() and
// getPickedPointList() should not change picking condition").
//
// SoHandleEventAction::getPickedPoint() redid the whole ray-pick scene
// graph traversal (SoRayPickAction::apply()) with PICK_ALL=FALSE
// whenever the *previous* pick on this same action instance had been a
// PICK_ALL=TRUE one (tracked via the private didpickall flag) -- even
// though a valid, already-computed pick-all result already contains
// the answer: getPickedPoint() (no args) forwards to getPickedPoint(0),
// and getPickedPointList() sorts its result by distance before
// returning it, so index 0 of an existing pick-all result is
// *identical* to what a fresh PICK_ALL=FALSE pick would produce.
//
// Reported real-world impact: independently-written scene graph nodes
// (e.g. two different draggers/custom SoEventCallback handlers) that
// alternate calling SoHandleEventAction::getPickedPoint() and
// getPickedPointList() during the *same* event-handling traversal each
// force a full extra ray-pick scene graph re-traversal, even though
// nothing about the pick geometry changed between those calls -- pure
// wasted work, reported as a real performance problem for large scene
// graphs with many independent event-handling nodes.
//
// This repro places two SoEventCallback nodes under the same
// SoSeparator, one calling getPickedPointList() (as some hypothetical
// "Node A") followed by another calling getPickedPoint() (as
// hypothetical "Node B") -- exactly the interleaving described in the
// issue -- and counts how many times the ray-pick action actually
// re-traverses the pickable geometry (via an SoCallback node inside it
// that only counts SoRayPickAction visits), plus checks that the two
// calls agree on which point/node was hit (correctness unaffected by
// the fix).
//
// See run.sh in this directory for how to build and run this against a
// given libCoin build.

#include "../CoinCleanup.h"
#include <cstdio>
#include <Inventor/SoDB.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/SbViewportRegion.h>

static int repick_count = 0;

static void
count_raypick_visits_cb(void *, SoAction * action)
{
  if (action->isOfType(SoRayPickAction::getClassTypeId())) {
    repick_count++;
  }
}

static SoNode * hit_from_list = NULL;
static SoNode * hit_from_point = NULL;

// "Node A": some independent scene graph node that only ever wants the
// full list of intersections.
static void
nodeA_getlist_cb(void *, SoEventCallback * node)
{
  const SoPickedPointList & ppl = node->getAction()->getPickedPointList();
  if (ppl.getLength() > 0) {
    hit_from_list = ppl[0]->getPath()->getTail();
  }
}

// "Node B": a different, independently-written node that only ever
// wants the single closest intersection.
static void
nodeB_getpoint_cb(void *, SoEventCallback * node)
{
  const SoPickedPoint * pp = node->getAction()->getPickedPoint();
  if (pp) {
    hit_from_point = pp->getPath()->getTail();
  }
}

int main()
{
  SoDB::init();
  CoinReproducerCleanup cleanup;

  SoSeparator * root = new SoSeparator;
  root->ref();

  SoPerspectiveCamera * camera = new SoPerspectiveCamera;
  camera->position.setValue(0, 0, 5);
  root->addChild(camera);

  SoSeparator * geom = new SoSeparator;
  root->addChild(geom);

  SoCallback * counter = new SoCallback;
  counter->setCallback(count_raypick_visits_cb);
  geom->addChild(counter);

  SoCube * cube = new SoCube;
  geom->addChild(cube);

  SoEventCallback * nodeA = new SoEventCallback;
  nodeA->addEventCallback(SoLocation2Event::getClassTypeId(), nodeA_getlist_cb);
  root->addChild(nodeA);

  SoEventCallback * nodeB = new SoEventCallback;
  nodeB->addEventCallback(SoLocation2Event::getClassTypeId(), nodeB_getpoint_cb);
  root->addChild(nodeB);

  SbViewportRegion vp(400, 400);
  camera->viewAll(root, vp);

  SoLocation2Event ev;
  ev.setPosition(SbVec2s(200, 200)); // dead center -- should hit the cube

  SoHandleEventAction action(vp);
  action.setEvent(&ev);
  repick_count = 0;
  hit_from_list = NULL;
  hit_from_point = NULL;
  action.apply(root);

  root->unref();

  fprintf(stderr, "[repro] ray-pick re-traversals for one list-then-point "
                  "call sequence: %d\n", repick_count);
  fprintf(stderr, "[repro] hit node from getPickedPointList(): %p\n", (void*)hit_from_list);
  fprintf(stderr, "[repro] hit node from getPickedPoint():     %p\n", (void*)hit_from_point);

  int failures = 0;
  if (hit_from_list == NULL || hit_from_point == NULL) {
    fprintf(stderr, "[repro] FAIL: expected both calls to hit the cube, got a NULL\n");
    failures++;
  }
  else if (hit_from_list != hit_from_point) {
    fprintf(stderr, "[repro] FAIL: getPickedPoint() and getPickedPointList() "
                    "disagree on which node was hit\n");
    failures++;
  }
  if (repick_count != 1) {
    fprintf(stderr, "[repro] FAIL: expected exactly 1 ray-pick re-traversal "
                    "(the redundant repick from issue #106 should be gone), got %d\n",
                    repick_count);
    failures++;
  }

  if (failures > 0) {
    fprintf(stderr, "[repro] FAIL: %d check(s) failed\n", failures);
    return 1;
  }
  fprintf(stderr, "[repro] PASS\n");
  return 0;
}
