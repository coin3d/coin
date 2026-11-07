#include <Inventor/SoDB.h>
#include <Inventor/SoTriangleSnapshot.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoAction.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTranslation.h>
#include <cstdio>
#include <limits>
#include <stdexcept>

namespace {
class NestedShape : public SoFaceSet {
public:
  explicit NestedShape(int & deaths) : children(nullptr), completed(0), destroyed(deaths) {}
  SoSeparator * children;
  int completed;
protected:
  ~NestedShape() override {
    if (children) children->unref();
    ++destroyed;
  }
  void generatePrimitives(SoAction * action) override {
    SoPrimitiveVertex v[3];
    for (int i = 0; i < 3; ++i) {
      v[i].setPoint(SbVec3f(float(i == 1), float(i == 2), 0));
      v[i].setNormal(SbVec3f(0,0,1));
      v[i].setMaterialIndex(0);
    }
    this->invokeTriangleCallbacks(action, &v[0], &v[1], &v[2]);
    if (children) {
      action->traverse(children);
      this->invokeTriangleCallbacks(action, &v[0], &v[1], &v[2]);
    }
    ++completed;
  }
private:
  int & destroyed;
};
}

int main()
{
  SoDB::init();
  int destroyed = 0;
  auto * root = new SoSeparator;
  root->ref();
  auto * parent = new NestedShape(destroyed);
  auto * child = new NestedShape(destroyed);
  parent->children = new SoSeparator;
  parent->children->ref();
  auto * shift = new SoTranslation;
  shift->translation.setValue(0,4,0);
  auto * blue = new SoMaterial;
  blue->diffuseColor.setValue(0,0,1);
  parent->children->addChild(shift);
  parent->children->addChild(blue);
  parent->children->addChild(child);
  for (int i = 0; i < 2; ++i) {
    auto * branch = new SoSeparator;
    auto * translation = new SoTranslation;
    translation->translation.setValue(float(i * 10),0,0);
    auto * color = new SoMaterial;
    color->diffuseColor.setValue(float(i == 0),float(i == 1),0);
    branch->addChild(translation);
    branch->addChild(color);
    branch->addChild(parent);
    root->addChild(branch);
  }
  // Failure during nested emission must allow both producers to finish and
  // must not poison the occurrence stack in the next capture.
  bool rejected = false;
  try {
    SoTriangleSnapshot failed(*root, SbViewportRegion(64,64),
      {1, std::numeric_limits<std::size_t>::max()});
  }
  catch (const std::length_error &) { rejected = true; }
  if (!rejected || parent->completed != 1 || child->completed != 1) return 1;
  SoTriangleSnapshot snapshot(*root, SbViewportRegion(64,64));
  root->unref();
  SoDB::finish();
  if (destroyed != 2 || snapshot.getNumOccurrences() != 4 ||
      snapshot.getNumTriangles() != 6) return 2;
  const std::size_t occurrences[] = {0,1,0,2,3,2};
  for (std::size_t i = 0; i < 6; ++i) {
    const auto & triangle = snapshot.getTriangle(i);
    if (triangle.occurrence != occurrences[i]) return 3;
    const bool nested = i % 3 == 1;
    const bool second = i >= 3;
    SbVec3f world;
    snapshot.getModelMatrix(triangle.occurrence).multVecMatrix(SbVec3f(0,0,0), world);
    if (world != SbVec3f(second ? 10.0f : 0.0f, nested ? 4.0f : 0.0f, 0)) return 4;
    const SbColor expected = nested ? SbColor(0,0,1) :
      (second ? SbColor(0,1,0) : SbColor(1,0,0));
    for (int j = 0; j < 3; ++j) {
      const auto & vertex = triangle.vertices[j];
      if (vertex.diffuse != expected || vertex.normal != SbVec3f(0,0,1) ||
          vertex.point != SbVec3f(float(j == 1),float(j == 2),0)) return 5;
    }
  }
  std::puts("nested occurrences, parent state restoration, limit recovery and source destruction verified");
  return 0;
}
