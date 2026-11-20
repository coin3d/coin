// Compile using the baseline's unmodified private/public headers. Only sizeof
// and alignment use its class definition; calls resolve to the library's
// historical Itanium symbols so no inline implementation masks ABI changes.
#include "SoVertexArrayTexCoordGuard.h"
#include <Inventor/SoDB.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoSeparator.h>
#include <cstdint>
#include <type_traits>

extern "C" void constructGuard(void *, SoState *, SoNode *,
                               const SoCoordinateElement *, SbBool)
  asm("_ZN26SoVertexArrayTexCoordGuardC1EP7SoStateP6SoNodePK19SoCoordinateElementi");
extern "C" void destroyGuard(void *) asm("_ZN26SoVertexArrayTexCoordGuardD1Ev");

static int failures = 0;
static const SbVec4f & uv(void *, const SbVec3f &, const SbVec3f &)
{
  static const SbVec4f value(0.25f, 0.75f, 0.0f, 1.0f);
  return value;
}
static void check(void *, SoAction * action)
{
  if (!action->isOfType(SoCallbackAction::getClassTypeId())) return;
  SoState * state = action->getState();
  SoNode * node = action->getCurPathTail();
  const SbVec3f points[] = { SbVec3f(0,0,0), SbVec3f(1,0,0), SbVec3f(0,1,0) };
  SoCoordinateElement::set3(state, node, 3, points);
  SoMultiTextureCoordinateElement::setFunction(state, node, 0, uv, NULL);
  const SoCoordinateElement * coords = SoCoordinateElement::getInstance(state);
  struct Frame {
    std::uint64_t before;
    std::aligned_storage<sizeof(SoVertexArrayTexCoordGuard),
                         alignof(SoVertexArrayTexCoordGuard)>::type storage;
    std::uint64_t after;
  } frame;
  frame.before = frame.after = UINT64_C(0x123456789abcdef0);
  constructGuard(&frame.storage, state, node, coords, TRUE);
  const auto element = SoMultiTextureCoordinateElement::getInstance(state);
  if (element->getNum(0) != 3 || element->get4(0, 2) != uv(NULL, points[0], points[0])) ++failures;
  destroyGuard(&frame.storage);
  if (frame.before != UINT64_C(0x123456789abcdef0) || frame.after != frame.before) ++failures;
  if (SoMultiTextureCoordinateElement::getType(state, 0) != SoMultiTextureCoordinateElement::FUNCTION) ++failures;
}
int main()
{
  SoDB::init();
  SoSeparator * root = new SoSeparator;
  root->ref();
  SoCallback * callback = new SoCallback;
  callback->setCallback(check, NULL);
  root->addChild(callback);
  { SoCallbackAction action; action.apply(root); }
  root->unref();
  SoDB::finish();
  return failures ? 1 : 0;
}
