// SoLinePatternElement must clamp the scale factor to [1, 256] in every build,
// not only when COIN_DEBUG is set.
#include <Inventor/SoDB.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/elements/SoLinePatternElement.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoSeparator.h>
#include <cstdio>

static int failures = 0;

static void
check(SoState * state, int factor, int expected)
{
  SoLinePatternElement::set(state, 0x1357, factor);
  const int pattern = SoLinePatternElement::get(state);
  const int scale = SoLinePatternElement::getScaleFactor(state);
  if (pattern != 0x1357 || scale != expected) {
    std::fprintf(stderr, "factor %d: got pattern 0x%x scale %d, expected 0x1357 scale %d\n",
                 factor, pattern, scale, expected);
    ++failures;
  }
}

static void
probe(void *, SoAction * action)
{
  SoState * state = action->getState();
  check(state, 0, 1);
  check(state, -5, 1);
  check(state, 1, 1);
  check(state, 256, 256);
  check(state, 257, 256);
  check(state, 600, 256);
}

int
main()
{
  SoDB::init();
  SoCallbackAction::enableElement(SoLinePatternElement::getClassTypeId(),
                                  SoLinePatternElement::getClassStackIndex());
  SoSeparator * root = new SoSeparator;
  root->ref();
  SoCallback * cb = new SoCallback;
  cb->setCallback(probe, NULL);
  root->addChild(cb);
  SoCallbackAction action;
  action.apply(root);
  root->unref();
  if (failures == 0) std::printf("line pattern scale factor clamped in all cases\n");
  return failures == 0 ? 0 : 1;
}
