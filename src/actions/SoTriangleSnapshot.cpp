#include <Inventor/SoTriangleSnapshot.h>

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/nodes/SoShape.h>
#include <memory>
#include <limits>
#include <new>
#include <stdexcept>
#include <vector>

namespace {
struct StorageLimit {};

struct StorageBudget {
  std::size_t limit, used, peak;
};

template <typename T>
struct SnapshotAllocator {
  typedef T value_type;
  StorageBudget * budget;

  explicit SnapshotAllocator(StorageBudget & value) : budget(&value) {}
  template <typename U>
  SnapshotAllocator(const SnapshotAllocator<U> & other) : budget(other.budget) {}

  T * allocate(std::size_t count)
  {
    if (count > (budget->limit - budget->used) / sizeof(T))
      throw StorageLimit();
    T * result = std::allocator<T>().allocate(count);
    budget->used += count * sizeof(T);
    if (budget->used > budget->peak) budget->peak = budget->used;
    return result;
  }

  void deallocate(T * pointer, std::size_t count)
  {
    budget->used -= count * sizeof(T);
    std::allocator<T>().deallocate(pointer, count);
  }
  template <typename U>
  bool operator==(const SnapshotAllocator<U> & other) const { return budget == other.budget; }
  template <typename U>
  bool operator!=(const SnapshotAllocator<U> & other) const { return budget != other.budget; }
};

typedef std::vector<SbMatrix, SnapshotAllocator<SbMatrix> > Matrices;
typedef std::vector<SoTriangleSnapshot::Triangle,
                    SnapshotAllocator<SoTriangleSnapshot::Triangle> > Triangles;
}

struct SoTriangleSnapshot::Data {
  // The budget outlives both containers that charge it.
  StorageBudget budget;
  Matrices matrices;
  Triangles triangles;

  explicit Data(std::size_t limit)
    : budget{limit, sizeof(Data), sizeof(Data)},
      matrices(SnapshotAllocator<SbMatrix>(budget)),
      triangles(SnapshotAllocator<Triangle>(budget)) {}
};

namespace {
struct Capture {
  Matrices & matrices;
  Triangles & triangles;
  // A shape can itself traverse other shapes. Restore its occurrence on exit.
  std::vector<std::size_t, SnapshotAllocator<std::size_t> > stack;
  std::size_t maxTriangles;
  bool invalidMaterial;
  bool changingMatrix;
  bool outOfMemory;
  const char * limitReason;

  Capture(Matrices & m, Triangles & t, StorageBudget & budget, std::size_t count)
    : matrices(m), triangles(t), stack(SnapshotAllocator<std::size_t>(budget)),
      maxTriangles(count), invalidMaterial(false), changingMatrix(false),
      outOfMemory(false), limitReason(nullptr) {}

  bool stopping() const { return outOfMemory || limitReason != nullptr; }
  static std::size_t unseen() { return static_cast<std::size_t>(-1); }

  static SoCallbackAction::Response enter(void * closure, SoCallbackAction *, const SoNode *)
  {
    Capture & capture = *static_cast<Capture *>(closure);
    if (capture.stopping()) return SoCallbackAction::ABORT;
    try {
      capture.stack.push_back(unseen());
    }
    catch (const StorageLimit &) {
      capture.limitReason = "triangle snapshot storage limit exceeded";
      return SoCallbackAction::ABORT;
    }
    catch (const std::bad_alloc &) {
      capture.outOfMemory = true;
      return SoCallbackAction::ABORT;
    }
    return SoCallbackAction::CONTINUE;
  }

  static SoCallbackAction::Response leave(void * closure, SoCallbackAction *, const SoNode *)
  {
    Capture & capture = *static_cast<Capture *>(closure);
    if (capture.stopping()) return SoCallbackAction::ABORT;
    capture.stack.pop_back();
    return SoCallbackAction::CONTINUE;
  }

