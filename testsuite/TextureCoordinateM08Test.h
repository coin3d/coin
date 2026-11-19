#ifndef COIN_TEXTURE_COORDINATE_M08_TEST_H
#define COIN_TEXTURE_COORDINATE_M08_TEST_H
#include "TextureCoordinateRangeTest.h"
#include <Inventor/actions/SoSubAction.h>
#include <Inventor/elements/SoMultiTextureEnabledElement.h>
#include <Inventor/elements/SoMultiTextureMatrixElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/bundles/SoTextureCoordinateBundle.h>
#include <Inventor/caches/SoTextureCoordinateCache.h>
#include <Inventor/SoPickedPoint.h>
#include <functional>
#include <limits>
namespace m08 {
using namespace range_test;
class Fixture : public SoCube {
public:
  SbBox3f box;
  std::function<void(SoCallbackAction *,Fixture *)> run;
  void computeBBox(SoAction *,SbBox3f & b,SbVec3f & center) override { b=box;center.setValue(0,0,0); }
  void callback(SoCallbackAction * a) override { run(a,this); }
};
inline void init() {
  SoDB::init();
  SO_ENABLE(SoCallbackAction,SoMultiTextureEnabledElement);
  SO_ENABLE(SoCallbackAction,SoMultiTextureCoordinateElement);
  SO_ENABLE(SoCallbackAction,SoMultiTextureMatrixElement);
  SO_ENABLE(SoCallbackAction,SoViewportRegionElement);
}
inline void visit(const std::function<void(SoCallbackAction *,Fixture *)> & f) {
  Fixture * p=new Fixture;p->ref();p->run=f;SoCallbackAction action;action.apply(p);p->unref();
}
inline void equal2(const SbVec2f & a,const SbVec2f & b,const char * label) {
  check(same(a[0],b[0]),std::string(label)+" s");check(same(a[1],b[1]),std::string(label)+" t");
}
}
#endif
