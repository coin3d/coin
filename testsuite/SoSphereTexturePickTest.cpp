// Public CPU picking regressions. No rendering context or generated mesh oracle.
#include <Inventor/SoDB.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/SoPath.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/elements/SoMultiTextureCoordinateElement.h>
#include <Inventor/lists/SoPickedPointList.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoClipPlane.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoTextureMatrixTransform.h>
#ifdef SPHERE_PICK_HAVE_VRML97
#include <Inventor/VRMLnodes/SoVRMLSphere.h>
#endif
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoTextureCoordinate2.h>
#include <Inventor/nodes/SoTextureCoordinateDefault.h>
#include <Inventor/nodes/SoTextureCoordinateNormalMap.h>
#include <Inventor/nodes/SoTextureCoordinateReflectionMap.h>
#include <Inventor/nodes/SoTextureCubeMap.h>
#include <Inventor/nodes/SoTextureUnit.h>
#include <Inventor/nodes/SoTransform.h>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <initializer_list>
#include <string>
#include <vector>

namespace {
int failures=0, checks=0;
std::string context;
void check(bool ok,const char * message) {
  ++checks;
  if(!ok) { ++failures; std::fprintf(stderr,"FAIL %s: %s\n",context.c_str(),message); }
}
void equal(const SbVec4f & actual,const SbVec4f & expected,const char * message) {
  bool ok=true;
  for(int i=0;i<4;++i) ok=ok && std::isfinite(actual[i]) && std::fabs(actual[i]-expected[i])<4e-5f;
  check(ok,message);
  if(!ok) std::fprintf(stderr,"  actual (%g,%g,%g,%g), expected (%g,%g,%g,%g)\n",
    actual[0],actual[1],actual[2],actual[3],expected[0],expected[1],expected[2],expected[3]);
}
void equal3(const SbVec3f & a,const SbVec3f & b,const char * what) {
  equal(SbVec4f(a[0],a[1],a[2],1),SbVec4f(b[0],b[1],b[2],1),what);
}
SbVec3f normalized(SbVec3f p) { p.normalize(); return p; }
// Preserve the historical angular mapping, including its Float products.
SbVec4f angular(const SbVec3f & p) {
  const double pi=std::acos(-1.0);
  return SbVec4f(float(std::atan2(p[0],p[2])*(1.0/(2*pi))+.5),
    float(std::atan2(p[1],std::sqrt(p[0]*p[0]+p[2]*p[2]))*(1.0/pi)+.5),0,1);
}
SbVec4f sentinelValue(const SbVec3f & p,const SbVec3f & n,float tag) {
  return SbVec4f(tag+2*p[0]-3*p[2]+n[1],-tag+5*p[1]+n[2],
    p[2]+7*n[0],tag+.25f*p[0]-.5f*n[2]);
}
struct Function {
  explicit Function(float t,int u=0):tag(t),unit(u),fixed(false) {}
  float tag; int unit; bool fixed;
  SbVec4f result;
  std::vector<SbVec3f> points,normals;
};
const SbVec4f & generate(void * data,const SbVec3f & p,const SbVec3f & n) {
  Function * f=static_cast<Function *>(data);
  f->points.push_back(p); f->normals.push_back(n);
  if(!f->fixed) f->result=sentinelValue(p,n,f->tag); return f->result;
}
void install(void * data,SoAction * a) {
  if(a->isOfType(SoRayPickAction::getClassTypeId())) {
    Function * f=static_cast<Function *>(data);
    SoMultiTextureCoordinateElement::setFunction(a->getState(),a->getCurPathTail(),f->unit,generate,f);
  }
}
void function(SoGroup * root,Function & f) {
  SoCallback * n=new SoCallback; n->setCallback(install,&f); root->addChild(n);
}
void unit(SoGroup * root,int u) {
  SoTextureUnit * n=new SoTextureUnit; n->unit=u; root->addChild(n);
}
void cube(SoGroup * root) {
  SoTextureCubeMap * n=new SoTextureCubeMap;
  const unsigned char pixels[]={17,29,43,255};
  n->imagePosX.setValue(SbVec2s(1,1),4,pixels); root->addChild(n);
}
SoSeparator * scene() {
  SoSeparator * n=new SoSeparator; n->ref(); n->pickCulling=SoSeparator::OFF; return n;
}
struct Geometry {
  SbVec3f scale,translation;
  float radius,x,y;
  Geometry(bool transformed=false,float r=2):scale(transformed?SbVec3f(2,3,4):SbVec3f(1,1,1)),
    translation(transformed?SbVec3f(7,-5,2):SbVec3f(0,0,0)),radius(r),x(.3f),y(.4f) {}
  SbVec3f world(const SbVec3f & p) const {
    return SbVec3f(p[0]*scale[0]+translation[0],p[1]*scale[1]+translation[1],p[2]*scale[2]+translation[2]);
  }
  SbVec3f point(int side) const {
    return SbVec3f(x,y,side*float(std::sqrt(double(radius)*radius-double(x)*x-double(y)*y)));
  }
  void transform(SoGroup * root) const {
    SoTransform * t=new SoTransform; t->scaleFactor=scale; t->translation=translation; root->addChild(t);
  }
  void ray(SoRayPickAction & a,float nearDistance=-1,float farDistance=-1) const {
    a.enableCulling(FALSE);
    a.setRay(world(SbVec3f(x,y,3)),SbVec3f(0,0,-1),nearDistance,farDistance);
  }
};
struct DerivedSphere : public SoSphere {};
SoSphere * sphere(SoGroup * root,float radius,bool derived=false) {
  SoSphere * s=derived?new DerivedSphere:new SoSphere; s->radius=radius; root->addChild(s); return s;
}
void geometry(const SoPickedPoint * p,SoNode * shape,const Geometry & g,int side) {
  equal3(p->getObjectPoint(),g.point(side),"analytic object intersection");
  equal3(p->getPoint(),g.world(g.point(side)),"analytic world intersection");
  equal3(p->getObjectNormal(),normalized(g.point(side)),"outward object normal");
  check(p->getPath()->getTail()==shape,"picked path ends in sphere");
  check(p->isOnGeometry(),"intersection lies on geometry");
  check(p->getMaterialIndex()==0,"sphere material index preserved");
  check(p->getDetail()==NULL,"sphere detail remains absent");
}
void userFunctions() {
  for(bool transformed : {false,true}) for(bool cubemap : {false,true}) for(bool all : {false,true}) {
    context="user function transformed="+std::to_string(transformed)+" cube="+std::to_string(cubemap)+" all="+std::to_string(all);
    Function zero(11),other(73,1);
    Geometry g(transformed);
    SoSeparator * root=scene(); if(cubemap) cube(root);
    function(root,zero); unit(root,1); function(root,other);
    // The currently selected unit is deliberately one; picked coordinates use zero.
    g.transform(root); SoSphere * s=sphere(root,g.radius,transformed);
    {
      SoRayPickAction a(SbViewportRegion(64,64)); a.setPickAll(all); g.ray(a); a.apply(root);
      check(a.getPickedPointList().getLength()==(all?2:1),"nearest or both roots retained");
      for(int i=0;i<a.getPickedPointList().getLength();++i) {
        const SoPickedPoint * p=a.getPickedPoint(i); const int side=i==0?1:-1;
        geometry(p,s,g,side);
        equal(p->getObjectTextureCoords(),sentinelValue(g.point(side),normalized(g.point(side)),11),"unit zero raw function result, including Q");
      }
      check(zero.points.size()==size_t(all?2:1),"only accepted nearest or pickAll roots evaluate unit zero callback");
      check(other.points.empty(),"unrelated texture unit callback not evaluated");
      for(size_t i=0;i<zero.points.size();++i) {
        check(std::fabs(zero.points[i].length()-g.radius)<4e-5f,"callback point on object sphere");
        equal3(zero.normals[i],normalized(zero.points[i]),"callback receives outward object normal");
      }
    }
    root->unref();
  }
}
void boundaryCases() {
  // These inputs use the baseline line/sphere behavior: radius is squared,
  // and coincident enter/exit points are recorded once, including radius zero.
  for(int kind=0;kind<4;++kind) for(bool functional : {false,true}) for(bool all : {false,true}) {
    context="boundary="+std::to_string(kind)+" function="+std::to_string(functional)+" all="+std::to_string(all);
    const float radius=kind==2?-2.0f:kind==3?0.0f:1.0f;
    const SbVec3f start=kind==0?SbVec3f(1,0,5):kind==1?SbVec3f(0,0,0):SbVec3f(0,0,5);
    const SbVec3f first=kind==0?SbVec3f(1,0,0):kind==1?SbVec3f(0,0,-1):kind==2?SbVec3f(0,0,2):SbVec3f(0,0,0);
    const int count=kind==2 && all?2:1;
    Function f(31); SoSeparator * root=scene();
    if(functional) function(root,f);
    SoSphere * shape=sphere(root,radius);
    {
      SoRayPickAction a(SbViewportRegion(64,64)); a.enableCulling(FALSE); a.setPickAll(all);
      a.setRay(start,SbVec3f(0,0,-1),0.0f); a.apply(root);
      check(a.getPickedPointList().getLength()==count,"tangent, inside, negative and zero radius retain baseline hit count");
      for(int i=0;i<a.getPickedPointList().getLength();++i) {
        const SoPickedPoint * p=a.getPickedPoint(i);
        const SbVec3f expected=i==0?first:SbVec3f(0,0,-2);
        equal3(p->getObjectPoint(),expected,"boundary analytic intersection");
        equal3(p->getObjectNormal(),normalized(expected),"boundary preserves baseline object normal");
        check(p->getPath()->getTail()==shape,"boundary path retains actual sphere");
        equal(p->getObjectTextureCoords(),functional?sentinelValue(expected,normalized(expected),31):angular(p->getObjectPoint()),"boundary functional result or legacy angular result");
      }
      check(f.points.size()==size_t(functional?count:0),"boundary evaluates function exactly once per accepted hit");
    }
    root->unref();
  }
}
void homogeneous() {
  const SbVec4f values[]={SbVec4f(2,-3,5,0),SbVec4f(2,-3,5,-7),SbVec4f(-0.0f,0,-0.0f,-0.0f)};
  for(const SbVec4f & value:values) {
    context="raw homogeneous result and texture matrix";
    Function f(0); f.fixed=true; f.result=value;
    SoSeparator * root=scene(); function(root,f);
    SoTextureMatrixTransform * texture=new SoTextureMatrixTransform;
    SbMatrix matrix; matrix.makeIdentity();
    matrix[0][0]=2; matrix[1][1]=3; matrix[2][2]=4;
    matrix[3][0]=5; matrix[3][1]=-6; matrix[3][2]=7;
    texture->matrix=matrix; root->addChild(texture); root->addChild(new SoSphere);
    {
      SoRayPickAction a(SbViewportRegion(64,64)); a.enableCulling(FALSE);
      a.setRay(SbVec3f(.3f,.4f,5),SbVec3f(0,0,-1)); a.apply(root);
      const SoPickedPoint * p=a.getPickedPoint(); check(p!=NULL,"homogeneous scene intersects");
      if(p) {
        const SbVec4f raw=p->getObjectTextureCoords();
        check(std::memcmp(raw.getValue(),value.getValue(),4*sizeof(float))==0,"raw result preserves zero Q, negative Q, and signed zero bits");
        equal(p->getTextureCoords(),SbVec4f(2*value[0]+5*value[3],3*value[1]-6*value[3],4*value[2]+7*value[3],value[3]),"public texture coordinate keeps historical matrix transform without Q divide");
      }
    }
    root->unref();
  }
}
void generators() {
  const SbVec3f eye(0,0,15);
  for(int kind=0;kind<3;++kind) for(bool transformed : {false,true}) {
    context="real CubeMap generator="+std::to_string(kind)+" transformed="+std::to_string(transformed);
    Geometry g(transformed); SoSeparator * root=scene();
    SoOrthographicCamera * camera=new SoOrthographicCamera; camera->position=eye; root->addChild(camera);
    cube(root);
    if(kind==0) root->addChild(new SoTextureCoordinateNormalMap);
    if(kind==1) root->addChild(new SoTextureCoordinateReflectionMap);
    // kind 2 is a pure CubeMap with no coordinate generator.
    g.transform(root); SoSphere * s=sphere(root,g.radius);
    {
      SoRayPickAction a(SbViewportRegion(64,64)); a.setPickAll(TRUE); g.ray(a); a.apply(root);
      check(a.getPickedPointList().getLength()==2,"generator scene has both exact intersections");
      for(int i=0;i<a.getPickedPointList().getLength();++i) {
        const SoPickedPoint * p=a.getPickedPoint(i); const int side=i==0?1:-1;
        geometry(p,s,g,side);
        const SbVec3f point=g.point(side),n=normalized(point);
        const SbVec3f viewnormal=normalized(SbVec3f(n[0]/g.scale[0],n[1]/g.scale[1],n[2]/g.scale[2]));
        SbVec4f expected=angular(point);
        if(kind==0) expected=SbVec4f(viewnormal[0],viewnormal[1],viewnormal[2],1);
        if(kind==1) {
          const SbVec3f incident=normalized(g.world(point)-eye);
          const SbVec3f reflection=incident-2*incident.dot(viewnormal)*viewnormal;
          expected=SbVec4f(reflection[0],reflection[1],reflection[2],1);
        }
        equal(p->getObjectTextureCoords(),expected,"analytic generator coordinate at exact hit");
      }
    }
    root->unref();
  }
}
void clipping() {
  for(int mode=0;mode<5;++mode) {
    context="clipping mode="+std::to_string(mode);
    Function f(17); Geometry g; SoSeparator * root=scene(); function(root,f);
    if(mode==3) {
      SoClipPlane * clip=new SoClipPlane;
      clip->plane=SbPlane(SbVec3f(0,0,-1),0); root->addChild(clip);
    }
    sphere(root,g.radius);
    {
      SoRayPickAction a(SbViewportRegion(64,64)); a.setPickAll(TRUE);
      // z roots are approximately +1.94 and -1.94; ray begins at z=3.
      g.ray(a,mode==1?2.0f:-1.0f,mode==2?2.0f:mode==4?.5f:-1.0f); a.apply(root);
      const int expected=mode==0?2:mode==4?0:1;
      check(a.getPickedPointList().getLength()==expected,"clipping preserves accepted sphere roots");
      for(int i=0;i<a.getPickedPointList().getLength();++i) {
        const int side=(mode==1 || mode==3 || i==1)?-1:1;
        equal3(a.getPickedPoint(i)->getObjectPoint(),g.point(side),"correct root survives clipping");
        equal(a.getPickedPoint(i)->getObjectTextureCoords(),sentinelValue(g.point(side),normalized(g.point(side)),17),"surviving hit receives function coordinate");
      }
      check(int(f.points.size())==expected,"clipped roots do not evaluate user function");
    }
    root->unref();
  }
  context="miss"; Function f(19); SoSeparator * root=scene(); function(root,f); sphere(root,1);
  {
    SoRayPickAction a(SbViewportRegion(64,64)); a.enableCulling(FALSE);
    a.setRay(SbVec3f(3,0,5),SbVec3f(0,0,-1)); a.apply(root);
    check(a.getPickedPoint()==NULL,"miss produces no picked point");
    check(f.points.empty(),"miss does not evaluate coordinate function");
  }
  root->unref();
}
void legacy() {
  for(int state=0;state<4;++state) for(float radius : {1.0f,2.5f}) {
    context="legacy state="+std::to_string(state)+" radius="+std::to_string(radius);
    SoSeparator * root=scene(); Function f(41,1);
    if(state==1) { SoTextureCoordinate2 * tc=new SoTextureCoordinate2; tc->point.set1Value(0,SbVec2f(.125f,.875f)); root->addChild(tc); }
    if(state==2) { unit(root,1); function(root,f); cube(root); }
    if(state==3) { cube(root); root->addChild(new SoTextureCoordinateDefault); }
    Geometry g(false,radius); SoSphere * s=sphere(root,radius);
    {
      SoRayPickAction a(SbViewportRegion(64,64)); a.setPickAll(TRUE); g.ray(a); a.apply(root);
      check(a.getPickedPointList().getLength()==2,"legacy has both roots");
      for(int i=0;i<a.getPickedPointList().getLength();++i) {
        const SoPickedPoint * p=a.getPickedPoint(i); geometry(p,s,g,i==0?1:-1);
        const SbVec4f want=angular(p->getObjectPoint()),got=p->getObjectTextureCoords();
        check(std::memcmp(got.getValue(),want.getValue(),4*sizeof(float))==0,"legacy Float angular coordinate retained exactly");
      }
      check(f.points.empty(),"unit one alone cannot replace legacy coordinates");
    }
    root->unref();
  }
}
void scopes() {
  context="separator restoration and action reuse";
  Function outer(23),inner(59);
  SoSeparator * root=scene(); function(root,outer);
  SoSphere * shared=new SoSphere;
  for(int branch=0;branch<3;++branch) {
    SoSeparator * sep=new SoSeparator; sep->pickCulling=SoSeparator::OFF; root->addChild(sep);
    SoTransform * t=new SoTransform; t->translation=SbVec3f(0,0,float(-4*branch)); sep->addChild(t);
    if(branch==0) function(sep,inner);
    if(branch==2) sep->addChild(new SoTextureCoordinateDefault);
    sep->addChild(shared);
  }
  {
    SoRayPickAction a(SbViewportRegion(64,64)); a.enableCulling(FALSE); a.setPickAll(TRUE);
    a.setRay(SbVec3f(.3f,.4f,5),SbVec3f(0,0,-1));
    for(int repeat=0;repeat<2;++repeat) {
      outer.points.clear(); outer.normals.clear(); inner.points.clear(); inner.normals.clear(); a.apply(root);
      check(a.getPickedPointList().getLength()==6,"shared sphere has six ordered roots");
      for(int i=0;i<a.getPickedPointList().getLength();++i) {
        const SoPickedPoint * p=a.getPickedPoint(i); const SbVec3f point=p->getObjectPoint();
        Geometry g(false,1); equal3(point,g.point(i%2==0?1:-1),"shared sphere local point");
        check(p->getPath()->getTail()==shared,"shared shape path retained");
        equal(p->getObjectTextureCoords(),i<4?sentinelValue(point,normalized(point),i<2?59:23):angular(point),"inner state, restored outer state, and reset default");
      }
      check(inner.points.size()==2 && outer.points.size()==2,"functions remain within restored scopes");
    }
  }
  root->unref();
}
}
int main(int argc,char ** argv) {
  if(argc!=2) return 2;
  SoDB::init(); const std::string mode=argv[1];
  if(mode=="functions") { userFunctions(); homogeneous(); boundaryCases(); }
  else if(mode=="generators") generators();
  else if(mode=="clipping") clipping();
  else if(mode=="legacy") legacy();
  else if(mode=="scopes") scopes();
  else return 2;
  std::printf("%d checks, %d failures\n",checks,failures); return failures?1:0;
}
