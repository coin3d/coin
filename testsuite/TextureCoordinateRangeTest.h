// Public callback probes shared by the independent numerical regressions.
#ifndef COIN_TEXTURE_COORDINATE_RANGE_TEST_H
#define COIN_TEXTURE_COORDINATE_RANGE_TEST_H
#include <Inventor/SoDB.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/SbBox3f.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/elements/SoMultiTextureCoordinateElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoViewingMatrixElement.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoMatrixTransform.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoTextureCoordinateNormalMap.h>
#include <Inventor/nodes/SoTextureCoordinateSphere.h>
#include <Inventor/nodes/SoTextureCoordinateCylinder.h>
#include <Inventor/nodes/SoTextureCoordinateCube.h>
#include <Inventor/nodes/SoTextureCoordinateObject.h>
#include <Inventor/nodes/SoTextureCoordinatePlane.h>
#include <Inventor/nodes/SoTextureCoordinateEnvironment.h>
#include <Inventor/nodes/SoTextureCoordinateReflectionMap.h>
#include <cmath>
#include <cfloat>
#include <cstdio>
#include <cstring>
#include <string>
#include <initializer_list>
namespace range_test {
static int checks=0, failures=0;
const double pi=3.141592653589793238462643383279502884;
inline bool same(float a,float b) {
  if (std::isnan(b)) return std::isnan(a);
  if (std::isinf(b)) return std::isinf(a)&&std::signbit(a)==std::signbit(b);
  return std::memcmp(&a,&b,sizeof(float))==0;
}
inline void check(bool ok,const std::string & label) {
  ++checks; if(!ok) { ++failures; std::fprintf(stderr,"FAIL %s\n",label.c_str()); }
}
inline void equal(const SbVec4f & got,const SbVec4f & want,const std::string & label,bool bits=false) {
  for(int i=0;i<4;++i) {
    const bool ok=bits ? same(got[i],want[i]) :
      (std::isnan(want[i]) ? std::isnan(got[i]) :
       std::isinf(want[i]) ? same(got[i],want[i]) :
       std::isfinite(got[i]) && std::fabs(double(got[i])-want[i])<=2e-6*std::fabs(double(want[i])));
    check(ok,label+" component="+std::to_string(i));
    if(!ok) std::fprintf(stderr,"  got %.9g want %.9g%s\n",got[i],want[i],bits?" (bits)":"");
  }
}
inline SbVec4f unit(double x,double y,double z) {
  const double scale=std::fmax(std::fabs(x),std::fmax(std::fabs(y),std::fabs(z)));
  if(scale==0) return SbVec4f(0,0,0,1);
  x/=scale; y/=scale; z/=scale;
  const double length=std::sqrt(x*x+y*y+z*z);
  return SbVec4f(float(x/length),float(y/length),float(z/length),1);
}
class Probe : public SoCube {
public:
  SbVec3f point, normal;
  SbBox3f box;
  SbVec4f result, normalLegacy;
  int visits=0;
  void computeBBox(SoAction *,SbBox3f & b,SbVec3f & center) override { b=box; center=box.getCenter(); }
  void callback(SoCallbackAction * action) override {
    ++visits;
    const SoMultiTextureCoordinateElement * element=SoMultiTextureCoordinateElement::getInstance(action->getState());
    result=element->get(0,point,normal);
    SbMatrix mv=SoModelMatrixElement::get(action->getState());
    mv.multRight(SoViewingMatrixElement::get(action->getState()));
    const SbMatrix nm=mv.inverse().transpose();
    double v[3];
    for(int i=0;i<3;++i) v[i]=double(normal[0])*nm[0][i]+double(normal[1])*nm[1][i]+double(normal[2])*nm[2][i];
    const double length=std::sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
    normalLegacy=length>0 ? SbVec4f(float(v[0]/length),float(v[1]/length),float(v[2]/length),1) : SbVec4f(0,0,0,1);
  }
};
inline SbVec4f evaluate(SoNode * generator,const SbVec3f & point,const SbVec3f & normal,
 const SbMatrix & model=SbMatrix::identity(),const SbBox3f & box=SbBox3f(-1,-1,-1,1,1,1),
 SbVec4f * normalLegacy=NULL,bool camera=false,bool generatorFirst=false) {
  SoGroup * root=new SoGroup; root->ref();
  if(camera) { SoOrthographicCamera * c=new SoOrthographicCamera;
    c->position.setValue(0,0,0); c->orientation=SbRotation(SbVec3f(0,0,1),.7853981633974483f); root->addChild(c); }
  if(generatorFirst) root->addChild(generator);
  SoMatrixTransform * transform=new SoMatrixTransform; transform->matrix=model; root->addChild(transform);
  if(!generatorFirst) root->addChild(generator);
  Probe * probe=new Probe; probe->point=point; probe->normal=normal; probe->box=box; root->addChild(probe);
  SoCallbackAction action; action.apply(root);
  check(probe->visits==1,"public shape callback reached");
  SbVec4f result=probe->result;
  if(normalLegacy) *normalLegacy=probe->normalLegacy;
  root->unref(); return result;
}
inline SbVec4f sphereLegacy(const SbVec3f & p) {
  return SbVec4f(float(atan2(p[0],p[2])*(1/(2*pi))+.5),
    float(atan2(p[1],sqrt(p[0]*p[0]+p[2]*p[2]))*(1/pi)+.5),0,1);
}
inline SbVec4f reflected(double px,double py,double pz,double nx,double ny,double nz,bool environment) {
  double ps=std::fmax(std::fabs(px),std::fmax(std::fabs(py),std::fabs(pz)));
  double ns=std::fmax(std::fabs(nx),std::fmax(std::fabs(ny),std::fabs(nz)));
  px/=ps; py/=ps; pz/=ps; nx/=ns; ny/=ns; nz/=ns;
  const double pl=std::sqrt(px*px+py*py+pz*pz),nl=std::sqrt(nx*nx+ny*ny+nz*nz);
  px/=pl; py/=pl; pz/=pl; nx/=nl; ny/=nl; nz/=nl;
  const double d=px*nx+py*ny+pz*nz;
  double r[3]={px-2*d*nx,py-2*d*ny,pz-2*d*nz};
  if(!environment) return SbVec4f(float(r[0]),float(r[1]),float(r[2]),1);
  double m=2*std::hypot(std::hypot(r[0],r[1]),r[2]+1);
  return m==0 ? SbVec4f(.5f,.5f,0,1) : SbVec4f(float(r[0]/m+.5),float(r[1]/m+.5),0,1);
}
inline SbVec4f degenerate(const SbVec3f & n,const SbMatrix & m,bool environment) {
  SbVec3f wn; m.multDirMatrix(n,wn); SbVec3f u=n; u.normalize(); wn.normalize();
  SbVec3f r=u-SbVec3f(2.0f*wn[0]*wn[0]*u[0],2.0f*wn[1]*wn[1]*u[1],2.0f*wn[2]*wn[2]*u[2]);
  r.normalize();
  if(!environment) return SbVec4f(r[0],r[1],r[2],1);
  const float z=1.0f+r[2]; float den=2.0f*float(std::sqrt(r[0]*r[0]+r[1]*r[1]+z*z));
  if(std::fabs(den)<=FLT_EPSILON) den=1;
  return SbVec4f(r[0]/den+.5f,r[1]/den+.5f,0,1);
}
inline SoNode * environmentNode(bool environment) { return environment ? static_cast<SoNode *>(new SoTextureCoordinateEnvironment) : static_cast<SoNode *>(new SoTextureCoordinateReflectionMap); }
inline bool mode(int argc,char **argv) { return argc==2 && std::strcmp(argv[1],"repair")==0; }
inline int finish() { SoDB::finish(); std::printf("%d checks, %d failures\n",checks,failures); return failures?1:0; }
}
#endif
