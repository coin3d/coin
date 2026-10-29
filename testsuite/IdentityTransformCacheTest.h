#ifndef COIN_IDENTITY_TRANSFORM_CACHE_TEST_H
#define COIN_IDENTITY_TRANSFORM_CACHE_TEST_H

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoRotationXYZ.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/SbViewportRegion.h>

// Shared by CoinTests and the standalone driver. No GL context is needed;
// these checks cover matrix traversal and bounding-box caches, not GL caches.
namespace IdentityTransformCacheTest {
typedef void Check(bool, const char *);

inline void countBBox(void * data, SoAction * action)
{
  if (action->isOfType(SoGetBoundingBoxAction::getClassTypeId()))
    ++*static_cast<int *>(data);
}

inline bool close(const SbVec3f & a, const SbVec3f & b)
{
  return (a - b).length() < 1.0e-5f;
}

inline void run(int kind, Check * check)
{
  SoSeparator * root = new SoSeparator;
  root->ref();
  // Set once: assigning this field again would itself invalidate the cache.
  root->boundingBoxCaching = SoSeparator::ON;
  SoNode * transform;
  if (kind == 0) transform = new SoScale;
  else if (kind == 1) transform = new SoRotation;
  else transform = new SoRotationXYZ;
  root->addChild(transform);

  int traversals = 0;
  SoCallback * counter = new SoCallback;
  counter->setCallback(countBBox, &traversals);
  root->addChild(counter);
  SoCube * cube = new SoCube;
  cube->width = 2.0f;
  cube->height = 4.0f;
  cube->depth = 6.0f;
  root->addChild(cube);

  // Each changed value must rebuild the cache, then an unchanged repeat
  // must reuse it. A 2x4x6 box distinguishes a stale Z-rotation result.
  for (int phase = 0; phase < 3; ++phase) {
    const bool changed = phase == 1;
    const float angle = changed ? 1.57079632679489661923f : 0.0f;
    if (kind == 0)
      static_cast<SoScale *>(transform)->scaleFactor =
        changed ? SbVec3f(2, 2, 2) : SbVec3f(1, 1, 1);
    else if (kind == 1)
      static_cast<SoRotation *>(transform)->rotation =
        SbRotation(SbVec3f(0, 0, 1), angle);
    else {
      SoRotationXYZ * rotation = static_cast<SoRotationXYZ *>(transform);
      rotation->axis = SoRotationXYZ::Z;
      rotation->angle = angle;
    }

    SbVec3f extent(1, 2, 3);
    SbMatrix expected = SbMatrix::identity();
    if (changed && kind == 0) {
      extent = SbVec3f(2, 4, 6);
      expected[0][0] = expected[1][1] = expected[2][2] = 2.0f;
    }
    else if (changed) {
      extent = SbVec3f(2, 1, 3);
      // Independently known row-vector matrix for a quarter-turn about Z.
      expected[0][0] = expected[1][1] = 0.0f;
      expected[0][1] = 1.0f;
      expected[1][0] = -1.0f;
    }

    SoGetMatrixAction matrixAction(SbViewportRegion(100, 100));
    matrixAction.apply(transform);
    check(matrixAction.getMatrix().equals(expected, 1.0e-5f),
          "matrix matches independent expected transform");

    for (int repeat = 0; repeat < 2; ++repeat) {
      SoGetBoundingBoxAction bboxAction(SbViewportRegion(100, 100));
      bboxAction.apply(root);
      const SbBox3f & box = bboxAction.getBoundingBox();
      check(close(box.getMin(), -extent) && close(box.getMax(), extent),
            "bounding box matches independent extents");
      check(traversals == phase + 1,
            repeat == 0 ? "mutation rebuilds bounding-box cache" :
                          "unchanged traversal reuses bounding-box cache");
    }
  }
  root->unref();
}
}
#endif
