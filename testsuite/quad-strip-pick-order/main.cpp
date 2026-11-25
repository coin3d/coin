#include <Inventor/SoDB.h>
#include <Inventor/SbColor.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/details/SoFaceDetail.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoQuadMesh.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoTextureCoordinate2.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/lists/SoPickedPointList.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>

static void require(bool value, const std::string & message) {
  if (!value) throw std::runtime_error(message);
}
struct Point {
  int coordinate, normal, material, texture;
  bool operator==(const Point & p) const {
    return coordinate == p.coordinate && normal == p.normal &&
      material == p.material && texture == p.texture;
  }
};
struct Face {
  int face, part;
  std::vector<Point> points;
  bool operator==(const Face & f) const {
    return face == f.face && part == f.part && points == f.points;
  }
};
static std::string describe(const Face & f) {
  std::ostringstream out;
  out << "face=" << f.face << " part=" << f.part << " points=";
  for (const Point & p : f.points)
    out << '(' << p.coordinate << ',' << p.normal << ',' << p.material << ',' << p.texture << ')';
  return out.str();
}
struct UnrefScene {
  void operator()(SoSeparator * root) const { root->unref(); }
};
typedef std::unique_ptr<SoSeparator, UnrefScene> SceneOwner;
static SceneOwner makeScene() {
  SoSeparator * root = new SoSeparator;
  root->ref();
  return SceneOwner(root);
}
struct DatabaseScope {
  DatabaseScope() { SoDB::init(); }
  ~DatabaseScope() { SoDB::finish(); }
};
static Face readFace(const SoDetail * detail) {
  require(detail && detail->isOfType(SoFaceDetail::getClassTypeId()), "missing SoFaceDetail");
  const SoFaceDetail * f = static_cast<const SoFaceDetail *>(detail);
  Face result;
  result.face = f->getFaceIndex(); result.part = f->getPartIndex();
  for (int i = 0; i < f->getNumPoints(); ++i) {
    const SoPointDetail * p = f->getPoint(i);
    result.points.push_back(Point{p->getCoordinateIndex(), p->getNormalIndex(),
                                 p->getMaterialIndex(), p->getTextureCoordIndex()});
  }
  return result;
}
static Point tagged(int id) {
  // Distinct streams detect swapping only coordinates or losing another index.
  return Point{id, 40 + id * 3, 100 + id * 5, 200 + id * 7};
}
static void setPoint(SoPointDetail & detail, const Point & p) {
  detail.setCoordinateIndex(p.coordinate); detail.setNormalIndex(p.normal);
  detail.setMaterialIndex(p.material); detail.setTextureCoordIndex(p.texture);
}

