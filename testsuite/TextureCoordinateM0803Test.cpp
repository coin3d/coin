#include "TextureCoordinateM08Test.h"
using namespace m08;
static SbVec2f expected(const SbBox3f & b,const SbVec3f & p,bool repair) {
  float size[3];b.getSize(size[0],size[1],size[2]);double rank[3];int axis[3]={0,1,2};
  bool valid=!b.isEmpty();for(int i=0;i<3;++i)valid=valid&&std::isfinite(b.getMin()[i])&&std::isfinite(b.getMax()[i]);
  for(int i=0;i<3;++i)rank[i]=repair&&valid&&!std::isfinite(size[i])?double(b.getMax()[i])-b.getMin()[i]:size[i];
  // Stable descending order retains the cache's X,Y,Z tie convention.
  for(int end=2;end>0;--end)for(int i=0;i<end;++i)if(rank[axis[i]]<rank[axis[i+1]])std::swap(axis[i],axis[i+1]);
  SbVec2f out;
  for(int c=0;c<2;++c){int i=axis[c];float n=p[i]-b.getMin()[i];float v=n/size[i];
    if(repair&&valid&&std::isfinite(p[i])&&(!std::isfinite(n)||!std::isfinite(size[i]))) {
      double numerator=std::isfinite(n)?double(n):double(p[i])-b.getMin()[i];
      double denominator=std::isfinite(size[i])?double(size[i]):double(b.getMax()[i])-b.getMin()[i];
      if(std::isfinite(numerator/denominator)&&std::fabs(numerator/denominator)<=FLT_MAX)v=float(numerator/denominator);
    }out[c]=v;
  }return out;
}
static void sample(const SbBox3f & b,bool repair) {
  visit([&](SoCallbackAction * a,Fixture *){
    SoTextureCoordinateCache cache(a->getState());
    const SbVec3f points[]={SbVec3f(.2f,.7f,.4f),SbVec3f(FLT_MAX,0,0),SbVec3f(INFINITY,.5f,0),SbVec3f(NAN,.5f,0),SbVec3f(-0.0f,0,0)};
    for(int count:{5,1,0,3,5}){
      cache.generate(b,points,count);check(cache.getNum()==count,"cache grows truncates and empties");
      for(int i=0;i<count;++i)equal2(cache.get()[i],expected(b,points[i],repair),"cache component policy and ordering");
    }
  });
}
static void mixed_lanes() {
  visit([&](SoCallbackAction * a,Fixture *){
    SoTextureCoordinateCache cache(a->getState());
    const float m=FLT_MAX;
    const SbBox3f wide(-m,-m,0,m,m,1);
    const SbVec3f points[]={SbVec3f(0,0,INFINITY),SbVec3f(INFINITY,0,0),
      SbVec3f(0,NAN,0),SbVec3f(m,m,NAN)};
    const SbVec2f want[]={SbVec2f(.5f,.5f),SbVec2f(NAN,.5f),
      SbVec2f(.5f,NAN),SbVec2f(1,1)};
    cache.generate(wide,points,4);
    check(cache.getNum()==4,"mixed wide cache count");
    for(int i=0;i<4;++i)equal2(cache.get()[i],want[i],"nonfinite point leaves finite lane intact");

    // The size stays finite in X, while the external X numerator overflows.
    // A nonfinite discarded Z must not disable recovery of the X quotient.
    const SbBox3f finite_size(-m,0,0,-m/2,1,0);
    const SbVec3f external(m,.25f,NAN);
    cache.generate(finite_size,&external,1);
    // The denominator is m/2 in Float, so (m-(-m))/(m/2) is exactly 4.
    equal2(cache.get()[0],SbVec2f(4.0f,.25f),"finite Float divisor with overflowing numerator");

    // Both extents round to 1 in Float. Keep the historical X,Y tie order,
    // even though the exact Y extent is larger and Z is nonfinite.
    const float half_ulp=std::ldexp(1.0f,-24);
    const SbBox3f tied(0,-half_ulp,0,1,1,.25f);
    const SbVec3f tied_point(1,0,NAN);
    cache.generate(tied,&tied_point,1);
    equal2(cache.get()[0],SbVec2f(1,half_ulp),"rounded Float extent tie keeps X first");
  });
}
int main(int argc,char **argv){init();bool repair=mode(argc,argv);const float m=FLT_MAX;
  if(argc==2 && std::strcmp(argv[1],"mixed")==0) mixed_lanes();
  else if(repair){
    sample(SbBox3f(-m,-m,-1,m/2,m,1),true);
    sample(SbBox3f(-m,-2,-1,-m/2,2,1),true);
    // Z becomes the longest axis and must carry its matching origin and size
    // through both sorting swaps.
    sample(SbBox3f(-1,-2,-m,1,2,m),true);
  }else{
    sample(SbBox3f(.1f,.2f,.3f,.9f,1.3f,.8f),false);
    sample(SbBox3f(-std::ldexp(1.0f,-24),0,0,1,1,.25f),false);
    // The exact Y extent is larger, but Float rounds both extents to one;
    // the cache must retain X first, observed at asymmetric sample points.
    sample(SbBox3f(0,-std::ldexp(1.0f,-24),0,1,1,.25f),false);
    sample(SbBox3f(-std::numeric_limits<float>::denorm_min(),0,0,1,1,.25f),false);
    sample(SbBox3f(0,0,0,1,1,1),false);
    sample(SbBox3f(2,3,4,2,3,4),false);
    sample(SbBox3f(),false);
    sample(SbBox3f(-m,-m,0,m,m,INFINITY),false);
  }return finish();}
