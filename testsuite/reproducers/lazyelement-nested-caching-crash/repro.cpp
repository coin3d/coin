// Reproducer for issue #402: segfault in SoGLLazyElement::endCaching().
//
// SoGLLazyElement::beginCaching()/endCaching() are called from two
// independent places in Coin:
//   - src/caches/SoGLCacheList.cpp (SoSeparator's display-list cache)
//   - src/caches/SoPrimitiveVertexCache.cpp (SoShape's per-shape vertex
//     cache, only used when transparency type is one of the
//     SORTED_OBJECT_SORTED_TRIANGLE_* modes)
//
// These two call sites legitimately nest: a SoShape inside a SoSeparator
// whose SoGLCacheList recording is already open (beginCaching() already
// called) will validate/rebuild its own SoPrimitiveVertexCache -- which
// calls beginCaching()/endCaching() again, nested inside the still-open
// outer scope.
//
// Before the fix, SoGLLazyElement::beginCaching()/endCaching() stored the
// currently-open scope's bookkeeping (precachestate, postcachestate, and
// related bitmasks) in plain scalar fields, not a stack. So:
//   1. outer beginCaching()  -- sets precachestate/postcachestate to the
//      outer (separator) scope's GLState pointers.
//   2. inner beginCaching()  -- OVERWRITES precachestate/postcachestate
//      with the inner (shape) scope's pointers, losing the outer ones.
//   3. inner endCaching()    -- finishes, then sets precachestate and
//      postcachestate to NULL.
//   4. outer endCaching()    -- dereferences postcachestate, which is now
//      NULL -- SIGSEGV.
//
// This reproducer calls the public beginCaching()/endCaching() API
// directly, nested, from within a real SoGLRenderAction traversal (via an
// SoCallback node), to exercise exactly this sequence without needing to
// coax Coin's cache-creation heuristics (frame-count thresholds,
// dependency invalidation, etc.) into aligning naturally -- which is
// possible (see the original issue, reproduced via SoQt + a
// SoCenterballDragger added to a live scene under
// SORTED_OBJECT_SORTED_TRIANGLE_BLEND) but timing-sensitive and awkward to
// force deterministically in a small headless test.
//
// See run.sh in this directory for how to build and run this against a
// given libCoin build.

#include "../CoinCleanup.h"
#include <cstdio>
#include <Inventor/SoDB.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/SbColor.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLLazyElement.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoCube.h>

static void checkCB(void *, SoAction * action)
{
  if (!action->isOfType(SoGLRenderAction::getClassTypeId())) return;
  SoState * state = action->getState();

  // outer scope: e.g. SoGLCacheList::open() for an ancestor SoSeparator
  SoGLLazyElement::GLState outer_pre, outer_post;
  SoGLLazyElement::beginCaching(state, &outer_pre, &outer_post);

  // inner scope, nested: e.g. SoPrimitiveVertexCache's constructor for a
  // descendant SoShape validating its cache while the outer scope is open
  SoGLLazyElement::GLState inner_pre, inner_post;
  SoGLLazyElement::beginCaching(state, &inner_pre, &inner_post);

  // inner scope closes first: e.g. SoPrimitiveVertexCache::close()
  SoGLLazyElement::endCaching(state);

  // outer scope closes: e.g. SoGLCacheList::close() -- this dereferenced
  // a NULL postcachestate before the fix, since the inner endCaching()
  // call had already zeroed it out from under the outer scope.
  SoGLLazyElement::endCaching(state);

  fprintf(stderr, "[repro] PASS: nested beginCaching()/endCaching() survived\n");
}

int main()
{
  SoDB::init();
  CoinReproducerCleanup cleanup;

  SoSeparator * root = new SoSeparator;
  root->ref();

  SoOrthographicCamera * cam = new SoOrthographicCamera;
  cam->position.setValue(0, 0, 5);
  cam->height = 4.0f;
  root->addChild(cam);

  SoMaterial * mat = new SoMaterial;
  root->addChild(mat);

  SoCallback * cb = new SoCallback;
  cb->setCallback(checkCB);
  root->addChild(cb);

  SoCube * cube = new SoCube;
  root->addChild(cube);

  SbViewportRegion vp(64, 64);
  SoOffscreenRenderer renderer(vp);
  renderer.setBackgroundColor(SbColor(0, 0, 0));
  SbBool ok = renderer.render(root);

  root->unref();

  if (!ok) {
    fprintf(stderr, "[repro] render() failed (no GL context available in this "
                     "environment) -- inconclusive, not a pass or a fail\n");
    return 2;
  }
  return 0;
}