class StripProbe : public SoShape {
  SO_NODE_HEADER(StripProbe);
public:
  static void initClass() { SO_NODE_INIT_CLASS(StripProbe, SoShape, "Shape"); }
  StripProbe() : mode(QUAD_STRIP) { SO_NODE_CONSTRUCTOR(StripProbe); }
  TriangleShape mode;
  static SbVec3f position(int strip, int column, bool bottom) {
    const float xs[] = {0, 2, 5, 9};
    return SbVec3f(xs[column], float(strip * 4 + (bottom ? 0 : 2)), 0);
  }
  void computeBBox(SoAction *, SbBox3f & box, SbVec3f & center) override {
    box.setBounds(SbVec3f(0, 0, 0), SbVec3f(9, 6, 0)); center = box.getCenter();
  }
protected:
  ~StripProbe() override {}
  void generatePrimitives(SoAction * action) override {
    SoFaceDetail face;
    SoPointDetail detail;
    SoPrimitiveVertex vertex;
    vertex.setDetail(&detail); vertex.setNormal(0, 0, 1);
    for (int strip = 0; strip < 2; ++strip) {
      face.setFaceIndex(17); face.setPartIndex(9 + strip);
      beginShape(action, mode, &face);
      if (mode == QUAD_STRIP) {
        for (int col = 0; col < 4; ++col) {
          for (int bottom = 0; bottom < 2; ++bottom) {
            const int id = strip * 8 + col * 2 + bottom;
            emit(vertex, detail, id, position(strip, col, bottom != 0));
          }
          if (col) face.incFaceIndex();
        }
      }
      else {
        for (int col = 0; col < 3; ++col) {
          const int ids[] = {strip * 8 + col * 2, strip * 8 + col * 2 + 1,
                             strip * 8 + col * 2 + 3, strip * 8 + col * 2 + 2};
          const SbVec3f p[] = {position(strip,col,false),position(strip,col,true),
                               position(strip,col+1,true),position(strip,col+1,false)};
          const int order[] = {0, 1, 2, 0, 2, 3};
          const int count = mode == QUADS ? 4 : 6;
          for (int i = 0; i < count; ++i) {
            int k = mode == QUADS ? i : order[i];
            emit(vertex, detail, ids[k], p[k]);
            if (mode == TRIANGLES && i == 2) face.incFaceIndex();
          }
          face.incFaceIndex();
        }
      }
      endShape();
    }
  }
  void emit(SoPrimitiveVertex & v, SoPointDetail & d, int id, const SbVec3f & p) {
    const Point t = tagged(id); setPoint(d, t);
    v.setPoint(p); v.setMaterialIndex(t.material);
    v.setTextureCoords(float(t.texture), 0); shapeVertex(&v);
  }
};
SO_NODE_SOURCE(StripProbe);

