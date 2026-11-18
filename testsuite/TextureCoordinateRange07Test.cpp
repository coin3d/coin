#include "TextureCoordinateRangeTest.h"
using namespace range_test;
int main(int argc,char **argv) {
  SoDB::init();const bool repair=mode(argc,argv);
  for(bool sphere:{false,true}) {
    if(repair) {
      const float values[]={-1e-12f,-1e-30f,-1e-40f,1e-12f};
      for(float x:values) {
        const SbVec3f p(x,0,-1);
        // Measure the acute angle to the negative z axis. Its small value is
        // retained before the [0,1] offset, independently of atan2(x,z)+pi.
        const double angle=std::atan2(std::fabs(double(x)),1.0)/(2*pi);
        const float s=float(x<0?angle:1-angle);
        equal(evaluate(sphere ? static_cast<SoNode *>(new SoTextureCoordinateSphere) : static_cast<SoNode *>(new SoTextureCoordinateCylinder),p,SbVec3f(1,0,0)),SbVec4f(s,.5f,0,1),"nonzero seam longitude retains representable angle");
      }
    } else {
      const SbVec3f points[]={SbVec3f(0,0,-1),SbVec3f(-0.0f,0,-1),SbVec3f(1,0,2),SbVec3f(-1,0,2),SbVec3f(INFINITY,0,-1),SbVec3f(NAN,0,-1)};
      for(const SbVec3f & p:points) {
        SbVec4f want=sphereLegacy(p);if(!sphere)want[1]=.5f;
        equal(evaluate(sphere ? static_cast<SoNode *>(new SoTextureCoordinateSphere) : static_cast<SoNode *>(new SoTextureCoordinateCylinder),p,SbVec3f(1,0,0)),want,"legacy seam signed zeros and ordinary angles",true);
      }
    }
  }
  return finish();
}
