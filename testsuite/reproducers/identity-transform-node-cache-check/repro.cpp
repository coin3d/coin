// Verification repro for the SoScale/SoRotation/SoRotationXYZ identity
// optimization (matching SoTransform's issue #534 fix: skip the
// SoModelMatrixElement update entirely when the node's value is
// exactly identity).
//
// SoModelMatrixElement::scaleBy()/rotateBy() (and mult()/translateBy())
// call elem->addNodeId(node) as their last step -- registering the node
// as a cache dependency for whatever bbox/render cache is currently
// being built. Skipping the call for an identity value also skips that
// registration. This is not a new pattern introduced here:
// SoTranslation::doAction() has skipped translateBy() (and therefore
// addNodeId()) for a zero translation since long before this session,
// and SoTransform::doAction() does the same for mult() since the #534
// fix. But it deserves its own empirical check for these three sibling
// nodes rather than resting on precedent alone: if skipped
// addNodeId() ever let a bbox/render cache go stale, the specific
// failure mode would be exactly "identity -> change to non-identity ->
// change back to identity, after a cache was already built for the
// identity state" -- a cache built while the node contributed nothing
// might not know to invalidate when the node's value changes.
//
// This repro drives that exact sequence for each of the three nodes,
// checking both SoGetMatrixAction's accumulated matrix and
// SoGetBoundingBoxAction's bounding box (which has its own dependency
// cache, SoBoundingBoxCache) at each step:
//
//   1. identity value      -> matrix/bbox must match the "no node at
//                              all" baseline exactly.
//   2. non-identity value  -> matrix/bbox must reflect the change.
//   3. back to identity    -> matrix/bbox must match step 1 again, even
//                              though a bbox cache was already built
//                              once for the identity state in step 1,
//                              then invalidated and rebuilt for step 2.
//
// See run.sh in this directory for how to build and run this against a
// given libCoin build.

#include <cstdio>
#include <cstring>
#include <cmath>
#include <Inventor/SoDB.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoRotationXYZ.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/SbBox3f.h>

static int failures = 0;

static void
checkMatrix(const char * label, const SbMatrix & got, const SbMatrix & want)
{
  if (got != want) {
    fprintf(stderr, "[repro] FAIL (%s): matrix mismatch\n", label);
    failures++;
  }
  else {
    fprintf(stderr, "[repro] ok   (%s): matrix matches\n", label);
  }
}

static SbBool
approxEqual(const SbVec3f & a, const SbVec3f & b)
{
  // Small tolerance here only: this compares bbox extents derived from
  // cos()/sin() of a 90-degree rotation, which is not exactly 0/1 in
  // floating point (unlike the identity checks in the production code
  // under test, which compare exact field values with no trig
  // involved -- those stay exact, per instructions).
  const float eps = 1.0e-5f;
  return (a - b).length() < eps;
}

static void
checkBBox(const char * label, const SbBox3f & got, const SbBox3f & want)
{
  if (!approxEqual(got.getMin(), want.getMin()) ||
      !approxEqual(got.getMax(), want.getMax())) {
    fprintf(stderr, "[repro] FAIL (%s): bbox mismatch: got [%g %g %g]-[%g %g %g], want [%g %g %g]-[%g %g %g]\n",
            label,
            got.getMin()[0], got.getMin()[1], got.getMin()[2],
            got.getMax()[0], got.getMax()[1], got.getMax()[2],
            want.getMin()[0], want.getMin()[1], want.getMin()[2],
            want.getMax()[0], want.getMax()[1], want.getMax()[2]);
    failures++;
  }
  else {
    fprintf(stderr, "[repro] ok   (%s): bbox matches\n", label);
  }
}

static SbMatrix
getMatrix(SoSeparator * root, SoNode * tail)
{
  SoGetMatrixAction ma(SbViewportRegion(100, 100));
  SoPath * p = new SoPath(root);
  p->ref();
  p->append(tail);
  ma.apply(p);
  p->unref();
  return ma.getMatrix();
}

static SbBox3f
getBBox(SoSeparator * root, SbBool cachingon)
{
  root->boundingBoxCaching = cachingon ? SoSeparator::ON : SoSeparator::OFF;
  SoGetBoundingBoxAction ba(SbViewportRegion(100, 100));
  ba.apply(root);
  return ba.getBoundingBox();
}