struct Triangle {
  Face detail;
  SbVec3f p[3];
};
static void triangleCB(void * data, SoCallbackAction *, const SoPrimitiveVertex * a,
                       const SoPrimitiveVertex * b, const SoPrimitiveVertex * c) {
  std::vector<Triangle> & triangles = *static_cast<std::vector<Triangle> *>(data);
  Triangle t; t.detail = readFace(a->getDetail());
  require(readFace(b->getDetail()) == t.detail && readFace(c->getDetail()) == t.detail,
          "triangle callback vertices disagree about polygon");
  t.p[0] = a->getPoint(); t.p[1] = b->getPoint(); t.p[2] = c->getPoint();
  triangles.push_back(t);
}
static std::vector<Triangle> callbacks(SoNode * root) {
  std::vector<Triangle> triangles;
  SoCallbackAction cb; cb.addTriangleCallback(SoShape::getClassTypeId(), triangleCB, &triangles);
  cb.apply(root); return triangles;
}
static std::unique_ptr<SoPickedPoint> pick(SoNode * root, const SbVec3f & p) {
  SoRayPickAction action(SbViewportRegion(64, 64));
  action.setPickAll(TRUE); action.setRay(p + SbVec3f(0,0,10), SbVec3f(0,0,-1), 0, 20);
  action.apply(root);
  require(action.getPickedPointList().getLength() == 1, "interior ray should hit exactly one triangle");
  return std::unique_ptr<SoPickedPoint>(action.getPickedPoint()->copy());
}
static void checkHits(SceneOwner & scene, const std::vector<Triangle> & triangles) {
  SoSeparator * root = scene.get();
  std::vector<std::unique_ptr<SoPickedPoint> > retained;
  std::vector<std::unique_ptr<SoDetail> > faces;
  std::vector<Face> expected;
  for (const Triangle & tri : triangles) {
    // Unequal barycentric weights keep hits away from edges and the shared diagonal.
    const SbVec3f p = tri.p[0]*0.23f + tri.p[1]*0.31f + tri.p[2]*0.46f;
    std::unique_ptr<SoPickedPoint> pp = pick(root, p);
    const Face got = readFace(pp->getDetail());
    require(got == tri.detail, "picked polygon point sequence differs from callback: got " +
            describe(got) + "; expected " + describe(tri.detail));
    require((pp->getPoint() - p).length() < 1e-4f, "hit location changed");
    require((pp->getNormal() - SbVec3f(0,0,1)).length() < 1e-4f, "hit normal changed");
    require(pp->isOnGeometry(), "hit must be on geometry");
    faces.emplace_back(pp->getDetail()->copy());
    expected.push_back(got); retained.push_back(std::move(pp));
  }
  {
    SoRayPickAction miss(SbViewportRegion(64,64));
    miss.setRay(SbVec3f(-10,-10,10), SbVec3f(0,0,-1), 0,20); miss.apply(root);
    require(miss.getPickedPoint() == NULL, "outside ray should miss");
  }
  // Saved details must survive more traversals, removal of the scene's reference,
  // and destruction of all actions. PickedPoint itself keeps its path alive.
  callbacks(root);
  scene.reset();
  for (size_t i = 0; i < retained.size(); ++i) {
    require(readFace(retained[i]->getDetail()) == expected[i], "picked point copy was aliased");
    std::unique_ptr<SoPickedPoint> copy(retained[i]->copy());
    retained[i].reset();
    require(readFace(copy->getDetail()) == expected[i], "second picked point copy was aliased");
  }
  retained.clear();
  for (size_t i = 0; i < faces.size(); ++i)
    require(readFace(faces[i].get()) == expected[i], "face detail copy did not outlive scene");
}
static void generic(SoShape::TriangleShape mode) {
  SceneOwner scene = makeScene();
  SoSeparator * root = scene.get();
  StripProbe * shape = new StripProbe; shape->mode = mode; root->addChild(shape);
  const std::vector<Triangle> tris = callbacks(root);
  require(tris.size() == 12, "generic shape triangulation count changed");
  for (size_t i = 0; i < tris.size(); ++i) {
    const int strip = int(i / 6), col = int((i % 6) / 2);
    int first = strip*8 + col*2;
    const int ids[] = {first, first+1, first+3, first+2};
    Face expected; expected.part = 9 + strip;
    if (mode == SoShape::TRIANGLES) {
      expected.face = 17 + col*2 + int(i%2);
      const int order[2][3] = {{0,1,2},{0,2,3}};
      for (int j = 0; j < 3; ++j) expected.points.push_back(tagged(ids[order[i%2][j]]));
    }
    else {
      expected.face = 17 + col;
      for (int j = 0; j < 4; ++j) expected.points.push_back(tagged(ids[j]));
    }
    require(tris[i].detail == expected, "generic callback perimeter, indices, or face metadata changed");
  }
  checkHits(scene, tris);
}
static void detailCopy() {
  std::unique_ptr<SoDetail> copy;
  Face expected;
  {
    SoFaceDetail original; original.setNumPoints(4);
    original.setFaceIndex(28); original.setPartIndex(37);
    for (int i = 0; i < 4; ++i) {
      SoPointDetail point; setPoint(point, tagged(i)); original.setPoint(i, &point);
      setPoint(point, tagged(99)); // setPoint must copy its input too.
    }
    expected = readFace(&original); copy.reset(original.copy());
    original.getPoints()[0].setCoordinateIndex(999);
    original.setNumPoints(1); original.setFaceIndex(99); original.setPartIndex(88);
    require(readFace(copy.get()) == expected, "SoFaceDetail::copy shares point storage");
  }
  require(readFace(copy.get()) == expected, "SoFaceDetail::copy depends on destroyed source");
}
static void mesh(int width, int height, int start, bool vp, bool pervertex) {
  SceneOwner scene = makeScene();
  SoSeparator * root = scene.get();
  std::vector<SbVec3f> coordinates(start, SbVec3f(-100,-100,0));
  std::vector<SbVec3f> normals(width*height, SbVec3f(0,0,1));
  std::vector<SbColor> colors(width*height);
  std::vector<SbVec2f> texcoords;
  for (int row = 0; row < height; ++row) {
    for (int col = 0; col < width; ++col) {
      coordinates.push_back(SbVec3f(float(col*2), float(-row*3), 0));
      texcoords.push_back(SbVec2f(float(col),float(row)));
      colors[row*width+col].setValue(float(col)/width, float(row)/height, 0.5f);
    }
  }
  SoQuadMesh * shape = new SoQuadMesh;
  shape->verticesPerRow = width; shape->verticesPerColumn = height; shape->startIndex = start;
  if (vp) {
    SoVertexProperty * vertices = new SoVertexProperty;
    vertices->vertex.setValues(0,int(coordinates.size()),coordinates.data());
    vertices->normal.setValues(0,int(normals.size()),normals.data());
    vertices->texCoord.setValues(0,int(texcoords.size()),texcoords.data());
    vertices->normalBinding = pervertex ? SoVertexProperty::PER_VERTEX : SoVertexProperty::OVERALL;
    vertices->materialBinding = pervertex ? SoVertexProperty::PER_VERTEX : SoVertexProperty::OVERALL;
    for (int i = 0; i < width*height; ++i)
      vertices->orderedRGBA.set1Value(i, uint32_t(0x204060ffu + uint32_t(i)*0x01010000u));
    shape->vertexProperty = vertices;
  }
  else {
    SoCoordinate3 * coords = new SoCoordinate3;
    coords->point.setValues(0,int(coordinates.size()),coordinates.data()); root->addChild(coords);
    SoNormal * n = new SoNormal;
    n->vector.setValues(0,int(normals.size()),normals.data()); root->addChild(n);
    SoNormalBinding * nb = new SoNormalBinding;
    nb->value = pervertex ? SoNormalBinding::PER_VERTEX : SoNormalBinding::OVERALL;
    root->addChild(nb);
    SoMaterial * material = new SoMaterial;
    material->diffuseColor.setValues(0,int(colors.size()),colors.data()); root->addChild(material);
    SoMaterialBinding * mb = new SoMaterialBinding;
    mb->value = pervertex ? SoMaterialBinding::PER_VERTEX : SoMaterialBinding::OVERALL;
    root->addChild(mb);
    SoTextureCoordinate2 * texture = new SoTextureCoordinate2;
    texture->point.setValues(0,int(texcoords.size()),texcoords.data()); root->addChild(texture);
  }
  root->addChild(shape);
  const std::vector<Triangle> tris = callbacks(root);
  require(tris.size() == size_t(2*(width-1)*(height-1)), "mesh triangle count changed");
  for (size_t i = 0; i < tris.size(); ++i) {
    const int quad = int(i/2), row = quad/(width-1), col = quad%(width-1);
    const int first = row*width+col;
    const int indices[] = {first, first+width, first+width+1, first+1};
    const Face & f = tris[i].detail;
    require(f.face == col && f.part == row, "mesh faceIndex/partIndex changed");
    require(f.points.size() == 4, "mesh detail must describe whole quad");
    for (int j = 0; j < 4; ++j) {
      require(f.points[j].coordinate == start+indices[j], "mesh callback perimeter changed");
      require(f.points[j].normal == (pervertex ? indices[j] : 0), "mesh normal index changed");
      require(f.points[j].material == (pervertex ? indices[j] : 0), "mesh material index changed");
    }
  }
  checkHits(scene, tris);
}
int main(int argc, char ** argv) {
  try {
    require(argc == 2, "provide scenario name");
    DatabaseScope database; StripProbe::initClass();
    const std::string scenario(argv[1]);
    if (scenario == "generic-strip") generic(SoShape::QUAD_STRIP);
    else if (scenario == "generic-quads") generic(SoShape::QUADS);
    else if (scenario == "generic-triangles") generic(SoShape::TRIANGLES);
    else if (scenario == "detail-copy") detailCopy();
    else if (scenario == "mesh-state" || scenario == "mesh-vertex-property") {
      const int dimensions[][2] = {{2,2},{3,2},{2,3},{3,3},{4,3}};
      for (const auto & d : dimensions)
        for (int start : {0,3})
          for (bool pervertex : {false,true})
            mesh(d[0], d[1], start, scenario == "mesh-vertex-property", pervertex);
    }
    else throw std::runtime_error("unknown scenario: " + scenario);
    return EXIT_SUCCESS;
  }
  catch (const std::exception & error) {
    std::cerr << (argc > 1 ? argv[1] : "test") << ": " << error.what() << '\n';
    return EXIT_FAILURE;
  }
}
