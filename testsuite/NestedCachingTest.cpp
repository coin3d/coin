#include "reproducers/CoinCleanup.h"

#include <Inventor/C/tidbits.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/caches/SoGLRenderCache.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/nodes/SoCallback.h>

#include <cstdio>
#include <cstring>

namespace {
const char * const nested_caching_env = "COIN_NESTED_CACHING";

struct Replay {
  SbBool enabled;
  bool ran = false;
  bool passed = false;

  explicit Replay(SbBool enabled) : enabled(enabled) {}
};

void
replay_nested_cache(void * data, SoAction * action)
{
  if (!action->isOfType(SoGLRenderAction::getClassTypeId())) { return; }

  Replay & replay = *static_cast<Replay *>(data);
  replay.ran = true;
  SoState * state = action->getState();

  state->push();
  SoGLRenderCache * parent = new SoGLRenderCache(state);
  parent->ref();
  SoCacheElement::set(state, parent);
  parent->open(state);
  parent->close();

  // Leave the parent cache active one level above the child, as in the
  // cacheelement-getcurrentcache-nesting reproducer.
  state->push();
  SoCacheElement::set(state, NULL);
  SoGLRenderCache * child = new SoGLRenderCache(state);
  child->ref();
  child->open(state);
  child->close();

  const bool parent_open = state->isCacheOpen();
  const SbBool saved_invalid = SoCacheElement::setInvalid(FALSE);
  child->call(state);
  const SbBool first_invalidated = SoCacheElement::setInvalid(FALSE);
  const SbBool changed_env = coin_setenv(nested_caching_env, replay.enabled ? "0" : "1", 1);
  const SbBool changed_api = SoGLRenderCache::setNestedCachingEnabled(!replay.enabled);
  child->call(state);
  const SbBool second_invalidated = SoCacheElement::setInvalid(saved_invalid);

  child->unref();
  state->pop();
  parent->unref();
  state->pop();

  // The disabled path raises the invalidation flag for the enclosing
  // cache recording. The enabled path records the cache dependency.
  replay.passed = parent_open && changed_env && !changed_api &&
    first_invalidated == !replay.enabled && second_invalidated == !replay.enabled &&
    SoGLRenderCache::getNestedCachingEnabled() == replay.enabled;
}
}

int
main(int argc, char ** argv)
{
  if (argc != 3) { return 1; }
  const bool enable = std::strcmp(argv[2], "enable") == 0;
  const bool disable = std::strcmp(argv[2], "disable") == 0;
  const bool from_env = std::strcmp(argv[2], "environment") == 0 || disable;
  const bool use_default = std::strcmp(argv[2], "default") == 0;

  if (use_default) { coin_unsetenv(nested_caching_env); }
  else if (!coin_setenv(nested_caching_env, from_env ? "1" : "0", 1)) { return 1; }

  const SbBool expected = (enable || (from_env && !disable)) ? TRUE : FALSE;
  const bool environment_matches = SoGLRenderCache::getNestedCachingEnabled() == from_env;
  const bool override_accepted = !(enable || disable) ||
    (SoGLRenderCache::setNestedCachingEnabled(!expected) &&
     SoGLRenderCache::getNestedCachingEnabled() == !expected &&
     SoGLRenderCache::setNestedCachingEnabled(expected));
  if (!environment_matches || !override_accepted ||
      SoGLRenderCache::getNestedCachingEnabled() != expected) {
    std::fprintf(stderr, "Nested caching configuration disagrees with case %s\n", argv[2]);
    return 1;
  }
  if (std::strcmp(argv[1], "config") == 0) { return 0; }

#ifdef COIN_TEST_HAVE_GLX
  // Coin's GLX fallback requires an X display even if EGL is selected.
  const char * display = coin_getenv("DISPLAY");
  if (!display || !*display) {
    std::fprintf(stderr, "Run the replay test with an X display (for example, xvfb-run)\n");
    return 77;
  }
#endif

  Replay replay(expected);
  SoDB::init();
  CoinReproducerCleanup cleanup;
  SoCallback * root = new SoCallback;
  root->ref();
  root->setCallback(replay_nested_cache, &replay);
  SoOffscreenRenderer renderer(SbViewportRegion(16, 16));
  const SbBool rendered = renderer.render(root);
  root->unref();

  if (!rendered && !replay.ran) {
    std::fprintf(stderr, "No offscreen GL context available\n");
    return 77;
  }
  if (!rendered || !replay.passed) {
    std::fprintf(stderr, "Nested caching replay disagrees with case %s\n", argv[2]);
    return 1;
  }
  return 0;
}
