#include "TextureCoordinateReflectionRangeTest.h"
using namespace range_test;
int main(int argc,char **argv) {
  SoDB::init(); const bool repair=mode(argc,argv);const float h=std::ldexp(1.0f,80);
  // Cofactor columns are (H^2,H,-H^2), (0,H,0), (H,0,0).
  // Thus the dual image of (1,1,1) is exactly H*(1,1,1).
  SbMatrix m=SbMatrix::identity();
  m[0][0]=0;m[0][2]=h;m[1][1]=h;m[2][0]=-1;m[2][1]=1;m[2][2]=h;
  const SbVec3f p(0,0,1),n(1,1,1);
  for(bool environment:{false,true}) {
    if(repair) for(int first=0;first<2;++first) {
      equal(evaluate(environmentNode(environment),p,n,m,SbBox3f(-1,-1,-1,1,1,1),NULL,false,first),reflected(-1,1,h,1,1,1,environment),"cofactor sum zero lost representable H residual");
      SbMatrix rotation;SbRotation(SbVec3f(0,0,1),.7853981633974483f).inverse().getValue(rotation);
      const double px=-double(rotation[0][0])+rotation[1][0],py=-double(rotation[0][1])+rotation[1][1];
      equal(evaluate(environmentNode(environment),p,n,m,SbBox3f(-1,-1,-1,1,1,1),NULL,true,first),reflected(px,py,h,double(rotation[0][0])+rotation[1][0],double(rotation[0][1])+rotation[1][1],1,environment),"cofactor residual followed by camera rotation");
    }
    else {
      const SbVec3f normals[]={SbVec3f(1,2,3),SbVec3f(0,0,0),SbVec3f(INFINITY,1,1),SbVec3f(NAN,1,1)};
      SbMatrix ordinary=SbMatrix::identity();ordinary[0][0]=2;ordinary[1][1]=3;ordinary[2][2]=4;ordinary[0][1]=1;
      for(const SbVec3f & normal:normals)equal(evaluate(environmentNode(environment),p,normal,ordinary),reflectionLegacy(p,normal,ordinary,environment),"legacy normal transform and undefined normals",true);
      SbMatrix singular=SbMatrix::identity();for(int i=0;i<3;++i)for(int j=0;j<3;++j)singular[i][j]=0;
      singular[3][0]=1;singular[3][1]=2;singular[3][2]=3;
      equal(evaluate(environmentNode(environment),p,n,singular),reflectionLegacy(p,n,singular,environment),"exact zero cofactor normal stays degenerate",true);
      // Removing the cancelling final contribution leaves a nonzero sum.
      equal(evaluate(environmentNode(environment),p,SbVec3f(1,1,0),m),reflectionLegacy(p,SbVec3f(1,1,0),m,environment),"nonzero finite cofactor sum preserves B06",true);
    }
  }
  return finish();
}
