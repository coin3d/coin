#ifndef COIN_TEXTURE_COORDINATE_REFLECTION_RANGE_TEST_H
#define COIN_TEXTURE_COORDINATE_REFLECTION_RANGE_TEST_H
#include "TextureCoordinateRangeTest.h"
namespace range_test {
inline void baselineNormalize(double v[3]) {
  double s=std::fabs(v[0]);for(int i=1;i<3;++i) if(std::fabs(v[i])>s)s=std::fabs(v[i]);
  if(s==0)return;
  for(int i=0;i<3;++i)v[i]/=s;
  const double l=std::sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
  for(int i=0;i<3;++i)v[i]/=l;
}
// A compatibility control, intentionally reproducing B06's arithmetic and
// intermediate normalization. Mathematical repair oracles live in each test.
inline SbVec4f reflectionLegacy(const SbVec3f & p,const SbVec3f & n,const SbMatrix & m,bool environment) {
  if(n==SbVec3f(0,0,0))return degenerate(n,m,environment);
  double incident[3],normal[3]={n[0],n[1],n[2]};
  for(int j=0;j<3;++j) incident[j]=double(p[0])*m[0][j]+double(p[1])*m[1][j]+double(p[2])*m[2][j]+double(m[3][j]);
  if(incident[0]==0&&incident[1]==0&&incident[2]==0)return degenerate(n,m,environment);
  baselineNormalize(incident);baselineNormalize(normal);
  double c[3][3];for(int i=0;i<3;++i)for(int j=0;j<3;++j)c[i][j]=double(m[(i+1)%3][(j+1)%3])*m[(i+2)%3][(j+2)%3]-double(m[(i+1)%3][(j+2)%3])*m[(i+2)%3][(j+1)%3];
  const double determinant=double(m[0][0])*c[0][0]+double(m[0][1])*c[0][1]+double(m[0][2])*c[0][2];
  double t[3];for(int i=0;i<3;++i)t[i]=(determinant<0?-1.0:1.0)*(normal[0]*c[0][i]+normal[1]*c[1][i]+normal[2]*c[2][i]);
  for(int i=0;i<3;++i)normal[i]=t[i];baselineNormalize(normal);
  // B06 applies the identity viewing matrix as a separate dual transform.
  baselineNormalize(normal);
  const double d=normal[0]*incident[0]+normal[1]*incident[1]+normal[2]*incident[2];
  double r[3];for(int i=0;i<3;++i)r[i]=incident[i]-2*d*normal[i];
  if(!environment)return SbVec4f(float(r[0]),float(r[1]),float(r[2]),1);
  const double den=2*std::hypot(std::hypot(r[0],r[1]),r[2]+1);
  return den==0?SbVec4f(.5f,.5f,0,1):SbVec4f(float(r[0]/den+.5),float(r[1]/den+.5),0,1);
}
}
#endif
