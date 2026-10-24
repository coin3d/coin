#include <Inventor/SoDB.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/nodes/SoQuadMesh.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoVertexProperty.h>

#include <climits>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace {
int failures = 0;

void expect(const std::string & context, const char * quantity,
            int actual, int expected)
{
  if (actual != expected) {
    std::cerr << context << ": " << quantity << " = " << actual
              << ", expected " << expected << '\n';
    ++failures;
  }
}

void expectCounts(SoGetPrimitiveCountAction & action, int triangles,
                  const std::string & context)
{
  expect(context, "triangles", action.getTriangleCount(), triangles);
  expect(context, "lines", action.getLineCount(), 0);
  expect(context, "points", action.getPointCount(), 0);
  expect(context, "text", action.getTextCount(), 0);
  expect(context, "images", action.getImageCount(), 0);
  expect(context, "containsNoPrimitives", !!action.containsNoPrimitives(),
         triangles == 0);
  expect(context, "containsNonTriangleShapes", !!action.containsNonTriangleShapes(), 0);
}

struct Scene {
  SoSeparator * root;
  Scene() : root(new SoSeparator) { root->ref(); }
  ~Scene() { root->unref(); }
private:
  Scene(const Scene &);
  Scene & operator=(const Scene &);
};

SoQuadMesh * mesh(Scene & scene, int rows, int columns)
{
  SoQuadMesh * result = new SoQuadMesh;
  result->verticesPerRow = rows;
  result->verticesPerColumn = columns;
  scene.root->addChild(result);
  return result;
}

struct Dimensions { int rows, columns, triangles; };

void countDimensions()
{
  // Literal expectations include both orientations and near-limit counts.
  const Dimensions cases[] = {
    {2, 2, 2}, {2, 3, 4}, {3, 2, 4}, {3, 3, 8}, {4, 3, 12},
    {3, 4, 12}, {2, 17, 32}, {17, 2, 32}, {7, 13, 144}, {13, 7, 144},
    {101, 31, 6000}, {32768, 32768, 2147352578},
    {2, 1073741824, 2147483646}, {1073741824, 2, 2147483646},
    {3, 536870912, 2147483644}, {536870912, 3, 2147483644},
    {0, 0, 0}, {0, 4, 0}, {4, 0, 0}, {1, 1, 0}, {1, 4, 0}, {4, 1, 0},
    {-1, 4, 0}, {4, -1, 0}, {-5, -7, 0}, {-1, 0, 0}, {0, -1, 0},
    {INT_MIN, 2, 0}, {2, INT_MIN, 0}, {INT_MIN, INT_MIN, 0},
    {INT_MIN, INT_MAX, 0}, {INT_MAX, INT_MIN, 0},
    {INT_MAX, 0, 0}, {0, INT_MAX, 0}, {INT_MAX, 1, 0}, {1, INT_MAX, 0}
  };
  for (int approximate = 0; approximate != 2; ++approximate) {
    Scene scene;
    // Deliberately no coordinates: topological counting must not generate geometry.
    SoQuadMesh * shape = new SoQuadMesh;
    scene.root->addChild(shape);
    SoGetPrimitiveCountAction action;
    action.setCanApproximate(approximate ? TRUE : FALSE);
    action.apply(scene.root);
    expectCounts(action, 0, "default 1x1");
    for (const Dimensions & item : cases) {
      shape->verticesPerRow = item.rows;
      shape->verticesPerColumn = item.columns;
      std::ostringstream label;
      label << item.rows << 'x' << item.columns << " approximate=" << approximate;
      action.apply(scene.root);
      expectCounts(action, item.triangles, label.str());
      expect(label.str(), "canApproximate", !!action.canApproximateCount(), approximate);
    }
  }
}

struct CallbackCounts {
  int triangles = 0;
  int lines = 0;
  int points = 0;
};

void triangle(void * data, SoCallbackAction *, const SoPrimitiveVertex *,
              const SoPrimitiveVertex *, const SoPrimitiveVertex *)
{ ++static_cast<CallbackCounts *>(data)->triangles; }
void line(void * data, SoCallbackAction *, const SoPrimitiveVertex *,
          const SoPrimitiveVertex *)
{ ++static_cast<CallbackCounts *>(data)->lines; }
void point(void * data, SoCallbackAction *, const SoPrimitiveVertex *)
{ ++static_cast<CallbackCounts *>(data)->points; }

