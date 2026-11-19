// VRML analytic picking through public APIs, with independent geometry oracles.
#include <Inventor/SoDB.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/SoPath.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/details/SoCubeDetail.h>
#include <Inventor/details/SoConeDetail.h>
#include <Inventor/details/SoCylinderDetail.h>
#include <Inventor/elements/SoMultiTextureCoordinateElement.h>
#include <Inventor/lists/SoPickedPointList.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoClipPlane.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoTextureCoordinate2.h>
#include <Inventor/nodes/SoTextureCoordinate3.h>
#include <Inventor/nodes/SoTextureCoordinateDefault.h>
#include <Inventor/nodes/SoTextureCoordinateNormalMap.h>
#include <Inventor/nodes/SoTextureCoordinateReflectionMap.h>
#include <Inventor/nodes/SoTextureCoordinateObject.h>
#include <Inventor/nodes/SoTextureCoordinatePlane.h>
#include <Inventor/nodes/SoTextureCubeMap.h>
#include <Inventor/nodes/SoTextureMatrixTransform.h>
#include <Inventor/nodes/SoTextureUnit.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/VRMLnodes/SoVRMLBox.h>
#include <Inventor/VRMLnodes/SoVRMLCone.h>
#include <Inventor/VRMLnodes/SoVRMLCylinder.h>
#include <math.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

