#include "TextureCoordinateRangeTest.h"
using namespace range_test;
int main(int argc,char **argv) {
  SoDB::init(); const bool repair=mode(argc,argv);
  const SbVec3f n(1,1,1);
  if(repair) {
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