void compareCallbacks()
{
  const Dimensions cases[] = {
    {2, 2, 2}, {2, 3, 4}, {3, 2, 4}, {3, 3, 8}, {4, 3, 12},
    {3, 4, 12}, {2, 9, 16}, {9, 2, 16}, {7, 5, 48}
  };
  for (const Dimensions & item : cases) {
    for (int useProperty = 0; useProperty != 2; ++useProperty) {
      for (int start : {0, 5}) {
        Scene scene;
        std::vector<SbVec3f> coordinates(start + item.rows * item.columns);
        for (int column = 0; column < item.columns; ++column) {
          for (int row = 0; row < item.rows; ++row) {
            coordinates[start + column * item.rows + row].setValue(
              static_cast<float>(row), static_cast<float>(column), 0.0f);
          }
        }
        SoCoordinate3 * stateCoordinates = new SoCoordinate3;
        stateCoordinates->point.setValues(0, static_cast<int>(coordinates.size()),
                                          coordinates.data());
        scene.root->addChild(stateCoordinates);
        SoNormal * normal = new SoNormal;
        normal->vector.set1Value(0, SbVec3f(0, 0, 1));
        scene.root->addChild(normal);
        SoNormalBinding * binding = new SoNormalBinding;
        binding->value = SoNormalBinding::OVERALL;
        scene.root->addChild(binding);
        SoQuadMesh * shape = mesh(scene, item.rows, item.columns);
        shape->startIndex = start;
        if (useProperty) {
          SoVertexProperty * property = new SoVertexProperty;
          property->vertex.setValues(0, static_cast<int>(coordinates.size()),
                                     coordinates.data());
          property->normal.set1Value(0, SbVec3f(0, 0, 1));
          property->normalBinding = SoVertexProperty::OVERALL;
          shape->vertexProperty = property;
          // The property supplies the complete coordinates in this variant.
          stateCoordinates->point.setNum(0);
        }
        CallbackCounts generated;
        SoCallbackAction callback;
        callback.addTriangleCallback(SoQuadMesh::getClassTypeId(), triangle, &generated);
        callback.addLineSegmentCallback(SoQuadMesh::getClassTypeId(), line, &generated);
        callback.addPointCallback(SoQuadMesh::getClassTypeId(), point, &generated);
        callback.apply(scene.root);
        std::ostringstream label;
        label << "callbacks " << item.rows << 'x' << item.columns
              << " vertexProperty=" << useProperty << " startIndex=" << start;
        expect(label.str(), "generated triangles", generated.triangles, item.triangles);
        expect(label.str(), "generated lines", generated.lines, 0);
        expect(label.str(), "generated points", generated.points, 0);
        SoGetPrimitiveCountAction action;
        for (int approximate = 0; approximate != 2; ++approximate) {
          action.setCanApproximate(approximate ? TRUE : FALSE);
          action.apply(scene.root);
          expectCounts(action, item.triangles, label.str());
          expect(label.str(), "count versus callbacks", action.getTriangleCount(),
                 generated.triangles);
        }
      }
    }
  }
}

void accumulationAndReset()
{
  Scene scene;
  SoQuadMesh * first = mesh(scene, 3, 3); // 8 triangles
  mesh(scene, 4, 3);                      // 12 triangles
  mesh(scene, -3, -4);                    // no cells
  Scene empty;
  Scene large;
  mesh(large, 2, 500000001);              // 1,000,000,000 triangles each
  mesh(large, 500000001, 2);
  SoGetPrimitiveCountAction action;
  for (int approximate = 0; approximate != 2; ++approximate) {
    action.setCanApproximate(approximate ? TRUE : FALSE);
    first->verticesPerRow = 3;
    first->verticesPerColumn = 3;
    action.apply(scene.root);
    expectCounts(action, 20, "multiple meshes");
    first->verticesPerRow = 2;
    action.apply(scene.root);
    expectCounts(action, 16, "edited row dimension");
    first->verticesPerColumn = 5;
    action.apply(scene.root);
    expectCounts(action, 20, "edited column dimension");
    first->verticesPerRow = 1;
    action.apply(scene.root);
    expectCounts(action, 12, "one mesh becomes empty");
    action.apply(empty.root);
    expectCounts(action, 0, "reset on empty graph");
    action.apply(large.root);
    expectCounts(action, 2000000000, "representable large sum");
    first->verticesPerRow = 3;
    first->verticesPerColumn = 3;
    action.apply(scene.root);
    expectCounts(action, 20, "reset after large graph");
    action.apply(first);
    expectCounts(action, 8, "apply a single mesh after graph");
  }
}

void stateModes()
{
  Scene scene;
  SoComplexity * complexity = new SoComplexity;
  scene.root->addChild(complexity);
  SoDrawStyle * style = new SoDrawStyle;
  scene.root->addChild(style);
  SoQuadMesh * shape = mesh(scene, 4, 3);
  shape->startIndex = 37;
  SoGetPrimitiveCountAction action;
  const SoComplexity::Type types[] = {
    SoComplexity::OBJECT_SPACE, SoComplexity::SCREEN_SPACE, SoComplexity::BOUNDING_BOX
  };
  const SoDrawStyle::Style styles[] = {
    SoDrawStyle::FILLED, SoDrawStyle::LINES, SoDrawStyle::POINTS, SoDrawStyle::INVISIBLE
  };
  for (int approximate = 0; approximate != 2; ++approximate) {
    action.setCanApproximate(approximate ? TRUE : FALSE);
    for (SoComplexity::Type type : types) {
      complexity->type = type;
      for (float value : {0.0f, 0.5f, 1.0f}) {
        complexity->value = value;
        for (SoDrawStyle::Style draw : styles) {
          style->style = draw;
          action.apply(scene.root);
          expectCounts(action, 12, "complexity/drawstyle/startIndex");
        }
      }
    }
  }
}
} // namespace

int main()
{
  SoDB::init();
  countDimensions();
  compareCallbacks();
  accumulationAndReset();
  stateModes();
  if (failures) std::cerr << failures << " assertion(s) failed\n";
  else std::cout << "SoQuadMesh primitive counts passed\n";
  return failures ? 1 : 0;
}
