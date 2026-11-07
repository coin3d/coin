#include <Inventor/SoTriangleSnapshot.h>

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/nodes/SoShape.h>
#include <memory>
#include <stdexcept>
#include <vector>

struct SoTriangleSnapshot::Data {
  std::vector<SbMatrix> matrices;
  std::vector<Triangle> triangles;
};

namespace {
struct Capture {
  std::vector<SbMatrix> & matrices;
  std::vector<SoTriangleSnapshot::Triangle> & triangles;
  // A shape can itself traverse other shapes. Restore its occurrence on exit.
  std::vector<std::size_t> stack;
  bool invalidMaterial;
  static std::size_t unseen() { return static_cast<std::size_t>(-1); }

  static SoCallbackAction::Response enter(void * closure, SoCallbackAction *, const SoNode *)
  {
    static_cast<Capture *>(closure)->stack.push_back(unseen());
    return SoCallbackAction::CONTINUE;
  }

  static SoCallbackAction::Response leave(void * closure, SoCallbackAction *, const SoNode *)
  {
    static_cast<Capture *>(closure)->stack.pop_back();
    return SoCallbackAction::CONTINUE;
  }

  static void emit(void * closure, SoCallbackAction * action, const SoPrimitiveVertex * a,
                   const SoPrimitiveVertex * b, const SoPrimitiveVertex * c)
  {
    Capture & capture = *static_cast<Capture *>(closure);
    if (capture.invalidMaterial) return;
    std::size_t & occurrence = capture.stack.back();
    if (occurrence == unseen()) {
      occurrence = capture.matrices.size();
      capture.matrices.push_back(action->getModelMatrix());
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
};
}

SoTriangleSnapshot::SoTriangleSnapshot(SoNode & root, const SbViewportRegion & viewport)
{
  std::unique_ptr<Data> result(new Data);
  Capture capture = {result->matrices, result->triangles, {}, false};
  SoCallbackAction action(viewport);
  const SoType shape = SoShape::getClassTypeId();
  action.addPreCallback(shape, Capture::enter, &capture);
  action.addPostCallback(shape, Capture::leave, &capture);
  action.addTriangleCallback(shape, Capture::emit, &capture);
  action.apply(&root);
  if (capture.invalidMaterial)
    throw std::out_of_range("triangle material index outside current diffuse array");
  this->data = result.release();
}

SoTriangleSnapshot::~SoTriangleSnapshot() { delete this->data; }
std::size_t SoTriangleSnapshot::getNumOccurrences() const { return this->data->matrices.size(); }
std::size_t SoTriangleSnapshot::getNumTriangles() const { return this->data->triangles.size(); }
const SbMatrix & SoTriangleSnapshot::getModelMatrix(std::size_t i) const { return this->data->matrices.at(i); }
const SoTriangleSnapshot::Triangle & SoTriangleSnapshot::getTriangle(std::size_t i) const { return this->data->triangles.at(i); }
