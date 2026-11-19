#include "TextureCoordinateRangeTest.h"
using namespace range_test;
int main(int argc,char **argv) {
  SoDB::init();
  if(mode(argc,argv)) {
    // Each radial square is a normal Float value. These fail independently
    // of the radial overflow/underflow mechanism tested in Range02.
    const SbVec3f points[]={SbVec3f(1e-12f,-1,0),SbVec3f(0,-1,1e-12f),SbVec3f(1e-18f,-1,0),SbVec3f(1e8f,-1e20f,0)};
    for(const SbVec3f & p:points) {
      const double radial=std::hypot(double(p[0]),double(p[2]));
      SbVec4f want=sphereLegacy(p);want[1]=float(std::atan2(radial,-double(p[1]))/pi);
      equal(evaluate(new SoTextureCoordinateSphere,p,SbVec3f(0,1,0)),want,"south latitude retains nonzero complement with normal radial square");
    }
  } else {
    const SbVec3f points[]={SbVec3f(0,-1,0),SbVec3f(-0.0f,-1,0),SbVec3f(0,-1,-0.0f),SbVec3f(1,2,3),SbVec3f(1e-12f,1,0),SbVec3f(1,-INFINITY,0),SbVec3f(NAN,-1,0)};
    for(const SbVec3f & p:points) equal(evaluate(new SoTextureCoordinateSphere,p,SbVec3f(0,0,0)),sphereLegacy(p),"legacy exact pole ordinary north and nonfinite",true);
  }
  return finish();
}