namespace {
int failures=0,checks=0;
std::string context;
void check(bool good,const char * message) {
  ++checks;
  if(!good) { ++failures; std::fprintf(stderr,"FAIL %s: %s\n",context.c_str(),message); }
}
void near4(const SbVec4f & a,const SbVec4f & b,const char * message) {
  bool good=true; for(int j=0;j<4;++j) good=good && fabs(a[j]-b[j])<8e-5f;
  check(good,message);
  if(!good) std::fprintf(stderr,"  actual (%g,%g,%g,%g) expected (%g,%g,%g,%g)\n",a[0],a[1],a[2],a[3],b[0],b[1],b[2],b[3]);
}
void near3(const SbVec3f & a,const SbVec3f & b,const char * message) {
  near4(SbVec4f(a[0],a[1],a[2],1),SbVec4f(b[0],b[1],b[2],1),message);
}
bool bits(const SbVec4f & a,const SbVec4f & b) { return std::memcmp(a.getValue(),b.getValue(),sizeof(float)*4)==0; }
SbVec3f norm(SbVec3f n) { n.normalize(); return n; }
SbVec4f value(const SbVec3f & p,const SbVec3f & n,float tag) {
  return SbVec4f(tag+2*p[0]-3*p[2]+n[1],-tag+5*p[1]+n[2],p[2]+7*n[0],tag+.25f*p[0]-.5f*n[2]);
}
struct Function {
  Function(float t,int u=0):tag(t),unit(u),fixed(false) {}
  float tag; int unit; bool fixed; SbVec4f result;
  std::vector<SbVec3f> points,normals;
  void clear() { points.clear(); normals.clear(); }
};
const SbVec4f & generate(void * data,const SbVec3f & p,const SbVec3f & n) {
  Function & f=*static_cast<Function *>(data); f.points.push_back(p); f.normals.push_back(n);
  if(!f.fixed) f.result=value(p,n,f.tag);
  return f.result;
}
void install(void * data,SoAction * action) {
  if(action->isOfType(SoRayPickAction::getClassTypeId())) {
    Function & f=*static_cast<Function *>(data);
    SoMultiTextureCoordinateElement::setFunction(action->getState(),action->getCurPathTail(),f.unit,generate,&f);
  }
}
void function(SoGroup * root,Function & f) { SoCallback * cb=new SoCallback; cb->setCallback(install,&f); root->addChild(cb); }
void unit(SoGroup * root,int u) { SoTextureUnit * n=new SoTextureUnit; n->unit=u; root->addChild(n); }
SoSeparator * scene() { SoSeparator * n=new SoSeparator; n->ref(); n->pickCulling=SoSeparator::OFF; return n; }
void cubeMap(SoGroup * root) {
  SoTextureCubeMap * n=new SoTextureCubeMap;
  const unsigned char pixel[]={17,29,43,255}; n->imagePosX.setValue(SbVec2s(1,1),4,pixel); root->addChild(n);
}
class DerivedBox : public SoVRMLBox {
  SO_NODE_HEADER(DerivedBox);
public:
  static void initClass();
  DerivedBox();
protected:
  ~DerivedBox() override = default;
};
class DerivedCone : public SoVRMLCone {
  SO_NODE_HEADER(DerivedCone);
public:
  static void initClass();
  DerivedCone();
protected:
  ~DerivedCone() override = default;
};
class DerivedCylinder : public SoVRMLCylinder {
  SO_NODE_HEADER(DerivedCylinder);
public:
  static void initClass();
  DerivedCylinder();
protected:
  ~DerivedCylinder() override = default;
};
SO_NODE_SOURCE(DerivedBox);
SO_NODE_SOURCE(DerivedCone);
SO_NODE_SOURCE(DerivedCylinder);
void DerivedBox::initClass() { SO_NODE_INIT_CLASS(DerivedBox,SoVRMLBox,"VRMLBox"); }
void DerivedCone::initClass() { SO_NODE_INIT_CLASS(DerivedCone,SoVRMLCone,"VRMLCone"); }
void DerivedCylinder::initClass() { SO_NODE_INIT_CLASS(DerivedCylinder,SoVRMLCylinder,"VRMLCylinder"); }
DerivedBox::DerivedBox() { SO_NODE_CONSTRUCTOR(DerivedBox); }
DerivedCone::DerivedCone() { SO_NODE_CONSTRUCTOR(DerivedCone); }
DerivedCylinder::DerivedCylinder() { SO_NODE_CONSTRUCTOR(DerivedCylinder); }
const int kinds=3;
SoNode * shape(int kind,bool derived=false,int parts=-1) {
  if(kind==0) {
    SoVRMLBox * n=derived?new DerivedBox:new SoVRMLBox; n->size=SbVec3f(4,6,8); return n;
  }
  if(kind==1) {
    SoVRMLCone * n=derived?new DerivedCone:new SoVRMLCone; n->bottomRadius=2; n->height=4;
    if(parts>=0) { n->side=(parts&1)!=0; n->bottom=(parts&2)!=0; } return n;
  }
  SoVRMLCylinder * n=derived?new DerivedCylinder:new SoVRMLCylinder; n->radius=2; n->height=4;
  if(parts>=0) { n->side=(parts&1)!=0; n->top=(parts&2)!=0; n->bottom=(parts&4)!=0; } return n;
}
struct Transform {
  Transform(bool transformed=false):scale(transformed?SbVec3f(2,3,4):SbVec3f(1,1,1)),translation(transformed?SbVec3f(7,-5,2):SbVec3f(0,0,0)) {}
  SbVec3f scale,translation;
  SbVec3f vector(const SbVec3f & p) const { return SbVec3f(p[0]*scale[0],p[1]*scale[1],p[2]*scale[2]); }
  SbVec3f world(const SbVec3f & p) const { return vector(p)+translation; }
  SbVec3f normal(const SbVec3f & n) const { return norm(SbVec3f(n[0]/scale[0],n[1]/scale[1],n[2]/scale[2])); }
  void add(SoGroup * root) const { SoTransform * t=new SoTransform; t->scaleFactor=scale; t->translation=translation; root->addChild(t); }
};
struct Hit { SbVec3f p,n; int part,material; };
struct Case {
  int kind,parts; SbVec3f start,direction; std::vector<Hit> hits;
  std::string name;
};
Case sideCase(int kind) {
  Case c; c.kind=kind; c.parts=-1; c.start=SbVec3f(.3f,.4f,10); c.direction=SbVec3f(0,0,-1); c.name="side";
  float z=kind==0?4.0f:kind==1?float(sqrt(0.8f*0.8f-.3f*.3f)):float(sqrt(4-.3f*.3f));
  for(int s=1;s>=-1;s-=2) {
    Hit h; h.p=SbVec3f(.3f,.4f,s*z);
    h.n=kind==0?SbVec3f(0,0,float(s)):kind==1?norm(SbVec3f(.3f,.4f,s*z)):norm(SbVec3f(.3f,0,s*z));
    h.part=kind==0?(s==1?0:1):1; h.material=0; c.hits.push_back(h);
  }
  return c;
}
std::vector<Case> cases() {
  std::vector<Case> result;
  for(int k=0;k<kinds;++k) result.push_back(sideCase(k));
  for(int axis=0;axis<2;++axis) {
    Case c; c.kind=0; c.parts=-1; c.name=axis==0?"cube x faces":"cube y faces";
    c.start=axis==0?SbVec3f(10,.4f,.7f):SbVec3f(.3f,10,.7f);
    c.direction=axis==0?SbVec3f(-1,0,0):SbVec3f(0,-1,0);
    for(int s=1;s>=-1;s-=2) {
      Hit h; h.p=axis==0?SbVec3f(2.0f*s,.4f,.7f):SbVec3f(.3f,3.0f*s,.7f);
      h.n=axis==0?SbVec3f(float(s),0,0):SbVec3f(0,float(s),0);
      h.part=axis==0?(s==1?3:2):(s==1?4:5); h.material=0; c.hits.push_back(h);
    }
    result.push_back(c);
  }
  for(int k=1;k<=2;++k) {
    Case c; c.kind=k; c.parts=-1; c.name=k==1?"cone side and bottom":"cylinder caps";
    c.start=SbVec3f(.3f,10,.4f); c.direction=SbVec3f(0,-1,0);
    Hit a,b; a.p=SbVec3f(.3f,k==1?1.0f:2.0f,.4f); b.p=SbVec3f(.3f,-2,.4f);
    a.n=k==1?norm(SbVec3f(.3f,.25f,.4f)):SbVec3f(0,1,0); b.n=SbVec3f(0,-1,0);
    a.part=k==1?int(SoCone::SIDES):int(SoCylinder::TOP);
    b.part=k==1?int(SoCone::BOTTOM):int(SoCylinder::BOTTOM);
    a.material=0; b.material=0;
    c.hits.push_back(a); c.hits.push_back(b); result.push_back(c);
    for(int mask=0;mask<(k==1?4:8);++mask) {
      Case limited=c; limited.name="parts="+std::to_string(mask); limited.parts=mask; limited.hits.clear();
      if(mask&a.part) limited.hits.push_back(a);
      if(mask&b.part) limited.hits.push_back(b);
      result.push_back(limited);
    }
  }
  for(int k=1;k<=2;++k) {
    for(int mask=0;mask<(k==1?4:8);++mask) {
      Case c=sideCase(k); c.parts=mask; c.name="side parts="+std::to_string(mask);
      if(!(mask&1)) c.hits.clear(); result.push_back(c);
    }
  }
  return result;
}
// Each legacy expression intentionally retains the old Float operations and
// global math overloads. Explicit coordinates have historically been ignored.
SbVec4f legacy(int kind,const Hit & h) {
  const SbVec3f & p=h.p; const SbVec3f & n=h.n;
  const double pi=3.14159265358979323846;
  if(kind==0) {
    if(n[0]!=0) return SbVec4f(n[0]>0?1-(p[2]+4)/8:(p[2]+4)/8,(p[1]+3)/6,0,1);
    if(n[1]!=0) return SbVec4f((p[0]+2)/4,n[1]>0?1-(p[2]+4)/8:(p[2]+4)/8,0,1);
    return SbVec4f(n[2]<0?1-(p[0]+2)/4:(p[0]+2)/4,(p[1]+3)/6,0,1);
  }
  if(kind==1) {
    if(h.part==SoCone::BOTTOM) return SbVec4f(.5f+p[0]/4,.5f+p[2]/4,0,1);
    return SbVec4f(float(atan2(p[0],p[2])*(1.0/(2.0*pi))+.5),-(p[1]-2.0f)/4.0f,0,1);
  }
  if(kind==2) {
    if(h.part==SoCylinder::TOP) return SbVec4f(.5f+p[0]/4,.5f-p[2]/4,0,1);
    if(h.part==SoCylinder::BOTTOM) return SbVec4f(.5f+p[0]/4,.5f+p[2]/4,0,1);
    return SbVec4f(float(atan2(p[0],p[2]))*(1.0f/(2.0f*float(pi)))+.5f,(p[1]+2.0f)/4.0f,0,1);
  }
  return SbVec4f(0,0,0,1);
}
void materials(SoGroup * root) {
  SoMaterial * m=new SoMaterial;
  for(int i=0;i<6;++i) m->diffuseColor.set1Value(i,SbColor(float(i)/6,.3f,.7f));
  root->addChild(m); SoMaterialBinding * b=new SoMaterialBinding; b->value=SoMaterialBinding::PER_PART; root->addChild(b);
}
void detail(const SoPickedPoint * pp,int kind,const Hit & h) {
  const SoDetail * d=pp->getDetail();
  if(kind==0) { check(d && d->isOfType(SoCubeDetail::getClassTypeId()),"cube detail type"); if(d && d->isOfType(SoCubeDetail::getClassTypeId())) check(static_cast<const SoCubeDetail *>(d)->getPart()==h.part,"cube face detail"); }
  if(kind==1) { check(d && d->isOfType(SoConeDetail::getClassTypeId()),"cone detail type"); if(d && d->isOfType(SoConeDetail::getClassTypeId())) check(static_cast<const SoConeDetail *>(d)->getPart()==h.part,"cone part detail"); }
  if(kind==2) { check(d && d->isOfType(SoCylinderDetail::getClassTypeId()),"cylinder detail type"); if(d && d->isOfType(SoCylinderDetail::getClassTypeId())) check(static_cast<const SoCylinderDetail *>(d)->getPart()==h.part,"cylinder part detail"); }
  check(pp->getMaterialIndex()==h.material,"legacy material assignment retained");
}
void prepare(SoRayPickAction & a,const Case & c,const Transform & t,bool all=true,float nearDistance=-1,float farDistance=-1) {
  a.enableCulling(FALSE); a.setPickAll(all); a.setRay(t.world(c.start),norm(t.vector(c.direction)),nearDistance,farDistance);
}
void analyticCases(bool functional) {
  for(const Case & c:cases()) for(int transformed=0;transformed<2;++transformed) for(int all=0;all<2;++all) for(int state=0;state<(functional?2:6);++state) {
    context="kind="+std::to_string(c.kind)+" "+c.name+" transform="+std::to_string(transformed)+" all="+std::to_string(all)+" state="+std::to_string(state);
    Function f(11),other(73,1); SoSeparator * root=scene();
    if(functional) { if(state) cubeMap(root); function(root,f); unit(root,1); function(root,other); }
    else {
      if(state==1) { SoTextureCoordinate2 * tc=new SoTextureCoordinate2; tc->point.set1Value(0,SbVec2f(.125f,.875f)); root->addChild(tc); }
      if(state==2) { unit(root,1); function(root,other); }
      if(state==3) { cubeMap(root); root->addChild(new SoTextureCoordinateDefault); }
      if(state==4) { function(root,f); root->addChild(new SoTextureCoordinateDefault); }
      if(state==5) {
        function(root,f); SoTextureCoordinate3 * tc=new SoTextureCoordinate3;
        tc->point.set1Value(0,SbVec3f(.25f,.75f,-3)); root->addChild(tc);
      }
    }
    Transform t(transformed); t.add(root); materials(root); SoNode * node=shape(c.kind,transformed,c.parts); root->addChild(node);
    const SoType base=c.kind==0?SoVRMLBox::getClassTypeId():c.kind==1?SoVRMLCone::getClassTypeId():SoVRMLCylinder::getClassTypeId();
    check(node->getTypeId().isDerivedFrom(base),"fixture has expected VRML ancestry");
    check((node->getTypeId()!=base)==bool(transformed),"derived fixture has a distinct registered Coin type");
    {
      SoRayPickAction a(SbViewportRegion(64,64)); prepare(a,c,t,all); a.apply(root);
      const int count=all?int(c.hits.size()):(c.hits.empty()?0:1);
      check(a.getPickedPointList().getLength()==count,"analytic hit count and parts retained");
      for(int i=0;i<a.getPickedPointList().getLength() && i<count;++i) {
        const SoPickedPoint * p=a.getPickedPoint(i); const Hit & h=c.hits[i];
        near3(p->getObjectPoint(),h.p,"real analytic object point"); near3(p->getPoint(),t.world(h.p),"world intersection");
        near3(p->getObjectNormal(),h.n,"legacy object normal"); near3(p->getNormal(),t.vector(h.n),"legacy transformed normal");
        check(p->getPath()->getTail()==node && p->isOnGeometry(),"actual shape path and geometry flag"); detail(p,c.kind,h);
        if(functional) {
          near4(p->getObjectTextureCoords(),value(h.p,h.n,11),"unit zero function uses real point and legacy normal, all four components");
          // Compare callback arguments to assigned data as well as the independent geometry oracle.
          bool found=false; for(size_t j=0;j<f.points.size();++j) {
            if((f.points[j]-p->getObjectPoint()).length()<8e-5f) {
              found=true; near3(f.normals[j],p->getObjectNormal(),"function receives exactly the assigned object normal");
              check(std::memcmp(f.normals[j].getValue(),p->getObjectNormal().getValue(),3*sizeof(float))==0,"function normal bit pattern matches picked normal");
            }
          }
          check(found,"retained hit evaluated its function");
        }
        else {
          // Use the actual analytic hit to avoid making the independent quadratic
          // oracle's last-bit rounding part of the legacy texture contract.
          Hit actual=h; actual.p=p->getObjectPoint(); actual.n=p->getObjectNormal();
          check(bits(p->getObjectTextureCoords(),legacy(c.kind,actual)),"legacy mapping and Float operations bitwise unchanged");
        }
      }
      check(other.points.empty(),"selected unit one callback never used");
      if(all) check(f.points.size()==size_t(functional?count:0),"pickAll evaluates only accepted intersections");
      // closest-only may first accept a farther provisional face or cap.
      if(!functional || count==0) check(f.points.empty(),"no function evaluation without accepted hits or unit zero function");
    }
    root->unref();
  }
}
void homogeneous() {
  const SbVec4f values[]={SbVec4f(2,-3,5,0),SbVec4f(2,-3,5,-7),SbVec4f(-0.0f,0,-0.0f,-0.0f)};
  for(const Case & c:cases()) for(const SbVec4f & v:values) {
    context="homogeneous kind="+std::to_string(c.kind)+" "+c.name; Function f(0); f.fixed=true; f.result=v;
    SoSeparator * root=scene(); function(root,f);
    SoTextureMatrixTransform * texture=new SoTextureMatrixTransform; SbMatrix matrix; matrix.makeIdentity();
    matrix[0][0]=2; matrix[1][1]=3; matrix[2][2]=4; matrix[3][0]=5; matrix[3][1]=-6; matrix[3][2]=7;
    texture->matrix=matrix; root->addChild(texture); root->addChild(shape(c.kind,false,c.parts));
    {
      Transform t; SoRayPickAction a(SbViewportRegion(64,64)); prepare(a,c,t); a.apply(root);
      check(a.getPickedPointList().getLength()==int(c.hits.size()),"homogeneous scene retains analytic hits");
      for(int i=0;i<a.getPickedPointList().getLength();++i) {
        const SoPickedPoint * p=a.getPickedPoint(i);
        check(bits(p->getObjectTextureCoords(),v),"raw S T R Q preserve negative Q, zero Q and signed zeros");
        near4(p->getTextureCoords(),SbVec4f(2*v[0]+5*v[3],3*v[1]-6*v[3],4*v[2]+7*v[3],v[3]),"texture matrix applied once with homogeneous Q, without division");
      }
    }
    root->unref();
  }
}
void generators() {
  for(const Case & c:cases()) for(int transformed=0;transformed<2;++transformed) for(int kind=0;kind<5;++kind) {
    context="generator="+std::to_string(kind)+" shape="+std::to_string(c.kind)+" "+c.name;
    const SbVec3f eye(0,0,30); Transform t(transformed); SoSeparator * root=scene();
    SoOrthographicCamera * camera=new SoOrthographicCamera; camera->position=eye; root->addChild(camera);
    cubeMap(root);
    if(kind==0) root->addChild(new SoTextureCoordinateNormalMap);
    if(kind==1) root->addChild(new SoTextureCoordinateReflectionMap);
    if(kind==2) {
      SoTextureCoordinateObject * g=new SoTextureCoordinateObject;
      g->factorS=SbVec4f(2,0,0,3); g->factorT=SbVec4f(0,-4,0,-5); g->factorR=SbVec4f(0,0,6,7); g->factorQ=SbVec4f(1,0,0,-2); root->addChild(g);
    }
    if(kind==3) {
      SoTextureCoordinatePlane * g=new SoTextureCoordinatePlane;
      g->directionS=SbVec3f(2,0,0); g->directionT=SbVec3f(0,-4,0); g->directionR=SbVec3f(0,0,6); root->addChild(g);
    }
    // kind 4 is CubeMap alone, whose legacy coordinate mapping must survive.
    t.add(root); root->addChild(shape(c.kind,false,c.parts));
    {
      SoRayPickAction a(SbViewportRegion(64,64)); prepare(a,c,t); a.apply(root);
      check(a.getPickedPointList().getLength()==int(c.hits.size()),"generator preserves analytic intersections");
      for(int i=0;i<a.getPickedPointList().getLength() && i<int(c.hits.size());++i) {
        const SoPickedPoint * pp=a.getPickedPoint(i); const Hit & h=c.hits[i];
        SbVec4f expected;
        if(kind==0 || kind==1) {
          SbVec3f v=t.normal(h.n);
          if(kind==1) { const SbVec3f incident=norm(t.world(h.p)-eye); v=incident-2*incident.dot(v)*v; }
          expected=SbVec4f(v[0],v[1],v[2],1);
        }
        if(kind==2) expected=SbVec4f(2*h.p[0]+3,-4*h.p[1]-5,6*h.p[2]+7,h.p[0]-2);
        if(kind==3) expected=SbVec4f(2*h.p[0],-4*h.p[1],6*h.p[2],1);
        if(kind==4) { Hit actual=h; actual.p=pp->getObjectPoint(); actual.n=pp->getObjectNormal(); expected=legacy(c.kind,actual); }
        near4(pp->getObjectTextureCoords(),expected,"real generator consumes actual point and assigned normal");
      }
    }
    root->unref();
  }
}
struct Snapshot {
  SbVec3f p,n; SbVec4f tc; int material,part; SoNode * tail;
};
int part(const SoPickedPoint * pp) {
  const SoDetail * d=pp->getDetail(); if(!d) return -1;
  if(d->isOfType(SoCubeDetail::getClassTypeId())) return static_cast<const SoCubeDetail *>(d)->getPart();
  if(d->isOfType(SoConeDetail::getClassTypeId())) return static_cast<const SoConeDetail *>(d)->getPart();
  if(d->isOfType(SoCylinderDetail::getClassTypeId())) return static_cast<const SoCylinderDetail *>(d)->getPart();
  return -2;
}
std::vector<Snapshot> snapshot(SoRayPickAction & a) {
  std::vector<Snapshot> result;
  for(int i=0;i<a.getPickedPointList().getLength();++i) {
    const SoPickedPoint * p=a.getPickedPoint(i); Snapshot s;
    s.p=p->getObjectPoint(); s.n=p->getObjectNormal(); s.tc=p->getObjectTextureCoords(); s.material=p->getMaterialIndex(); s.part=part(p); s.tail=p->getPath()->getTail(); result.push_back(s);
  }
  return result;
}
void unchangedGeometry(SoRayPickAction & a,const std::vector<Snapshot> & expected) {
  check(a.getPickedPointList().getLength()==int(expected.size()),"function leaves legacy hit count unchanged");
  for(int i=0;i<a.getPickedPointList().getLength() && i<int(expected.size());++i) {
    const SoPickedPoint * p=a.getPickedPoint(i); const Snapshot & s=expected[i];
    check(p->getObjectPoint()==s.p && p->getObjectNormal()==s.n,"function leaves legacy geometry and order unchanged");
    check(p->getMaterialIndex()==s.material && part(p)==s.part && p->getPath()->getTail()==s.tail,"function leaves material/detail/path unchanged");
    near4(p->getObjectTextureCoords(),value(s.p,s.n,17),"function on retained legacy intersection");
  }
}
void boundaries() {
  // Keep degeneracy and clipping expectations separate from texture semantics:
  // compare the same real shape before/after installing a function.
  for(int k=0;k<kinds;++k) for(int all=0;all<2;++all) for(int mode=0;mode<10;++mode) {
    context="boundary kind="+std::to_string(k)+" all="+std::to_string(all)+" mode="+std::to_string(mode);
    Case c=sideCase(k); Transform t; Function f(17); SoSeparator * root=scene();
    SoCallback * setter=new SoCallback; root->addChild(setter);
    if(mode==3) { SoClipPlane * clip=new SoClipPlane; clip->plane=SbPlane(SbVec3f(0,0,-1),0); root->addChild(clip); }
    SoNode * node=shape(k); root->addChild(node);
    if(mode==5) c.start=SbVec3f(20,.4f,10); // miss
    if(mode==6) { c.start=SbVec3f(0,0,0); } // inside
    if(mode==7) { c.start=SbVec3f(k==1?.8f:2.0f,.4f,10); } // tangent, or cube edge
    if(mode==8) {
      if(k==0) static_cast<SoVRMLBox *>(node)->size=SbVec3f(0,6,8);
      if(k==1) { static_cast<SoVRMLCone *>(node)->side=FALSE; static_cast<SoVRMLCone *>(node)->bottom=FALSE; }
      if(k==2) { SoVRMLCylinder * n=static_cast<SoVRMLCylinder *>(node); n->side=FALSE; n->top=FALSE; n->bottom=FALSE; }
    }
    if(mode==9) { c.start=SbVec3f(.3f,-10,.4f); c.direction=SbVec3f(0,1,0); } // caps replace provisional hits in closest mode
    {
      SoRayPickAction a(SbViewportRegion(64,64));
      const float nearDistance=mode==1?10.0f:-1,farDistance=mode==2?10.0f:mode==4?.5f:-1;
      prepare(a,c,t,all,nearDistance,farDistance); a.apply(root); const std::vector<Snapshot> before=snapshot(a);
      setter->setCallback(install,&f); a.apply(root); unchangedGeometry(a,before);
      if(all) check(f.points.size()==before.size(),"only unclipped successfully added pickAll hits invoke function");
      if(before.empty()) check(f.points.empty(),"miss and fully clipped roots never invoke function");
      for(size_t j=0;j<f.points.size();++j) {
        if(mode==3) check(f.points[j][2]<=0,"clip plane rejected point never invokes function");
        const float distance=(f.points[j]-c.start).dot(c.direction);
        if(nearDistance>=0) check(distance>=nearDistance,"near rejected point never invokes function");
        if(farDistance>=0) check(distance<=farDistance,"far rejected point never invokes function");
      }
      // Reusing an action may not leak a function after the callback is removed.
      setter->setCallback(NULL,NULL); a.apply(root);
      const std::vector<Snapshot> after=snapshot(a); check(after.size()==before.size(),"action reuse restores legacy hit count");
      for(size_t i=0;i<after.size() && i<before.size();++i) check(bits(after[i].tc,before[i].tc),"action reuse restores legacy texture bits");
    }
    root->unref();
  }
  // A prior, nearer intersection makes addIntersection reject the target.
  for(int k=0;k<kinds;++k) {
    context="rejected addIntersection kind="+std::to_string(k); Function f(17); SoSeparator * root=scene();
    SoSeparator * front=new SoSeparator; root->addChild(front); SoTransform * tr=new SoTransform; tr->translation=SbVec3f(0,0,6); front->addChild(tr); front->addChild(new SoCube);
    function(root,f); root->addChild(shape(k));
    { SoRayPickAction a(SbViewportRegion(64,64)); Case c=sideCase(k); Transform t; prepare(a,c,t,false); a.apply(root); check(a.getPickedPoint()!=NULL,"front occluder has hit"); check(f.points.empty(),"failed addIntersection never evaluates target callback"); }
    root->unref();
  }
}
void scopes() {
  for(int k=0;k<kinds;++k) {
    context="scopes and reuse kind="+std::to_string(k); Function outer(23),inner(59);
    SoSeparator * root=scene(); function(root,outer); SoNode * shared=shape(k);
    for(int branch=0;branch<3;++branch) {
      SoSeparator * sep=new SoSeparator; sep->pickCulling=SoSeparator::OFF; root->addChild(sep);
      SoTransform * t=new SoTransform; t->translation=SbVec3f(0,0,float(-12*branch)); sep->addChild(t);
      if(branch==0) function(sep,inner);
      if(branch==2) sep->addChild(new SoTextureCoordinateDefault);
      sep->addChild(shared);
    }
    {
      SoRayPickAction a(SbViewportRegion(64,64)); Case c=sideCase(k); Transform t; prepare(a,c,t);
      for(int repeat=0;repeat<2;++repeat) {
        outer.clear(); inner.clear(); a.apply(root); check(a.getPickedPointList().getLength()==6,"shared shape yields ordered six hits");
        for(int i=0;i<a.getPickedPointList().getLength();++i) {
          const SoPickedPoint * p=a.getPickedPoint(i); Hit h; h.p=p->getObjectPoint(); h.n=p->getObjectNormal(); h.part=part(p);
          check(p->getPath()->getTail()==shared,"shared shape preserves actual path");
          near4(p->getObjectTextureCoords(),i<4?value(h.p,h.n,i<2?59:23):legacy(k,h),"inner function, restored outer function and default reset");
        }
        check(inner.points.size()==2 && outer.points.size()==2,"function evaluation respects separator restoration");
      }
    }
    root->unref();
  }
}

}
int main(int argc,char ** argv) {
  if(argc!=2) return 2; SoDB::init();
  DerivedBox::initClass(); DerivedCone::initClass(); DerivedCylinder::initClass();
  const std::string mode=argv[1];
  if(mode=="functions") { analyticCases(true); homogeneous(); }
  else if(mode=="legacy") analyticCases(false);
  else if(mode=="generators") generators();
  else if(mode=="boundaries") boundaries();
  else if(mode=="scopes") scopes();
  else return 2;
  std::printf("%d checks, %d failures\n",checks,failures); return failures?1:0;
}
