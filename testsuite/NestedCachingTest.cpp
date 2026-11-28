#include "reproducers/CoinCleanup.h"

#include <Inventor/C/tidbits.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/caches/SoGLRenderCache.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/misc/SoRefPtr.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoSeparator.h>

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

void
count_render_traversal(void * data, SoAction * action)
{
  if (action->isOfType(SoGLRenderAction::getClassTypeId())) {
    ++*static_cast<int *>(data);
  }
}

SoCallback *
make_render_counter(int & traversals)
{
  SoCallback * counter = new SoCallback;
  counter->setCallback(count_render_traversal, &traversals);
  return counter;
}

int
test_nested_scene(SbBool enabled)
{
  int parent_traversals = 0;
  int child_traversals = 0;
  SoRefPtr<SoSeparator> root(new SoSeparator);
  root->renderCaching = SoSeparator::OFF;

  SoOrthographicCamera * camera = new SoOrthographicCamera;
  camera->position.setValue(0.0f, 0.0f, 5.0f);
  camera->height = 4.0f;
  root->addChild(camera);
  SoLightModel * light_model = new SoLightModel;
  light_model->model = SoLightModel::BASE_COLOR;
  root->addChild(light_model);

  SoSeparator * parent = new SoSeparator;
  parent->renderCaching = SoSeparator::OFF;
  root->addChild(parent);
  parent->addChild(make_render_counter(parent_traversals));

  SoSeparator * child = new SoSeparator;
  child->renderCaching = SoSeparator::ON;
  parent->addChild(child);
  child->addChild(make_render_counter(child_traversals));
  SoBaseColor * color = new SoBaseColor;
  color->rgb.setValue(1.0f, 0.0f, 0.0f);
  child->addChild(color);
  child->addChild(new SoCube);

  const int size = 64;
  const int parent_cache_frame = 6;
  const int color_change_frame = 12;
  SoOffscreenRenderer renderer(SbViewportRegion(size, size));
  renderer.setComponents(SoOffscreenRenderer::RGB);

  for (int frame = 0; frame < 18; ++frame) {
    // Give the child time to populate its cache before recording the parent.
    if (frame == parent_cache_frame) { parent->renderCaching = SoSeparator::ON; }
    const bool changed_color = frame >= color_change_frame;
    if (frame == color_change_frame) { color->rgb.setValue(0.0f, 1.0f, 0.0f); }

    const int previous_parent = parent_traversals;
    const int previous_child = child_traversals;
    if (!renderer.render(root.get())) {
      std::fprintf(stderr, "Scene rendering failed at frame %d\n", frame);
      return frame == 0 && parent_traversals == 0 ? 77 : 1;
    }

    const unsigned char * pixels = renderer.getBuffer();
    if (!pixels) {
      std::fprintf(stderr, "Scene produced no pixel buffer at frame %d\n", frame);
      return 1;
    }
    const unsigned char * center = pixels + 3 * (size * (size / 2) + size / 2);
    if (center[changed_color ? 1 : 0] < 240 ||
        center[changed_color ? 0 : 1] > 15 || center[2] > 15) {
      std::fprintf(stderr, "Unexpected center pixel at frame %d: RGB %u,%u,%u\n", frame,
                   unsigned(center[0]), unsigned(center[1]), unsigned(center[2]));
      return 1;
    }

    const bool traversed_parent = parent_traversals > previous_parent;
    const bool traversed_child = child_traversals > previous_child;
    if ((frame == 0 || frame == color_change_frame) &&
        (!traversed_parent || !traversed_child)) {
      std::fprintf(stderr, "Expected traversal of both separators at frame %d\n", frame);
      return 1;
    }
    if (frame >= parent_cache_frame + 3 && frame < color_change_frame &&
        (traversed_child || traversed_parent != (enabled == FALSE))) {
      std::fprintf(stderr, "Unexpected cache reuse at frame %d (parent traversed: %d, child: %d)\n",
                   frame, traversed_parent, traversed_child);
      return 1;
    }
  }
  return 0;
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
    std::fprintf(stderr, "Run the rendering test with an X display (for example, xvfb-run)\n");
    return 77;
  }
#endif

  SoDB::init();
  CoinReproducerCleanup cleanup;
  if (std::strcmp(argv[1], "scene") == 0) { return test_nested_scene(expected); }

  Replay replay(expected);
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
