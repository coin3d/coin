#include <Inventor/SoDB.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/SbBox3f.h>
#include <Inventor/SbString.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/VRMLnodes/SoVRMLFontStyle.h>
#include <Inventor/VRMLnodes/SoVRMLText.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/details/SoTextDetail.h>
#include <Inventor/nodes/SoSeparator.h>

#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>

namespace {

struct Triangle {
  SbVec3f point[3];
  int stringindex;
  int characterindex;
};

struct Capture {
  std::vector<Triangle> triangles;
};

int failures = 0;

void check(const bool condition, const char * expression, const int line)
{
  if (!condition) {
    std::fprintf(stderr, "line %d: check failed: %s\n", line, expression);
    ++failures;
  }
}

#define CHECK(expr) check((expr), #expr, __LINE__)

bool near(const float lhs, const float rhs, const float epsilon = 2.0e-3f)
{
  return std::fabs(lhs - rhs) <= epsilon;
}

bool contains(const SbBox3f & box, const SbVec3f & point,
              const float epsilon = 2.0e-3f)
{
  if (box.isEmpty()) return false;
  const SbVec3f & minimum = box.getMin();
  const SbVec3f & maximum = box.getMax();
  for (int axis = 0; axis < 3; ++axis) {
    if (point[axis] < minimum[axis] - epsilon ||
        point[axis] > maximum[axis] + epsilon) return false;
  }
  return true;
}

void triangleCB(void * data, SoCallbackAction *,
                const SoPrimitiveVertex * a,
                const SoPrimitiveVertex * b,
                const SoPrimitiveVertex * c)
{
  Capture * capture = static_cast<Capture *>(data);
  const SoPrimitiveVertex * vertices[3] = { a, b, c };
  Triangle triangle;
  triangle.stringindex = -1;
  triangle.characterindex = -1;
  for (int i = 0; i < 3; ++i) {
    triangle.point[i] = vertices[i]->getPoint();
    const SoDetail * detail = vertices[i]->getDetail();
    if (detail && detail->isOfType(SoTextDetail::getClassTypeId())) {
      const SoTextDetail * text = static_cast<const SoTextDetail *>(detail);
      if (i == 0) {
        triangle.stringindex = text->getStringIndex();
        triangle.characterindex = text->getCharacterIndex();
      }
      else {
        CHECK(triangle.stringindex == text->getStringIndex());
        CHECK(triangle.characterindex == text->getCharacterIndex());
      }
    }
  }
  capture->triangles.push_back(triangle);
}

Capture capture(SoVRMLText * text)
{
  Capture result;
  SoCallbackAction action;
  action.addTriangleCallback(SoVRMLText::getClassTypeId(), triangleCB, &result);
  action.apply(text);
  return result;
}

SbBox3f bounds(const Capture & capture, const int stringindex = -1,
               const int characterindex = -1)
{
  SbBox3f box;
  box.makeEmpty();
  for (size_t i = 0; i < capture.triangles.size(); ++i) {
    const Triangle & triangle = capture.triangles[i];
    if (stringindex >= 0 && triangle.stringindex != stringindex) continue;
    if (characterindex >= 0 && triangle.characterindex != characterindex) continue;
    for (int vertex = 0; vertex < 3; ++vertex) box.extendBy(triangle.point[vertex]);
  }
  return box;
}

SbVec3f center(const Capture & capture, const int stringindex,
               const int characterindex = -1)
{
  const SbBox3f box = bounds(capture, stringindex, characterindex);
  CHECK(!box.isEmpty());
  return box.getCenter();
}

SbBox3f boundingBox(SoNode * node)
{
  SoGetBoundingBoxAction action(SbViewportRegion(128, 128));
  action.apply(node);
  return action.getBoundingBox();
}

int triangleCount(SoNode * node)
{
  SoGetPrimitiveCountAction action;
  action.setCount3DTextAsTriangles(TRUE);
  action.apply(node);
  return action.getTriangleCount();
}

SoVRMLText * makeText(const std::vector<SbString> & strings,
                      const SbBool horizontal,
                      const SbBool lefttoright,
                      const SbBool toptobottom,
                      const char * major = "BEGIN",
                      const char * minor = "FIRST",
                      const float size = 1.0f,
                      const float spacing = 1.0f)
{
  SoVRMLText * text = new SoVRMLText;
  text->ref();
  if (!strings.empty()) {
    text->string.setValues(0, static_cast<int>(strings.size()), &strings[0]);
  }

  SoVRMLFontStyle * style = new SoVRMLFontStyle;
  style->horizontal = horizontal;
  style->leftToRight = lefttoright;
  style->topToBottom = toptobottom;
  style->size = size;
  style->spacing = spacing;
  style->justify.set1Value(0, major);
  style->justify.set1Value(1, minor);
  text->fontStyle = style;
  return text;
}

Capture configuredCapture(const std::vector<SbString> & strings,
                          const SbBool horizontal,
                          const SbBool lefttoright,
                          const SbBool toptobottom,
                          const char * major = "BEGIN",
                          const char * minor = "FIRST",
                          const float size = 1.0f,
                          const float spacing = 1.0f)
{
  SoVRMLText * text = makeText(strings, horizontal, lefttoright, toptobottom,
                              major, minor, size, spacing);
  Capture result = capture(text);
  text->unref();
  return result;
}

void testDirections(void)
{
  std::vector<SbString> lines;
  lines.push_back(SbString("A"));
  lines.push_back(SbString("A"));

  Capture horizontaldown = configuredCapture(lines, TRUE, TRUE, TRUE);
  Capture horizontalup = configuredCapture(lines, TRUE, TRUE, FALSE);
  CHECK(center(horizontaldown, 1)[1] < center(horizontaldown, 0)[1]);
  CHECK(center(horizontalup, 1)[1] > center(horizontalup, 0)[1]);

  Capture verticalright = configuredCapture(lines, FALSE, TRUE, TRUE);
  Capture verticalleft = configuredCapture(lines, FALSE, FALSE, TRUE);
  CHECK(center(verticalright, 1)[0] > center(verticalright, 0)[0]);
  CHECK(center(verticalleft, 1)[0] < center(verticalleft, 0)[0]);

  Capture rtl = configuredCapture(std::vector<SbString>(1, SbString("AB")),
                                  TRUE, FALSE, TRUE);
  CHECK(center(rtl, 0, 0)[0] > center(rtl, 0, 1)[0]);
}

void checkMinorTranslations(const SbBool horizontal,
                            const SbBool lefttoright,
                            const SbBool toptobottom,
                            const int axis)
{
  std::vector<SbString> lines;
  lines.push_back(SbString("g"));
  lines.push_back(SbString("A"));
  Capture begin = configuredCapture(lines, horizontal, lefttoright, toptobottom,
                                    "BEGIN", "BEGIN", 1.0f, 0.0f);
  Capture middle = configuredCapture(lines, horizontal, lefttoright, toptobottom,
                                     "BEGIN", "MIDDLE", 1.0f, 0.0f);
  Capture end = configuredCapture(lines, horizontal, lefttoright, toptobottom,
                                  "BEGIN", "END", 1.0f, 0.0f);

  const SbBox3f beginbounds = bounds(begin, 0);
  const SbBox3f middlebounds = bounds(middle);
  const SbBox3f endbounds = bounds(end, 1);
  const float direction = horizontal ? (toptobottom ? -1.0f : 1.0f) :
                                       (lefttoright ? 1.0f : -1.0f);
  CHECK(near(direction > 0.0f ? beginbounds.getMin()[axis] :
                                beginbounds.getMax()[axis], 0.0f));
  CHECK(near(middlebounds.getCenter()[axis], 0.0f));
  CHECK(near(direction > 0.0f ? endbounds.getMax()[axis] :
                                endbounds.getMin()[axis], 0.0f));

  const float beginadvance = center(begin, 1)[axis] - center(begin, 0)[axis];
  const float middleadvance = center(middle, 1)[axis] - center(middle, 0)[axis];
  const float endadvance = center(end, 1)[axis] - center(end, 0)[axis];
  CHECK(near(beginadvance, middleadvance));
  CHECK(near(beginadvance, endadvance));
}

void testMinorJustification(void)
{
  checkMinorTranslations(TRUE, TRUE, TRUE, 1);
  checkMinorTranslations(TRUE, TRUE, FALSE, 1);
  checkMinorTranslations(FALSE, TRUE, TRUE, 0);
  checkMinorTranslations(FALSE, FALSE, TRUE, 0);
}

void checkMajorTranslations(const SbBool horizontal,
                            const SbBool lefttoright,
                            const SbBool toptobottom,
                            const int axis)
{
  std::vector<SbString> strings(1, SbString("AV"));
  Capture begin = configuredCapture(strings, horizontal, lefttoright, toptobottom,
                                    "BEGIN", "FIRST");
  Capture middle = configuredCapture(strings, horizontal, lefttoright, toptobottom,
                                     "MIDDLE", "FIRST");
  Capture end = configuredCapture(strings, horizontal, lefttoright, toptobottom,
                                  "END", "FIRST");
  const float middleshift = center(middle, 0)[axis] - center(begin, 0)[axis];
  const float endshift = center(end, 0)[axis] - center(begin, 0)[axis];
  CHECK(std::fabs(endshift) > 0.1f);
  CHECK(near(endshift, 2.0f * middleshift, 1.0e-2f));
}

void testMajorJustification(void)
{
  checkMajorTranslations(TRUE, TRUE, TRUE, 0);
  checkMajorTranslations(TRUE, FALSE, TRUE, 0);
  checkMajorTranslations(FALSE, TRUE, TRUE, 1);
  checkMajorTranslations(FALSE, TRUE, FALSE, 1);
}

void testUtf8Details(void)
{
  SoVRMLText * text = makeText(
    std::vector<SbString>(1, SbString("A\xc2\xa0" "B")), TRUE, TRUE, TRUE);
  Capture result = capture(text);
  CHECK(!bounds(result, 0, 0).isEmpty());
  CHECK(bounds(result, 0, 1).isEmpty());
  CHECK(!bounds(result, 0, 2).isEmpty());
  for (size_t i = 0; i < result.triangles.size(); ++i) {
    CHECK(result.triangles[i].stringindex == 0);
    CHECK(result.triangles[i].characterindex == 0 ||
          result.triangles[i].characterindex == 2);
  }
  CHECK(triangleCount(text) == static_cast<int>(result.triangles.size()));
  text->unref();
}

float width(const Capture & capture)
{
  const SbBox3f box = bounds(capture);
  CHECK(!box.isEmpty());
  return box.getMax()[0] - box.getMin()[0];
}

void testLengthMaxExtentAndSize(void)
{
  std::vector<SbString> strings(1, SbString("AVAV"));
  SoVRMLText * sizeone = makeText(strings, TRUE, TRUE, TRUE,
                                  "BEGIN", "FIRST", 1.0f);
  sizeone->length.set1Value(0, 4.0f);
  const float widthone = width(capture(sizeone));

  SoVRMLText * sizetwo = makeText(strings, TRUE, TRUE, TRUE,
                                  "BEGIN", "FIRST", 2.0f);
  sizetwo->length.set1Value(0, 4.0f);
  const float widthtwo = width(capture(sizetwo));
  CHECK(near(widthone, widthtwo, 2.0e-2f));

  SoVRMLText * limited = makeText(strings, TRUE, TRUE, TRUE,
                                  "BEGIN", "FIRST", 2.0f);
  limited->maxExtent = 1.5f;
  const float limitedwidth = width(capture(limited));
  CHECK(limitedwidth <= 1.5f + 2.0e-2f);

  SoVRMLText * both = makeText(strings, TRUE, FALSE, TRUE,
                               "END", "FIRST", 2.0f);
  both->length.set1Value(0, 4.0f);
  both->maxExtent = 2.0f;
  CHECK(width(capture(both)) <= 2.0f + 2.0e-2f);

  sizeone->unref();
  sizetwo->unref();
  limited->unref();
  both->unref();
}

void testSpacingScalesOnce(void)
{
  std::vector<SbString> strings(2, SbString("A"));
  Capture unit = configuredCapture(strings, TRUE, TRUE, TRUE,
                                   "BEGIN", "FIRST", 2.0f, 1.0f);
  Capture spaced = configuredCapture(strings, TRUE, TRUE, TRUE,
                                     "BEGIN", "FIRST", 2.0f, 1.75f);
  const float unitadvance = std::fabs(center(unit, 1)[1] - center(unit, 0)[1]);
  const float spacedadvance = std::fabs(center(spaced, 1)[1] - center(spaced, 0)[1]);
  CHECK(near(spacedadvance, unitadvance * 1.75f, 2.0e-2f));
}

void testBoundingBoxAndCachedRayPick(void)
{
  std::vector<SbString> strings;
  strings.push_back(SbString("AV"));
  strings.push_back(SbString("A\xc3\xa9"));
  SoVRMLText * text = makeText(strings, FALSE, FALSE, FALSE,
                               "END", "MIDDLE", 1.5f, 1.25f);
  text->length.set1Value(0, 3.0f);
  text->length.set1Value(1, 5.0f);
  text->maxExtent = 4.0f;
  Capture result = capture(text);
  CHECK(!result.triangles.empty());

  SoSeparator * root = new SoSeparator;
  root->ref();
  root->addChild(text);
  text->unref();
  const SbBox3f box = boundingBox(root);
  CHECK(!box.isEmpty());
  for (size_t i = 0; i < result.triangles.size(); ++i) {
    for (int vertex = 0; vertex < 3; ++vertex) {
      CHECK(contains(box, result.triangles[i].point[vertex]));
    }
  }

  const Triangle & triangle = result.triangles[result.triangles.size() / 2];
  const SbVec3f target = (triangle.point[0] + triangle.point[1] + triangle.point[2]) / 3.0f;
  SoRayPickAction pick(SbViewportRegion(128, 128));
  pick.setRay(target + SbVec3f(0, 0, 10), SbVec3f(0, 0, -1));
  pick.apply(root);
  CHECK(pick.getPickedPoint() != NULL);
  root->unref();
}

void testEmptyStrings(void)
{
  SoVRMLText * empty = makeText(
    std::vector<SbString>(1, SbString("")), TRUE, TRUE, TRUE);
  Capture emptycapture = capture(empty);
  CHECK(emptycapture.triangles.empty());
  CHECK(boundingBox(empty).isEmpty());
  CHECK(triangleCount(empty) == 0);
  empty->unref();

  std::vector<SbString> mixed;
  mixed.push_back(SbString("A"));
  mixed.push_back(SbString(""));
  mixed.push_back(SbString("A"));
  Capture withgap = configuredCapture(mixed, TRUE, TRUE, TRUE,
                                      "BEGIN", "MIDDLE", 1.0f, 1.5f);
  CHECK(!bounds(withgap, 0).isEmpty());
  CHECK(bounds(withgap, 1).isEmpty());
  CHECK(!bounds(withgap, 2).isEmpty());

  std::vector<SbString> adjacent(2, SbString("A"));
  Capture withoutgap = configuredCapture(adjacent, TRUE, TRUE, TRUE,
                                         "BEGIN", "MIDDLE", 1.0f, 1.5f);
  const float gapadvance = center(withgap, 2)[1] - center(withgap, 0)[1];
  const float adjacentadvance = center(withoutgap, 1)[1] - center(withoutgap, 0)[1];
  CHECK(near(gapadvance, 2.0f * adjacentadvance, 2.0e-2f));
}

} // namespace

int main(int argc, char ** argv)
{
  SoDB::init();
  SoInteraction::init();
  if (argc != 2) {
    std::fprintf(stderr, "usage: %s CASE\n", argv[0]);
    return 2;
  }

  if (std::strcmp(argv[1], "directions") == 0) testDirections();
  else if (std::strcmp(argv[1], "minor-justify") == 0) testMinorJustification();
  else if (std::strcmp(argv[1], "major-justify") == 0) testMajorJustification();
  else if (std::strcmp(argv[1], "utf8-details") == 0) testUtf8Details();
  else if (std::strcmp(argv[1], "length-maxextent-size") == 0) testLengthMaxExtentAndSize();
  else if (std::strcmp(argv[1], "spacing") == 0) testSpacingScalesOnce();
  else if (std::strcmp(argv[1], "bbox-raypick-cache") == 0) testBoundingBoxAndCachedRayPick();
  else if (std::strcmp(argv[1], "empty-strings") == 0) testEmptyStrings();
  else {
    std::fprintf(stderr, "unknown case: %s\n", argv[1]);
    return 2;
  }

  SoDB::finish();
  return failures == 0 ? 0 : 1;
}
