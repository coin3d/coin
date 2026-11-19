// CPU-only public picking regressions: the Float barycentrics come from the
// same ray action and vertex order as the callback under test.
#include "TextureCoordinateRangeTest.h"
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/SoPath.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/actions/SoSubAction.h>
#include <Inventor/elements/SoMultiTextureEnabledElement.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/details/SoFaceDetail.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoSeparator.h>
#include <limits>
#include <algorithm>

namespace {
using namespace range_test;
class TriangleProbe : public SoCube {
public:
  SoPrimitiveVertex v[3];
  SbVec3f weights, hit;
  SbBool front = FALSE;
  bool intersects = false;
  int kind = 0, details = 0;
  TriangleProbe() : weights(0,0,0), hit(0,0,0) {
    const SbVec3f points[] = { SbVec3f(0,0,0), SbVec3f(1,0,0), SbVec3f(0,1,0) };
    for (int i=0;i<3;++i) {
      v[i].setPoint(points[i]);
      v[i].setNormal(i==0?1:0,i==1?1:0,i==2?1:0);
      const int materials[]={17,29,43}; v[i].setMaterialIndex(materials[i]);
    }
  }
  void rayPick(SoRayPickAction * a) override {
    if (!shouldRayPick(a)) return;
    computeObjectSpaceRay(a);
    if (kind == 1) { invokeLineSegmentCallbacks(a,&v[0],&v[1]); return; }
    if (kind == 2) { invokePointCallbacks(a,&v[0]); return; }
    intersects = a->intersect(v[0].getPoint(),v[1].getPoint(),v[2].getPoint(),hit,weights,front);
    invokeTriangleCallbacks(a,&v[0],&v[1],&v[2]);
  }
  SoDetail * createTriangleDetail(SoRayPickAction *,const SoPrimitiveVertex * a,
      const SoPrimitiveVertex * b,const SoPrimitiveVertex * c,SoPickedPoint *) override {
    ++details;
    check(a == &v[0] && b == &v[1] && c == &v[2],"detail receives original vertices");
    SoFaceDetail * d = new SoFaceDetail;
    d->setFaceIndex(23); d->setPartIndex(11); return d;
  }
};
SbVec4f legacy(const TriangleProbe * p) {
  return p->v[0].getTextureCoords()*p->weights[0] +
         p->v[1].getTextureCoords()*p->weights[1] +
         p->v[2].getTextureCoords()*p->weights[2];
}
struct Result { SbVec4f tex, old; SbVec3f weights; };
Result pick(const SbVec4f (&tex)[3],float x=.25f,float y=.25f,
            const int * permutation=NULL,int kind=0,bool back=false) {
  SoSeparator * root=new SoSeparator; root->ref();
  TriangleProbe * p=new TriangleProbe; p->kind=kind;
  for(int i=0;i<3;++i) p->v[i].setTextureCoords(tex[i]);
  if(permutation) {
    SoPrimitiveVertex copy[3]={p->v[0],p->v[1],p->v[2]};
    for(int i=0;i<3;++i) p->v[i]=copy[permutation[i]];
  }
  root->addChild(p);
  Result result = {SbVec4f(0,0,0,0),SbVec4f(0,0,0,0),SbVec3f(0,0,0)};
  {
    SoRayPickAction a(SbViewportRegion(64,64));
    a.setRay(SbVec3f(x,y,back?-1:1),SbVec3f(0,0,back?1:-1));
    a.apply(root);
    SoPickedPoint * pp=a.getPickedPoint();
    check(pp!=NULL,"ray produces picked point");
    if(pp) {
      result.tex=pp->getObjectTextureCoords(); result.weights=p->weights;
      check(pp->getPath()->getHead()==root && pp->getPath()->getTail()==p,"picked path retained");
      check(pp->isOnGeometry(),"point lies on geometry");
      SbVec3f expectedNormal;
      int materialVertex=0;
      if(kind==0) {
        expectedNormal=p->v[0].getNormal()*p->weights[0]+
          p->v[1].getNormal()*p->weights[1]+p->v[2].getNormal()*p->weights[2];
        for(int i=1;i<3;++i) if(p->weights[i]>p->weights[materialVertex]) materialVertex=i;
      } else if(kind==1) {
        // This baseline's independent line callback reverses these weights.
        expectedNormal=p->v[0].getNormal()*.25f+p->v[1].getNormal()*.75f;
        materialVertex=1;
      } else expectedNormal=p->v[0].getNormal();
      expectedNormal.normalize();
      check(pp->getMaterialIndex()==p->v[materialVertex].getMaterialIndex(),"material follows legacy weights");
      const SbVec3f actualNormal=pp->getObjectNormal();
      check(same(actualNormal[0],expectedNormal[0]) && same(actualNormal[1],expectedNormal[1]) &&
        same(actualNormal[2],expectedNormal[2]),"normal follows legacy Float weights and normalization");
      const SbVec3f point=pp->getObjectPoint();
      check(std::fabs(point[0]-x)<1e-6f && std::fabs(point[1]-y)<1e-6f && point[2]==0,"intersection retained");
      if(kind==0) {
        check(p->intersects,"public intersect agrees with callback");
        check(p->hit==point,"callback keeps public intersect point");
        check(p->details==1,"one triangle detail callback");
        const SoDetail * d=pp->getDetail(p);
        check(d && d->getTypeId()==SoFaceDetail::getClassTypeId(),"triangle detail type retained");
        if(d && d->getTypeId()==SoFaceDetail::getClassTypeId()) {
          const SoFaceDetail * f=static_cast<const SoFaceDetail *>(d);
          check(f->getFaceIndex()==23 && f->getPartIndex()==11,"triangle detail contents retained");
        }
        result.old=legacy(p);
      }
    }
  }
  root->unref(); return result;
}
void repairTests() {
  const float h=std::ldexp(1.0f,100),d=std::numeric_limits<float>::denorm_min();
  // The small middle term is analytic, rather than a double accumulation
  // oracle (which also loses the term next to H).
  const SbVec4f cancellation[]={SbVec4f(h,3,0,h),SbVec4f(1,5,0,1),SbVec4f(-2*h,7,0,-2*h)};
  Result r=pick(cancellation);
  equal(r.weights==SbVec3f(.5f,.25f,.25f)?r.tex:SbVec4f(NAN,NAN,NAN,NAN),SbVec4f(.25f,4.5f,0,.25f),"zero cancellation and raw Q recovered",true);
  const SbVec4f tiny[]={SbVec4f(d,-d,2*d,4*d),SbVec4f(d,-d,2*d,4*d),SbVec4f(d,-d,2*d,4*d)};
  r=pick(tiny);
  // Only products rounded all the way to zero are in the required domain.
  check(same(r.tex[0],d),"all zero products recover positive minimum subnormal");
  check(same(r.tex[1],-d),"all zero products recover negative minimum subnormal");
  check(same(r.tex[3],4*d),"ordinary Q beside repaired components retained");
  const SbVec4f partiallyLost[]={SbVec4f(4*d,-4*d,1,d),SbVec4f(2*d,-2*d,1,d),SbVec4f(2*d,-2*d,1,d)};
  r=pick(partiallyLost);
  equal(r.tex,SbVec4f(3*d,-3*d,1,d),"zero products repair even when legacy sum is nonzero",true);
  const SbVec4f smallResidual[]={SbVec4f(-h,h,0,2),SbVec4f(-std::ldexp(1.0f,-120),std::ldexp(1.0f,-120),0,4),SbVec4f(2*h,-2*h,0,8)};
  r=pick(smallResidual);
  equal(r.tex,SbVec4f(-std::ldexp(1.0f,-122),std::ldexp(1.0f,-122),0,4.0f),"large exponent gaps retain signed residuals and raw Q",true);
  const SbVec4f mixed[]={SbVec4f(h,1,d,h),SbVec4f(1,std::ldexp(1.0f,-24),d,1),SbVec4f(-2*h,-2+std::ldexp(1.0f,-23),d,-2*h)};
  r=pick(mixed);
  check(same(r.tex[0],.25f) && same(r.tex[2],d) && same(r.tex[3],.25f),"component repairs are independent");
  check(same(r.old[1],std::ldexp(1.0f,-25)),"finite nonzero cancellation discriminator fixture");
  check(same(r.tex[1],r.old[1]),"repair does not widen finite nonzero sibling");
  int order[]={0,1,2};
  do {
    r=pick(cancellation,.25f,.25f,order);
    check(same(r.tex[0],.25f) && same(r.tex[3],.25f),"all six vertex orders recover the same residual");
    check(same(r.tex[1],r.old[1]),"vertex order preserves ordinary Float component");
  } while(std::next_permutation(order,order+3));
  r=pick(cancellation,.25f,.25f,NULL,0,true);
  check(same(r.tex[0],.25f) && same(r.tex[3],.25f),"back face interpolation also repairs");
}
void enableCpuTexture(void *,SoAction * action) {
  SoMultiTextureEnabledElement::set(action->getState(),NULL,0,TRUE);
}
void functionalTest() {
  SoSeparator * root=new SoSeparator; root->ref();
  SoCallback * enable=new SoCallback; enable->setCallback(enableCpuTexture); root->addChild(enable);
  SoTextureCoordinatePlane * tc=new SoTextureCoordinatePlane;
  tc->directionS=SbVec3f(2,0,0); tc->directionT=SbVec3f(0,3,0); tc->directionR=SbVec3f(0,0,4);
  root->addChild(tc);
  SoCoordinate3 * c=new SoCoordinate3;
  const SbVec3f points[]={SbVec3f(0,0,0),SbVec3f(1,0,0),SbVec3f(0,1,0)};
  c->point.setValues(0,3,points); root->addChild(c);
  SoFaceSet * face=new SoFaceSet; face->numVertices.setValue(3); root->addChild(face);
  {
    SoRayPickAction a(SbViewportRegion(64,64));
    a.setRay(SbVec3f(.25f,.25f,1),SbVec3f(0,0,-1)); a.apply(root);
    SoPickedPoint * pp=a.getPickedPoint(); check(pp!=NULL,"functional face picked");
    if(pp) equal(pp->getObjectTextureCoords(),SbVec4f(.5f,.75f,0,1),"functional texture semantics",true);
  }
  root->unref();
}
void legacyTests() {
  const float d=std::numeric_limits<float>::denorm_min(),h=std::ldexp(1.0f,100);
  const float inf=std::numeric_limits<float>::infinity(),nan=std::numeric_limits<float>::quiet_NaN();
  const SbVec4f cases[][3]={
    {SbVec4f(1,2,3,2),SbVec4f(4,5,6,4),SbVec4f(7,8,9,8)},
    {SbVec4f(1,0,-0.0f,2),SbVec4f(std::ldexp(1.0f,-24),-0.0f,-0.0f,4),SbVec4f(-2+std::ldexp(1.0f,-23),0,-0.0f,8)},
    {SbVec4f(inf,nan,-inf,-0.0f),SbVec4f(2,3,4,-0.0f),SbVec4f(5,6,7,-0.0f)},
    {SbVec4f(h,1,-h,3),SbVec4f(h,2,-h,5),SbVec4f(h,3,-h,7)}
  };
  const float points[][2]={{.25f,.25f},{0,0},{1,0},{0,1},{.5f,0},{0,.5f},{.5f,.5f},{.2f,.3f},{1.0f/3,1.0f/3}};
  for(const auto & tex:cases) for(const auto & xy:points) {
    Result r=pick(tex,xy[0],xy[1]); equal(r.tex,r.old,"legacy sum using actual Float weights",true);
  }
  // Subnormal products that remain nonzero do not require a correctly rounded
  // widened sum; ordinary Float arithmetic is an admissible preserved result.
  const SbVec4f nonzeroProducts[]={SbVec4f(8*d,16*d,32*d,64*d),SbVec4f(8*d,16*d,32*d,64*d),SbVec4f(8*d,16*d,32*d,64*d)};
  Result r=pick(nonzeroProducts); equal(r.tex,r.old,"nonzero subnormal products preserve Float result",true);
  // The independent line path still uses its legacy weights in this baseline.
  const SbVec4f line[]={SbVec4f(2,4,6,8),SbVec4f(10,12,14,16),SbVec4f(0,0,0,0)};
  r=pick(line,.25f,0,NULL,1);
  equal(r.tex,SbVec4f(8,10,12,14),"line legacy reversed texture weights",true);
  const SbVec4f point[]={SbVec4f(h,-0.0f,d,7),SbVec4f(0,0,0,0),SbVec4f(0,0,0,0)};
  r=pick(point,0,0,NULL,2); equal(r.tex,point[0],"point texture unchanged",true);
  functionalTest();
}
}
int main(int argc,char ** argv) {
  SoDB::init();
  SO_ENABLE(SoRayPickAction,SoMultiTextureEnabledElement);
  if(mode(argc,argv)) repairTests(); else legacyTests();
  return finish();
}
