#include <Inventor/SoDB.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/details/SoFaceDetail.h>
#include <Inventor/details/SoLineDetail.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTextureCoordinate2.h>
#include <Inventor/nodes/SoTextureCoordinateBinding.h>
#include <Inventor/nodes/SoVertexProperty.h>

#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>

namespace {
int failures = 0;
const char * currentcase = "";
void check(bool ok, const char * message) {
  if (!ok) { std::fprintf(stderr, "%s: %s\n", currentcase, message); ++failures; }
}
bool close(float a, float b) { return std::fabs(a-b) < 0.0001f; }
bool same(const SbVec3f & a, const SbVec3f & b) {
  return close(a[0],b[0]) && close(a[1],b[1]) && close(a[2],b[2]);
}
SbVec3f normal(int i) { return SbVec3f(0.01f * (i+1), 0.02f * (i+1), 1.0f); }
SbVec2f uv(int i) { return SbVec2f(0.03f * (i+1), 0.04f * (i+1)); }
struct Endpoint {
  int coord, norm, mat, tex;
  Endpoint(int c=0, int n=0, int m=0, int t=0) : coord(c), norm(n), mat(m), tex(t) {}
};
struct Segment {
  Endpoint a,b;
  int line,part;
  Segment(Endpoint x, Endpoint y, int l, int p) : a(x),b(y),line(l),part(p) {}
};
void pointDetail(const SoPointDetail * p, const Endpoint & e) {
  check(p->getCoordinateIndex()==e.coord, "coordinate detail must identify this occurrence");
  check(p->getNormalIndex()==e.norm, "normal detail must identify this occurrence");
  check(p->getMaterialIndex()==e.mat, "material detail must identify this occurrence");
  check(p->getTextureCoordIndex()==e.tex, "texture detail must identify this occurrence");
}
void lineDetail(const SoLineDetail * d, const Segment & s) {
  check(d->getLineIndex()==s.line, "lineIndex must identify the polyline");
  check(d->getPartIndex()==s.part, "partIndex must identify the segment part");
  pointDetail(d->getPoint0(),s.a);
  pointDetail(d->getPoint1(),s.b);
}
struct Capture {
  std::vector<Segment> expected;
  std::vector<SbVec3f> coords;
  std::vector<SoDetail *> copies;
  unsigned int count;
  Capture() : count(0) {}
  ~Capture() { for (unsigned int i=0;i<copies.size();++i) delete copies[i]; }
  void verifyCopies() const {
    check(copies.size()==expected.size(), "one retained detail per segment");
    for (unsigned int i=0;i<copies.size() && i<expected.size();++i)
      lineDetail(static_cast<const SoLineDetail *>(copies[i]), expected[i]);
  }
};
void vertex(const SoPrimitiveVertex * v, const Endpoint & e, const Capture & c) {
  check(same(v->getPoint(),c.coords[e.coord]), "callback position must match occurrence");
  check(same(v->getNormal(),normal(e.norm)), "callback normal must match its detail index");
  check(v->getMaterialIndex()==e.mat, "callback material must match its detail index");
  const SbVec4f & t=v->getTextureCoords();
  check(close(t[0],uv(e.tex)[0]) && close(t[1],uv(e.tex)[1]),
        "callback UV must match its detail index");
}
void lineCB(void * data, SoCallbackAction *, const SoPrimitiveVertex * a,
            const SoPrimitiveVertex * b) {
  Capture & c=*static_cast<Capture *>(data);
  const unsigned int i=c.count++;
  check(i<c.expected.size(), "unexpected segment callback");
  if (i>=c.expected.size()) return;
  vertex(a,c.expected[i].a,c); vertex(b,c.expected[i].b,c);
  const SoDetail * da=a->getDetail();
  const SoDetail * db=b->getDetail();
  check(da && da->isOfType(SoLineDetail::getClassTypeId()), "first endpoint has line detail");
  check(db && db->isOfType(SoLineDetail::getClassTypeId()), "second endpoint has line detail");
  if (da && da->isOfType(SoLineDetail::getClassTypeId())) {
    lineDetail(static_cast<const SoLineDetail *>(da), c.expected[i]);
    c.copies.push_back(da->copy());
  }
  if (db && db->isOfType(SoLineDetail::getClassTypeId()))
    lineDetail(static_cast<const SoLineDetail *>(db), c.expected[i]);
}
void callbacks(SoNode * root, Capture & c) {
  SoCallbackAction action;
  action.addLineSegmentCallback(SoShape::getClassTypeId(), lineCB, &c);
  action.apply(root);
  check(c.count==c.expected.size(), "segment callback count");
  c.verifyCopies();
}
void picking(SoNode * root, Capture & c, bool retainedCopies=true) {
  if(c.expected.empty()) {
    SoRayPickAction action(SbViewportRegion(256,256));
    action.setRay(SbVec3f(0,0,4),SbVec3f(0,0,-1),0,8);
    action.setRadius(0); action.apply(root);
    check(action.getPickedPoint()==NULL,"short lines have no pickable segment");
  }
  // Every tested segment has a distinct interior, so the ray has one exact hit.
  for (unsigned int i=0;i<c.expected.size();++i) {
    const Segment & s=c.expected[i];
    const SbVec3f mid=(c.coords[s.a.coord]+c.coords[s.b.coord])*0.5f;
    SoRayPickAction action(SbViewportRegion(256,256));
    action.setRay(mid+SbVec3f(0,0,4),SbVec3f(0,0,-1),0,8);
    action.setRadius(0);
    action.apply(root);
    const SoPickedPoint * p=action.getPickedPoint();
    check(p!=NULL, "ray must hit segment interior");
    if (!p) continue;
    const SoDetail * d=p->getDetail();
    check(d && d->isOfType(SoLineDetail::getClassTypeId()), "pick must contain SoLineDetail");
    if (d && d->isOfType(SoLineDetail::getClassTypeId()))
      lineDetail(static_cast<const SoLineDetail *>(d),s);
  }
  if(retainedCopies) c.verifyCopies(); // Later actions must not mutate consumer copies.
}
struct Scene {
  SoSeparator * root;
  Scene(bool indexed) : root(new SoSeparator) {
    root->ref();
    SoCoordinate3 * co=new SoCoordinate3;
    SoNormal * no=new SoNormal;
    SoMaterial * ma=new SoMaterial;
    SoTextureCoordinate2 * te=new SoTextureCoordinate2;
    for (int i=0;i<24;++i) {
      co->point.set1Value(i,SbVec3f(float(i),0,0));
      no->vector.set1Value(i,normal(i));
      ma->diffuseColor.set1Value(i,SbColor(0.01f*i,0.02f*i,0.03f*i));
      te->point.set1Value(i,uv(i));
    }
    root->addChild(co); root->addChild(no); root->addChild(ma); root->addChild(te);
    // Explicit coordinates are used only when a texture unit is enabled.
    // An in-memory image activates the CPU callback/picking texture state.
    SoTexture2 * texture=new SoTexture2;
    const unsigned char pixel[]={255,255,255};
    texture->image.setValue(SbVec2s(1,1),3,pixel);
    root->addChild(texture);
    SoNormalBinding * nb=new SoNormalBinding;
    nb->value=indexed ? SoNormalBinding::PER_VERTEX_INDEXED : SoNormalBinding::PER_VERTEX;
    SoMaterialBinding * mb=new SoMaterialBinding;
    mb->value=indexed ? SoMaterialBinding::PER_VERTEX_INDEXED : SoMaterialBinding::PER_VERTEX;
    SoTextureCoordinateBinding * tb=new SoTextureCoordinateBinding;
    tb->value=indexed ? SoTextureCoordinateBinding::PER_VERTEX_INDEXED : SoTextureCoordinateBinding::PER_VERTEX;
    root->addChild(nb); root->addChild(mb); root->addChild(tb);
  }
  ~Scene() { root->unref(); }
  void coordinates(Capture & c) {
    SoCoordinate3 * co=static_cast<SoCoordinate3 *>(root->getChild(0));
    co->point.setValues(0,int(c.coords.size()),&c.coords[0]);
  }
};

void nativeLineSet(int shortsize=-1) {
  currentcase=shortsize==0 ? "LineSet empty/startIndex" : shortsize==1 ? "LineSet singleton/startIndex" : "LineSet per-vertex/startIndex";
  Scene scene(false); Capture c;
  for(int i=0;i<12;++i) c.coords.push_back(SbVec3f(float(i),float(i/5)*4,0));
  SoLineSet * shape=new SoLineSet;
  shape->startIndex=2;
  if (shortsize>=0) {
    shape->numVertices.setValue(shortsize);
  } else {
    const int32_t counts[]={4,3};
    shape->numVertices.setValues(0,2,counts);
    for(int k=0;k<3;++k) c.expected.push_back(Segment(Endpoint(2+k,k,k,k),Endpoint(3+k,k+1,k+1,k+1),0,k));
    for(int k=0;k<2;++k) c.expected.push_back(Segment(Endpoint(6+k,4+k,4+k,4+k),Endpoint(7+k,5+k,5+k,5+k),1,3+k));
  }
  scene.coordinates(c); scene.root->addChild(shape);
  callbacks(scene.root,c); picking(scene.root,c);
}
void nativeIndexed(bool repeated, int shortlines) {
  currentcase=shortlines==2 ? "IndexedLineSet empty" : shortlines==1 ? "IndexedLineSet singleton" : repeated ? "IndexedLineSet repeated occurrences" : "IndexedLineSet explicit independent indices";
  Scene scene(true); Capture c;
  c.coords.push_back(SbVec3f(2,0,0)); c.coords.push_back(SbVec3f(5,0,0));
  c.coords.push_back(SbVec3f(0,0,0)); c.coords.push_back(SbVec3f(3,3,0));
  c.coords.push_back(SbVec3f(1,0,0)); c.coords.push_back(SbVec3f(4,3,0));
  SoIndexedLineSet * shape=new SoIndexedLineSet;
  const int32_t ci[]={2,4,0,1,-1,3,5,0,-1};
  const int32_t ni[]={7,3,9,1,-1,6,2,8,-1};
  const int32_t mi[]={4,8,2,6,-1,3,7,1,-1};
  const int32_t ti[]={9,1,5,2,-1,8,4,6,-1};
  shape->coordIndex.setValues(0,9,ci); shape->normalIndex.setValues(0,9,ni);
  shape->materialIndex.setValues(0,9,mi); shape->textureCoordIndex.setValues(0,9,ti);
  if (repeated) {
    // Reuse a coordinate at the boundary, with different independent indices.
    shape->coordIndex.set1Value(5,1);
  }
  if (shortlines) {
    const int count=shortlines==2 ? 0 : 1;
    shape->coordIndex.setNum(count); shape->normalIndex.setNum(count);
    shape->materialIndex.setNum(count); shape->textureCoordIndex.setNum(count);
  } else {
    int part=0;
    for(int k=0;k<3;++k) c.expected.push_back(Segment(Endpoint(ci[k],ni[k],mi[k],ti[k]),Endpoint(ci[k+1],ni[k+1],mi[k+1],ti[k+1]),0,part++));
    for(int k=5;k<7;++k) c.expected.push_back(Segment(Endpoint(shape->coordIndex[k],ni[k],mi[k],ti[k]),Endpoint(ci[k+1],ni[k+1],mi[k+1],ti[k+1]),1,part++));
  }
  scene.coordinates(c); scene.root->addChild(shape);
  callbacks(scene.root,c); picking(scene.root,c);
}

// Public extension API fixture. Reusing a single mutable SoPointDetail mirrors
// clients streaming primitives; consumers retain deep copies in lineCB.
class AssemblerShape : public SoShape {
public:
  TriangleShape mode;
  int n;
  AssemblerShape() : mode(LINE_STRIP),n(6) {}
  static SbVec3f position(int i) { return SbVec3f(float(i/2),float(i%2),0); }
  void computeBBox(SoAction *,SbBox3f & b,SbVec3f & center) override {
    b.setBounds(SbVec3f(0,0,0),SbVec3f(3,1,0)); center=b.getCenter();
  }
protected:
  ~AssemblerShape() override {}
  void generatePrimitives(SoAction * action) override {
    SoLineDetail ld; SoFaceDetail fd; SoPointDetail pd; SoPrimitiveVertex v;
    SoDetail * detail=(mode==LINES || mode==LINE_STRIP) ? static_cast<SoDetail *>(&ld) : static_cast<SoDetail *>(&fd);
    ld.setLineIndex(13); ld.setPartIndex(17);
    fd.setFaceIndex(13); fd.setPartIndex(17);
    beginShape(action,mode,detail);
    for(int k=0;k<n;++k) {
      const int polygonOrder[]={0,2,3,1};
      const int i=mode==POLYGON ? polygonOrder[k] : k;
      pd.setCoordinateIndex(i); pd.setNormalIndex(i+4);
      pd.setMaterialIndex(i+8); pd.setTextureCoordIndex(i+12);
      v.setPoint(position(i)); v.setNormal(normal(0));
      v.setMaterialIndex(0); v.setTextureCoords(uv(0)); v.setDetail(&pd);
      shapeVertex(&v);
    }
    endShape();
    // Overwrite producer storage after all callbacks. Retained copies must survive.
    pd.setCoordinateIndex(999); ld.setLineIndex(999); fd.setFaceIndex(999);
  }
};
struct AssemblerCapture {
  bool lines;
  int count;
  std::vector<SoDetail *> copies;
  AssemblerCapture(bool l) : lines(l),count(0) {}
  ~AssemblerCapture() { for(unsigned int i=0;i<copies.size();++i) delete copies[i]; }
};
void assemblerPoint(const SoPointDetail * p,int i) { pointDetail(p,Endpoint(i,i+4,i+8,i+12)); }
void assemblerLineCB(void * data,SoCallbackAction *,const SoPrimitiveVertex * a,const SoPrimitiveVertex * b) {
  AssemblerCapture & c=*static_cast<AssemblerCapture *>(data);
  int i=c.lines ? c.count*2 : c.count;
  check(same(a->getPoint(),AssemblerShape::position(i)),"assembler first position");
  check(same(b->getPoint(),AssemblerShape::position(i+1)),"assembler second position");
  const SoDetail * d=a->getDetail();
  check(d && d->isOfType(SoLineDetail::getClassTypeId()),"assembler line detail type");
  if(d && d->isOfType(SoLineDetail::getClassTypeId())) {
    const SoLineDetail * ld=static_cast<const SoLineDetail *>(d);
    assemblerPoint(ld->getPoint0(),i); assemblerPoint(ld->getPoint1(),i+1);
    check(ld->getLineIndex()==13 && ld->getPartIndex()==17,"assembler caller line/part indices");
    c.copies.push_back(d->copy());
  }
  ++c.count;
}
void assemblerTriangleCB(void * data,SoCallbackAction *,const SoPrimitiveVertex * a,const SoPrimitiveVertex * b,const SoPrimitiveVertex * v) {
  AssemblerCapture & c=*static_cast<AssemblerCapture *>(data);
  const SoPrimitiveVertex * vertices[]={a,b,v};
  for(int j=0;j<3;++j) {
    const SoDetail * d=vertices[j]->getDetail();
    check(d && d->isOfType(SoFaceDetail::getClassTypeId()),"assembler face detail type");
    if(!d || !d->isOfType(SoFaceDetail::getClassTypeId())) continue;
    const SoFaceDetail * fd=static_cast<const SoFaceDetail *>(d);
    bool found=false;
    for(int k=0;k<fd->getNumPoints();++k) {
      const SoPointDetail * pd=fd->getPoint(k);
      int i=pd->getCoordinateIndex();
      check(i>=0 && i<6,"face coordinate index range");
      if(i<0 || i>=6) continue;
      assemblerPoint(pd,i);
      if(same(vertices[j]->getPoint(),AssemblerShape::position(i))) found=true;
    }
    check(found,"triangle position occurs in its face detail");
    check(fd->getPartIndex()==17,"face part index preserved");
    if(j==0) c.copies.push_back(d->copy());
  }
  ++c.count;
}
void assembler(SoShape::TriangleShape mode,const char * name,int expected,int vertices=6) {
  currentcase=name;
  AssemblerShape * shape=new AssemblerShape; shape->ref(); shape->mode=mode; shape->n=vertices;
  if(mode==SoShape::POLYGON) shape->n=4;
  AssemblerCapture c(mode==SoShape::LINES);
  {
    SoCallbackAction action;
    action.addLineSegmentCallback(SoShape::getClassTypeId(),assemblerLineCB,&c);
    action.addTriangleCallback(SoShape::getClassTypeId(),assemblerTriangleCB,&c);
    action.apply(shape);
  }
  shape->unref();
  check(c.count==expected,"assembler primitive count");
  for(unsigned int j=0;j<c.copies.size();++j) {
    if(mode==SoShape::LINES || mode==SoShape::LINE_STRIP) {
      const SoLineDetail * ld=static_cast<const SoLineDetail *>(c.copies[j]);
      int i=(mode==SoShape::LINES) ? int(j)*2 : int(j);
      assemblerPoint(ld->getPoint0(),i); assemblerPoint(ld->getPoint1(),i+1);
      check(ld->getLineIndex()==13 && ld->getPartIndex()==17,"retained line indices");
    } else {
      const SoFaceDetail * fd=static_cast<const SoFaceDetail *>(c.copies[j]);
      for(int k=0;k<fd->getNumPoints();++k) assemblerPoint(fd->getPoint(k),fd->getPoint(k)->getCoordinateIndex());
      check(fd->getPartIndex()==17,"retained face part index");
    }
  }
}

// A vertexProperty affects only its owning shape. A following sibling must
// observe the inherited coordinate/attribute state that preceded that shape.
struct SiblingCapture {
  SoNode * sibling;
  Capture * capture;
  int ownerCount;
};
void siblingCB(void * data,SoCallbackAction * action,
               const SoPrimitiveVertex * a,const SoPrimitiveVertex * b) {
  SiblingCapture & c=*static_cast<SiblingCapture *>(data);
  if(action->getCurPathTail()==c.sibling) lineCB(c.capture,action,a,b);
  else {
    const SoDetail * d=a->getDetail();
    check(d && d->isOfType(SoLineDetail::getClassTypeId()),"owner line detail");
    if(d && d->isOfType(SoLineDetail::getClassTypeId())) {
      const SoLineDetail * line=static_cast<const SoLineDetail *>(d);
      check(line->getLineIndex()==2,"owner keeps empty/singleton line ordinals");
      check(line->getPartIndex()==c.ownerCount,"owner counts real segments only");
    }
    ++c.ownerCount;
  }
}
void vertexPropertySibling(bool indexed,bool pick) {
  currentcase=indexed ? "IndexedLineSet vertexProperty sibling state" : "LineSet vertexProperty sibling state";
  Scene scene(false); Capture c;
  for(int i=0;i<24;++i) c.coords.push_back(SbVec3f(float(i),0,0));
  c.expected.push_back(Segment(Endpoint(0,0,0,0),Endpoint(1,1,1,1),0,0));
  c.expected.push_back(Segment(Endpoint(1,1,1,1),Endpoint(2,2,2,2),0,1));
  SoVertexShape * owner;
  SoVertexShape * sibling;
  if(indexed) {
    SoIndexedLineSet * a=new SoIndexedLineSet;
    SoIndexedLineSet * b=new SoIndexedLineSet;
    const int32_t indices[]={0,1,2,-1};
    const int32_t ownerIndices[]={-1,0,-1,1,2,3,-1};
    a->coordIndex.setValues(0,7,ownerIndices); b->coordIndex.setValues(0,4,indices);
    owner=a; sibling=b;
  } else {
    SoLineSet * a=new SoLineSet;
    SoLineSet * b=new SoLineSet;
    const int32_t ownerCounts[]={0,1,3};
    a->numVertices.setValues(0,3,ownerCounts); b->numVertices=3;
    owner=a; sibling=b;
  }
  SoVertexProperty * vp=new SoVertexProperty;
  for(int i=0;i<4;++i) {
    vp->vertex.set1Value(i,SbVec3f(float(i),10,0));
    vp->normal.set1Value(i,normal(i+12));
    vp->texCoord.set1Value(i,uv(i+12));
    vp->orderedRGBA.set1Value(i,0xff0000ffu+uint32_t(i)*0x00010000u);
  }
  // PER_PART selects the per-segment LINES path; inherited state is PER_VERTEX.
  vp->normalBinding=SoVertexProperty::PER_PART;
  vp->materialBinding=SoVertexProperty::PER_PART;
  owner->vertexProperty=vp;
  scene.root->addChild(owner); scene.root->addChild(sibling);
  if(pick) picking(scene.root,c,false);
  else {
    SiblingCapture data={sibling,&c,0};
    SoCallbackAction action;
    action.addLineSegmentCallback(SoShape::getClassTypeId(),siblingCB,&data);
    action.apply(scene.root);
    check(data.ownerCount==2,"owner callback count excludes short groups");
    check(c.count==c.expected.size(),"following sibling callback count");
    c.verifyCopies();
  }
}

// SoLineDetail describes a segment globally within its shape, while lineIndex
// describes the containing polyline (OIV 2024.2 SoLineDetail reference).
// SGI's Nodes Quick Reference permits numVertices == -1 or >= 0. Keeping
// empty/singleton groups in the line ordinal is the defensive policy exercised
// here; they contribute max(vertexCount - 1, 0) segments.
enum ContractBinding { C_OVERALL, C_LINE, C_SEGMENT, C_VERTEX };
int bindingValue(int binding, bool explicitIndex) {
  // Coin names a segment PER_PART and a polyline PER_FACE.
  const int values[]={SoMaterialBinding::OVERALL, SoMaterialBinding::PER_FACE,
                      SoMaterialBinding::PER_PART, SoMaterialBinding::PER_VERTEX};
  return values[binding]+((explicitIndex && binding!=C_OVERALL) ? 1 : 0);
}
int independentIndex(int ordinal, int salt) { return (ordinal*7+salt)%23; }
int attribute(int binding, int occurrence, int line, int segment,
              bool explicitIndex, int salt) {
  if(binding==C_OVERALL) return 0;
  const int ordinal=binding==C_LINE ? line : binding==C_SEGMENT ? segment : occurrence;
  return explicitIndex ? independentIndex(ordinal,salt) : ordinal;
}
void attributeIndices(SoMFInt32 & field, int binding, int salt,
                      const std::vector<int> & sizes, bool terminated) {
  std::vector<int32_t> indices;
  int ordinal=0;
  for(unsigned int l=0;l<sizes.size();++l) {
    if(binding==C_VERTEX) {
      for(int k=0;k<sizes[l];++k) indices.push_back(independentIndex(ordinal++,salt));
      if(terminated || l+1<sizes.size()) indices.push_back(-1);
    } else if(binding==C_LINE) indices.push_back(independentIndex(ordinal++,salt));
    else if(binding==C_SEGMENT) {
      for(int k=1;k<sizes[l];++k) indices.push_back(independentIndex(ordinal++,salt));
    }
  }
  if(!indices.empty()) field.setValues(0,int(indices.size()),&indices[0]);
}
void contractCase(bool indexed,const char * layout,const char * bindings,
                  const char * source,const char * operation,const char * indexMode) {
  const bool explicitIndex=!std::strcmp(indexMode,"independent");
  const bool implicitIndex=!std::strcmp(indexMode,"implicit");
  const bool vrml1=!std::strcmp(source,"vrml1");
  char description[200];
  std::snprintf(description,sizeof(description),"%s/%s/%s/%s/%s/%s",
    indexed ? "indexed" : "lineset",layout,bindings,source,operation,
    indexMode);
  currentcase=description;
  int nb=C_VERTEX,mb=C_VERTEX;
  if(!std::strcmp(bindings,"part")) nb=mb=C_SEGMENT;
  else if(!std::strcmp(bindings,"face")) nb=mb=C_LINE;
  else if(!std::strcmp(bindings,"overall")) nb=mb=C_OVERALL;
  else if(!std::strcmp(bindings,"vf")) mb=C_LINE;
  else if(!std::strcmp(bindings,"fv")) nb=C_LINE;
  else if(!std::strcmp(bindings,"pv")) nb=C_SEGMENT;
  else if(!std::strcmp(bindings,"vp")) mb=C_SEGMENT;
  else if(!std::strcmp(bindings,"pf")) { nb=C_SEGMENT; mb=C_LINE; }
  else if(!std::strcmp(bindings,"fp")) { nb=C_LINE; mb=C_SEGMENT; }
  std::vector<int> sizes;
  if(!std::strcmp(layout,"shorts")) {
    const int n[]={0,1,3,0,1,4,1,0}; sizes.assign(n,n+8);
  } else if(!std::strcmp(layout,"allshort")) {
    const int n[]={0,1,0,1,0}; sizes.assign(n,n+5);
  } else if(!std::strcmp(layout,"sentinel")) {
    // Coin supports -1 as the sole entry: consume all coordinates after startIndex.
    sizes.push_back(7);
  } else {
    const int n[]={3,4}; sizes.assign(n,n+2);
  }
  const bool terminated=std::strcmp(layout,"unterminated")!=0;
  const bool vp=std::strcmp(source,"vp")==0;
  Scene scene(indexed); Capture c;
  int total=0;
  for(unsigned int l=0;l<sizes.size();++l) total+=sizes[l];
  // Coordinates are deliberately independent of attribute ordinals. Two unused
  // leading coordinates exercise Coin's existing relative startIndex semantics.
  c.coords.resize(indexed ? 24 : total+2,SbVec3f(-20,-20,0));
  std::vector<int32_t> ci;
  int occurrence=0,part=0;
  for(unsigned int l=0;l<sizes.size();++l) {
    std::vector<Endpoint> endpoints;
    for(int k=0;k<sizes[l];++k,++occurrence) {
      const int coord=indexed ? independentIndex(occurrence,4) : occurrence+2;
      c.coords[coord]=SbVec3f(float(k),float(l)*4,0);
      ci.push_back(coord);
      endpoints.push_back(Endpoint(coord,0,0,
        indexed && explicitIndex ? independentIndex(occurrence,9) : occurrence));
    }
    for(int k=1;k<sizes[l];++k) {
      Endpoint a=endpoints[k-1],b=endpoints[k];
      const int first=occurrence-sizes[l]+k-1;
      a.norm=attribute(nb,first,int(l),part,explicitIndex,3);
      b.norm=attribute(nb,first+1,int(l),part,explicitIndex,3);
      a.mat=attribute(mb,first,int(l),part,explicitIndex,5);
      b.mat=attribute(mb,first+1,int(l),part,explicitIndex,5);
      // Coin's VRML1 compatibility resolves PER_VERTEX normal/material
      // through coordIndex; texture binding retains its independent meaning.
      if(indexed && vrml1 && !explicitIndex) {
        if(nb==C_VERTEX) { a.norm=a.coord; b.norm=b.coord; }
        if(mb==C_VERTEX) { a.mat=a.coord; b.mat=b.coord; }
      }
      if(implicitIndex) {
        if(nb==C_VERTEX) { a.norm=a.coord; b.norm=b.coord; }
        if(mb==C_VERTEX) { a.mat=a.coord; b.mat=b.coord; }
        a.tex=a.coord; b.tex=b.coord;
      }
      c.expected.push_back(Segment(a,b,int(l),part++));
    }
    if(terminated || l+1<sizes.size()) ci.push_back(-1);
  }
  scene.coordinates(c);
  static_cast<SoCoordinate3 *>(scene.root->getChild(0))->point.setNum(int(c.coords.size()));
  SoNormalBinding * normals=static_cast<SoNormalBinding *>(scene.root->getChild(5));
  SoMaterialBinding * materials=static_cast<SoMaterialBinding *>(scene.root->getChild(6));
  SoTextureCoordinateBinding * textures=static_cast<SoTextureCoordinateBinding *>(scene.root->getChild(7));
  normals->value=bindingValue(nb,explicitIndex || implicitIndex);
  materials->value=bindingValue(mb,explicitIndex || implicitIndex);
  textures->value=indexed && (explicitIndex || implicitIndex) ? SoTextureCoordinateBinding::PER_VERTEX_INDEXED : SoTextureCoordinateBinding::PER_VERTEX;
  SoVertexShape * shape;
  if(indexed) {
    SoIndexedLineSet * line=new SoIndexedLineSet;
    line->coordIndex.setValues(0,int(ci.size()),&ci[0]);
    if(explicitIndex) {
      attributeIndices(line->normalIndex,nb,3,sizes,terminated);
      attributeIndices(line->materialIndex,mb,5,sizes,terminated);
      attributeIndices(line->textureCoordIndex,C_VERTEX,9,sizes,terminated);
    }
    shape=line;
  } else {
    SoLineSet * line=new SoLineSet;
    line->startIndex=2;
    std::vector<int32_t> counts(sizes.begin(),sizes.end());
    if(!std::strcmp(layout,"sentinel")) counts.back()=-1;
    line->numVertices.setValues(0,int(counts.size()),&counts[0]);
    shape=line;
  }
  if(vrml1) shape->setNodeType(SoNode::VRML1);
  if(vp) {
    SoVertexProperty * data=new SoVertexProperty;
    data->vertex.setValues(0,int(c.coords.size()),&c.coords[0]);
    for(int i=0;i<24;++i) {
      data->normal.set1Value(i,normal(i)); data->texCoord.set1Value(i,uv(i));
      data->orderedRGBA.set1Value(i,0x112233ffu+uint32_t(i)*0x01010000u);
    }
    data->normalBinding=bindingValue(nb,explicitIndex || implicitIndex);
    data->materialBinding=bindingValue(mb,explicitIndex || implicitIndex);
    shape->vertexProperty=data;
    // State deliberately conflicts with vertexProperty to verify precedence.
    static_cast<SoCoordinate3 *>(scene.root->getChild(0))->point.setValue(99,99,99);
  }
  scene.root->addChild(shape);
  if(!std::strcmp(operation,"count")) {
    SoGetPrimitiveCountAction action; action.setCanApproximate(FALSE);
    action.apply(scene.root);
    check(action.getLineCount()==int(c.expected.size()),"exact line count equals sum max(n-1,0)");
    check(action.getTriangleCount()==0 && action.getPointCount()==0,"short lines emit neither points nor triangles");
  } else if(!std::strcmp(operation,"pick")) picking(scene.root,c,false);
  else callbacks(scene.root,c);
}

}
int main(int argc,char ** argv) {
  SoDB::init();
  {
    const char * mode=argc>1 ? argv[1] : "all";
    if(!std::strcmp(mode,"vp-sibling") && argc==4)
      vertexPropertySibling(!std::strcmp(argv[2],"indexed"),!std::strcmp(argv[3],"pick"));
    if(!std::strcmp(mode,"contract") && argc==8)
      contractCase(!std::strcmp(argv[2],"indexed"),argv[3],argv[4],argv[5],argv[6],
                   argv[7]);
    const bool native=!std::strcmp(mode,"all") || !std::strcmp(mode,"native");
    if(native || !std::strcmp(mode,"lineset")) nativeLineSet();
    if(native || !std::strcmp(mode,"lineset-short")) { nativeLineSet(0); nativeLineSet(1); }
    if(native || !std::strcmp(mode,"indexed")) nativeIndexed(false,false);
    if(native || !std::strcmp(mode,"indexed-repeated")) nativeIndexed(true,false);
    if(native || !std::strcmp(mode,"indexed-short")) { nativeIndexed(false,1); nativeIndexed(false,2); }
    if(!std::strcmp(mode,"all") || !std::strcmp(mode,"assembler")) {
      assembler(SoShape::LINE_STRIP,"assembler LINE_STRIP repeated attributes",5);
      assembler(SoShape::LINE_STRIP,"assembler LINE_STRIP empty",0,0);
      assembler(SoShape::LINE_STRIP,"assembler LINE_STRIP singleton",0,1);
      assembler(SoShape::LINE_STRIP,"assembler LINE_STRIP one segment",1,2);
      assembler(SoShape::LINES,"assembler LINES control",3);
      assembler(SoShape::TRIANGLE_STRIP,"assembler TRIANGLE_STRIP control",4);
      assembler(SoShape::QUAD_STRIP,"assembler QUAD_STRIP control",4);
      assembler(SoShape::POLYGON,"assembler POLYGON control",2);
    }
  }
  SoDB::finish();
  return failures ? 1 : 0;
}
