#include "TextureCoordinateM08Test.h"
using namespace m08;
int main(int argc,char **argv) {
  init();const bool repair=mode(argc,argv);
  visit([&](SoCallbackAction * a,Fixture * node) {
    SoState * state=a->getState();
    const float d=std::numeric_limits<float>::denorm_min();
    const SbVec4f cases[]={SbVec4f(d,-2*d,7,d),SbVec4f(0,-0.0f,7,-d),SbVec4f(INFINITY,d,7,d),
      SbVec4f(7,11,3,3),SbVec4f(1,-0.0f,3,0),SbVec4f(-0.0f,1,3,-0.0f),
      SbVec4f(1,INFINITY,3,INFINITY),SbVec4f(1,2,3,NAN)};
    for(unsigned i=0;i<sizeof(cases)/sizeof(cases[0]);++i) {
      if((i<3)!=repair)continue;
      SbVec4f data[2]={SbVec4f(9,8,7,1),cases[i]};
      SoMultiTextureCoordinateElement::set4(state,node,3,2,data);
      const auto * e=SoMultiTextureCoordinateElement::getInstance(state);
      const float q=data[1][3],r=q==0?1:1/q;
      SbVec2f want(data[1][0]*r,data[1][1]*r);
      if(repair) for(int c=0;c<2;++c) if(std::isfinite(data[1][c])&&std::isfinite(data[1][c]/q))want[c]=data[1][c]/q;
      equal2(e->get2(3,1),want,"4D per-component reciprocal conversion");
      check(e->getDimension(3)==4 && e->getNum(3)==2,"dimension and count survive conversion");
      equal2(e->get2(3,0),SbVec2f(9,8),"index selection");
    }
    if(!repair) {
      SbVec2f p[2]={SbVec2f(1,-0.0f),SbVec2f(2,3)};
      SoMultiTextureCoordinateElement::set2(state,node,0,2,p);
      const auto * e=SoMultiTextureCoordinateElement::getInstance(state);
      check(&e->get2(0,1)==&p[1],"2D reference remains array reference");
      SbVec3f v(3,-0.0f,8);SoMultiTextureCoordinateElement::set3(state,node,2,1,&v);
      equal2(e->get2(2,0),SbVec2f(3,-0.0f),"3D drops z");
      equal2(e->get2(2,-1),SbVec2f(0,0),"negative index guard");
      equal2(e->get2(2,1),SbVec2f(0,0),"past-end index guard");
      // 7*(1/3) differs from 7/3 in Float.
      check(!same(7.0f*(1.0f/3.0f),7.0f/3.0f),"ordinary reciprocal control discriminates division");
    }
  });return finish();
}
