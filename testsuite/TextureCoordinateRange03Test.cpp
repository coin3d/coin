#include "TextureCoordinateRangeTest.h"
using namespace range_test;
// The wide oracle works directly in the original box; it never constructs a
// Float expanded cube. The legacy oracle deliberately repeats that expansion.
SbVec4f boxOracle(const SbBox3f & input,const SbVec3f & p,const SbVec3f & n,bool cube,bool legacy) {
  SbBox3f box=input;
  double lo[3],hi[3];
  if(legacy && cube) {
    const SbVec3f c=box.getCenter(); float x,y,z; box.getSize(x,y,z);
    if(y>x) x=y; if(z>x) x=z; x*=.5f;
    box.setBounds(c[0]-x,c[1]-x,c[2]-x,c[0]+x,c[1]+x,c[2]+x);
  }
  for(int i=0;i<3;++i) {lo[i]=box.getMin()[i];hi[i]=box.getMax()[i];}
  if(cube && !legacy) {
    const double side=std::fmax(hi[0]-lo[0],std::fmax(hi[1]-lo[1],hi[2]-lo[2]));
    for(int i=0;i<3;++i) {double center=(lo[i]+hi[i])*.5;lo[i]=center-side*.5;hi[i]=center+side*.5;}
  }
  int axis=0; double v=std::fabs(n[0]);
  if(std::fabs(n[1])>v) {axis=1;v=std::fabs(n[1]);} if(std::fabs(n[2])>v) axis=2;
  float ratio[3];
  for(int i=0;i<3;++i) {
    if(legacy) {float d=float(hi[i])-float(lo[i]);if(d==0)d=1;ratio[i]=(p[i]-float(lo[i]))/d;}
    else {double d=hi[i]-lo[i];if(d==0)d=1;ratio[i]=float((double(p[i])-lo[i])/d);}
  }
  if(!cube) {
    if(axis==1) return SbVec4f(ratio[0],n[1]>0 ? 1.0f-ratio[2] : ratio[2],0,1);
    return SbVec4f(float(atan2(p[0],p[2])*(1/(2*pi))+.5),ratio[1],0,1);
  }
  const float s=ratio[(axis+1)%3],t=ratio[(axis+2)%3];
  SbVec4f result(s,t,0,1);
  if(axis==0) {result[0]=1.0f-t;result[1]=s;}
  if(axis==1) {result[0]=t;result[1]=1.0f-s;}
  if(n[axis]<0) {if(axis==1)result[1]=1.0f-result[1];else result[0]=1.0f-result[0];}
  return result;
}
int main(int argc,char **argv) {
  SoDB::init(); const bool repair=mode(argc,argv);
  const SbVec3f normals[]={SbVec3f(1,0,0),SbVec3f(0,1,0),SbVec3f(0,-1,0),SbVec3f(0,0,1),SbVec3f(1,1,1),SbVec3f(-1,-1,-1),SbVec3f(0,0,0)};
  const float f=FLT_MAX;
  const SbBox3f boxes[]={SbBox3f(.6f*f,.6f*f,.6f*f,.9f*f,.9f*f,.9f*f),SbBox3f(-.75f*f,-.75f*f,-.75f*f,.75f*f,.75f*f,.75f*f),SbBox3f(-.75f*f,-.75f*f,-.75f*f,0,0,0)};
  for(bool cube:{false,true}) {
    if(repair) {
      for(float x:{INFINITY,NAN}) {
        const SbVec3f p(x,0,0),n=cube?SbVec3f(0,0,1):SbVec3f(0,-1,0);
        equal(evaluate(cube ? static_cast<SoNode *>(new SoTextureCoordinateCube) : static_cast<SoNode *>(new SoTextureCoordinateCylinder),p,n,SbMatrix::identity(),boxes[1]),SbVec4f(NAN,.5f,0,1),"nonfinite point component preserves class while finite component recovers");
      }
    }
    if(repair) for(int i=0;i<3;++i) for(const SbVec3f & n:normals) {
      // Case 0: center overflow; 1: extent overflow; 2: numerator overflow.
      const SbVec3f p=i==1 ? SbVec3f(0,0,0) : SbVec3f(.75f*f,.75f*f,.75f*f);
      equal(evaluate(cube ? static_cast<SoNode *>(new SoTextureCoordinateCube) : static_cast<SoNode *>(new SoTextureCoordinateCylinder),p,n,SbMatrix::identity(),boxes[i]),boxOracle(boxes[i],p,n,cube,false),std::string(cube?"cube":"cylinder")+" finite box range case="+std::to_string(i));
    }
    else {
      // Decimal bounds expose changes to the historical Float center,
      // expansion and ratio intermediates even when all are in range.
      const SbBox3f decimalBox(-.3f,.2f,-.1f,1.7f,2.4f,3.1f);
      const SbVec3f decimalPoint(.1f,.4f,.6f),decimalNormal(0,0,1);
      equal(evaluate(cube ? static_cast<SoNode *>(new SoTextureCoordinateCube) : static_cast<SoNode *>(new SoTextureCoordinateCylinder),decimalPoint,decimalNormal,SbMatrix::identity(),decimalBox),boxOracle(decimalBox,decimalPoint,decimalNormal,cube,true),"legacy Float decimal bounding box intermediates",true);
      const SbBox3f controls[]={SbBox3f(-2,-3,-4,4,5,6),SbBox3f(2,3,4,2,3,4),SbBox3f(-1,-1,-1,INFINITY,1,1)};
      for(const SbBox3f & b:controls) for(const SbVec3f & n:normals) {
        const SbVec3f p(1,2,3);
        equal(evaluate(cube ? static_cast<SoNode *>(new SoTextureCoordinateCube) : static_cast<SoNode *>(new SoTextureCoordinateCylinder),p,n,SbMatrix::identity(),b),boxOracle(b,p,n,cube,true),"legacy Float box arithmetic and OIV ties",true);
      }
    }
  }
  return finish();
}
