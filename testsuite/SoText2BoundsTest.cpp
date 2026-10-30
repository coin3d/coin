#include <Inventor/SoDB.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoTranslation.h>
#include <cmath>
#include <cstdio>
int main() {
  SoDB::init();
  int failures = 0, cases = 0;
  const SbVec3f anchors[] = {SbVec3f(.25f, 0, .25f), SbVec3f(5, 0, 0),
                             SbVec3f(100, 0, 0),     SbVec3f(-100, 0, 0),
                             SbVec3f(0, 100, 0),     SbVec3f(0, -100, 0)};
  for (int orthographic = 0; orthographic < 2; ++orthographic)
    for (int alignment = SoText2::LEFT; alignment <= SoText2::CENTER; ++alignment)
      for (const auto &anchor : anchors) {
        auto *root = new SoSeparator;
        root->ref();
        SoCamera *camera =
            orthographic ? static_cast<SoCamera *>(new SoOrthographicCamera)
                         : static_cast<SoCamera *>(new SoPerspectiveCamera);
        root->addChild(camera);
        auto *font = new SoFont;
        font->size = 24;
        root->addChild(font);
        auto *transform = new SoTranslation;
        transform->translation = anchor;
        root->addChild(transform);
        auto *text = new SoText2;
        text->string = "Hello World!";
        text->justification = alignment;
        root->addChild(text);
        SbViewportRegion vp(640, 480);
        SoGetBoundingBoxAction bounds(vp);
        bounds.apply(root);
        const SbBox3f first = bounds.getBoundingBox();
        bool ok = !first.isEmpty();
        if (ok) {
          const SbVec3f center = first.getCenter();
          for (int j = 0; j < 3; ++j)
            ok = ok && std::isfinite(center[j]) &&
                 std::fabs(center[j] - anchor[j]) < 4;
        }
        bounds.apply(root);
        ok = ok && bounds.getBoundingBox() == first;
        camera->viewAll(root, vp);
        bounds.apply(root);
        ok = ok && !bounds.getBoundingBox().isEmpty();
        if (!ok) {
          ++failures;
          std::fprintf(stderr, "FAIL ortho=%d alignment=%d anchor=(%g,%g,%g)\n",
                       orthographic, alignment, anchor[0], anchor[1],
                       anchor[2]);
        }
        ++cases;
        root->unref();
      }
  // Keep picking visible text working while retaining offscreen rejection.
  for (int offscreen = 0; offscreen < 2; ++offscreen) {
    auto *root = new SoSeparator;
    root->ref();
    auto *camera = new SoOrthographicCamera;
    camera->position.setValue(0, 0, 5);
    camera->nearDistance = 1;
    camera->farDistance = 10;
    root->addChild(camera);
    auto *transform = new SoTranslation;
    transform->translation.setValue(offscreen ? 100 : 0, 0, 0);
    root->addChild(transform);
    auto *text = new SoText2;
    text->string = "Hello";
    root->addChild(text);
    SbViewportRegion vp(640, 480);
    SoGetBoundingBoxAction bounds(vp);
    bounds.apply(root);
    const SbVec3f center = offscreen ? SbVec3f(100.1f, 0, 0)
                                    : bounds.getBoundingBox().getCenter();
    SoRayPickAction pick(vp);
    pick.setRay(center + SbVec3f(0, 0, 3), SbVec3f(0, 0, -1), 0, 10);
    pick.apply(root);
    const bool hit = pick.getPickedPoint() != nullptr;
    if (hit == bool(offscreen)) {
      ++failures;
      std::fprintf(stderr, "FAIL pick offscreen=%d hit=%d\n", offscreen, hit);
    }
    ++cases;
    root->unref();
  }
  SoDB::finish();
  std::printf("SoText2: %d cases, %d failures\n", cases, failures);
  return failures ? 1 : 0;
}
