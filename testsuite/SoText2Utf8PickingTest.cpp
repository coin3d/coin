#include <Inventor/SoDB.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/details/SoTextDetail.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoText2.h>
#include <cstdio>
#include <string>

int main() {
  SoDB::init();
  int cases = 0, failures = 0;
  // Measure the advance as the width difference between two and one equal
  // glyphs. This remains valid for padded fallback bitmaps and overhangs.
  const char * characters[] = {"M", "\xc3\xa9", "\xe4\xb8\x96", "\xf0\x9f\x98\x80"};
  for (int ortho = 0; ortho < 2; ++ortho) {
    auto *root = new SoSeparator;
    root->ref();
    SoCamera *camera = ortho ? static_cast<SoCamera *>(new SoOrthographicCamera)
                            : static_cast<SoCamera *>(new SoPerspectiveCamera);
    camera->position.setValue(0, 0, 5);
    camera->nearDistance = 1;
    camera->farDistance = 10;
    root->addChild(camera);
    auto *font = new SoFont;
    font->name = "DejaVu Sans";
    font->size = 24;
    root->addChild(font);
    auto *text = new SoText2;
    root->addChild(text);
    const SbViewportRegion vp(640, 480);
    // Reuse the node while changing strings/alignment to exercise cache rebuilds.
    for (int align = SoText2::LEFT; align <= SoText2::LEFT; ++align) {
      text->justification = align;
      for (const char *character : characters) {
        SoGetBoundingBoxAction measure(vp);
        text->string = character;
        measure.apply(root);
        const float singlewidth = measure.getBoundingBox().getSize()[0];
        const std::string twice = std::string(character) + character;
        text->string = twice.c_str();
        measure.apply(root);
        const float advance = measure.getBoundingBox().getSize()[0] - singlewidth;
        for (int count : {1, 3}) {
          std::string value;
          for (int i = 0; i < count; ++i) value += character;
          text->string = value.c_str();
          SoGetBoundingBoxAction bounds(vp);
          bounds.apply(root);
          const SbBox3f box = bounds.getBoundingBox();
          for (int i = 0; i < count; ++i) {
            ++cases;
            if (box.isEmpty()) { ++failures; continue; }
            const SbVec3f lo = box.getMin(), hi = box.getMax();
            const float x = lo[0] + advance * (i+0.5f);
            SoRayPickAction pick(vp);
            pick.setRay(SbVec3f(x, (lo[1]+hi[1])*0.5f, 3), SbVec3f(0,0,-1), 0, 10);
            pick.apply(root);
            const SoPickedPoint *point = pick.getPickedPoint();
            const SoTextDetail *detail = point ? static_cast<const SoTextDetail *>(point->getDetail(text)) : nullptr;
            if (!detail || detail->getCharacterIndex() != i || detail->getStringIndex() != 0) {
              ++failures;
              std::fprintf(stderr, "FAIL ortho=%d align=%d bytes=%zu count=%d expected=%d actual=%d\n",
                           ortho, align, value.size(), count, i, detail ? detail->getCharacterIndex() : -1);
            }
          }
        }
      }
    }
    root->unref();
  }
  SoDB::finish();
  std::printf("SoText2 UTF-8 picking: %d cases, %d failures\n", cases, failures);
  return failures ? 1 : 0;
}
