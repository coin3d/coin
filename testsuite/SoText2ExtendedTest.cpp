#include <Inventor/SoDB.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoTranslation.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <string>
#include <vector>

static int cases = 0, failures = 0;
static void check(const char *name, bool ok) {
  ++cases;
  if (!ok) {
    ++failures;
    std::fprintf(stderr, "FAIL %s\n", name);
  }
}
struct Scene {
  SoSeparator *root;
  SoCamera *camera;
  SoFont *font;
  SoText2 *text;
  SoTranslation *translation;
  SbViewportRegion viewport;
  Scene(int count, int lines = 1, float spacing = 1, bool ortho = false,
        int alignment = SoText2::LEFT)
      : viewport(640, 480) {
    root = new SoSeparator;
    root->ref();
    root->renderCaching = SoSeparator::OFF;
    camera = ortho ? static_cast<SoCamera *>(new SoOrthographicCamera)
                   : static_cast<SoCamera *>(new SoPerspectiveCamera);
    camera->position.setValue(0, 0, 5);
    camera->nearDistance = 1;
    camera->farDistance = 10;
    root->addChild(camera);
    font = new SoFont;
    font->name = "Times-Roman";
    font->size = 12;
    root->addChild(font);
    translation = new SoTranslation;
    root->addChild(translation);
    text = new SoText2;
    text->spacing = spacing;
    text->justification = alignment;
    text->string.setNum(lines);
    const std::string value(count, 'M');
    for (int i = 0; i < lines; ++i)
      text->string.set1Value(i, value.c_str());
    root->addChild(text);
  }
  ~Scene() { root->unref(); }
  SbBox3f bounds() {
    SoGetBoundingBoxAction action(viewport);
    action.apply(root);
    return action.getBoundingBox();
  }
  std::vector<unsigned char> render() {
    SoOffscreenRenderer renderer(viewport);
    renderer.setComponents(SoOffscreenRenderer::RGB);
    renderer.setBackgroundColor(SbColor(0, 0, 0));
    if (!renderer.render(root))
      return {};
    const auto *buffer = renderer.getBuffer();
    return std::vector<unsigned char>(buffer, buffer + 640 * 480 * 3);
  }
};
static double extent(const SbBox3f &box, int axis) {
  return box.isEmpty() ? 0 : double(box.getMax()[axis]) - box.getMin()[axis];
}
static bool finite(const SbBox3f &box) {
  if (box.isEmpty())
    return false;
  for (int axis = 0; axis < 3; ++axis)
    if (!std::isfinite(box.getMin()[axis]) ||
        !std::isfinite(box.getMax()[axis]))
      return false;
  return true;
}
static bool nonblack(const std::vector<unsigned char> &image) {
  for (auto pixel : image)
    if (pixel)
      return true;
  return false;
}
int main(int argc, char **argv) {
  const bool rendering = argc > 1 && std::strcmp(argv[1], "--render") == 0;
  if (rendering && !std::getenv("DISPLAY"))
    return 77;
  SoDB::init();
  if (!rendering) {
    for (int ortho = 0; ortho < 2; ++ortho)
      for (int alignment = SoText2::LEFT; alignment <= SoText2::CENTER;
           ++alignment) {
        Scene shortline(1000, 1, 1, ortho, alignment),
            longline(6000, 1, 1, ortho, alignment);
        const auto first = shortline.bounds(), last = longline.bounds();
        check("long line grows linearly",
              finite(last) &&
                  std::fabs(extent(last, 0) / extent(first, 0) - 6) < 0.02);
        check("long line alignment",
              alignment == SoText2::LEFT ? last.getMin()[0] >= -0.01f
              : alignment == SoText2::RIGHT
                  ? last.getMax()[0] <= 0.01f
                  : std::fabs(last.getCenter()[0]) < 0.01f);
      }
    Scene one(1), small(1, 1000), large(1, 4000), three(1, 3),
        spaced(1, 3, 10000);
    const double glyphheight = extent(one.bounds(), 1);
    const auto tallbox = large.bounds();
    check("many lines grow linearly",
          finite(tallbox) &&
              std::fabs((extent(tallbox, 1) - glyphheight) /
                            (extent(small.bounds(), 1) - glyphheight) -
                        3999.0 / 999) < 0.001);
    check("many lines below origin", tallbox.getCenter()[1] < 0);
    check("large spacing remains below origin",
          spaced.bounds().getCenter()[1] < 0);
    check("large spacing grows linearly",
          std::fabs((extent(spaced.bounds(), 1) - glyphheight) /
                        (extent(three.bounds(), 1) - glyphheight) -
                    10000) < 0.1);
    for (float z : {4.9999f, 5.f, 5.0001f, 6.f}) {
      Scene scene(12);
      scene.translation->translation.setValue(0, 0, z);
      check("eye plane finite bounds",
            finite(scene.bounds()) && extent(scene.bounds(), 0) > 0);
      scene.camera->viewAll(scene.root, scene.viewport);
      check("eye plane viewAll recovers",
            finite(scene.bounds()) &&
                scene.camera->position.getValue()[2] > z &&
                scene.camera->nearDistance.getValue() > 0);
    }
    for (float x : {1000.f, 1000000.f, 1000000000.f}) {
      Scene scene(12);
      scene.translation->translation.setValue(x, 0, 0);
      const auto box = scene.bounds();
      check("large coordinates retain footprint",
            finite(box) && extent(box, 0) > 0 && box.getMin()[0] <= x &&
                box.getMax()[0] >= x);
    }
    for (float spacing :
         {std::numeric_limits<float>::quiet_NaN(),
          std::numeric_limits<float>::infinity(), 1e30f, 1e8f}) {
      Scene scene(1, 3, spacing);
      check("invalid spacing rejected", scene.bounds().isEmpty());
      scene.text->spacing = 1;
      check("spacing cache recovers", finite(scene.bounds()));
    }
    for (float size : {0.f, -1.f, std::numeric_limits<float>::quiet_NaN(),
                       std::numeric_limits<float>::infinity()}) {
      Scene scene(1);
      scene.font->size = size;
      check("invalid font size rejected", scene.bounds().isEmpty());
      scene.font->size = 12;
      check("font cache recovers", finite(scene.bounds()));
    }
    Scene empty(0, 0);
    check("empty text has empty bounds", empty.bounds().isEmpty());
    Scene zero(12);
    zero.viewport.setWindowSize(0, 480);
    check("zero viewport rejected", zero.bounds().isEmpty());
  } else {
    for (int alignment = SoText2::LEFT; alignment <= SoText2::CENTER;
         ++alignment)
      for (float x : {0.f, -1.34f}) {
        Scene reference(200, 1, 1, true, alignment),
            wide(6000, 1, 1, true, alignment);
        reference.translation->translation.setValue(x, 0, 0);
        wide.translation->translation.setValue(x, 0, 0);
        const auto first = reference.render(), second = wide.render();
        check("wide text clipping matches reference",
              !first.empty() && first == second &&
                  nonblack(second) == (alignment != SoText2::RIGHT || x >= 0));
      }
    // Compare partially clipped glyphs to an independent pixel translation
    // of the same unclipped glyph, including source rows and columns.
    Scene glyph(1, 1, 1, true);
    const auto glyphimage = glyph.render();
    const int shifts[][2] = {{-326, 0}, {0, 238}, {0, -244}};
    for (const auto &delta : shifts) {
      Scene clipped(1, 1, 1, true);
      clipped.translation->translation.setValue(delta[0] / 240.0f,
                                                delta[1] / 240.0f, 0);
      const auto actual = clipped.render();
      std::vector<unsigned char> expected(640 * 480 * 3, 0);
      if (!glyphimage.empty()) {
        for (int y = 0; y < 480; ++y)
          for (int x = 0; x < 640; ++x) {
            const int dx = x + delta[0], dy = y + delta[1];
            if (dx >= 0 && dx < 640 && dy >= 0 && dy < 480) {
              for (int component = 0; component < 3; ++component)
                expected[(dy * 640 + dx) * 3 + component] =
                    glyphimage[(y * 640 + x) * 3 + component];
            }
          }
      }
      check("partial glyph clipping matches translated pixels",
            nonblack(actual) && actual == expected);
    }
    Scene reference(1, 100, 1, true), tall(1, 6000, 1, true);
    const auto first = reference.render(), second = tall.render();
    check("tall text clipping matches reference",
          nonblack(second) && first == second);
    Scene one(6000, 1, 1, true), huge(6000, 3, 10000, true);
    const auto single = one.render(), multiple = huge.render();
    check("huge full extent uses visible buffer",
          nonblack(multiple) && single == multiple);
    for (float z : {5.f, 6.f}) {
      Scene scene(12);
      scene.translation->translation.setValue(0, 0, z);
      scene.camera->viewAll(scene.root, scene.viewport);
      check("eye plane viewAll draws text", nonblack(scene.render()));
    }
    for (float size : {0.f, -1.f, std::numeric_limits<float>::quiet_NaN(),
                       std::numeric_limits<float>::infinity()}) {
      Scene invalid(1);
      invalid.font->size = size;
      const auto image = invalid.render();
      check("invalid size renders empty safely",
            !image.empty() && !nonblack(image));
    }
    for (float spacing : {std::numeric_limits<float>::quiet_NaN(),
                          std::numeric_limits<float>::infinity(), 1e8f}) {
      Scene invalid(1, 3, spacing);
      const auto image = invalid.render();
      check("invalid layout renders empty safely",
            !image.empty() && !nonblack(image));
    }
    Scene distant(12);
    distant.translation->translation.setValue(1e9f, 0, 0);
    distant.camera->viewAll(distant.root, distant.viewport);
    check("large coordinate viewAll draws text", nonblack(distant.render()));
  }
  SoDB::finish();
  std::printf("SoText2 extended %s: %d cases, %d failures\n",
              rendering ? "rendering" : "bounds", cases, failures);
  return failures ? 1 : 0;
}
