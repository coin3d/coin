// A cached default mapping must carry its bounds dependencies to ancestors.
#include "../CoinCleanup.h"
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/bundles/SoTextureCoordinateBundle.h>
#include <Inventor/caches/SoCache.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoComplexityElement.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTexture2.h>
#include <cstdio>

class DependencyFaceSet : public SoFaceSet {
public:
  DependencyFaceSet() : ok(true), runs(0), computations(0) {}
  bool ok;
  int runs, computations;

  void computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center) override
  {
    ++this->computations;
    const float width = SoComplexityElement::get(action->getState()) > 0.5f ? 2.0f : 1.0f;
    box.setBounds(SbVec3f(0,0,0), SbVec3f(width,1,0));
    center = box.getCenter();
  }

  void GLRender(SoGLRenderAction * action) override
  {
    SoState * state = action->getState();
    SoComplexityElement::set(state, 0.25f);
    { SoTextureCoordinateBundle warm(action, TRUE); }
    const int warmed = this->computations;

    const SbBool invalid = SoCacheElement::setInvalid(FALSE);
    state->push();
    SoCache * parent = new SoCache(state);
    parent->ref();
    SoCacheElement::set(state, parent);
    state->push();
    SoCacheElement::set(state, NULL);
    const bool current = SoCacheElement::getCurrentCache(state) == parent;
    SbVec4f compiled;
    {
      SoTextureCoordinateBundle use(action, TRUE);
      compiled = use.get(SbVec3f(0.75f,0.25f,0), SbVec3f(0,0,1));
    }
    const bool reused = this->computations == warmed;
    state->pop();
    state->pop();
    SoCacheElement::setInvalid(invalid);

    const bool before = parent->isValid(state);
    SoComplexityElement::set(state, 0.25f);
    const bool unchanged = parent->isValid(state);
    SoComplexityElement::set(state, 0.75f);
    const bool after = parent->isValid(state);
    SbVec4f fresh;
    {
      SoTextureCoordinateBundle now(action, TRUE);
      fresh = now.get(SbVec3f(0.75f,0.25f,0), SbVec3f(0,0,1));
    }
    const bool values = compiled == SbVec4f(0.25f,0.75f,0,1) &&
      fresh == SbVec4f(0.375f,0.25f,0,1);
    this->ok = this->ok && current && before && unchanged && !after && values;
    ++this->runs;
    std::printf("context=%u cached_bbox=%d before=%d unchanged=%d after=%d compiled=%g,%g fresh=%g,%g valid=%d\n",
      action->getCacheContext(), reused, before, unchanged, after,
      compiled[0], compiled[1], fresh[0], fresh[1], this->ok);
    parent->unref();
  }
};

int main()
{
  SoDB::init();
  CoinReproducerCleanup cleanup;
  SoSeparator * root = new SoSeparator;
  root->ref();
  root->renderCaching = SoSeparator::OFF;
  SoTexture2 * texture = new SoTexture2;
  const unsigned char pixel[] = {255,255,255};
  texture->image.setValue(SbVec2s(1,1), 3, pixel);
  root->addChild(texture);
  root->addChild(new SoCoordinate3);
  DependencyFaceSet * shape = new DependencyFaceSet;
  root->addChild(shape);
  SoOffscreenRenderer first(SbViewportRegion(16,16)), second(SbViewportRegion(16,16));
  const bool firstok = first.render(root);
  const bool secondok = second.render(root);
  const bool ok = firstok && secondok && shape->ok && shape->runs == 2;
  root->unref();
  return ok ? 0 : 1;
}
