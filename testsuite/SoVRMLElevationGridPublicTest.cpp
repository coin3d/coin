#include <Inventor/SoDB.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/SbBox3f.h>
#include <Inventor/SbColor.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/VRMLnodes/SoVRMLColor.h>
#include <Inventor/VRMLnodes/SoVRMLElevationGrid.h>
#include <Inventor/VRMLnodes/SoVRMLNormal.h>
#include <Inventor/VRMLnodes/SoVRMLTextureCoordinate.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/details/SoFaceDetail.h>
#include <Inventor/details/SoPointDetail.h>
#include <Inventor/nodes/SoSeparator.h>

#include <climits>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>

namespace {

struct VertexRecord {
  SbVec3f point;
  int coordinate;
  int material;
  int normal;
  int texture;
  int face;
  SbVec3f normalvalue;
};

struct TriangleRecord {
  VertexRecord vertex[3];
};

struct Capture {
  int xdimension;
  std::vector<TriangleRecord> triangles;
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

bool near(const float lhs, const float rhs, const float epsilon = 1.0e-5f)
{
  return std::fabs(lhs - rhs) <= epsilon;
}

void triangleCB(void * data, SoCallbackAction *,
                const SoPrimitiveVertex * a,
                const SoPrimitiveVertex * b,
                const SoPrimitiveVertex * c)
{
  Capture * capture = static_cast<Capture *>(data);
  const SoPrimitiveVertex * vertices[3] = { a, b, c };
  TriangleRecord triangle;
  for (int i = 0; i < 3; ++i) {
    triangle.vertex[i].point = vertices[i]->getPoint();
    triangle.vertex[i].coordinate = -1;
    triangle.vertex[i].material = vertices[i]->getMaterialIndex();
    triangle.vertex[i].normal = -1;
    triangle.vertex[i].normalvalue = vertices[i]->getNormal();
    triangle.vertex[i].texture = -1;
    triangle.vertex[i].face = -1;

    const SoDetail * detail = vertices[i]->getDetail();
    if (detail && detail->isOfType(SoFaceDetail::getClassTypeId())) {
      const SoFaceDetail * face = static_cast<const SoFaceDetail *>(detail);
      triangle.vertex[i].face = face->getFaceIndex();
      const int expectedcoordinate =
        static_cast<int>(std::lround(triangle.vertex[i].point[0] / 2.0f)) +
        static_cast<int>(std::lround(triangle.vertex[i].point[2] / 3.0f)) *
        capture->xdimension;
      for (int pointidx = 0; pointidx < face->getNumPoints(); ++pointidx) {
        const SoPointDetail * point = face->getPoint(pointidx);
        if (point->getCoordinateIndex() == expectedcoordinate) {
          triangle.vertex[i].coordinate = point->getCoordinateIndex();
          triangle.vertex[i].material = point->getMaterialIndex();
          triangle.vertex[i].normal = point->getNormalIndex();
          triangle.vertex[i].texture = point->getTextureCoordIndex();
          break;
        }
      }
    }
  }
  capture->triangles.push_back(triangle);
}

Capture capture(SoVRMLElevationGrid * grid)
{
  Capture result;
  result.xdimension = grid->xDimension.getValue();
  SoCallbackAction action;
  action.addTriangleCallback(SoVRMLElevationGrid::getClassTypeId(),
                             triangleCB, &result);
  action.apply(grid);
  return result;
}

SbBox3f boundingBox(SoNode * node)
{
  SoGetBoundingBoxAction action(SbViewportRegion(64, 64));
  action.apply(node);
  return action.getBoundingBox();
}

int triangleCount(SoNode * node)
{
  SoGetPrimitiveCountAction action;
  action.apply(node);
  return action.getTriangleCount();
}

SoVRMLElevationGrid * makeGrid(const int xdim, const int zdim,
                               const std::vector<float> & heights)
{
  SoVRMLElevationGrid * grid = new SoVRMLElevationGrid;
  grid->ref();
  grid->xDimension = xdim;
  grid->zDimension = zdim;
  grid->xSpacing = 2.0f;
  grid->zSpacing = 3.0f;
  if (!heights.empty()) {
    grid->height.setValues(0, static_cast<int>(heights.size()), &heights[0]);
  }
  return grid;
}

void testExcessHeightsAndDetails(void)
{
  const float values[] = { 1, 2, 3, 4, 1000, -1000 };
  SoVRMLElevationGrid * grid = makeGrid(2, 2,
    std::vector<float>(values, values + sizeof(values) / sizeof(values[0])));

  Capture result = capture(grid);
  CHECK(result.triangles.size() == 2);
  CHECK(triangleCount(grid) == 2);

  const SbBox3f box = boundingBox(grid);
  CHECK(!box.isEmpty());
  CHECK(near(box.getMin()[0], 0.0f));
  CHECK(near(box.getMin()[1], 1.0f));
  CHECK(near(box.getMin()[2], 0.0f));
  CHECK(near(box.getMax()[0], 2.0f));
  CHECK(near(box.getMax()[1], 4.0f));
  CHECK(near(box.getMax()[2], 3.0f));

  for (size_t triangle = 0; triangle < result.triangles.size(); ++triangle) {
    for (int vertex = 0; vertex < 3; ++vertex) {
      const VertexRecord & record = result.triangles[triangle].vertex[vertex];
      CHECK(record.face == 0);
      CHECK(record.coordinate >= 0 && record.coordinate < 4);
      CHECK(record.texture == record.coordinate);
    }
  }
  grid->unref();
}

void testInvalidDimensionsAndOverflow(void)
{
  {
    const float values[] = { 0, 1, 2 };
    SoVRMLElevationGrid * grid = makeGrid(2, 2,
      std::vector<float>(values, values + 3));
    CHECK(capture(grid).triangles.empty());
    CHECK(triangleCount(grid) == 0);
    CHECK(boundingBox(grid).isEmpty());
    grid->unref();
  }
  {
    const float values[] = { 0, 1, 2 };
    SoVRMLElevationGrid * grid = makeGrid(1, 3,
      std::vector<float>(values, values + 3));
    CHECK(capture(grid).triangles.empty());
    CHECK(triangleCount(grid) == 0);
    CHECK(boundingBox(grid).isEmpty());
    grid->unref();
  }
  {
    SoVRMLElevationGrid * grid = makeGrid(INT_MAX, INT_MAX,
                                          std::vector<float>());
    CHECK(capture(grid).triangles.empty());
    CHECK(triangleCount(grid) == 0);
    CHECK(boundingBox(grid).isEmpty());
    grid->unref();
  }
}

void testPerQuadBindings(void)
{
  const float heights[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
  SoVRMLElevationGrid * grid = makeGrid(3, 3,
    std::vector<float>(heights, heights + 9));

  SoVRMLColor * colors = new SoVRMLColor;
  const SbColor colorvalues[] = {
    SbColor(1, 0, 0), SbColor(0, 1, 0),
    SbColor(0, 0, 1), SbColor(1, 1, 0)
  };
  colors->color.setValues(0, 4, colorvalues);
  grid->color = colors;
  grid->colorPerVertex = FALSE;

  SoVRMLNormal * normals = new SoVRMLNormal;
  const SbVec3f normalvalues[] = {
    SbVec3f(0, 1, 0), SbVec3f(0, 0, 1),
    SbVec3f(1, 0, 0), SbVec3f(0, -1, 0)
  };
  normals->vector.setValues(0, 4, normalvalues);
  grid->normal = normals;
  grid->normalPerVertex = FALSE;

  Capture result = capture(grid);
  CHECK(result.triangles.size() == 8);
  for (size_t triangle = 0; triangle < result.triangles.size(); ++triangle) {
    const int expectedquad = static_cast<int>(triangle / 2);
    for (int vertex = 0; vertex < 3; ++vertex) {
      const VertexRecord & record = result.triangles[triangle].vertex[vertex];
      CHECK(record.face == expectedquad);
      CHECK(record.material == expectedquad);
      CHECK(record.normal == expectedquad);
      CHECK(record.texture == record.coordinate);
    }
  }
  grid->unref();
}

void testRayPickAfterBoundingBoxCache(void)
{
  const float values[] = { 0, 0, 0, 0, 10000 };
  SoVRMLElevationGrid * grid = makeGrid(2, 2,
    std::vector<float>(values, values + 5));
  SoSeparator * root = new SoSeparator;
  root->ref();
  root->addChild(grid);
  grid->unref();

  const SbBox3f box = boundingBox(root);
  CHECK(near(box.getMax()[1], 0.0f));

  SoRayPickAction pick(SbViewportRegion(64, 64));
  pick.setRay(SbVec3f(1.0f, 10.0f, 1.5f), SbVec3f(0, -1, 0));
  pick.apply(root);
  CHECK(pick.getPickedPoint() != NULL);
  root->unref();
}

void testNormalCacheNotificationsAndCcw(void)
{
  const float initial[] = { 0, 0, 0, 1 };
  SoVRMLElevationGrid * grid = makeGrid(2, 2,
    std::vector<float>(initial, initial + 4));
  Capture before = capture(grid);
  CHECK(before.triangles.size() == 2);

  grid->height.set1Value(3, -1.0f);
  Capture changed = capture(grid);
  CHECK(changed.triangles.size() == 2);
  CHECK(before.triangles[0].vertex[0].normalvalue.dot(
          changed.triangles[0].vertex[0].normalvalue) < 0.99f);

  grid->ccw = FALSE;
  Capture reversed = capture(grid);
  CHECK(reversed.triangles.size() == 2);
  CHECK(changed.triangles[0].vertex[0].normalvalue.dot(
          reversed.triangles[0].vertex[0].normalvalue) < -0.99f);
  grid->unref();
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
  if (std::strcmp(argv[1], "excess-details") == 0) {
    testExcessHeightsAndDetails();
  }
  else if (std::strcmp(argv[1], "invalid-overflow") == 0) {
    testInvalidDimensionsAndOverflow();
  }
  else if (std::strcmp(argv[1], "per-quad-bindings") == 0) {
    testPerQuadBindings();
  }
  else if (std::strcmp(argv[1], "raypick-cache") == 0) {
    testRayPickAfterBoundingBoxCache();
  }
  else if (std::strcmp(argv[1], "normal-cache-ccw") == 0) {
    testNormalCacheNotificationsAndCcw();
  }
  else {
    std::fprintf(stderr, "unknown case: %s\n", argv[1]);
    return 2;
  }

  SoDB::finish();
  return failures == 0 ? 0 : 1;
}
