#include <Inventor/SoDB.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/SoPath.h>
#include <Inventor/lists/SoPickedPointList.h>
#include <Inventor/SbRotation.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/details/SoPointDetail.h>
#include <Inventor/nodes/SoMatrixTransform.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoShape.h>
#include <cmath>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
const SbVec3f tangentU(1, 0, 1), tangentV(0, 1, 1);
const SbVec4f texture(0.2f, 0.3f, 0.4f, 1);
const float tolerance = 0.00015f;
SbVec3f unit(SbVec3f v) { v.normalize(); return v; }
SbVec3f authored() { return unit(tangentU.cross(tangentV)); }
void require(bool condition, const std::string & what) {
  if (!condition) throw std::runtime_error(what);
}
void near(const SbVec3f & actual, const SbVec3f & expected,
          const std::string & what) {
  require(std::isfinite(actual[0]) && std::isfinite(actual[1]) &&
          std::isfinite(actual[2]) && (actual - expected).length() < tolerance, what);
}
SbVec3f direction(const SbMatrix & m, const SbVec3f & v) {
  SbVec3f result; m.multDirMatrix(v, result); return result;
}
SbVec3f point(const SbMatrix & m, const SbVec3f & v) {
  SbVec3f result; m.multVecMatrix(v, result); return result;
}
// A plane's transformed tangents are an independent geometric oracle. The
// determinant sign preserves authored orientation under a reflection.
SbVec3f geometricNormal(const SbMatrix & m) {
  return unit(direction(m, tangentU).cross(direction(m, tangentV))) *
    (m.det3() < 0 ? -1.0f : 1.0f);
}
void planeNormal(const SbVec3f & n, const SbMatrix & m, const std::string & label) {
  near(n, geometricNormal(m), label + ": authored geometric normal");
  require(std::fabs(n.length() - 1) < tolerance, label + ": unit length");
  require(std::fabs(n.dot(unit(direction(m, tangentU)))) < tolerance &&
          std::fabs(n.dot(unit(direction(m, tangentV)))) < tolerance,
          label + ": perpendicular to both transformed tangents");
}

class ProbeTriangle : public SoShape {
  SO_NODE_HEADER(ProbeTriangle);
public:
  static void initClass() { SO_NODE_INIT_CLASS(ProbeTriangle, SoShape, "Shape"); }
  ProbeTriangle() : rawNormal(authored()) { SO_NODE_CONSTRUCTOR(ProbeTriangle); }
  SbVec3f rawNormal;
  void rayPick(SoRayPickAction * action) override {
    if (!shouldRayPick(action)) return;
    computeObjectSpaceRay(action);
    SbVec3f hit, barycentric; SbBool front;
    if (!action->intersect(-tangentU - tangentV, tangentU, tangentV,
                           hit, barycentric, front)) return;
    SoPickedPoint * pp = action->addIntersection(hit, front);
    if (!pp) return;
    // Setters are called only while the action's SoState is alive.
    pp->setObjectNormal(rawNormal);
    pp->setObjectTextureCoords(texture);
    pp->setMaterialIndex(7);
    SoPointDetail * detail = new SoPointDetail;
    detail->setCoordinateIndex(23);
    pp->setDetail(detail, this);
  }
  void computeBBox(SoAction *, SbBox3f & box, SbVec3f & center) override {
    box.makeEmpty(); box.extendBy(-tangentU - tangentV);
    box.extendBy(tangentU); box.extendBy(tangentV); center.setValue(0, 0, 0);
  }
protected:
  ~ProbeTriangle() override {}
  void generatePrimitives(SoAction * action) override {
    beginShape(action, TRIANGLES);
    SoPrimitiveVertex vertex;
    vertex.setNormal(authored()); vertex.setTextureCoords(texture);
    vertex.setMaterialIndex(7);
    vertex.setPoint(-tangentU - tangentV); shapeVertex(&vertex);
    vertex.setPoint(tangentU); shapeVertex(&vertex);
    vertex.setPoint(tangentV); shapeVertex(&vertex);
    endShape();
  }
};
SO_NODE_SOURCE(ProbeTriangle);

