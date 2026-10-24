#include <Inventor/SbRotation.h>
#include <Inventor/SoDB.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/details/SoLineDetail.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoMatrixTransform.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

// Public extension API isolates interpolation from built-in vertex assembly.
class Segment : public SoShape {
  SO_NODE_HEADER(Segment);
public:
  static void initClass() { SO_NODE_INIT_CLASS(Segment, SoShape, "Shape"); }
  Segment() { SO_NODE_CONSTRUCTOR(Segment); }
  SoPrimitiveVertex first, second;
  bool direct = false;
  void computeBBox(SoAction *, SbBox3f & box, SbVec3f & center) override {
    box.makeEmpty(); box.extendBy(first.getPoint()); box.extendBy(second.getPoint());
    center = (first.getPoint() + second.getPoint()) * 0.5f;
  }
protected:
  ~Segment() override {}
  void generatePrimitives(SoAction * action) override {
    if (direct) invokeLineSegmentCallbacks(action, &first, &second);
    else {
      beginShape(action, LINES);
      shapeVertex(&first); shapeVertex(&second); endShape();
    }
  }
};
SO_NODE_SOURCE(Segment);

struct Database {
  Database() { SoDB::init(); Segment::initClass(); }
  ~Database() { SoDB::finish(); }
};
struct Scene {
  SoSeparator * root;
  Scene() : root(new SoSeparator) { root->ref(); }
  ~Scene() { root->unref(); }
  Scene(const Scene &) = delete;
  Scene & operator=(const Scene &) = delete;
};
void require(bool value, const std::string & message) {
  if (!value) throw std::runtime_error(message);
}
void close(float actual, float expected, const std::string & label) {
  if (!std::isfinite(actual) || std::fabs(actual - expected) > 3e-4f) {
    throw std::runtime_error(label + ": expected " + std::to_string(expected) +
                             ", got " + std::to_string(actual));
  }
}
void close(const SbVec3f & actual, const SbVec3f & expected, const std::string & label) {
  for (int i = 0; i != 3; ++i) close(actual[i], expected[i], label + "[" + std::to_string(i) + "]");
}
void close(const SbVec4f & actual, const SbVec4f & expected, const std::string & label) {
  for (int i = 0; i != 4; ++i) close(actual[i], expected[i], label + "[" + std::to_string(i) + "]");
}
SoPrimitiveVertex vertex(const SbVec3f & p, bool second) {
  SoPrimitiveVertex v;
  v.setPoint(p);
  v.setNormal(second ? SbVec3f(0, 0.6f, 0.8f) : SbVec3f(1, 0, 0));
  v.setTextureCoords(second ? SbVec4f(11, 13, 17, 19) : SbVec4f(2, 3, 5, 7));
  v.setMaterialIndex(second ? 7 : 3);
  return v;
}
std::unique_ptr<SoPickedPoint> pick(SoNode * root, const SbVec3f & point,
                                  const SbVec3f & direction = SbVec3f(0, 0, -1),
                                  float nearplane = 0, float farplane = 20) {
  SoRayPickAction action(SbViewportRegion(100, 100));
  action.setPickAll(TRUE);
  action.setRay(point - direction * 10, direction, nearplane, farplane);
  action.apply(root);
  SoPickedPoint * hit = action.getPickedPoint();
  return std::unique_ptr<SoPickedPoint>(hit ? hit->copy() : nullptr);
}
SbVec3f normalAt(const SoPrimitiveVertex & a, const SoPrimitiveVertex & b, float t) {
  SbVec3f normal = a.getNormal() * (1 - t) + b.getNormal() * t;
  normal.normalize();
  return normal;
}
void attributes(const SoPickedPoint & hit, const SoPrimitiveVertex & a,
                const SoPrimitiveVertex & b, float t, const std::string & group) {
  // Contract: endpoint weights are (1-t,t), including all four texture components.
  if (group == "normal" || group == "all")
    close(hit.getObjectNormal(), normalAt(a, b, t), "object normal");
  if (group == "texture" || group == "all")
    close(hit.getObjectTextureCoords(), a.getTextureCoords() * (1-t) + b.getTextureCoords() * t,
          "object texture (no homogenization)");
  if (group == "material" || group == "all")
    require(hit.getMaterialIndex() == (t <= 0.5f ? a.getMaterialIndex() : b.getMaterialIndex()),
            "nearest endpoint material; midpoint belongs to first endpoint");
}
void customCase(const std::string & group, const SbVec3f & a, const SbVec3f & b,
                float t, bool reversed, bool direct, bool same = false,
                bool transformed = false) {
  Scene scene;
  SbMatrix matrix = SbMatrix::identity();
  if (transformed) {
    matrix.setTransform(SbVec3f(3,-4,2), SbRotation::identity(), SbVec3f(2,3,4));
    SoMatrixTransform * transform = new SoMatrixTransform;
    transform->matrix = matrix; scene.root->addChild(transform);
  }
  Segment * shape = new Segment;
  scene.root->addChild(shape);
  shape->first = vertex(a, false); shape->second = vertex(b, true);
  if (same) {
    shape->second.setNormal(shape->first.getNormal());
    shape->second.setTextureCoords(shape->first.getTextureCoords());
    shape->second.setMaterialIndex(shape->first.getMaterialIndex());
  }
  if (reversed) std::swap(shape->first, shape->second);
  shape->direct = direct;
  const SbVec3f objectpoint = shape->first.getPoint() * (1-t) + shape->second.getPoint() * t;
  SbVec3f worldpoint; matrix.multVecMatrix(objectpoint, worldpoint);
  std::unique_ptr<SoPickedPoint> hit = pick(scene.root, worldpoint);
  require(bool(hit), "expected segment hit");
  close(hit->getObjectPoint(), objectpoint, "object point");
  close(hit->getPoint(), worldpoint, "world point");
  require(hit->isOnGeometry(), "pick must remain on geometry");
  attributes(*hit, shape->first, shape->second, t, group);
  if (transformed && (group == "normal" || group == "all")) {
    SbVec3f worldnormal;
    // Preserve the existing SoPickedPoint transform: model matrix direction,
    // with no additional normalization of the world-space result.
    matrix.multDirMatrix(normalAt(shape->first, shape->second, t), worldnormal);
    close(hit->getNormal(), worldnormal, "existing world normal transform");
  }
  // Copies must keep the attributes after the ray action and original picked point die.
  std::unique_ptr<SoPickedPoint> copy(hit->copy()); hit.reset();
  attributes(*copy, shape->first, shape->second, t, group);
}
struct Primitive { SoPrimitiveVertex a, b; };
void capture(void * data, SoCallbackAction *, const SoPrimitiveVertex * a,
             const SoPrimitiveVertex * b) {
  // Details have callback lifetime; only value attributes are used after traversal.
  Primitive p; p.a = *a; p.b = *b; p.a.setDetail(nullptr); p.b.setDetail(nullptr);
  static_cast<std::vector<Primitive> *>(data)->push_back(p);
}
std::vector<Primitive> primitives(SoNode * root) {
  std::vector<Primitive> result;
  SoCallbackAction action;
  action.addLineSegmentCallback(SoShape::getClassTypeId(), capture, &result);
  action.apply(root); return result;
}
void controls() {
  for (bool direct : {false, true}) {
    for (float t : {0.0f, 0.25f, 0.5f, 0.75f, 1.0f})
      customCase("all", SbVec3f(0,0,0), SbVec3f(8,0,0), t, false, direct, true);
    customCase("all", SbVec3f(0,0,0), SbVec3f(8,0,0), 0.5f, false, direct);
    customCase("all", SbVec3f(0,0,0), SbVec3f(8,0,0), 0.25f, false, direct, true, true);
    // Zero-length fallback remains first endpoint, without new divisions or NaNs.
    customCase("all", SbVec3f(2,3,4), SbVec3f(2,3,4), 0, false, direct);
    Scene scene; Segment * shape = new Segment; scene.root->addChild(shape);
    shape->first = vertex(SbVec3f(0,0,0), false);
    shape->second = vertex(SbVec3f(8,0,0), true); shape->direct = direct;
    const auto emitted = primitives(scene.root);
    require(emitted.size() == 1, "one segment callback");
    close(emitted[0].a.getNormal(), shape->first.getNormal(), "callback first normal");
    close(emitted[0].b.getTextureCoords(), shape->second.getTextureCoords(), "callback second texture");
    require(emitted[0].a.getMaterialIndex() == 3 && emitted[0].b.getMaterialIndex() == 7,
            "callback endpoint materials");
    SoGetPrimitiveCountAction count; count.apply(scene.root);
    require(count.getLineCount() == 1 && count.getTriangleCount() == 0 && count.getPointCount() == 0,
            "primitive counts unchanged");
    require(!pick(scene.root, SbVec3f(4,2,0)), "ray outside segment must miss");
    require(!pick(scene.root, SbVec3f(-2,0,0)), "ray beyond endpoint must miss");
    require(!pick(scene.root, SbVec3f(4,0,0), SbVec3f(0,0,-1), 11, 20), "near plane excludes hit");
    require(!pick(scene.root, SbVec3f(4,0,0), SbVec3f(0,0,-1), 0, 9), "far plane excludes hit");
  }
}
void nativeCase(bool vp, bool overall, int start, bool polyline) {
  Scene scene;
  const SbVec3f points[] = {SbVec3f(-8,3,0), SbVec3f(0,0,0), SbVec3f(8,0,0), SbVec3f(8,6,0)};
  const SbVec3f normals[] = {SbVec3f(1,0,0), SbVec3f(0,0.6f,0.8f), SbVec3f(0,1,0), SbVec3f(0,0,1)};
  SoLineSet * lines = new SoLineSet;
  lines->startIndex = start; lines->numVertices.set1Value(0, polyline ? 3 : 2);
  if (vp) {
    SoVertexProperty * property = new SoVertexProperty;
    property->vertex.setValues(0, 4, points); property->normal.setValues(0, 4, normals);
    const uint32_t rgba[] = {0xff0000ff, 0x00ff00ff, 0x0000ffff, 0xffffffff};
    property->orderedRGBA.setValues(0, 4, rgba);
    property->normalBinding = overall ? SoVertexProperty::OVERALL : SoVertexProperty::PER_VERTEX;
    property->materialBinding = overall ? SoVertexProperty::OVERALL : SoVertexProperty::PER_VERTEX;
    lines->vertexProperty = property;
  } else {
    SoCoordinate3 * coordinates = new SoCoordinate3;
    coordinates->point.setValues(0, 4, points); scene.root->addChild(coordinates);
    SoNormal * normal = new SoNormal; normal->vector.setValues(0,4,normals); scene.root->addChild(normal);
    SoNormalBinding * nb = new SoNormalBinding;
    nb->value = overall ? SoNormalBinding::OVERALL : SoNormalBinding::PER_VERTEX;
    scene.root->addChild(nb);
    SoMaterial * material = new SoMaterial;
    material->diffuseColor.set1Value(0, 1,0,0); material->diffuseColor.set1Value(1, 0,1,0);
    material->diffuseColor.set1Value(2, 0,0,1); scene.root->addChild(material);
    SoMaterialBinding * mb = new SoMaterialBinding;
    mb->value = overall ? SoMaterialBinding::OVERALL : SoMaterialBinding::PER_VERTEX;
    scene.root->addChild(mb);
  }
  scene.root->addChild(lines);
  const auto emitted = primitives(scene.root);
  require(emitted.size() == (polyline ? 2u : 1u), "native callback count");
  SoGetPrimitiveCountAction count; count.apply(scene.root);
  require(count.getLineCount() == int(emitted.size()), "native primitive count");
  for (size_t segment = 0; segment < emitted.size(); ++segment) {
    const Primitive & p = emitted[segment];
    // startIndex selects coordinates; normal/material arrays start at zero in this baseline.
    close(p.a.getPoint(), points[start + segment], "native first coordinate");
    close(p.b.getPoint(), points[start + segment + 1], "native second coordinate");
    close(p.a.getNormal(), normals[overall ? 0 : segment], "native first normal mapping");
    close(p.b.getNormal(), normals[overall ? 0 : segment+1], "native second normal mapping");
    require(p.a.getMaterialIndex() == (overall ? 0 : int(segment)) &&
            p.b.getMaterialIndex() == (overall ? 0 : int(segment+1)), "native material mapping");
    for (float t : {0.25f, 0.5f, 0.75f}) {
      auto hit = pick(scene.root, p.a.getPoint() * (1-t) + p.b.getPoint() * t);
      require(bool(hit), "expected native hit");
      attributes(*hit, p.a, p.b, t, "normal");
      attributes(*hit, p.a, p.b, t, "material");
      // Longer strips have independent baseline detail issues: compare only primitive values there.
      if (!polyline) {
        const SoDetail * detail = hit->getDetail(lines);
        require(detail && detail->isOfType(SoLineDetail::getClassTypeId()), "line detail type");
        const SoLineDetail * line = static_cast<const SoLineDetail *>(detail);
        require(line->getPoint0()->getCoordinateIndex() == start &&
                line->getPoint1()->getCoordinateIndex() == start+1, "line coordinate details");
        require(line->getPoint0()->getNormalIndex() == 0 &&
                line->getPoint1()->getNormalIndex() == (overall ? 0 : 1), "line normal details");
        require(line->getPoint0()->getMaterialIndex() == 0 &&
                line->getPoint1()->getMaterialIndex() == (overall ? 0 : 1), "line material details");
      }
    }
  }
}
int main(int argc, char ** argv) {
  Database database;
  int failures = 0;
  auto run = [&failures](const std::string & label, const std::function<void()> & body) {
    try { body(); }
    catch (const std::exception & e) { ++failures; std::cerr << label << ": " << e.what() << '\n'; }
  };
  const std::string group = argc == 2 ? argv[1] : "";
  if (group == "normal" || group == "texture" || group == "material") {
    const SbVec3f starts[] = {SbVec3f(0,0,0), SbVec3f(-3,2,5), SbVec3f(20,-10,7)};
    const SbVec3f ends[] = {SbVec3f(8,0,0), SbVec3f(5,6,9), SbVec3f(20.5f,-10,7)};
    for (int geometry = 0; geometry < 3; ++geometry)
      for (bool reversed : {false, true}) for (bool direct : {false, true})
        for (float t : {0.0f, 0.25f, 0.499f, 0.5f, 0.501f, 0.75f, 1.0f}) {
          const std::string label = group + " geometry=" + std::to_string(geometry) +
            " reversed=" + std::to_string(reversed) + " direct=" + std::to_string(direct) +
            " t=" + std::to_string(t);
          run(label, [&] { customCase(group, starts[geometry], ends[geometry], t, reversed, direct); });
        }
    for (float t : {0.25f, 0.75f})
      run(group + " nonuniform transform", [&] {
        customCase(group, starts[1], ends[1], t, false, false, false, true);
      });
  } else if (group == "controls") run("controls", controls);
  else if (group == "native") {
    for (bool vp : {false,true}) for (bool overall : {false,true}) for (int start : {0,1})
      for (bool polyline : {false,true}) {
        if (polyline && start == 0) continue; // Keep longer-strip test non-self-overlapping.
        run("native vp=" + std::to_string(vp) + " overall=" + std::to_string(overall) +
            " start=" + std::to_string(start) + " polyline=" + std::to_string(polyline),
            [&] { nativeCase(vp, overall, start, polyline); });
      }
  } else { std::cerr << "expected normal|texture|material|controls|native\n"; return 2; }
  return failures ? 1 : 0;
}
