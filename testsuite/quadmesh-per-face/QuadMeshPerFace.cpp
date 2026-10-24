// Public CPU regression tests for SoQuadMesh binding semantics.
// PER_FACE means one attribute per quadrilateral, PER_PART one per row.
// See Coin SoQuadMesh documentation and Open Inventor 2023.2 SoQuadMesh.
#include <Inventor/SoDB.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/details/SoFaceDetail.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/nodes/SoQuadMesh.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTextureCoordinate2.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
enum Binding { Overall, Part, Face, Vertex };
struct Spec {
  int w, h, start;
  Binding material, normal;
  bool indexed, generated, vp;
  Spec(int width, int height, int offset, Binding m, Binding n,
       bool alias = false, bool gen = false, bool property = false)
    : w(width), h(height), start(offset), material(m), normal(n),
      indexed(alias), generated(gen), vp(property) {}
};
std::string label;
void require(bool ok, const std::string & what) {
  if (!ok) throw std::runtime_error(label + ": " + what);
}
void equal(int actual, int expected, const char * what) {
  std::ostringstream out;
  out << what << ": got " << actual << ", expected " << expected;
  require(actual == expected, out.str());
}
bool close(float a, float b) { return std::fabs(a-b) < 0.0001f; }
void vectorEqual(const SbVec3f & a, const SbVec3f & b, const char * what) {
  require(close(a[0],b[0]) && close(a[1],b[1]) && close(a[2],b[2]), what);
}
int bindingValue(Binding b, bool indexed) {
  switch (b) {
  case Overall: return SoMaterialBinding::OVERALL;
  case Part: return indexed ? SoMaterialBinding::PER_PART_INDEXED : SoMaterialBinding::PER_PART;
  case Face: return indexed ? SoMaterialBinding::PER_FACE_INDEXED : SoMaterialBinding::PER_FACE;
  case Vertex: return indexed ? SoMaterialBinding::PER_VERTEX_INDEXED : SoMaterialBinding::PER_VERTEX;
  }
  return 0;
}
int count(const Spec & s, Binding b) {
  switch (b) {
  case Overall: return 1;
  case Part: return std::max(0,s.h-1);
  case Face: return std::max(0,s.h-1)*std::max(0,s.w-1);
  case Vertex: return s.w*s.h;
  }
  return 0;
}
int index(const Spec & s, Binding b, int row, int col, int coord) {
  switch (b) {
  case Overall: return 0;
  case Part: return row;
  case Face: return row*(s.w-1)+col;
  case Vertex: return coord-s.start;
  }
  return -1;
}
SbVec3f authoredNormal(int i) {
  SbVec3f n(0.11f*(i+1), 0.07f*(i+1)*(i+1), 1.0f);
  n.normalize();
  return n;
}
float height(const Spec & s, int row) { return s.generated ? 0.2f*row*row : 0.0f; }
SbVec2f uv(int i) { return SbVec2f(0.031f*(i+1), 0.047f*(i+2)); }
struct Scene {
  SoSeparator * root;
  SoQuadMesh * mesh;
  SoQuadMesh * sibling = NULL;
  explicit Scene(const Spec & s) : root(new SoSeparator), mesh(new SoQuadMesh) {
    root->ref();
    std::vector<SbVec3f> coords(s.start+s.w*s.h, SbVec3f(-10,-10,-10));
    std::vector<SbVec2f> tex(coords.size());
    for (int i=0; i<int(coords.size()); ++i) tex[i]=uv(i);
    for (int r=0; r<s.h; ++r) for (int c=0; c<s.w; ++c)
      coords[s.start+r*s.w+c] = SbVec3f(float(c),float(r),height(s,r));
    std::vector<SbVec3f> normals(count(s,s.normal));
    for (int i=0; i<int(normals.size()); ++i) normals[i]=authoredNormal(i);
    std::vector<SbColor> colors(count(s,s.material));
    std::vector<uint32_t> packed(colors.size());
    for (int i=0; i<int(colors.size()); ++i) {
      colors[i]=SbColor(0.03f*(i+1),0.02f*(i+1),0.01f*(i+1));
      packed[i] = (uint32_t(i+1)<<24) | 0x0000ffffU;
    }
    SoCoordinate3 * coord = new SoCoordinate3;
    coord->point.setValues(0,int(coords.size()),coords.data());
    root->addChild(coord);
    SoNormal * norm = new SoNormal;
    norm->vector.setNum(0);
    if (!s.generated && !normals.empty()) norm->vector.setValues(0,int(normals.size()),normals.data());
    root->addChild(norm);
    SoNormalBinding * nb = new SoNormalBinding;
    nb->value=bindingValue(s.normal,s.indexed);
    root->addChild(nb);
    SoMaterial * mat = new SoMaterial;
    mat->diffuseColor.setNum(0);
    if (!colors.empty()) mat->diffuseColor.setValues(0,int(colors.size()),colors.data());
    root->addChild(mat);
    SoMaterialBinding * mb = new SoMaterialBinding;
    mb->value=bindingValue(s.material,s.indexed);
    root->addChild(mb);
    SoTextureCoordinate2 * tc = new SoTextureCoordinate2;
    tc->point.setValues(0,int(tex.size()),tex.data());
    root->addChild(tc);
    SoTexture2 * texture = new SoTexture2;
    const unsigned char pixel[4]={255,255,255,255};
    texture->image.setValue(SbVec2s(1,1),4,pixel);
    root->addChild(texture);
    mesh->verticesPerRow=s.w;
    mesh->verticesPerColumn=s.h;
    mesh->startIndex=s.start;
    if (s.vp) {
      // Deliberately conflicting inherited data. The attached property wins.
      for (int i=0; i<int(coords.size()); ++i) coord->point.set1Value(i,SbVec3f(-20,-20,-20));
      norm->vector.setNum(1); norm->vector.set1Value(0,SbVec3f(1,0,0));
      nb->value=SoNormalBinding::OVERALL;
      mb->value=SoMaterialBinding::OVERALL;
      mat->diffuseColor.setNum(1); mat->diffuseColor.set1Value(0,SbColor(1,0,0));
      for (int i=0; i<int(tex.size()); ++i) tc->point.set1Value(i,SbVec2f(-0.5f,-0.5f));
      SoVertexProperty * prop = new SoVertexProperty;
      prop->vertex.setValues(0,int(coords.size()),coords.data());
      prop->texCoord.setValues(0,int(tex.size()),tex.data());
      if (!s.generated && !normals.empty()) prop->normal.setValues(0,int(normals.size()),normals.data());
      prop->normalBinding=bindingValue(s.normal,s.indexed);
      prop->materialBinding=bindingValue(s.material,s.indexed);
      if (!packed.empty()) prop->orderedRGBA.setValues(0,int(packed.size()),packed.data());
      mesh->vertexProperty=prop;
    }
    root->addChild(mesh);
    if (s.vp) {
      sibling=new SoQuadMesh; // A genuine sibling sees the original inherited state.
      sibling->verticesPerRow=1; sibling->verticesPerColumn=1;
      root->addChild(sibling);
    }
  }
  ~Scene() { root->unref(); }
  Scene(const Scene &) = delete;
  Scene & operator=(const Scene &) = delete;
};
std::set<int> corners(const Spec & s, int row, int col) {
  const int a=s.start+row*s.w+col;
  return {a,a+1,a+s.w,a+s.w+1};
}
void pointDetail(const Spec & s, const SoPointDetail & p, int r, int c) {
  const int coord=p.getCoordinateIndex();
  require(corners(s,r,c).count(coord)!=0,"coordinate detail belongs to quad");
  equal(p.getMaterialIndex(),index(s,s.material,r,c,coord),"point material index");
  if (!s.generated) equal(p.getNormalIndex(),index(s,s.normal,r,c,coord),"point normal index");
  equal(p.getTextureCoordIndex(),coord-s.start,"relative texture coordinate index");
}
void faceDetail(const Spec & s, const SoFaceDetail & detail, int r, int c) {
  equal(detail.getFaceIndex(),c,"faceIndex (column)");
  equal(detail.getPartIndex(),r,"partIndex (row)");
  equal(detail.getNumPoints(),4,"quad detail point count");
  std::set<int> actual;
  for (int k=0;k<detail.getNumPoints();++k) {
    const SoPointDetail & p=*detail.getPoint(k);
    pointDetail(s,p,r,c);
    actual.insert(p.getCoordinateIndex());
  }
  require(actual==corners(s,r,c),"face detail retains the four quad coordinates");
}
void normalValue(const Spec & s, const SbVec3f & actual, int idx) {
  if (s.generated) {
    require(close(actual.length(),1),"generated unit normal");
    if (s.normal==Face||s.normal==Part) {
      const int row=s.normal==Face ? idx/(s.w-1) : idx;
      SbVec3f expected(0,-0.2f*(2*row+1),1); expected.normalize();
      require(close(std::fabs(actual.dot(expected)),1),"generated normal for folded mesh row");
    }
  } else vectorEqual(actual,authoredNormal(idx),"authored normal value");
}
struct SavedDetail {
  std::unique_ptr<SoFaceDetail> detail;
  int row,col;
  SavedDetail(const SoFaceDetail & d,int r,int c)
    : detail(static_cast<SoFaceDetail *>(d.copy())),row(r),col(c) {}
  SavedDetail(SavedDetail &&) = default;
};
struct Capture {
  const Spec & s;
  std::vector<int> counts;
  std::vector<SavedDetail> saved;
  const SoNode * sibling = NULL;
  bool restored = false;
  std::string error;
  explicit Capture(const Spec & spec) : s(spec),counts(std::max(0,s.w-1)*std::max(0,s.h-1),0) {}
};
SoCallbackAction::Response beforeShapeImpl(void * userdata, SoCallbackAction * action,
                                      const SoNode * node) {
  Capture & capture=*static_cast<Capture *>(userdata);
  if (node!=capture.sibling) return SoCallbackAction::CONTINUE;
  equal(action->getMaterialBinding(),SoMaterialBinding::OVERALL,"restored inherited material binding");
  equal(action->getNormalBinding(),SoNormalBinding::OVERALL,"restored inherited normal binding");
  vectorEqual(action->getCoordinate3(0),SbVec3f(-20,-20,-20),"restored inherited coordinates");
  vectorEqual(action->getNormal(0),SbVec3f(1,0,0),"restored inherited normal");
  SbColor ambient,diffuse,specular,emission;
  float shininess=0,transparency=0;
  action->getMaterial(ambient,diffuse,specular,emission,shininess,transparency);
  vectorEqual(diffuse,SbVec3f(1,0,0),"restored inherited diffuse material");
  const SbVec2f & tex=action->getTextureCoordinate2(0);
  require(close(tex[0],-0.5f)&&close(tex[1],-0.5f),"restored inherited texture coordinates");
  capture.restored=true;
  return SoCallbackAction::CONTINUE;
}
void triangleImpl(void * userdata, SoCallbackAction *, const SoPrimitiveVertex * a,
              const SoPrimitiveVertex * b, const SoPrimitiveVertex * c) {
  Capture & capture=*static_cast<Capture *>(userdata);
  const Spec & s=capture.s;
  const SoPrimitiveVertex * v[3]={a,b,c};
  const int col=int(std::floor((a->getPoint()[0]+b->getPoint()[0]+c->getPoint()[0])/3));
  const int row=int(std::floor((a->getPoint()[1]+b->getPoint()[1]+c->getPoint()[1])/3));
  require(row>=0&&row<s.h-1&&col>=0&&col<s.w-1,"triangle lies in mesh");
  ++capture.counts[row*(s.w-1)+col];
  std::set<int> triangleCoords;
  for (int k=0;k<3;++k) {
    const int coord=s.start+int(v[k]->getPoint()[1])*s.w+int(v[k]->getPoint()[0]);
    require(corners(s,row,col).count(coord)!=0,"triangle coordinate belongs to quad");
    vectorEqual(v[k]->getPoint(),SbVec3f(float((coord-s.start)%s.w),float((coord-s.start)/s.w),height(s,(coord-s.start)/s.w)),"explicit coordinate");
    triangleCoords.insert(coord);
    equal(v[k]->getMaterialIndex(),index(s,s.material,row,col,coord),"vertex material index");
    normalValue(s,v[k]->getNormal(),index(s,s.normal,row,col,coord));
    const SbVec4f & t=v[k]->getTextureCoords();
    require(close(t[0],uv(coord-s.start)[0])&&close(t[1],uv(coord-s.start)[1])&&close(t[2],0)&&close(t[3],1),"explicit callback UV");
    const SoDetail * raw=v[k]->getDetail();
    require(raw&&raw->isOfType(SoFaceDetail::getClassTypeId()),"callback SoFaceDetail");
    const SoFaceDetail & d=*static_cast<const SoFaceDetail *>(raw);
    faceDetail(s,d,row,col);
    capture.saved.emplace_back(d,row,col);
  }
  equal(int(triangleCoords.size()),3,"three distinct triangle coordinates");
}
// Do not unwind through Coin traversal: let its temporary state close normally.
SoCallbackAction::Response beforeShape(void * userdata, SoCallbackAction * action,
                                      const SoNode * node) {
  Capture & capture=*static_cast<Capture *>(userdata);
  if (!capture.error.empty()) return SoCallbackAction::CONTINUE;
  try { return beforeShapeImpl(userdata,action,node); }
  catch (const std::exception & e) { capture.error=e.what(); }
  return SoCallbackAction::CONTINUE;
}
void triangle(void * userdata, SoCallbackAction * action, const SoPrimitiveVertex * a,
              const SoPrimitiveVertex * b, const SoPrimitiveVertex * c) {
  Capture & capture=*static_cast<Capture *>(userdata);
  if (!capture.error.empty()) return;
  try { triangleImpl(userdata,action,a,b,c); }
  catch (const std::exception & e) { capture.error=e.what(); }
}
void callbacks(Scene & scene,const Spec & s) {
  Capture capture(s);
  capture.sibling=scene.sibling;
  {
    SoCallbackAction action;
    action.addPreCallback(SoQuadMesh::getClassTypeId(),beforeShape,&capture);
    action.addTriangleCallback(SoQuadMesh::getClassTypeId(),triangle,&capture);
    action.apply(scene.root);
  }
  if (!capture.error.empty()) throw std::runtime_error(capture.error);
  if (s.vp) require(capture.restored,"visited sibling after vertex property");
  for (int n:capture.counts) equal(n,2,"two callback triangles per quadrilateral");
  equal(int(capture.saved.size()),6*int(capture.counts.size()),"total callback vertices");
  // Copies outlive the action and all subsequent mutations of its transient detail.
  for (const SavedDetail & d:capture.saved) faceDetail(s,*d.detail,d.row,d.col);
}
void picks(Scene & scene,const Spec & s) {
  std::vector<std::unique_ptr<SoPickedPoint> > saved;
  for (int r=0;r<s.h-1;++r) for (int c=0;c<s.w-1;++c) {
    // Two interior rays exercise both constituent triangles without choosing a
    // different QUAD_STRIP winding or demanding a new diagonal convention.
    for (int half=0;half<2;++half) {
      const float x=c+(half?0.78f:0.19f), y=r+(half?0.64f:0.31f);
      const float z=height(s,r)+(y-r)*(height(s,r+1)-height(s,r));
      {
        SoRayPickAction action(SbViewportRegion(64,64));
        action.setRay(SbVec3f(x,y,5),SbVec3f(0,0,-1));
        action.apply(scene.root);
        SoPickedPoint * pp=action.getPickedPoint();
        require(pp!=NULL,"ray hits quadrilateral");
        vectorEqual(pp->getPoint(),SbVec3f(x,y,z),"picked location");
        const SoDetail * raw=pp->getDetail(scene.mesh);
        require(raw&&raw->isOfType(SoFaceDetail::getClassTypeId()),"picked SoFaceDetail");
        faceDetail(s,*static_cast<const SoFaceDetail *>(raw),r,c);
        if (s.material!=Vertex) equal(pp->getMaterialIndex(),index(s,s.material,r,c,0),"picked material");
        else {
          std::set<int> valid;
          for (int coord:corners(s,r,c)) valid.insert(coord-s.start);
          require(valid.count(pp->getMaterialIndex())!=0,"picked vertex material belongs to quad");
        }
        if (s.generated||s.normal!=Vertex) normalValue(s,pp->getNormal(),index(s,s.normal,r,c,0));
        saved.emplace_back(pp->copy());
      }
      const SoPickedPoint & copy=*saved.back();
      vectorEqual(copy.getPoint(),SbVec3f(x,y,z),"copied pick location after action destruction");
      faceDetail(s,*static_cast<const SoFaceDetail *>(copy.getDetail(scene.mesh)),r,c);
    }
  }
  // Keep all copies while subsequent actions overwrite their own details.
  int at=0;
  for (int r=0;r<s.h-1;++r) for (int c=0;c<s.w-1;++c) for (int k=0;k<2;++k)
    faceDetail(s,*static_cast<const SoFaceDetail *>(saved[at++]->getDetail(scene.mesh)),r,c);
  if (s.w<2||s.h<2) {
    SoRayPickAction action(SbViewportRegion(64,64));
    action.setRay(SbVec3f(0.2f,0.3f,5),SbVec3f(0,0,-1)); action.apply(scene.root);
    require(action.getPickedPoint()==NULL,"no cells means no picked surface");
  }
}
void defaults() {
  SoQuadMesh * mesh=new SoQuadMesh; mesh->ref();
  equal(mesh->startIndex.getValue(),0,"Coin default startIndex");
  equal(mesh->verticesPerRow.getValue(),1,"Coin default row vertices");
  equal(mesh->verticesPerColumn.getValue(),1,"Coin default column vertices");
  require(mesh->vertexProperty.getValue()==NULL,"Coin default vertex property");
  mesh->unref();
  SoMaterialBinding * mb=new SoMaterialBinding; mb->ref();
  equal(mb->value.getValue(),SoMaterialBinding::OVERALL,"default material binding"); mb->unref();
  SoNormalBinding * nb=new SoNormalBinding; nb->ref();
  equal(nb->value.getValue(),SoNormalBinding::PER_VERTEX_INDEXED,"Coin default normal binding"); nb->unref();
}
void run(const std::string & mode,const std::string & action) {
  if (mode=="defaults") { defaults(); return; }
  std::vector<Spec> cases;
  const int sizes[][2]={{2,2},{3,2},{2,3},{3,3},{4,3}};
  for (const auto & size:sizes) for (int start:{0,3}) {
    const int w=size[0],h=size[1];
    if (mode=="controls") for (Binding b:{Overall,Part,Vertex}) cases.emplace_back(w,h,start,b,b);
    else if (mode=="material") for (Binding n:{Overall,Part,Vertex}) cases.emplace_back(w,h,start,Face,n);
    else if (mode=="normal") for (Binding m:{Overall,Part,Vertex}) cases.emplace_back(w,h,start,m,Face);
    else if (mode=="combined") cases.emplace_back(w,h,start,Face,Face);
    else if (mode=="aliases") for (Binding b:{Overall,Part,Face,Vertex}) {
      cases.emplace_back(w,h,start,Face,b,true);
      cases.emplace_back(w,h,start,b,Face,true);
    }
    else if (mode=="generated") for (Binding n:{Face,Part,Vertex}) cases.emplace_back(w,h,start,Face,n,false,true);
    else if (mode=="vertex-property") for (Binding b:{Overall,Part,Face,Vertex}) {
      cases.emplace_back(w,h,start,Face,b,false,false,true);
      cases.emplace_back(w,h,start,b,Face,true,false,true);
    }
  }
  if (mode=="empty") for (int w:{0,1,3}) for (int h:{0,1,3})
    if (w<2||h<2) cases.emplace_back(w,h,0,Overall,Overall);
  require(!cases.empty(),"unknown mode");
  for (const Spec & s:cases) {
    std::ostringstream name;
    name<<mode<<"/"<<action<<" "<<s.w<<"x"<<s.h<<" start="<<s.start
        <<" material="<<s.material<<" normal="<<s.normal;
    label=name.str(); std::cout<<label<<std::endl;
    Scene scene(s);
    if (action=="callback") callbacks(scene,s);
    else if (action=="pick") picks(scene,s);
    else require(false,"unknown action");
  }
}
}
int main(int argc,char ** argv) {
  if (argc!=3) { std::cerr<<"usage: coin-quadmesh-per-face MODE callback|pick\n"; return 2; }
  SoDB::init();
  int result=0;
  try { run(argv[1],argv[2]); }
  catch (const std::exception & e) { std::cerr<<e.what()<<std::endl; result=1; }
  // All fixtures, copied details and actions have been destroyed at this point.
  SoDB::finish();
  return result;
}
