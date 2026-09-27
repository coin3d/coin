/**************************************************************************\
* Copyright (c) Kongsberg Oil & Gas Technologies AS
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are
* met:
*
* Redistributions of source code must retain the above copyright notice,
* this list of conditions and the following disclaimer.
*
* Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the distribution.
*
* Neither the name of the copyright holder nor the names of its
* contributors may be used to endorse or promote products derived from
* this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
* HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

// Regression coverage for issue #374. Uses the public API; no GL context.
#include "CoinTest.h"
#include <Inventor/SoDB.h>
#include <Inventor/SoPath.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoLevelOfDetail.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTranslation.h>

namespace {
struct Scene {
  SoGroup * root;
  SoLevelOfDetail * lod;
  SoCube * farcube;
  int traversals;
  SoAction::PathCode observed;

  static void count(void * data, SoAction * action) {
    if (action->isOfType(SoGetBoundingBoxAction::getClassTypeId())) {
      Scene * scene = static_cast<Scene *>(data);
      ++scene->traversals;
      scene->observed = action->getCurPathCode();
    }
  }

  Scene() : root(new SoGroup), lod(new SoLevelOfDetail),
            traversals(0), observed(SoAction::NO_PATH) {
    root->ref();
    root->addChild(lod);
    SoCallback * counter = new SoCallback;
    counter->setCallback(count, this);
    lod->addChild(counter);
    for (int i = 0; i < 2; ++i) {
      SoSeparator * branch = new SoSeparator;
      // The traversal counter must measure the LOD cache, not child caches.
      branch->boundingBoxCaching = SoSeparator::OFF;
      SoTranslation * translation = new SoTranslation;
      translation->translation = SbVec3f(i == 0 ? -5 : 10, 0, 0);
      branch->addChild(translation);
      SoCube * cube = new SoCube;
      cube->width = i == 0 ? 2 : 6;
      cube->height = i == 0 ? 4 : 2;
      cube->depth = i == 0 ? 6 : 2;
      branch->addChild(cube);
      lod->addChild(branch);
      if (i == 1) farcube = cube;
    }
    root->addChild(new SoCube);
  }
  ~Scene() { root->unref(); }
};

void checkBox(SoGetBoundingBoxAction & action,
              const SbVec3f & minimum, const SbVec3f & maximum,
              const SbVec3f & center) {
  BOOST_CHECK(!action.getBoundingBox().isEmpty());
  BOOST_CHECK((action.getBoundingBox().getMin() - minimum).length() < 1e-5f);
  BOOST_CHECK((action.getBoundingBox().getMax() - maximum).length() < 1e-5f);
  BOOST_CHECK(action.isCenterSet());
  BOOST_CHECK((action.getCenter() - center).length() < 1e-5f);
}

void checkFull(SoGetBoundingBoxAction & action) {
  checkBox(action, SbVec3f(-6, -2, -3), SbVec3f(13, 2, 3),
           SbVec3f(2.5f, 0, 0));
}
}

BOOST_AUTO_TEST_CASE(lod_bbox_full_traversal_cache_and_invalidation) {
  Scene scene;
  SoGetBoundingBoxAction action(SbViewportRegion(100, 100));
  action.apply(scene.lod);
  checkFull(action);
  BOOST_CHECK_EQUAL(scene.traversals, 1);
  BOOST_CHECK_EQUAL(scene.observed, SoAction::NO_PATH);
  action.apply(scene.lod);
  checkFull(action);
  BOOST_CHECK_EQUAL(scene.traversals, 1);

  // Changing geometry must invalidate the cache and change its extents.
  scene.farcube->width = 10;
  for (int repeat = 0; repeat < 2; ++repeat) {
    action.apply(scene.lod);
    checkBox(action, SbVec3f(-6, -2, -3), SbVec3f(15, 2, 3),
             SbVec3f(2.5f, 0, 0));
    BOOST_CHECK_EQUAL(scene.traversals, 2);
  }
}

BOOST_AUTO_TEST_CASE(lod_bbox_below_path_traverses_all_children_and_caches) {
  Scene scene;
  SoPath * path = new SoPath(scene.root);
  path->ref();
  path->append(0); // The path ends at LOD: its children are BELOW_PATH.
  SoGetBoundingBoxAction action(SbViewportRegion(100, 100));
  for (int repeat = 0; repeat < 2; ++repeat) {
    action.apply(path);
    checkFull(action);
    BOOST_CHECK_EQUAL(scene.traversals, 1);
    BOOST_CHECK_EQUAL(scene.observed, SoAction::BELOW_PATH);
  }
  path->unref();
}

BOOST_AUTO_TEST_CASE(lod_bbox_in_path_continues_without_using_or_poisoning_cache) {
  for (int warm = 0; warm < 2; ++warm) {
    Scene scene;
    SoGetBoundingBoxAction action(SbViewportRegion(100, 100));
    if (warm) {
      action.apply(scene.lod);
      checkFull(action);
    }
    SoPath * path = new SoPath(scene.root);
    path->ref();
    path->append(0); // LOD is IN_PATH.
    path->append(1); // Only the first geometry branch is on the path.
    path->append(1); // Cube after its translation.
    for (int repeat = 0; repeat < 2; ++repeat) {
      action.apply(path);
      checkBox(action, SbVec3f(-6, -2, -3), SbVec3f(-4, 2, 3),
               SbVec3f(-5, 0, 0));
      BOOST_CHECK_EQUAL(scene.traversals, warm + repeat + 1);
    }
    path->unref();
    // A partial traversal must neither create nor overwrite a full cache.
    for (int repeat = 0; repeat < 2; ++repeat) {
      action.apply(scene.lod);
      checkFull(action);
      BOOST_CHECK_EQUAL(scene.traversals, 3);
    }
  }
}

BOOST_AUTO_TEST_CASE(lod_bbox_off_path_continues_without_using_or_poisoning_cache) {
  for (int warm = 0; warm < 2; ++warm) {
    Scene scene;
    SoGetBoundingBoxAction action(SbViewportRegion(100, 100));
    if (warm) {
      action.apply(scene.lod);
      checkFull(action);
    }
    SoPath * path = new SoPath(scene.root);
    path->ref();
    path->append(1); // Earlier sibling LOD is OFF_PATH.
    for (int repeat = 0; repeat < 2; ++repeat) {
      action.apply(path);
      checkBox(action, SbVec3f(-1, -1, -1), SbVec3f(1, 1, 1),
               SbVec3f(0, 0, 0));
      BOOST_CHECK_EQUAL(scene.traversals, warm + repeat + 1);
      BOOST_CHECK_EQUAL(scene.observed, SoAction::OFF_PATH);
    }
    path->unref();
    for (int repeat = 0; repeat < 2; ++repeat) {
      action.apply(scene.lod);
      checkFull(action);
      BOOST_CHECK_EQUAL(scene.traversals, 3);
    }
  }
}

BOOST_AUTO_TEST_CASE(lod_bbox_camera_space_bypasses_cache) {
  Scene scene;
  SoGetBoundingBoxAction action(SbViewportRegion(100, 100));
  action.apply(scene.lod);
  checkFull(action);
  action.setInCameraSpace(TRUE);
  for (int repeat = 0; repeat < 2; ++repeat) {
    action.apply(scene.lod);
    checkFull(action);
    BOOST_CHECK_EQUAL(scene.traversals, repeat + 2);
  }
  action.setInCameraSpace(FALSE);
  action.apply(scene.lod);
  checkFull(action);
  BOOST_CHECK_EQUAL(scene.traversals, 3);
}

int main() {
  SoDB::init();
  const int result = CoinTest::run_all();
  SoDB::finish();
  return result;
}
