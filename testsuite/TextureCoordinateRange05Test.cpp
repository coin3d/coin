#include "TextureCoordinateReflectionRangeTest.h"
using namespace range_test;
int main(int argc,char **argv) {
  SoDB::init();const bool repair=mode(argc,argv);const float h=std::ldexp(1.0f,100);
  SbMatrix m=SbMatrix::identity();m[1][0]=1;m[2][0]=1;m[3][1]=-1;m[3][2]=h;
  const SbVec3f p(h,1,-h),n(0,1,0);
  for(bool environment:{false,true}) {
    if(repair) for(int first=0;first<2;++first) {
      // Exact map: (H + 1 - H, 1 - 1, -H + H) = (1,0,0).
      // No larger-precision summation is used to obtain the oracle.
      equal(evaluate(environmentNode(environment),p,n,m,SbBox3f(-1,-1,-1,1,1,1),NULL,false,first),reflected(1,0,0,0,1,0,environment),"transformed point zero lost unit residual");
      SbMatrix rotation;SbRotation(SbVec3f(0,0,1),.7853981633974483f).inverse().getValue(rotation);
      equal(evaluate(environmentNode(environment),p,n,m,SbBox3f(-1,-1,-1,1,1,1),NULL,true,first),reflected(rotation[0][0],rotation[0][1],0,rotation[1][0],rotation[1][1],0,environment),"point residual followed by camera rotation");
    }
    else {
      const SbVec3f points[]={SbVec3f(0,0,0),SbVec3f(2,3,-4),SbVec3f(INFINITY,1,1),SbVec3f(NAN,1,1)};
      const SbVec3f normals[]={SbVec3f(1,2,3),SbVec3f(0,0,0)};
      for(const SbVec3f & q:points)for(const SbVec3f & normal:normals) equal(evaluate(environmentNode(environment),q,normal),reflectionLegacy(q,normal,SbMatrix::identity(),environment),"legacy ordinary nonfinite and exact degeneracy",true);
      // Same cancellation family, but the final point is exactly zero.
      SbMatrix zero=m;zero[1][0]=0;
      equal(evaluate(environmentNode(environment),p,n,zero),degenerate(n,zero,environment),"exact transformed zero retains historical path",true);
      // Finite nonzero result must not trigger a magnitude threshold.
      const SbVec3f q(h,1,-h);SbMatrix nonzero=m;nonzero[3][0]=2;
      equal(evaluate(environmentNode(environment),q,n,nonzero),reflectionLegacy(q,n,nonzero,environment),"finite nonzero transformed sum remains B06",true);
      // A companion axis keeps normalization from hiding a change to the
      // nonzero legacy x sum: B06 gives (2,1,0), a repaired sum gives (3,1,0).
      nonzero[3][1]=0;
      equal(evaluate(environmentNode(environment),q,n,nonzero),reflectionLegacy(q,n,nonzero,environment),"nonzero transformed sum retains direction with companion axis",true);
    }
  }
  return finish();
}
