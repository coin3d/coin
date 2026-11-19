#include "TextureCoordinateM08Test.h"
#include <Inventor/nodes/SoTextureMatrixTransform.h>
using namespace m08;
static SbMatrix column(float a,float b,float c,float d){SbMatrix m=SbMatrix::identity();m[0][0]=a;m[1][0]=b;m[2][0]=c;m[3][0]=d;return m;}
static void sample(const SbVec4f & input,const SbMatrix & matrix,const SbVec4f & want,bool alias=false){
  visit([&](SoCallbackAction * action,Fixture * n){
    SoMultiTextureMatrixElement::set(action->getState(),n,0,matrix);
    SoPickedPoint pp(action->getCurPath(),action->getState(),SbVec3f(0,0,0));
    pp.setObjectTextureCoords(input);
    equal(pp.getTextureCoords(),want,"picked setter transforms raw four components",true);
    equal(pp.getObjectTextureCoords(),input,"setter retains raw object Q",true);
    equal(pp.getObjectTextureCoords(n),input,"tail lookup retains raw object coordinate",true);
    SoPickedPoint copy(pp);equal(copy.getTextureCoords(),want,"copy retains transformed texture",true);
    if(alias){
      SoMultiTextureMatrixElement::makeIdentity(action->getState(),n);
      pp.setObjectTextureCoords(pp.getTextureCoords());
      equal(pp.getTextureCoords(),want,"setter accepts transformed reference alias",true);
      equal(pp.getObjectTextureCoords(),want,"alias copied to object coordinates",true);
      pp.setObjectTextureCoords(pp.getObjectTextureCoords());
      equal(pp.getTextureCoords(),want,"setter accepts object reference alias",true);
    }
  });
}
static void ancestor(bool repair){
  SoGroup * root=new SoGroup;root->ref();
  SoTextureMatrixTransform * transform=new SoTextureMatrixTransform;
  // This inverse is exactly representable. Its first column is (2,-2,0,0).
  SbMatrix forward=SbMatrix::identity();forward[0][0]=.5f;forward[1][0]=1;
  transform->matrix=forward;root->addChild(transform);
  SoGroup * parent=new SoGroup;root->addChild(parent);Fixture * n=new Fixture;parent->addChild(n);
  n->run=[&](SoCallbackAction * a,Fixture * leaf){
    SoMultiTextureMatrixElement::makeIdentity(a->getState(),leaf);
    SoPickedPoint pp(a->getCurPath(),a->getState(),SbVec3f(0,0,0));
    const SbVec4f input=repair?SbVec4f(FLT_MAX,FLT_MAX,3,2):SbVec4f(.3f,.7f,-0.0f,2);
    pp.setObjectTextureCoords(input);
    SbVec4f want;pp.getImageToObject(parent).multVecMatrix(input,want);
    if(repair)want[0]=0;
    equal(pp.getObjectTextureCoords(parent),want,"ancestor transforms texture using path inverse",true);
    equal(pp.getObjectTextureCoords(),input,"ancestor lookup leaves object coordinate intact",true);
  };
  SoCallbackAction action;action.apply(root);root->unref();
}
int main(int argc,char **argv){init();bool repair=mode(argc,argv);
  if(repair){
    SbVec4f p(FLT_MAX,FLT_MAX,1,2);SbMatrix m=column(2,-2,1,0);SbVec4f want;m.multVecMatrix(p,want);want[0]=1;sample(p,m,want,true);
    p.setValue(std::ldexp(1.0f,-75),std::ldexp(1.0f,-75),std::ldexp(1.0f,-75),std::ldexp(1.0f,-75));
    const float t=std::ldexp(1.0f,-76);m=column(t,t,t,t);m.multVecMatrix(p,want);want[0]=std::numeric_limits<float>::denorm_min();sample(p,m,want);
    // Finite, nonzero legacy output in one column must survive while the
    // companion's exact-zero cancellation recovers a representable residual.
    p.setValue(16777216,1,-16777216,2);m=column(1,1,1,1);
    m[0][1]=1;m[1][1]=1;m[2][1]=1;m[3][1]=0;
    m.multVecMatrix(p,want);want[1]=1;sample(p,m,want,true);
    // Intermediate sum overflow with finite products, followed by cancellation.
    p.setValue(FLT_MAX,FLT_MAX,-FLT_MAX,2);m=column(1,1,1,0);m.multVecMatrix(p,want);want[0]=FLT_MAX;sample(p,m,want);
    // The setter input aliases its image coordinate while a new matrix is
    // installed. Fallback must read the original four values, including the
    // large cancelling component, after the legacy Float pass writes S.
    visit([&](SoCallbackAction * action,Fixture * n){
      SoState * state=action->getState();
      SoMultiTextureMatrixElement::makeIdentity(state,n);
      SoPickedPoint pp(action->getCurPath(),state,SbVec3f(0,0,0));
      const SbVec4f original(16777216,1,-16777216,2);
      pp.setObjectTextureCoords(original);
      SoMultiTextureMatrixElement::set(state,n,0,column(1,1,1,0));
      pp.setObjectTextureCoords(pp.getTextureCoords());
      equal(pp.getTextureCoords(),SbVec4f(1,1,-16777216,2),"nonidentity alias retains fallback source",true);
      equal(pp.getObjectTextureCoords(),original,"nonidentity alias retains original object source",true);
    });
    ancestor(true);
  }else{
    const SbVec4f values[]={SbVec4f(.3f,.7f,-.2f,2),SbVec4f(-0.0f,0,-0.0f,-0.0f),SbVec4f(INFINITY,2,3,4),SbVec4f(NAN,2,3,4)};
    for(const auto & p:values)for(const SbMatrix & m:{SbMatrix::identity(),column(.1f,.2f,.3f,.4f)}){SbVec4f want;m.multVecMatrix(p,want);sample(p,m,want,true);}
    SbVec4f p(16777216,1,-16777216,2);SbMatrix m=column(1,1,1,1);SbVec4f want;m.multVecMatrix(p,want);sample(p,m,want,true);
    ancestor(false);
  }return finish();}
