#include "TextureCoordinateM08Test.h"
using namespace m08;
// Retain the documented Float intermediates and OIV tie ordering in the oracle.
static SbVec4f expected(const SbBox3f & b,const SbVec3f & p,bool three,bool recover) {
  float s[3]={1,1,1};SbVec3f lo(0,0,0);double rank[3];
  if(!b.isEmpty()){lo=b.getMin();b.getSize(s[0],s[1],s[2]);}
  bool valid=!b.isEmpty();for(int i=0;i<3;++i)valid=valid&&std::isfinite(b.getMin()[i])&&std::isfinite(b.getMax()[i]);
  for(int i=0;i<3;++i)rank[i]=recover&&valid&&!std::isfinite(s[i])?double(b.getMax()[i])-lo[i]:s[i];
  int x=0,y=1;
  if(!three){int small=0;for(int i=1;i<3;++i)if(rank[i]<rank[small])small=i;
    x=(small+1)%3;y=(small+2)%3;
    if(rank[x]<rank[y]||(rank[x]==rank[y]&&x<y))std::swap(x,y);
  }
  SbVec4f out(0,0,0,1);
  for(int c=0;c<(three?3:2);++c){int i=c==0?x:c==1?y:2;
    float den=s[i]<=0?1:s[i];float num=p[i]-lo[i];float value=num/den;
    if(recover&&valid&&std::isfinite(p[i])&&(!std::isfinite(num)||!std::isfinite(den))) {
      const double wd=std::isfinite(den)?double(den):double(b.getMax()[i])-lo[i];
      const double wn=std::isfinite(num)?double(num):double(p[i])-lo[i];
      if(std::isfinite(wn/wd)&&std::fabs(wn/wd)<=FLT_MAX)value=float(wn/wd);
    }out[c]=value;
  }return out;
}
static void sample(const SbBox3f & box,const SbVec3f & point,bool three,bool recover) {
  visit([&](SoCallbackAction * a,Fixture * n) {
    n->box=box;SoState * s=a->getState();
    if(three)SoMultiTextureEnabledElement::enableTexture3(s,n,0);
    else SoMultiTextureEnabledElement::set(s,n,0,TRUE);
    SoMultiTextureEnabledElement::set(s,n,2,TRUE);
    const auto * before=SoMultiTextureCoordinateElement::getInstance(s);
    check(before->getType(0)==SoMultiTextureCoordinateElement::DEFAULT,"fixture starts default");
    for(int repeat=0;repeat<2;++repeat){
      SoTextureCoordinateBundle bundle(a,FALSE,TRUE);
      check(bundle.needCoordinates()&&bundle.isFunction(),"default bundle enabled without GL");
      const SbVec4f want=expected(box,point,three,recover);
      equal(bundle.get(point,SbVec3f(0,1,0)),want,"bundle direct",true);
      const auto * e=SoMultiTextureCoordinateElement::getInstance(s);
      equal(e->get(0,point,SbVec3f(0,1,0)),want,"default callback unit zero",true);
      equal(e->get(2,point,SbVec3f(0,1,0)),want,"default callback additional unit",true);
    }
    check(SoMultiTextureCoordinateElement::getInstance(s)->getType(0)==SoMultiTextureCoordinateElement::DEFAULT,"bundle pop restores coordinates");
  });
}
int main(int argc,char **argv){init();bool repair=mode(argc,argv);const float m=FLT_MAX;
  if(repair){
    for(bool three:{false,true}){
      sample(SbBox3f(-m,-m,-1,m,m/2,1),SbVec3f(0,m/4,0),three,true);
      sample(SbBox3f(-m,-2,-1,-m/2,2,1),SbVec3f(m,1,0),three,true);
      sample(SbBox3f(-m,-m,-1,m,m/2,1),SbVec3f(INFINITY,0,0),three,true);
      sample(SbBox3f(-m,-m,-1,m,m/2,1),SbVec3f(NAN,0,0),three,true);
    }
  }else{
    const float d=std::numeric_limits<float>::denorm_min();
    for(bool three:{false,true}){
      sample(SbBox3f(.1f,.2f,.3f,.9f,1.3f,.8f),SbVec3f(.37f,.61f,.44f),three,false);
      // Exact x extent exceeds y by half an ULP, but both Float extents are one.
      sample(SbBox3f(-std::ldexp(1.0f,-24),0,0,1,1,.25f),SbVec3f(.2f,.7f,.1f),three,false);
      sample(SbBox3f(-d,0,0,1,1,.25f),SbVec3f(.2f,.7f,.1f),three,false);
      sample(SbBox3f(0,0,0,1,1,1),SbVec3f(.2f,.7f,.4f),three,false);
      sample(SbBox3f(2,3,4,2,3,4),SbVec3f(2,3,4),three,false);
      sample(SbBox3f(),SbVec3f(.2f,.7f,.4f),three,false);
      sample(SbBox3f(-m,-m,0,m,m,INFINITY),SbVec3f(0,1,0),three,false);
    }
  }return finish();}
