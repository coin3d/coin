#include "TriangleSnapshotTestDatabase.h"
#include <Inventor/SoTriangleSnapshot.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <cstdio>
#include <thread>

namespace {
class TrackedRoot : public SoSeparator {
public:
  explicit TrackedRoot(bool & destroyed) : destroyed(destroyed) {}
  ~TrackedRoot() { this->destroyed = true; }
private:
  bool & destroyed;
};

class TrackedFace : public SoFaceSet {
public:
  explicit TrackedFace(bool & destroyed) : destroyed(destroyed) {}
  ~TrackedFace() { this->destroyed = true; }
private:
  bool & destroyed;
};

bool check(const SoTriangleSnapshot & snapshot)
{
  if (snapshot.getNumOccurrences() != 2 || snapshot.getNumTriangles() != 2) return false;
  const SbVec3f local[] = {SbVec3f(0,0,0), SbVec3f(2,0,0), SbVec3f(0,1,0)};
  const SbVec3f world[][3] = {
    {SbVec3f(-3,1,2), SbVec3f(-1,1,2), SbVec3f(-3,2,2)},
    {SbVec3f(7,-2,1), SbVec3f(11,-2,1), SbVec3f(7,1,1)}
  };
  for (int i = 0; i < 2; ++i) {
    const auto & triangle = snapshot.getTriangle(i);
    if (triangle.occurrence != static_cast<std::size_t>(i)) return false;
    const SbColor expected = i ? SbColor(0,1,0) : SbColor(1,0,0);
    for (int j = 0; j < 3; ++j) {
      const auto & vertex = triangle.vertices[j];
      SbVec3f transformed;
      snapshot.getModelMatrix(i).multVecMatrix(vertex.point, transformed);
      if (vertex.point != local[j] || transformed != world[i][j] ||
          vertex.normal != SbVec3f(0,0,1) || vertex.diffuse != expected ||
          vertex.transparency != (i ? 0.5f : 0.0f)) return false;
    }
  }
  return true;
}
}

int main()
{
  SnapshotTestDatabase database;
  bool destroyed = false;
  bool rootDestroyed = false;
  auto * root = new TrackedRoot(rootDestroyed);
  root->ref();
  auto * coords = new SoCoordinate3;
  coords->point.set1Value(0,0,0,0);
  coords->point.set1Value(1,2,0,0);
  coords->point.set1Value(2,0,1,0);
  root->addChild(coords);
  auto * face = new TrackedFace(destroyed);
  face->numVertices.setValue(3);
  for (int i = 0; i < 2; ++i) {
    auto * branch = new SoSeparator;
    root->addChild(branch);
    auto * transform = new SoTransform;
    transform->translation = i ? SbVec3f(7,-2,1) : SbVec3f(-3,1,2);
    if (i) transform->scaleFactor.setValue(2,3,1);
    branch->addChild(transform);
    auto * material = new SoMaterial;
    material->diffuseColor = i ? SbColor(0,1,0) : SbColor(1,0,0);
    material->transparency = i ? 0.5f : 0.0f;
    branch->addChild(material);
    branch->addChild(face);
  }
  SoTriangleSnapshot snapshot(*root, SbViewportRegion(64,64));
  coords->point.set1Value(1,99,99,99);
  root->unref();
  if (!rootDestroyed || !destroyed) return 1;
  bool valid = false;
  std::thread reader([&] { valid = check(snapshot); });
  reader.join();
  if (!valid) return 2;
  std::puts("Two transformed occurrences verified on worker after source destruction.");
  return 0;
}
