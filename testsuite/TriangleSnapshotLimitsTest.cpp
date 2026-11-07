#include "TriangleSnapshotTestDatabase.h"
#include <Inventor/SoTriangleSnapshot.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoAction.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoSeparator.h>
#include <cstdio>
#include <limits>
#include <stdexcept>

namespace {
class LimitShape : public SoFaceSet {
public:
  bool completed = false;
protected:
  void generatePrimitives(SoAction * action) override
  {
    SoPrimitiveVertex vertices[3];
    for (int i = 0; i < 3; ++i) {
      vertices[i].setPoint(SbVec3f(float(i == 1),float(i == 2),0));
      vertices[i].setNormal(SbVec3f(0,0,1));
      vertices[i].setMaterialIndex(0);
    }
    for (int i = 0; i < 2; ++i)
      this->invokeTriangleCallbacks(action,&vertices[0],&vertices[1],&vertices[2]);
    completed = true;
  }
};
}

int run()
{
  SnapshotTestDatabase database;
  auto * root = new SoSeparator;
  root->ref();
  const SbViewportRegion viewport(64,64);
  const std::size_t unlimited = std::numeric_limits<std::size_t>::max();
  std::size_t emptyBytes = 0;
  {
    SoTriangleSnapshot empty(*root,viewport, {0,unlimited});
    if (empty.getNumTriangles() || empty.getNumOccurrences()) return 1;
    emptyBytes = empty.getPeakStorageBytes();
  }
  {
    SoTriangleSnapshot empty(*root,viewport, {0,emptyBytes});
    if (empty.getPeakStorageBytes() != emptyBytes) return 2;
  }
  auto * shape = new LimitShape;
  root->addChild(shape);
  std::size_t peak = 0;
  {
    SoTriangleSnapshot measured(*root,viewport);
    peak = measured.getPeakStorageBytes();
    if (measured.getNumTriangles() != 2) return 3;
  }
  {
    SoTriangleSnapshot exact(*root,viewport,{2,peak});
    if (exact.getNumTriangles() != 2 || exact.getPeakStorageBytes() != peak) return 4;
  }
  // Two buffers coexist while the triangle vector grows from one to two.
  const std::size_t finalBuffers = emptyBytes + sizeof(std::size_t) +
    sizeof(SbMatrix) + 2 * sizeof(SoTriangleSnapshot::Triangle);
  if (peak <= finalBuffers) return 5;
  const SoTriangleSnapshot::Limits cases[] = {
    {1,unlimited}, {0,unlimited}, {2,peak-1}, {2,finalBuffers},
    {2,0}, {2,emptyBytes}, {2,emptyBytes-1}
  };
  for (const auto & limits : cases) {
    shape->completed = false;
    bool rejected = false;
    try { SoTriangleSnapshot failed(*root,viewport,limits); }
    catch (const std::length_error &) { rejected = true; }
    if (!rejected || root->getRefCount() != 1 || shape->getRefCount() != 1) return 6;
    if (limits.maxStorageBytes >= peak-1 && !shape->completed) return 7;
    SoTriangleSnapshot recovery(*root,viewport,{2,peak});
    if (recovery.getNumTriangles() != 2 ||
        recovery.getTriangle(1).vertices[1].point != SbVec3f(1,0,0)) return 8;
  }
  root->unref();
  std::printf("exact triangle and byte limits pass; seven rejections recover; peak=%zu\n",peak);
  return 0;
}

int main()
{
  try { return run(); }
  catch (const std::exception & error) {
    std::fprintf(stderr, "unexpected exception: %s\n", error.what());
    return 9;
  }
  catch (...) {
    std::fputs("unexpected non-standard exception\n", stderr);
    return 10;
  }
}
