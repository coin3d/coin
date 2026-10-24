#include <Inventor/SoDB.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/details/SoFaceDetail.h>
#include <Inventor/details/SoLineDetail.h>
#include <Inventor/details/SoPointDetail.h>
#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

static void check(bool ok, const char * message) {
  if (!ok) throw std::runtime_error(message);
}
struct Database {
  Database() { SoDB::init(); }
  ~Database() { SoDB::finish(); }
};
struct Unref { void operator()(SoNode * n) const { n->unref(); } };
template<class T> static std::unique_ptr<T, Unref> retain(T * n) {
  n->ref(); return std::unique_ptr<T, Unref>(n);
}

// Only the documented protected SoShape extension API is used by this fixture.
class StreamShape : public SoShape {
  SO_NODE_HEADER(StreamShape);
public:
  static void initClass() { SO_NODE_INIT_CLASS(StreamShape, SoShape, "Shape"); }
  StreamShape() : id(1), type(TRIANGLES), sequences(2), limit(-1), throwing(false), convex(false) {
    SO_NODE_CONSTRUCTOR(StreamShape);
  }
  int id;
  TriangleShape type;
  int sequences;
  int limit;
  bool throwing;
  bool convex;
  std::function<void(SoAction *, int, int)> hook;
  std::function<void()> detailHook;
  std::vector<SbVec3f> points() const {
    std::vector<SbVec3f> p;
    switch (type) {
    case TRIANGLES:
      p = {{0,0,0},{2,0,0},{0,2,0},{0,0,-1},{2,0,-1},{0,2,-1}}; break;
    case TRIANGLE_STRIP:
      p = {{0,0,0},{0,2,0},{2,0,0},{2,2,0},{4,0,0},{4,2,0}}; break;
    case TRIANGLE_FAN:
      p = {{1,1,0},{0,0,0},{2,0,0},{2,2,0},{0,2,0},{0,0,0}}; break;
    case QUADS:
      p = {{0,0,0},{2,0,0},{2,2,0},{0,2,0},{3,0,0},{5,0,0},{5,2,0},{3,2,0}}; break;
    case QUAD_STRIP:
      p = {{0,0,0},{0,2,0},{2,0,0},{2,2,0},{4,0,0},{4,2,0}}; break;
    case POLYGON:
      // An L-shaped polygon, area 7; no triangle order is assumed.
      p = convex ? std::vector<SbVec3f>{{0,0,0},{4,0,0},{4,4,0},{0,4,0}}
                 : std::vector<SbVec3f>{{0,0,0},{4,0,0},{4,1,0},{1,1,0},{1,4,0},{0,4,0}}; break;
    case LINES:
      p = {{0,0,0},{1,0,0},{2,0,0},{3,0,0},{4,0,0},{5,0,0}}; break;
    case LINE_STRIP:
      p = {{0,0,0},{1,0,0},{2,1,0},{3,1,0},{4,2,0}}; break;
    case POINTS:
      p = {{0,0,0},{1,0,0},{2,1,0},{3,1,0}}; break;
    }
    if (limit >= 0 && static_cast<int>(p.size()) > limit) p.resize(limit);
    return p;
  }
  void computeBBox(SoAction *, SbBox3f & box, SbVec3f & center) override {
    box.setBounds(SbVec3f(-1,-1,-2), SbVec3f(6,5,1)); center.setValue(2,2,0);
  }
protected:
  ~StreamShape() override {}
  void generatePrimitives(SoAction * action) override {
    // Capture per-invocation values, so same-node recursion does not change fixtures.
    const std::vector<SbVec3f> p = points();
    for (int s = 0; s < sequences; ++s) {
      SoFaceDetail face; face.setFaceIndex(100 * id + s); face.setPartIndex(200 * id + s);
      SoLineDetail line; line.setLineIndex(100 * id + s); line.setPartIndex(200 * id + s);
      const bool lines = type == LINES || type == LINE_STRIP;
      beginShape(action, type, type == POINTS ? NULL : lines ? static_cast<SoDetail *>(&line) : static_cast<SoDetail *>(&face));
      for (size_t i = 0; i < p.size(); ++i) {
        SoPointDetail detail;
        detail.setCoordinateIndex(1000 * id + 100 * s + static_cast<int>(i));
        detail.setMaterialIndex(10 * id + static_cast<int>(i));
        detail.setNormalIndex(20 * id + static_cast<int>(i));
        detail.setTextureCoordIndex(30 * id + static_cast<int>(i));
        SoPrimitiveVertex v;
        v.setPoint(p[i]); v.setNormal(0,0,1);
        v.setTextureCoords(static_cast<float>(id), static_cast<float>(s), static_cast<float>(i), 1);
        v.setMaterialIndex(id); v.setPackedColor(0x10203000u + static_cast<unsigned>(id));
        v.setDetail(&detail);
        shapeVertex(&v);
        if (hook) hook(action, s, static_cast<int>(i));
        if (throwing && i == 0) throw std::runtime_error("fixture throw");
      }
      endShape();
    }
  }
  SoDetail * createTriangleDetail(SoRayPickAction * a, const SoPrimitiveVertex * a0,
                                  const SoPrimitiveVertex * a1, const SoPrimitiveVertex * a2,
                                  SoPickedPoint * pp) override {
    if (detailHook) detailHook();
    return SoShape::createTriangleDetail(a, a0, a1, a2, pp);
  }
};
SO_NODE_SOURCE(StreamShape);

