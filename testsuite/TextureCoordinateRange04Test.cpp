#include "TextureCoordinateRangeTest.h"
using namespace range_test;
void sample(const SbVec3f & p,const SbVec3f & f,float offset,float wanted,const char * label,bool bits) {
  for(bool object:{false,true}) {
    SoNode * node;
    if(object) {
      SoTextureCoordinateObject * o=new SoTextureCoordinateObject;
      o->factorS.setValue(f[0],f[1],f[2],offset); o->factorT=o->factorS; o->factorR=o->factorS; o->factorQ=o->factorS; node=o;
    } else {
      SoTextureCoordinatePlane * o=new SoTextureCoordinatePlane;
      o->directionS=f;o->directionT=f;o->directionR=f;node=o;
    }
    equal(evaluate(node,p,SbVec3f(0,0,0)),SbVec4f(wanted,wanted,wanted,object?wanted:1),label,bits);
  }
}
int main(int argc,char **argv) {
  SoDB::init();
  const float h=std::ldexp(1.0f,80),tiny=std::ldexp(1.0f,-75);
  if(mode(argc,argv)) {
    sample(SbVec3f(h,h,1),SbVec3f(h,-h,3),0,3,"overflowed products cancel to representable value",false);
    const float large=.75f*FLT_MAX;
    // The three products are finite; only the first addition overflows.
    // Algebraically h+h-h=h, which is representable without rounding ambiguity.
    sample(SbVec3f(1,1,1),SbVec3f(large,large,-large),0,large,"overflowed addition of finite products has representable sum",false);
    sample(SbVec3f(tiny,tiny,tiny),SbVec3f(.75f*tiny,.75f*tiny,.75f*tiny),0,float(3*double(tiny)*(.75f*tiny)),"underflowed products accumulate to nonzero subnormal",false);
    sample(SbVec3f(1,1,1),SbVec3f(h,1,-h),0,1,"exact zero Float sum lost representable residual",false);
  } else {
    // Every product is normal. The Float sum is nonzero, but a widened dot
    // differs by a factor of two. A magnitude/cancellation threshold must not
    // replace this result, however small it is relative to the products.
    const float e=FLT_EPSILON;
    const SbVec3f p(1+e,1,e),f(1+e,-(1+2*e),e);
    sample(p,f,0,f.dot(p),"nonzero finite legacy result despite severe cancellation",true);
    const SbVec3f q(2,-3,7),g(.3f,-.4f,.7f);
    sample(q,g,0,g.dot(q),"ordinary Float dot order",true);
    sample(SbVec3f(0,0,0),SbVec3f(2,3,4),0,0,"exact zero dot",true);
    sample(SbVec3f(INFINITY,0,0),SbVec3f(1,0,0),0,INFINITY,"nonfinite input remains legacy",true);
    sample(SbVec3f(NAN,0,0),SbVec3f(1,0,0),0,NAN,"NaN input remains legacy",true);
  }
  return finish();
}
