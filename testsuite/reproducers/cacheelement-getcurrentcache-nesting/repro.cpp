// Reproducer for a NULL-pointer virtual call in SoGLRenderCache::call(),
// reachable only when the experimental COIN_NESTED_CACHING env var is
// enabled (off by default).
//
// SoCacheElement::anyOpen()/invalidate()/addCacheDependency() all walk up
// the SoElement stack (via getNextInStack()) to find an ancestor's open
// cache -- this is what makes state->isCacheOpen() correctly report TRUE
// even several state->push() calls below where a cache was actually
// SoCacheElement::set(). SoCacheElement::getCurrentCache() did NOT do
// this: it only looked at the SoCacheElement instance for the CURRENT
// depth, which is freshly reset to cache=NULL on every state->push()
// (see SoCacheElement::push()).
//
// SoGLRenderCache::call(), when COIN_NESTED_CACHING is enabled, trusts
// state->isCacheOpen() to decide whether to look up "the current cache"
// via getCurrentCache(), then unconditionally does a virtual call through
// it: `static_cast<SoGLRenderCache*>(getCurrentCache(state))->
// addNestedCache(...)`. Whenever the open cache was set() at a
// shallower depth than the current one (an entirely ordinary situation --
// it's exactly what happens once child nodes push additional state depths
// while an ancestor SoSeparator's own cache recording is still open),
// getCurrentCache() returned NULL while isCacheOpen() was TRUE, so this
// dereferences a NULL pointer for the virtual dispatch.
//
// This builds the exact call sequence via the public SoCacheElement/
// SoGLRenderCache APIs, matching the documented SoCache usage pattern
// (see the class doc comment in src/caches/SoCache.cpp), instead of
// relying on Coin's own cache-creation heuristics to naturally line up.
//
// See run.sh in this directory for how to build and run this against a
// given libCoin build. Needs COIN_NESTED_CACHING=1 in the environment.

#include "../CoinCleanup.h"
#include <cstdio>
#include <cstdlib>
#include <Inventor/SoDB.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/SbColor.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/caches/SoGLRenderCache.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoCube.h>

static void checkCB(void *, SoAction * action)
{
  if (!action->isOfType(SoGLRenderAction::getClassTypeId())) return;
  SoState * state = action->getState();

  // Outer cache scope: e.g. a SoSeparator's SoGLCacheList recording.
  state->push();
  SoGLRenderCache * outer = new SoGLRenderCache(state);
  outer->ref();
  SoCacheElement::set(state, outer);
  outer->open(state);
  outer->close();
  // Deliberately not popped/closed off the element stack yet -- the
  // outer cache is still "current" for any deeper state depth, matching
  // SoSeparator::GLRenderBelowPath keeping its own cache set() while
  // traversing (and further push()ing into) its children.

  // A child scope one level deeper, e.g. entered by a descendant node.
  state->push();

  // Give this depth its own, genuinely fresh SoCacheElement instance with
  // cache == NULL -- SoElement only creates a per-depth copy on a WRITE
  // access (SoCacheElement::set()), not merely from state->push(), so we
  // need at least one set() call at this depth to reproduce the same
  // situation as e.g. SoFaceSet.cpp's "SoCacheElement::set(state, NULL);
  // // close cache" (used after building/discarding its convex-triangle
  // cache) while nested inside an ancestor's still-open cache scope.
  SoCacheElement::set(state, NULL);

  SoGLRenderCache * inner = new SoGLRenderCache(state);
  inner->ref();
  inner->open(state);
  inner->close();

  fprintf(stderr, "[repro] isCacheOpen()=%d, about to call inner->call() nested inside the still-open outer scope\n",
          state->isCacheOpen());
  inner->call(state);
  fprintf(stderr, "[repro] PASS: inner->call() survived while nested inside an ancestor's open cache\n");

  inner->unref();
  state->pop();
  outer->unref();
  state->pop();
}

int main()
{
  setenv("COIN_NESTED_CACHING", "1", 1);
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