// Explicitly request general polygon tessellation, including concave polygons.
static void apply(SoAction & action, StreamShape * shape) {
  auto root=retain(new SoSeparator);
  SoShapeHints * hints=new SoShapeHints;
  hints->faceType=SoShapeHints::UNKNOWN_FACE_TYPE;
  root->addChild(hints); root->addChild(shape);
  action.apply(root.get());
}

struct Vertex {
  std::vector<double> data;
  bool operator==(const Vertex & other) const { return data == other.data; }
};
static void pointDetail(std::vector<double> & d, const SoPointDetail * p) {
  d.push_back(p->getCoordinateIndex()); d.push_back(p->getMaterialIndex());
  d.push_back(p->getNormalIndex()); d.push_back(p->getTextureCoordIndex());
}
static Vertex snapshot(const SoPrimitiveVertex * v) {
  Vertex r;
  for (int i=0;i<3;++i) r.data.push_back(v->getPoint()[i]);
  for (int i=0;i<3;++i) r.data.push_back(v->getNormal()[i]);
  for (int i=0;i<4;++i) r.data.push_back(v->getTextureCoords()[i]);
  r.data.push_back(v->getMaterialIndex()); r.data.push_back(v->getPackedColor());
  const SoDetail * d = v->getDetail();
  if (!d) { r.data.push_back(0); return r; }
  // Copies exercise independent ownership of the public detail snapshot, too.
  std::unique_ptr<SoDetail> copy(d->copy());
  if (copy->isOfType(SoFaceDetail::getClassTypeId())) {
    const SoFaceDetail * f = static_cast<const SoFaceDetail *>(copy.get());
    r.data.push_back(1); r.data.push_back(f->getFaceIndex()); r.data.push_back(f->getPartIndex());
    r.data.push_back(f->getNumPoints());
    for (int i=0;i<f->getNumPoints();++i) pointDetail(r.data, f->getPoint(i));
  } else if (copy->isOfType(SoLineDetail::getClassTypeId())) {
    const SoLineDetail * l = static_cast<const SoLineDetail *>(copy.get());
    r.data.push_back(2); r.data.push_back(l->getLineIndex()); r.data.push_back(l->getPartIndex());
    pointDetail(r.data,l->getPoint0()); pointDetail(r.data,l->getPoint1());
  } else if (copy->isOfType(SoPointDetail::getClassTypeId())) {
    r.data.push_back(3); pointDetail(r.data, static_cast<const SoPointDetail *>(copy.get()));
  } else check(false,"unexpected detail type");
  return r;
}
struct Primitive {
  std::vector<Vertex> vertices;
  bool operator==(const Primitive & p) const { return vertices == p.vertices; }
};
struct Recorder {
  std::vector<Primitive> primitives;
  std::function<void()> nesting;
  bool fired = false;
  int expectedId = 1;
  SoNode * expectedNode = NULL;
  void record(SoCallbackAction * action, std::initializer_list<const SoPrimitiveVertex *> vs) {
    check(action->getCurPathTail() == expectedNode,"callback routed to wrong shape");
    Primitive before;
    for (const SoPrimitiveVertex * v : vs) {
      check(v->getMaterialIndex() == expectedId,"wrong callback material/shape");
      check(v->getTextureCoords()[0] == expectedId,"wrong callback texture/shape");
      check(v->getNormal() == SbVec3f(0,0,1),"wrong callback normal");
      before.vertices.push_back(snapshot(v));
    }
    if (nesting && !fired) { fired = true; nesting(); }
    Primitive after;
    for (const SoPrimitiveVertex * v : vs) after.vertices.push_back(snapshot(v));
    check(before == after,"callback vertices/details changed across nested traversal");
    primitives.push_back(before);
  }
  static void tri(void * d, SoCallbackAction * a, const SoPrimitiveVertex * x,
                  const SoPrimitiveVertex * y, const SoPrimitiveVertex * z) { static_cast<Recorder *>(d)->record(a,{x,y,z}); }
  static void line(void * d, SoCallbackAction * a, const SoPrimitiveVertex * x,
                   const SoPrimitiveVertex * y) { static_cast<Recorder *>(d)->record(a,{x,y}); }
  static void point(void * d, SoCallbackAction * a, const SoPrimitiveVertex * x) { static_cast<Recorder *>(d)->record(a,{x}); }
};
static std::vector<Primitive> callbacks(StreamShape * shape, std::function<void()> nested = {}) {
  Recorder r; r.expectedId = shape->id; r.expectedNode = shape; r.nesting = nested;
  SoCallbackAction action;
  action.addTriangleCallback(StreamShape::getClassTypeId(), Recorder::tri, &r);
  action.addLineSegmentCallback(StreamShape::getClassTypeId(), Recorder::line, &r);
  action.addPointCallback(StreamShape::getClassTypeId(), Recorder::point, &r);
  apply(action,shape);
  if (nested) check(r.fired,"outer callback never nested");
  return r.primitives;
}
static int expected(StreamShape * s) {
  int n = static_cast<int>(s->points().size());
  switch (s->type) {
  case SoShape::TRIANGLES: return n/3*s->sequences;
  case SoShape::QUADS: return n/4*2*s->sequences;
  case SoShape::TRIANGLE_STRIP: case SoShape::TRIANGLE_FAN: case SoShape::POLYGON: return std::max(0,n-2)*s->sequences;
  case SoShape::QUAD_STRIP: return std::max(0,n/2-1)*2*s->sequences;
  case SoShape::LINES: return n/2*s->sequences;
  case SoShape::LINE_STRIP: return std::max(0,n-1)*s->sequences;
  case SoShape::POINTS: return n*s->sequences;
  }
  return -1;
}
static void validate(StreamShape * s, const std::vector<Primitive> & p) {
  check(static_cast<int>(p.size()) == expected(s),"incorrect primitive count");
  if (s->type == SoShape::TRIANGLES) {
    for (const Primitive & t : p) {
      check(t.vertices.size()==3,"TRIANGLES emitted non-triangle");
      std::vector<int> expectedCoordinates;
      for (const Vertex & v : t.vertices)
        expectedCoordinates.push_back(1000*s->id+100*static_cast<int>(v.data[7])+static_cast<int>(v.data[8]));
      std::sort(expectedCoordinates.begin(),expectedCoordinates.end());
      for (const Vertex & v : t.vertices) {
        check(v.data.size()>=28 && v.data[12]==1 && v.data[15]==3,"triangle face detail missing points");
        std::vector<int> actualCoordinates;
        for (int i=0;i<3;++i) actualCoordinates.push_back(static_cast<int>(v.data[16+4*i]));
        std::sort(actualCoordinates.begin(),actualCoordinates.end());
        check(actualCoordinates==expectedCoordinates,"triangle face detail coordinates disagree with vertices");
      }
    }
  }
  if (s->type == SoShape::POLYGON) {
    double area = 0;
    for (const Primitive & t : p) {
      check(t.vertices.size()==3,"polygon emitted a non-triangle");
      const auto & a=t.vertices[0].data; const auto & b=t.vertices[1].data; const auto & c=t.vertices[2].data;
      area += std::abs((b[0]-a[0])*(c[1]-a[1])-(b[1]-a[1])*(c[0]-a[0]))/2;
    }
    check(std::abs(area-(s->convex?16:7)*s->sequences)<1e-6,"concave polygon area changed");
  }
}
static void count(StreamShape * s) {
  SoGetPrimitiveCountAction a; a.setCanApproximate(FALSE); apply(a,s);
  const bool line = s->type == SoShape::LINES || s->type == SoShape::LINE_STRIP;
  // Compare generic count to its isolated behavior: historical segment weighting
  // is deliberately outside this regression's contract.
  check(a.getTriangleCount() == (line || s->type == SoShape::POINTS ? 0 : expected(s)),"generic triangle count");
  check(a.getPointCount() == (s->type == SoShape::POINTS ? expected(s) : 0),"generic point count");
}
static std::vector<double> counts(StreamShape * s) {
  SoGetPrimitiveCountAction a; a.setCanApproximate(FALSE); apply(a,s);
  return {double(a.getTriangleCount()),double(a.getLineCount()),double(a.getPointCount())};
}
static std::vector<double> pick(StreamShape * s) {
  SoRayPickAction a(SbViewportRegion(100,100));
  a.setRay(SbVec3f(.25f,.25f,5),SbVec3f(0,0,-1)); a.setPickAll(TRUE); apply(a,s);
  std::vector<double> result;
  for (int i=0; a.getPickedPoint(i); ++i) {
    const SoPickedPoint * p=a.getPickedPoint(i);
    for (int j=0;j<3;++j) result.push_back(p->getPoint()[j]);
    result.push_back(p->getMaterialIndex());
    const SoDetail * d=p->getDetail(s);
    if (d && d->isOfType(SoFaceDetail::getClassTypeId())) {
      const auto * f=static_cast<const SoFaceDetail *>(d);
      result.push_back(f->getFaceIndex()); result.push_back(f->getPartIndex());
      for(int j=0;j<f->getNumPoints();++j) pointDetail(result,f->getPoint(j));
    }
  }
  return result;
}
struct StopState { SoCallbackAction::Response response; int visits; };
static SoCallbackAction::Response stop(void * data, SoCallbackAction *, const SoNode *) {
  StopState & state=*static_cast<StopState *>(data);
  return ++state.visits==2 ? state.response : SoCallbackAction::CONTINUE;
}
static void stopped(StreamShape * s, SoCallbackAction::Response response) {
  // Generate a complete inner stream before stopping at the following node.
  auto root=retain(new SoSeparator);
  auto * hints=new SoShapeHints; hints->faceType=SoShapeHints::UNKNOWN_FACE_TYPE;
  root->addChild(hints); root->addChild(s); root->addChild(s);
  StopState state={response,0}; Recorder r; r.expectedId=s->id; r.expectedNode=s;
  SoCallbackAction a;
  a.addPreCallback(StreamShape::getClassTypeId(),stop,&state);
  a.addTriangleCallback(StreamShape::getClassTypeId(),Recorder::tri,&r);
  a.addLineSegmentCallback(StreamShape::getClassTypeId(),Recorder::line,&r);
  a.addPointCallback(StreamShape::getClassTypeId(),Recorder::point,&r);
  a.apply(root.get());
  check(state.visits==2,"abort/prune boundary was not reached");
  validate(s,r.primitives);
}
static void run(const std::string & scenario) {
  auto outer=retain(new StreamShape); auto inner=retain(new StreamShape); inner->id=7;
  const SoShape::TriangleShape types[] = {SoShape::TRIANGLES,SoShape::TRIANGLE_STRIP,SoShape::TRIANGLE_FAN,
    SoShape::QUADS,SoShape::QUAD_STRIP,SoShape::POLYGON,SoShape::LINES,SoShape::LINE_STRIP,SoShape::POINTS};
  if (scenario=="controls") {
    for (auto t:types) { outer->type=t; auto p=callbacks(outer.get()); validate(outer.get(),p); count(outer.get()); }
    outer->type=SoShape::TRIANGLES;
    check(!pick(outer.get()).empty(),"pick control missed shape"); return;
  }
  if (scenario=="deep" || scenario=="same") {
    auto reference=callbacks(outer.get());
    std::function<void(int)> recurse;
    recurse=[&](int n) {
      auto got=callbacks(outer.get(),n ? std::function<void()>([&]{recurse(n-1);}) : std::function<void()>());
      check(got==reference,"same-node nested traversal differs");
    };
    recurse(scenario=="deep"?4:1); return;
  }
  if (scenario=="count_callback" || scenario=="pick_callback") {
    auto refcount=counts(outer.get()); auto refpick=pick(outer.get()); check(!refpick.empty(),"pick control missed");
    auto innerReference=callbacks(inner.get()); bool fired=false;
    auto nest=[&] { if (!fired) { fired=true; check(callbacks(inner.get())==innerReference,"inner callback mismatch"); } };
    if (scenario=="count_callback") {
      outer->hook=[&](SoAction *,int s,int i) { if(s==0 && i==0) nest(); };
      check(counts(outer.get())==refcount,"count->callback changed counts");
    } else {
      outer->detailHook=nest;
      check(pick(outer.get())==refpick,"pick->callback changed intersections/details");
    }
    check(fired,"extension hook did not run"); return;
  }
  for (auto t:types) {
    outer->type=t; inner->type=t;
    if (scenario=="different") inner->type=t==SoShape::POLYGON ? SoShape::TRIANGLES : SoShape::POLYGON;
    if ((scenario=="polygon" || scenario=="convex") && t!=SoShape::POLYGON) continue;
    outer->convex=inner->convex=scenario=="convex";
    if (scenario=="empty") inner->limit=0;
    if (scenario=="single") { inner->type=SoShape::TRIANGLES; inner->limit=3; inner->sequences=1; }
    const auto reference=callbacks(outer.get()); validate(outer.get(),reference);
    const auto innerReference=callbacks(inner.get()); validate(inner.get(),innerReference);
    const auto innerCount=counts(inner.get());
    auto nesting=[&] {
      if (scenario=="count") check(counts(inner.get())==innerCount,"inner count mismatch");
      else if (scenario=="pick") { inner->type=SoShape::TRIANGLES; auto p=pick(inner.get()); check(!p.empty(),"nested picking missed triangles"); inner->type=t; }
      else if (scenario=="abort" || scenario=="prune") {
        stopped(inner.get(),scenario=="abort"?SoCallbackAction::ABORT:SoCallbackAction::PRUNE);
        check(callbacks(inner.get())==innerReference,"generation after abort/prune changed");
      } else if (scenario=="exception") {
        inner->throwing=true; bool caught=false;
        try { callbacks(inner.get()); } catch(const std::runtime_error & e) { caught=std::string(e.what())=="fixture throw"; }
        inner->throwing=false; check(caught,"inner fixture did not throw");
        check(callbacks(inner.get())==innerReference,"generation after exception changed");
      } else check(callbacks(inner.get())==innerReference,"inner callback data mismatch");
    };
    if (scenario=="partial") {
      bool fired=false;
      outer->hook=[&](SoAction *,int s,int i) { if(!fired && s==0 && i==0) { fired=true; nesting(); } };
      check(callbacks(outer.get())==reference,"partial primitive corrupted"); check(fired,"partial hook did not fire"); outer->hook={};
    } else check(callbacks(outer.get(),nesting)==reference,"outer primitive stream corrupted");
    check(callbacks(outer.get())==reference,"following traversal corrupted");
  }
}
int main(int argc,char ** argv) {
  try { Database db; StreamShape::initClass(); check(argc==2,"one scenario required"); run(argv[1]); }
  catch(const std::exception & e) { std::cerr<<e.what()<<'\n'; return 1; }
  return 0;
}
