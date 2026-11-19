#include "TextureCoordinateRangeTest.h"
using namespace range_test;
int main(int argc,char **argv) {
  SoDB::init();
  if(mode(argc,argv)) {
    for(int exponent:{80,-80}) for(float sign:{-1.0f,1.0f}) {
      const float h=std::ldexp(1.0f,exponent); const SbVec3f p(h,sign*h,.5f*h);
      const double radial=std::hypot(double(p[0]),double(p[2]));
      const SbVec4f want(float(std::atan2(double(p[0]),double(p[2]))/(2*pi)+.5),float(std::atan2(double(p[1]),radial)/pi+.5),0,1);
      equal(evaluate(new SoTextureCoordinateSphere,p,SbVec3f(0,0,1)),want,"finite nonzero radial squared outside Float range");
    }
  } else {
    const SbVec3f points[]={SbVec3f(2,3,4),SbVec3f(-3,-2,5),SbVec3f(0,1,0),SbVec3f(-0.0f,-1,0),SbVec3f(INFINITY,1,1),SbVec3f(NAN,1,1)};
    for(const SbVec3f & p:points) equal(evaluate(new SoTextureCoordinateSphere,p,SbVec3f(1,0,0)),sphereLegacy(p),"legacy sphere expression including nonfinite and pole",true);
  }
  return finish();
}
