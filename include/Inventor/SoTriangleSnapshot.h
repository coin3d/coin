#ifndef COIN_SOTRIANGLESNAPSHOT_H
#define COIN_SOTRIANGLESNAPSHOT_H

#include <Inventor/SbColor.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/SbViewportRegion.h>
#include <cstddef>

class SoNode;

/*!
  An owned copy of the triangles emitted by SoCallbackAction for a scene.
  The caller owns a reference to root and prevents mutation during traversal.
  Construct on a thread permitted to traverse the scene. After construction
  readers need only the snapshot, even after the scene has been destroyed.
  Keep the snapshot alive until all readers have finished.

  Positions and normals are shape-local. The occurrence matrix maps positions
  into Coin world coordinates in scene units. Occurrence indices are local to
  this traversal. Storage grows with the number of emitted triangles.
  Shared shapes can emit different geometry or materials on
  different visits. An invalid diffuse material index throws std::out_of_range.
  Each emitting shape visit must keep a single model matrix. A matrix change
  within that visit or an aborted traversal throws std::runtime_error.
  Constructor failure publishes no snapshot. Allocation failures propagate.
  Allocation failures in capture callbacks are deferred until the emitting
  shape returns, allowing primitive-generation cleanup before traversal stops.
  Accessors throw std::out_of_range for indices at or beyond the count.
  Returned references belong to this snapshot and expire with it.
  Const access may run concurrently; construction traversals follow Coin
  threading rules. Occurrence indices cannot identify a later traversal.
  Empty traversals have zero triangles and zero occurrences.
  Lines, points, textures and application details are omitted.
*/
class COIN_DLL_API SoTriangleSnapshot {
public:
  struct Vertex {
    SbVec3f point, normal;
    SbColor ambient, diffuse, specular, emission;
    float shininess, transparency;
  };
  struct Triangle {
    std::size_t occurrence;
    Vertex vertices[3];
  };

  struct Limits {
    std::size_t maxTriangles;
    // Peak requested bytes for Data and matrix, triangle and occurrence-stack
    // buffers, including old and new buffers during reallocation. Excludes
    // allocator metadata, the source graph and Coin's traversal allocations.
    std::size_t maxStorageBytes;
  };

  // Compatibility overload with no caller-specified storage or triangle cap.
  SoTriangleSnapshot(SoNode & root, const SbViewportRegion & viewport);
  // Exceeding either inclusive cap throws std::length_error after producer
  // cleanup. Zero triangles admits empty captures; zero storage rejects all.
  SoTriangleSnapshot(SoNode & root, const SbViewportRegion & viewport, const Limits & limits);
  ~SoTriangleSnapshot();

  std::size_t getNumOccurrences() const;
  std::size_t getNumTriangles() const;
  // High-water mark under the same accounting used by Limits::maxStorageBytes.
  std::size_t getPeakStorageBytes() const;
  const SbMatrix & getModelMatrix(std::size_t occurrence) const;
  const Triangle & getTriangle(std::size_t index) const;

private:
  SoTriangleSnapshot(const SoTriangleSnapshot &) = delete;
  SoTriangleSnapshot & operator=(const SoTriangleSnapshot &) = delete;
  struct Data;
  Data * data;
};

#endif
