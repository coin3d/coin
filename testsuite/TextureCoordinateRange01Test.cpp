#include "TextureCoordinateRangeTest.h"
#include <Inventor/nodes/SoCallback.h>
using namespace range_test;
namespace {
void installView(void * data,SoAction * action) {
  SoViewingMatrixElement::set(action->getState(),action->getCurPathTail(),
                             *static_cast<SbMatrix *>(data));
}
void composedLegacy(const SbMatrix & model,const SbMatrix & view,bool generatorFirst) {
  SoGroup * root=new SoGroup; root->ref();
  SoTextureCoordinateNormalMap * generator=new SoTextureCoordinateNormalMap;
  if(generatorFirst) root->addChild(generator);
  SoMatrixTransform * transform=new SoMatrixTransform;
  transform->matrix=model; root->addChild(transform);
  SbMatrix savedView=view;
  SoCallback * setter=new SoCallback; setter->setCallback(installView,&savedView);
  root->addChild(setter);
  if(!generatorFirst) root->addChild(generator);
  Probe * probe=new Probe;
  probe->point=SbVec3f(3,-2,1); probe->normal=SbVec3f(1,1,1);
  probe->box=SbBox3f(-1,-1,-1,1,1,1); root->addChild(probe);
  SoCallbackAction action; action.apply(root);
  check(probe->visits==1,"composed fixture reaches public callback");
  equal(probe->result,probe->normalLegacy,
        "composition range trigger retains finite Float classifier",true);
  root->unref();
}
void classifierCompatibility() {
  // The Float determinant is finite and nonzero, but SbMatrix rejects its
  // ratio. More precise products lie just on the other side of that cutoff.
  for(float tiny:{0.0f,1e-30f}) {
    SbMatrix m=SbMatrix::identity();
    m[0][0]=1.0000030994415283f; m[0][1]=1.0000076293945312f;
    m[1][0]=.9999977350234985f; m[1][1]=1.0000025033950806f;
    m[0][2]=tiny; m[2][1]=tiny;
    const float pos=m[0][0]*m[1][1],neg=-(m[0][1]*m[1][0]);
    const float det=pos+neg,den=pos-neg;
    const double widePos=double(m[0][0])*m[1][1];
    const double wideNeg=double(m[0][1])*m[1][0];
    check(det!=0 && std::isfinite(det) && std::isfinite(den) &&
          std::fabs(det/den)<FLT_EPSILON,
          "fixture has a finite nonzero rejected Float classifier");
    check(std::fabs((widePos-wideNeg)/(widePos+wideNeg))>=FLT_EPSILON,
          "fixture distinguishes Float cutoff from widened cutoff");
    check(m.inverse()==m,"fixture retains historical singularity fallback");
    for(bool first:{false,true}) {
      SbVec4f legacy;
      SbVec4f got=evaluate(new SoTextureCoordinateNormalMap,SbVec3f(3,-2,1),
        SbVec3f(1,1,1),m,SbBox3f(-1,-1,-1,1,1,1),&legacy,false,first);
      equal(got,legacy,"irrelevant determinant underflow preserves Float rejection",true);
    }
  }
  // Both triangular factors have a well-defined inverse. Only their product
  // hits the historical singularity threshold. The tiny cross product
  // underflows during composition without changing its finite classifier.
  for(int exponent:{-23,-22}) for(bool first:{false,true}) {
    const float tiny=1e-30f;
    SbMatrix model=SbMatrix::identity(),view=SbMatrix::identity();
    model[1][0]=1; model[1][1]=std::ldexp(1.0f,exponent); model[0][2]=tiny;
    view[0][1]=1; view[2][1]=tiny;
    SbMatrix combined=model; combined.multRight(view);
    check(model.inverse()!=model && view.inverse()!=view,
          "composition factors are independently invertible");
    const float det=combined[1][1]-1.0f;
    check(det!=0 && std::isfinite(det) && combined.inverse()==combined,
          "composed matrix has finite nonzero rejected Float classifier");
    check(double(tiny)*tiny!=0 && tiny*tiny==0,
          "composition fixture has an irrelevant underflowed product");
    composedLegacy(model,view,first);
  }
  // A normal accepted Float composition still follows the original result.
  SbMatrix model=SbMatrix::identity(),view=SbMatrix::identity();
  model[1][0]=1; model[1][1]=std::ldexp(1.0f,-20); view[0][1]=1;
  for(bool first:{false,true}) composedLegacy(model,view,first);
}
void finiteDeterminantRepairs() {
  for(bool first:{false,true}) {
    // Finite, nonzero determinant; only the inverse reciprocal overflows.
    const float small=std::ldexp(1.0f,-70);
    SbMatrix reciprocal=SbMatrix::identity();
    reciprocal[0][0]=small; reciprocal[1][1]=small;
    equal(evaluate(new SoTextureCoordinateNormalMap,SbVec3f(0,0,0),
      SbVec3f(1,1,1),reciprocal,SbBox3f(-1,-1,-1,1,1,1),NULL,false,first),
      unit(1,1,small),"finite determinant with overflowing reciprocal");
    // Determinant evaluation stays finite, but a cofactor product overflows.
    const float large=std::ldexp(1.0f,64);
    SbMatrix cofactor=SbMatrix::identity();
    cofactor[0][0]=1/large; cofactor[1][1]=large; cofactor[2][2]=large;
    equal(evaluate(new SoTextureCoordinateNormalMap,SbVec3f(0,0,0),
      SbVec3f(1,1,1),cofactor,SbBox3f(-1,-1,-1,1,1,1),NULL,false,first),
      unit(1,std::ldexp(1.0,-128),std::ldexp(1.0,-128)),
      "finite determinant with overflowing cofactor");
    // Underflowing a cofactor loses a representable final normal component.
    const float big=std::ldexp(1.0f,80),tiny=std::ldexp(1.0f,-80);
    cofactor[0][0]=big; cofactor[1][1]=tiny; cofactor[2][2]=tiny;
    equal(evaluate(new SoTextureCoordinateNormalMap,SbVec3f(0,0,0),
      SbVec3f(std::ldexp(1.0f,100),1,1),cofactor,
      SbBox3f(-1,-1,-1,1,1,1),NULL,false,first),
      unit(std::ldexp(1.0,-60),1,1),"finite determinant with underflowing cofactor");
  }
}
}
int main(int argc,char **argv) {
  SoDB::init(); const bool repair=mode(argc,argv);
  const SbVec3f n(1,1,1);
  if(repair) {
    finiteDeterminantRepairs();
    for(int exponent:{80,-80}) for(int first=0;first<2;++first) {
      SbMatrix m=SbMatrix::identity(); const float h=std::ldexp(1.0f,exponent);
      m[0][0]=h; m[1][1]=2*h; m[2][2]=4*h;
      equal(evaluate(new SoTextureCoordinateNormalMap,SbVec3f(0,0,0),n,m,SbBox3f(-1,-1,-1,1,1,1),NULL,false,first),unit(4,2,1),exponent>0?"overflowing affine determinant":"underflowing affine determinant");
    }
    // Composition overflows in Float although both matrices and the final
    // normalized dual direction are finite. For isotropic scaling, only the
    // camera rotation affects the result. Use an ordinary scale as control.
    SbMatrix huge=SbMatrix::identity();
    for(int i=0;i<3;++i) huge[i][i]=FLT_MAX*.9f;
    // A row containing two equal positive coefficients makes the rotated
    // composition overflow, without making the model singular.
    huge[0][1]=huge[0][0];
    SbMatrix rotation; SbRotation(SbVec3f(0,0,1),.7853981633974483f).inverse().getValue(rotation);
    // The shear maps the dual normal to (0,1,1); uniform scale cancels.
    const SbVec4f want=unit(rotation[1][0],rotation[1][1],1);
    equal(evaluate(new SoTextureCoordinateNormalMap,SbVec3f(2,3,4),n,huge,SbBox3f(-1,-1,-1,1,1,1),NULL,true),want,"affine composition range");
  } else {
    classifierCompatibility();
    const float scales[]={1,2,0};
    for(float s:scales) {
      SbMatrix m=SbMatrix::identity(); m[0][0]=s; m[1][1]=2*s; m[2][2]=4*s;
      SbVec4f legacy;
      SbVec4f got=evaluate(new SoTextureCoordinateNormalMap,SbVec3f(3,-2,1),n,m,SbBox3f(-1,-1,-1,1,1,1),&legacy);
      equal(got,legacy,"finite classifier and historical singularity",true);
    }
    // Finite determinant arithmetic must retain SbMatrix's historical
    // singularity threshold even when an algebraic inverse exists.
    SbMatrix nearSingular=SbMatrix::identity();
    nearSingular[0][1]=1; nearSingular[1][0]=1;
    nearSingular[1][1]=1+std::ldexp(1.0f,-23);
    SbVec4f legacy;
    SbVec4f got=evaluate(new SoTextureCoordinateNormalMap,SbVec3f(3,-2,1),n,nearSingular,SbBox3f(-1,-1,-1,1,1,1),&legacy);
    equal(got,legacy,"finite near-singular classifier retains historical threshold",true);
    got=evaluate(new SoTextureCoordinateNormalMap,SbVec3f(0,0,0),SbVec3f(0,-0.0f,0),SbMatrix::identity(),SbBox3f(-1,-1,-1,1,1,1),&legacy);
    equal(got,legacy,"exact zero normal",true);
  }
  return finish();
}
