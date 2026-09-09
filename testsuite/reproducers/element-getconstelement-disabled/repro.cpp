// Reproducer for a NULL-pointer member call in
// SoElement::getConstElement(SoState*, int) (include/Inventor/elements/
// SoElement.h), found by review on branch fix/null-dereference
// (commit b76dc45e01).
//
// SoState::getConstElement(stackIndex) is a fast, unchecked primitive
// that assert()s (Debug builds) or returns a stale/NULL element (Release
// builds -- isElementEnabled() is never checked there) when stackIndex
// isn't enabled for the current action; its own doc comment says as much
// and tells callers to check SoState::isElementEnabled() first if unsure.
//
// SoElement::getConstElement(state, stackIndex) -- the *static* method
// used throughout the codebase (~45 files) as e.g.
// "coin_safe_cast<const SoXElement*>(getConstElement(state,
// classStackIndex))" followed by an "if (!elem) { ...; return default; }"
// guard -- has a *different*, and explicitly documented, contract: "If no
// instance can be returned, NULL is returned" (src/elements/SoElement.cpp).
// Callers rely on that promise to avoid calling SoState::isElementEnabled()
// themselves. But the implementation broke that promise: it called
// state->getConstElement(stackIndex) and then unconditionally
// element->capture(state) on the result, in that order -- so in a Release
// build (NDEBUG, where SoState::getConstElement()'s own assert is
// compiled out and it silently returns NULL for a disabled element),
// capture() dereferenced NULL before the caller ever got a pointer back
// to null-check.
//
// This reaches exactly that path via SoBumpMapElement::get(), called on a
// bare SoState constructed with an empty enabled-elements list (so
// SoBumpMapElement is not enabled at all) -- the minimal way to reach a
// disabled element's static get() accessor without needing a full
// SoAction/scene graph.
//
// See run.sh in this directory for how to build and run this against a
// given libCoin build (built without NDEBUG defined away, i.e. a normal
// Release build -- COIN_DEBUG's own assert() in SoState::getConstElement()
// would otherwise abort first and mask this specific bug).

#include "../CoinCleanup.h"
#include <cstdio>
#include <Inventor/SoDB.h>
#include <Inventor/lists/SoTypeList.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/elements/SoBumpMapElement.h>

int main()
{
  SoDB::init();
  CoinReproducerCleanup cleanup;

  // No elements enabled at all -- SoBumpMapElement::classStackIndex is
  // not among them, so it's disabled for this state.
  SoTypeList enabled;
  SoState state(NULL, enabled);

  // SoElement::capture() checks state->isCacheOpen() before touching
  // `this` at all -- with no cache open, the buggy code's
  // element->capture(state) call on a NULL element happens to return
  // immediately without dereferencing anything, so nothing crashes even
  // though calling a member function through a NULL pointer is undefined
  // behavior regardless (this is exactly what needs a UBSan build to
  // catch reliably, see run.sh). Setting cacheopen TRUE, matching the
  // state while e.g. a SoSeparator is recording a display-list cache,
  // makes it call the virtual captureThis(state) instead -- a real,
  // unconditional dereference of the NULL this pointer's vtable.
  state.setCacheOpen(TRUE);

  fprintf(stderr, "[repro] calling SoBumpMapElement::get() on a state with "
                  "no elements enabled and a cache open\n");
  SoGLImage * result = SoBumpMapElement::get(&state);

  fprintf(stderr, "[repro] PASS: returned %p without crashing\n", (void*)result);
  return 0;
}