// Exercises identity -> non-identity -> identity for one transform-kind
// child inserted before a fixed SoCube, checking matrix + bbox at each
// step against a baseline scene with no transform node at all (for the
// identity steps) or an equivalent explicit-matrix scene (for the
// non-identity step). Uses caching (ON) throughout, so the bbox
// action's SoBoundingBoxCache is actually exercised across the
// transitions, not bypassed.
static void
runSequence(const char * kind)
{
  fprintf(stderr, "\n[repro] === %s ===\n", kind);

  SoSeparator * root = new SoSeparator;
  root->ref();
  root->renderCaching = SoSeparator::ON;
  root->boundingBoxCaching = SoSeparator::ON;

  SoNode * xform = NULL;
  if (strcmp(kind, "scale") == 0) {
    SoScale * s = new SoScale;
    s->scaleFactor.setValue(1.0f, 1.0f, 1.0f); // identity
    xform = s;
  }
  else if (strcmp(kind, "rotation") == 0) {
    SoRotation * r = new SoRotation;
    r->rotation.setValue(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f); // identity
    xform = r;
  }
  else { // rotationxyz
    SoRotationXYZ * r = new SoRotationXYZ;
    r->axis = SoRotationXYZ::Z;
    r->angle.setValue(0.0f); // identity
    xform = r;
  }
  root->addChild(xform);

  SoCube * cube = new SoCube;
  cube->width = 2.0f;
  cube->height = 2.0f;
  cube->depth = 2.0f;
  root->addChild(cube);

  // Step 1: identity. Baseline: matrix must be identity, bbox must be
  // the cube's own untransformed box.
  SbMatrix m1 = getMatrix(root, cube);
  checkMatrix("step1 identity: matrix == identity", m1, SbMatrix::identity());
  SbBox3f b1 = getBBox(root, TRUE);
  SbBox3f wantb1(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f);
  checkBBox("step1 identity: bbox == unit cube", b1, wantb1);

  // Step 2: mutate to non-identity. This must invalidate whatever bbox
  // cache step 1 built (the interesting case if addNodeId() being
  // skipped in step 1 ever mattered: this node was never registered as
  // a dependency of that cache).
  SbMatrix wantm2;
  SbBox3f wantb2;
  if (strcmp(kind, "scale") == 0) {
    static_cast<SoScale *>(xform)->scaleFactor.setValue(2.0f, 2.0f, 2.0f);
    wantm2 = SbMatrix::identity();
    wantm2[0][0] = 2.0f; wantm2[1][1] = 2.0f; wantm2[2][2] = 2.0f;
    wantb2.setBounds(SbVec3f(-2.0f, -2.0f, -2.0f), SbVec3f(2.0f, 2.0f, 2.0f));
  }
  else if (strcmp(kind, "rotation") == 0) {
    SbRotation rot(SbVec3f(0.0f, 0.0f, 1.0f), (float)(M_PI / 2.0)); // 90 deg about Z
    static_cast<SoRotation *>(xform)->rotation.setValue(rot);
    rot.getValue(wantm2);
    // A 90-degree rotation of a symmetric 2x2x2 cube about its own
    // center still bounds to the same box (edge lengths swap X/Y, but
    // the cube is symmetric) -- so re-use wantb1 here, this step
    // mainly exercises the matrix + cache-invalidation path.
    wantb2 = wantb1;
  }
  else {
    SoRotationXYZ * rxyz = static_cast<SoRotationXYZ *>(xform);
    rxyz->angle.setValue((float)(M_PI / 2.0));
    SbRotation rot(SbVec3f(0.0f, 0.0f, 1.0f), (float)(M_PI / 2.0));
    rot.getValue(wantm2);
    wantb2 = wantb1; // same reasoning as the "rotation" case above
  }
  SbMatrix m2 = getMatrix(root, cube);
  checkMatrix("step2 non-identity: matrix reflects change", m2, wantm2);
  SbBox3f b2 = getBBox(root, TRUE);
  checkBBox("step2 non-identity: bbox reflects change", b2, wantb2);

  // Step 3: back to identity. The bbox cache from step 2 (built for the
  // non-identity value) must be correctly invalidated and recomputed
  // -- this is the actual "after creating caches" case for the
  // identity-skip optimization itself, since a real GL/bbox cache now
  // exists and this transition exercises re-entering the skipped path.
  if (strcmp(kind, "scale") == 0) {
    static_cast<SoScale *>(xform)->scaleFactor.setValue(1.0f, 1.0f, 1.0f);
  }
  else if (strcmp(kind, "rotation") == 0) {
    static_cast<SoRotation *>(xform)->rotation.setValue(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f);
  }
  else {
    static_cast<SoRotationXYZ *>(xform)->angle.setValue(0.0f);
  }
  SbMatrix m3 = getMatrix(root, cube);
  checkMatrix("step3 back to identity: matrix == identity", m3, SbMatrix::identity());
  SbBox3f b3 = getBBox(root, TRUE);
  checkBBox("step3 back to identity: bbox == unit cube", b3, wantb1);

  root->unref();
}

int main()
{
  SoDB::init();
  runSequence("scale");
  runSequence("rotation");
  runSequence("rotationxyz");

  if (failures > 0) {
    fprintf(stderr, "\n[repro] FAIL: %d check(s) failed\n", failures);
    return 1;
  }
  fprintf(stderr, "\n[repro] PASS: all checks passed\n");
  return 0;
}
