// Regression test for Phase 5 of the SoPath/SoFullPath downcast UB fix
// (src/nodekits/SoBaseKit.cpp, src/sensors/SoPathSensor.cpp -- see the
// Phase 1 foundation commit on this branch for the new
// SoPath::getFullXxx() methods used throughout).
//
// Three checks:
//
// 1. SoBaseKit::getPartString()/createPathToPart(): build a path to a
//    named part of a real nodekit (SoTransformerDragger's
//    "translator1"), confirm createPathToPart() returns a path whose
//    tail really is that part, and that getPartString() on that same
//    path round-trips back to "translator1" -- both exercise the
//    getFullTail()/getFullLength() migration in these two functions
//    directly (the path necessarily runs through the kit's hidden
//    internal catalog structure to reach a named part).
//
// 2. SoBaseKit::rayPick()/SoBaseKitP::addKitDetail(): a real
//    SoRayPickAction pick against the dragger, confirming the picked
//    point gets a valid SoNodeKitDetail attached with a non-empty part
//    name (addKitDetail()'s migrated getFullLength() bounds check).
//
// 3. SoPathSensor: attach a sensor to a path into the same nodekit,
//    trigger a change on the tail node, confirm the sensor fires
//    (PRIVATE(this)->path, now plain SoPath* instead of
//    reinterpret_cast<SoFullPath*>'d, still correctly audits the path).

#include <cstdio>
#include <Inventor/SoDB.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/SoPath.h>
#include <Inventor/SoNodeKitPath.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/details/SoNodeKitDetail.h>
#include <Inventor/draggers/SoTransformerDragger.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/sensors/SoPathSensor.h>
#include <Inventor/sensors/SoSensorManager.h>
#include <Inventor/SbViewportRegion.h>

static int sensortriggercount = 0;

static void
path_sensor_cb(void *, SoSensor *)
{
  sensortriggercount++;
}

static SbBool
test_partstring_roundtrip(SoTransformerDragger * dragger)
{
  SoNodeKitPath * path = dragger->createPathToPart("translator1", TRUE);
  if (!path) {
    fprintf(stderr, "[repro] FAIL: createPathToPart(\"translator1\") returned NULL\n");
    return FALSE;
  }
  path->ref();

  SoNode * tail = path->getFullTail();
  SbString name = dragger->getPartString(path);
  fprintf(stderr, "[repro] createPathToPart tail=%p getPartString()=\"%s\"\n",
          (void *)tail, name.getString());

  SbBool ok = (tail != NULL) && (name == "translator1");
  path->unref();
  return ok;
}

static SbBool
test_raypick_kitdetail(SoTransformerDragger * dragger, SoSeparator * root)
{
  SbViewportRegion vp(400, 400);
  SoPerspectiveCamera * camera = (SoPerspectiveCamera *) root->getChild(0);
  camera->viewAll(root, vp);

  SoRayPickAction pick(vp);
  pick.setPoint(SbVec2s(200, 200));
  pick.apply(root);

  const SoPickedPoint * pp = pick.getPickedPoint();
  if (!pp) {
    fprintf(stderr, "[repro] FAIL: expected a hit at viewport center\n");
    return FALSE;
  }

  const SoDetail * det = pp->getDetail(dragger);
  if (!det || !det->isOfType(SoNodeKitDetail::getClassTypeId())) {
    fprintf(stderr, "[repro] FAIL: expected an SoNodeKitDetail on the dragger\n");
    return FALSE;
  }
  const SoNodeKitDetail * kd = static_cast<const SoNodeKitDetail *>(det);
  fprintf(stderr, "[repro] pick kit detail: partName=\"%s\"\n", kd->getPartName().getString());
  return kd->getPartName().getLength() > 0;
}

static SbBool
test_path_sensor(SoTransformerDragger * dragger)
{
  SoNodeKitPath * path = dragger->createPathToPart("translator1", TRUE);
  if (!path) {
    fprintf(stderr, "[repro] FAIL: createPathToPart for sensor test returned NULL\n");
    return FALSE;
  }
  path->ref();

  SoNode * tail = path->getFullTail();

  SoPathSensor sensor(path_sensor_cb, NULL);
  sensor.attach(path);

  sensortriggercount = 0;
  tail->touch(); // force a notification on the tail node

  // SoPathSensor is a delay sensor by default; trigger manually is not
  // public API, so process pending sensors via the sensor manager.
  SoDB::getSensorManager()->processImmediateQueue();
  SoDB::getSensorManager()->processDelayQueue(TRUE);

  fprintf(stderr, "[repro] path sensor trigger count = %d\n", sensortriggercount);

  sensor.detach();
  path->unref();
  return sensortriggercount > 0;
}

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

  int failures = 0;
  if (!test_partstring_roundtrip(dragger)) { fprintf(stderr, "[repro] FAIL: partstring roundtrip\n"); failures++; }
  if (!test_raypick_kitdetail(dragger, root)) { fprintf(stderr, "[repro] FAIL: raypick kit detail\n"); failures++; }
  if (!test_path_sensor(dragger)) { fprintf(stderr, "[repro] FAIL: path sensor\n"); failures++; }

  root->unref();

  if (failures > 0) {
    fprintf(stderr, "[repro] FAIL: %d check(s) failed\n", failures);
    return 1;
  }
  fprintf(stderr, "[repro] PASS\n");
  return 0;
}