struct Scene {
  SoSeparator * root;
  SoSeparator * ancestor1;
  SoSeparator * ancestor2;
  ProbeTriangle * shape;
  Scene(const SbMatrix & outer, const SbMatrix & middle, const SbMatrix & inner) {
    root = new SoSeparator; root->ref();
    SoMatrixTransform * a = new SoMatrixTransform; a->matrix = outer; root->addChild(a);
    ancestor1 = new SoSeparator; root->addChild(ancestor1);
    SoMatrixTransform * b = new SoMatrixTransform; b->matrix = middle; ancestor1->addChild(b);
    ancestor2 = new SoSeparator; ancestor1->addChild(ancestor2);
    SoMatrixTransform * c = new SoMatrixTransform; c->matrix = inner; ancestor2->addChild(c);
    shape = new ProbeTriangle; ancestor2->addChild(shape);
  }
  ~Scene() { root->unref(); }
  Scene(const Scene &) = delete;
  Scene & operator=(const Scene &) = delete;
  SbMatrix matrix() const {
    // A path action supplies the scene's matrix without duplicating the
    // ordering convention used by scene traversal.
    SoPath * path = new SoPath(root); path->ref();
    path->append(1); path->append(1); path->append(1);
    SoGetMatrixAction action(SbViewportRegion(100, 100)); action.apply(path);
    SbMatrix result = action.getMatrix(); path->unref(); return result;
  }
};
SbMatrix scale(float x, float y, float z) { SbMatrix m; m.setScale(SbVec3f(x,y,z)); return m; }
SbMatrix rotation() { SbMatrix m; m.setRotate(SbRotation(unit(SbVec3f(2,3,1)), 0.71f)); return m; }
SbMatrix translation() { SbMatrix m; m.setTranslate(SbVec3f(2,-3,4)); return m; }
SbMatrix shear() {
  SbMatrix m = SbMatrix::identity();
  m[0][1] = 0.6f; m[1][2] = -0.35f; m[2][0] = 0.2f;
  return m;
}
void aim(SoRayPickAction & action, const SbMatrix & m) {
  const SbVec3f center = point(m, SbVec3f(0,0,0));
  const SbVec3f n = geometricNormal(m);
  action.setRay(center + n * 10, -n, 0, 30);
  action.setPickAll(TRUE);
}
void attributes(const SoPickedPoint & pp, const Scene & scene, const SbMatrix & m) {
  near(pp.getPoint(), point(m, SbVec3f(0,0,0)), "world hit position");
  near(pp.getObjectPoint(), SbVec3f(0,0,0), "object hit position");
  require(pp.getPath()->getHead() == scene.root && pp.getPath()->getTail() == scene.shape,
          "picked path endpoints");
  require(pp.getMaterialIndex() == 7 && pp.isOnGeometry(), "material and geometry status");
  for (int i=0; i<4; ++i) {
    require(std::fabs(pp.getTextureCoords()[i] - texture[i]) < tolerance, "world texture");
    require(std::fabs(pp.getObjectTextureCoords()[i] - texture[i]) < tolerance, "object texture");
  }
  const SoDetail * detail = pp.getDetail();
  require(detail && detail->isOfType(SoPointDetail::getClassTypeId()), "detail type");
  require(static_cast<const SoPointDetail *>(detail)->getCoordinateIndex() == 23,
          "detail contents");
}
void hierarchy(const SoPickedPoint & pp, const Scene & scene, const SbMatrix & m) {
  const SoNode * nodes[] = {scene.root, scene.ancestor1, scene.ancestor2};
  for (int i=0; i<3; ++i) {
    const SbMatrix worldToNode = pp.getWorldToObject(nodes[i]);
    SbMatrix tailToNode = m; tailToNode.multRight(worldToNode);
    const SbVec3f n = pp.getObjectNormal(nodes[i]);
    planeNormal(n, tailToNode, "ancestor " + std::to_string(i));
    near(pp.getObjectPoint(nodes[i]), point(worldToNode, pp.getPoint()), "ancestor position");
    const SbMatrix nodeToWorld = pp.getObjectToWorld(nodes[i]);
    // Secondary round trip checks the direction in addition to geometry.
    const SbVec3f roundtrip = unit(direction(nodeToWorld.inverse().transpose(), n));
    near(roundtrip, pp.getNormal(), "ancestor world round trip");
  }
  near(pp.getObjectNormal(), scene.shape->rawNormal, "NULL keeps original object normal");
  near(pp.getObjectNormal(scene.shape), scene.shape->rawNormal, "tail keeps original object normal");
}
void callback(void * userdata, SoCallbackAction *, const SoPrimitiveVertex *,
              const SoPrimitiveVertex *, const SoPrimitiveVertex *) {
  ++*static_cast<int *>(userdata);
}
void traversalControls(Scene & scene) {
  int triangles = 0;
  SoCallbackAction cb; cb.addTriangleCallback(ProbeTriangle::getClassTypeId(), callback, &triangles);
  cb.apply(scene.root); require(triangles == 1, "triangle callback count");
  SoGetPrimitiveCountAction count; count.apply(scene.root);
  require(count.getTriangleCount() == 1 && count.getLineCount() == 0 &&
          count.getPointCount() == 0, "primitive counts");
}
void runScene(Scene & scene, bool checkHierarchy, bool ownership, bool zero = false) {
  const SbMatrix m = scene.matrix();
  std::unique_ptr<SoPickedPoint> copy, constructedCopy;
  {
    SoRayPickAction action(SbViewportRegion(100,100)); aim(action, m); action.apply(scene.root);
    SoPickedPoint * pp = action.getPickedPoint(); require(pp != NULL, "triangle must be hit");
    require(action.getPickedPointList().getLength() == 1, "exactly one hit");
    attributes(*pp, scene, m);
    if (zero) near(pp->getNormal(), SbVec3f(0,0,0), "zero remains finite zero");
    else planeNormal(pp->getNormal(), m, "world normal");
    near(pp->getObjectNormal(), scene.shape->rawNormal, "raw normal storage");
    near(pp->getObjectNormal(scene.shape), scene.shape->rawNormal, "raw tail storage");
    if (checkHierarchy) hierarchy(*pp, scene, m);
    copy.reset(pp->copy()); constructedCopy.reset(new SoPickedPoint(*pp));
  }
  if (ownership) {
    traversalControls(scene);
    { SoRayPickAction later(SbViewportRegion(100,100)); aim(later,m); later.apply(scene.root); }
    for (const SoPickedPoint * pp : {copy.get(), constructedCopy.get()}) {
      attributes(*pp, scene, m);
      if (zero) near(pp->getNormal(), SbVec3f(0,0,0), "copied zero");
      else planeNormal(pp->getNormal(), m, "copied normal after actions");
      near(pp->getObjectNormal(), scene.shape->rawNormal, "copied original normal");
      if (checkHierarchy) hierarchy(*pp, scene, m);
    }
    require(copy->getDetail() != constructedCopy->getDetail(), "details deep copied");
  }
}
void run(const std::string & group) {
  const SbMatrix identity = SbMatrix::identity();
  if (group == "world") {
    std::vector<SbMatrix> matrices = {scale(2,3,0.5f), shear(), scale(-2,3,0.5f)};
    SbMatrix rs = rotation(); rs.multRight(scale(2,3,0.5f)); matrices.push_back(rs);
    SbMatrix sr = scale(2,3,0.5f); sr.multRight(rotation()); matrices.push_back(sr);
    for (std::size_t i=0; i<matrices.size(); ++i) {
      try { Scene scene(identity,identity,matrices[i]); runScene(scene,false,false); }
      catch (const std::exception & e) { throw std::runtime_error("world case " + std::to_string(i) + ": " + e.what()); }
    }
  }
  else if (group == "hierarchy") {
    // World identity isolates the dual conversion into intermediate object spaces.
    {
      const SbMatrix outer = scale(2, 3, 0.5f);
      Scene cancelled(outer, identity, outer.inverse());
      runScene(cancelled, true, false);
    }
    Scene scene(shear(), rotation(), scale(-2,3,0.5f)); runScene(scene,true,false);
  }
  else if (group == "controls") {
    for (const SbMatrix & m : {identity, translation(), rotation(), scale(3,3,3)}) {
      Scene scene(identity,identity,m); runScene(scene,false,false); traversalControls(scene);
    }
  }
  else if (group == "ownership") {
    Scene scene(translation(), shear(), scale(2,3,0.5f));
    scene.shape->rawNormal = authored() * 4.25f; runScene(scene,true,true);
    scene.shape->rawNormal.setValue(0,0,0); runScene(scene,false,true,true);
  }
  else throw std::runtime_error("unknown test group: " + group);
}
} // namespace
int main(int argc, char ** argv) {
  SoDB::init(); ProbeTriangle::initClass();
  int status = 0;
  try { require(argc == 2, "one test group is required"); run(argv[1]); }
  catch (const std::exception & e) { std::cerr << e.what() << '\n'; status = 1; }
  SoDB::finish(); return status;
}