  static void emit(void * closure, SoCallbackAction * action, const SoPrimitiveVertex * a,
                   const SoPrimitiveVertex * b, const SoPrimitiveVertex * c)
  {
    Capture & capture = *static_cast<Capture *>(closure);
    if (capture.invalidMaterial || capture.changingMatrix || capture.stopping()) return;
    if (capture.triangles.size() >= capture.maxTriangles) {
      capture.limitReason = "triangle snapshot triangle limit exceeded";
      return;
    }
    try {
      std::size_t & occurrence = capture.stack.back();
      if (occurrence == unseen()) {
        occurrence = capture.matrices.size();
        capture.matrices.push_back(action->getModelMatrix());
      }
      if (capture.matrices[occurrence] != action->getModelMatrix()) {
        capture.changingMatrix = true;
        return;
      }
      SoTriangleSnapshot::Triangle triangle;
      triangle.occurrence = occurrence;
      const SoPrimitiveVertex * vertices[] = {a, b, c};
      const int diffuseCount = SoLazyElement::getInstance(action->getState())->getNumDiffuse();
      for (int i = 0; i < 3; ++i) {
        const SoPrimitiveVertex & src = *vertices[i];
        const int index = src.getMaterialIndex();
        if (index < 0 || index >= diffuseCount) {
          capture.invalidMaterial = true;
          return;
        }
        SoTriangleSnapshot::Vertex & dst = triangle.vertices[i];
        dst.point = src.getPoint();
        dst.normal = src.getNormal();
        action->getMaterial(dst.ambient, dst.diffuse, dst.specular, dst.emission,
                            dst.shininess, dst.transparency, index);
      }
      capture.triangles.push_back(triangle);
    }
    catch (const StorageLimit &) {
      capture.limitReason = "triangle snapshot storage limit exceeded";
    }
    catch (const std::bad_alloc &) {
      // Let the shape finish its primitive-generation cleanup before aborting.
      capture.outOfMemory = true;
    }
  }
};
}

SoTriangleSnapshot::SoTriangleSnapshot(SoNode & root, const SbViewportRegion & viewport)
: SoTriangleSnapshot(root, viewport,
      Limits{std::numeric_limits<std::size_t>::max(), std::numeric_limits<std::size_t>::max()})
{
}

SoTriangleSnapshot::SoTriangleSnapshot(SoNode & root, const SbViewportRegion & viewport,
                                       const Limits & limits)
{
  if (limits.maxStorageBytes < sizeof(Data))
    throw std::length_error("triangle snapshot storage limit exceeded");
  std::unique_ptr<Data> result(new Data(limits.maxStorageBytes));
  Capture capture(result->matrices, result->triangles, result->budget, limits.maxTriangles);
  SoCallbackAction action(viewport);
  const SoType shape = SoShape::getClassTypeId();
  action.addPreCallback(shape, Capture::enter, &capture);
  action.addPostCallback(shape, Capture::leave, &capture);
  action.addTriangleCallback(shape, Capture::emit, &capture);
  action.apply(&root);
  if (capture.outOfMemory) throw std::bad_alloc();
  if (capture.limitReason) throw std::length_error(capture.limitReason);
  if (capture.invalidMaterial)
    throw std::out_of_range("triangle material index outside current diffuse array");
  if (capture.changingMatrix)
    throw std::runtime_error("model matrix changed within a shape occurrence");
  if (action.hasTerminated())
    throw std::runtime_error("triangle snapshot traversal was aborted");
  this->data = result.release();
}

SoTriangleSnapshot::~SoTriangleSnapshot() { delete this->data; }
std::size_t SoTriangleSnapshot::getNumOccurrences() const { return this->data->matrices.size(); }
std::size_t SoTriangleSnapshot::getNumTriangles() const { return this->data->triangles.size(); }
const SbMatrix & SoTriangleSnapshot::getModelMatrix(std::size_t i) const { return this->data->matrices.at(i); }
const SoTriangleSnapshot::Triangle & SoTriangleSnapshot::getTriangle(std::size_t i) const { return this->data->triangles.at(i); }

std::size_t SoTriangleSnapshot::getPeakStorageBytes() const { return this->data->budget.peak; }
