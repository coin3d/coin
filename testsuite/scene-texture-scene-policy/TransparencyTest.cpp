#include <Inventor/SbViewportRegion.h>
#include <Inventor/SoDB.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/misc/SoContextHandler.h>
#include <Inventor/misc/SoGLDriverDatabase.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/nodes/SoSceneTexture2.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransparencyType.h>

#include <GL/glx.h>
#include <X11/Xlib.h>

#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace {

using Policy = SoGLRenderAction::TransparencyType;

constexpr short textureSize = 16;
constexpr int skipped = 77;
constexpr Policy outerActionPolicy = SoGLRenderAction::NONE;
constexpr Policy outerStatePolicy = SoGLRenderAction::ADD;

enum class TestCase { Initial, Replace, Edit, NullNode, InvalidNode, Fbo };

[[noreturn]] void fail(const char * stage, const char * message)
{
  std::fprintf(stderr, "%s: %s\n", stage, message);
  std::exit(EXIT_FAILURE);
}

int skip(const char * message)
{
  std::printf("Skipped: %s\n", message);
  return skipped;
}

void checkGl(const char * stage)
{
  const GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    std::fprintf(stderr, "%s: OpenGL error 0x%x\n", stage, static_cast<unsigned>(error));
    std::exit(EXIT_FAILURE);
  }
}

void checkPolicy(SoGLRenderAction * action, Policy expectedAction, Policy expectedState,
                 const char * stage)
{
  const Policy actionPolicy = action->getTransparencyType();
  const int statePolicy = SoShapeStyleElement::getTransparencyType(action->getState());
  if (actionPolicy != expectedAction || statePolicy != expectedState) {
    std::fprintf(stderr, "%s: expected action/state policy %d/%d, got %d/%d\n",
                 stage, expectedAction, expectedState, actionPolicy, statePolicy);
    std::exit(EXIT_FAILURE);
  }
}

Display * coinOffscreenDisplay()
{
  void * context = cc_glglue_context_create_offscreen(textureSize, textureSize);
  if (!context) return nullptr;

  Display * display = nullptr;
  if (cc_glglue_context_make_current(context)) {
    display = glXGetCurrentDisplay();
    checkGl("activating the offscreen capability probe");
    cc_glglue_context_reinstate_previous(context);
  }
  cc_glglue_context_destruct(context);
  if (glXGetCurrentContext()) {
    fail("offscreen capability probe", "left a GLX context current");
  }
  return display;
}

struct GlxWindow {
  Display * display = XOpenDisplay(nullptr);
  bool ownsDisplay = true;
  XVisualInfo * visual = nullptr;
  GLXContext context = nullptr;
  Colormap colormap = None;
  Window drawable = None;

  ~GlxWindow()
  {
    if (context) {
      glXMakeCurrent(display, None, nullptr);
      glXDestroyContext(display, context);
    }
    if (drawable) XDestroyWindow(display, drawable);
    if (colormap) XFreeColormap(display, colormap);
    if (visual) XFree(visual);
    if (display && ownsDisplay) XCloseDisplay(display);
  }
};

struct Probe {
  Probe(bool fbo, const GlxWindow & window)
    : fbo(fbo), consumerContext(window.context), consumerDrawable(window.drawable)
  {
  }

  bool fbo;
  GLXContext consumerContext;
  GLXDrawable consumerDrawable;
  const char * stage = nullptr;
  Policy expected = outerStatePolicy;
  int producerVisits = 0;
  int consumerVisits = 0;
};

int policyColor(int policy)
{
  return 20 * policy + 32;
}

void observeProducer(void * userdata, SoAction * action)
{
  auto & probe = *static_cast<Probe *>(userdata);
  auto * renderAction = static_cast<SoGLRenderAction *>(action);
  const GLXContext context = glXGetCurrentContext();
  const GLXDrawable drawable = glXGetCurrentDrawable();
  if (probe.fbo) {
    GLint framebuffer = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &framebuffer);
    if (context != probe.consumerContext || drawable != probe.consumerDrawable ||
        framebuffer == 0) {
      fail(probe.stage, "the producer did not use an FBO in the consumer context");
    }
  }
  else {
    if (!context || context == probe.consumerContext ||
        !drawable || drawable == probe.consumerDrawable) {
      fail(probe.stage, "the producer did not use a separate offscreen GLX context");
    }
  }

  checkPolicy(renderAction, probe.fbo ? outerActionPolicy : probe.expected,
              probe.expected, probe.stage);
  // Write a texel marker from the producer's effective policy so the
  // consumer can detect a redraw whose results were never published.
  const int policy = SoShapeStyleElement::getTransparencyType(action->getState());
  glClearColor(0.0f, policyColor(policy) / 255.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  checkGl(probe.stage);
  ++probe.producerVisits;
}

void observeConsumer(void * userdata, SoAction * action)
{
  auto & probe = *static_cast<Probe *>(userdata);
  if (glXGetCurrentContext() != probe.consumerContext ||
      glXGetCurrentDrawable() != probe.consumerDrawable) {
    fail(probe.stage, "the consumer GLX context was not restored");
  }

  checkPolicy(static_cast<SoGLRenderAction *>(action), outerActionPolicy,
              outerStatePolicy, probe.stage);
  GLint binding = 0;
  GLint width = 0;
  GLint height = 0;
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &binding);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
  if (binding == 0 || width != textureSize || height != textureSize) {
    fail(probe.stage, "no rendered texture of the requested size");
  }

  std::array<GLubyte, textureSize * textureSize * 4> texels = {};
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, texels.data());
  checkGl(probe.stage);
  const int center = 4 * ((textureSize / 2) * textureSize + textureSize / 2);
  const int expected = policyColor(probe.expected);
  const int actual = texels[center + 1];
  if (std::abs(actual - expected) > 1) {
    std::fprintf(stderr, "%s: expected texture green channel %d, got %d\n",
                 probe.stage, expected, actual);
    std::exit(EXIT_FAILURE);
  }
  ++probe.consumerVisits;
}

void render(SoGLRenderAction & action, SoSeparator * root, Probe & probe,
            const char * stage, Policy expected, bool redraw)
{
  probe.stage = stage;
  probe.expected = expected;
  const int producerVisits = probe.producerVisits;
  const int consumerVisits = probe.consumerVisits;
  action.apply(root);
  checkGl(stage);
  const int visits = probe.producerVisits - producerVisits;
  if (visits != int(redraw)) {
    std::fprintf(stderr, "%s: expected %d producer visits, got %d\n", stage, int(redraw), visits);
    std::exit(EXIT_FAILURE);
  }
  if (probe.consumerVisits != consumerVisits + 1) {
    fail(stage, "expected one consumer traversal");
  }
}

SoTransparencyType * transparency(Policy policy)
{
  auto * node = new SoTransparencyType;
  node->value = policy;
  return node;
}

int exercise(TestCase test, const GlxWindow & window)
{
  const bool fbo = test == TestCase::Fbo;
  const uint32_t cacheContext = SoGLCacheContextElement::getUniqueCacheContext();
  const bool fboSupported = SoGLDriverDatabase::isSupported(cc_glglue_instance(cacheContext),
                                                            SO_GL_FRAMEBUFFER_OBJECT);
  if (fbo && !fboSupported) {
    SoContextHandler::destructingContext(cacheContext);
    return skip("The GL context has no FBO support in Coin");
  }
  if (!fbo && fboSupported) fail("setup", "COIN_DONT_USE_FBO=1 did not disable FBOs");
  checkGl("initializing the OpenGL context");

  Probe probe(fbo, window);
  auto * root = new SoSeparator;
  root->ref();
  root->renderCaching = SoSeparator::OFF;
  root->addChild(transparency(outerStatePolicy));

  auto * complexity = new SoComplexity;
  complexity->textureQuality = 0.25f;
  root->addChild(complexity);

  auto * texture = new SoSceneTexture2;
  texture->size.setValue(textureSize, textureSize);
  auto * producer = new SoSeparator;
  producer->renderCaching = SoSeparator::OFF;
  auto * producerObserver = new SoCallback;
  producerObserver->setCallback(observeProducer, &probe);
  producer->addChild(producerObserver);
  texture->scene = producer;
  root->addChild(texture);

  auto * consumerObserver = new SoCallback;
  consumerObserver->setCallback(observeConsumer, &probe);
  root->addChild(consumerObserver);

  {
    SoGLRenderAction action(SbViewportRegion(32, 32));
    action.setCacheContext(cacheContext);
    action.setTransparencyType(outerActionPolicy);

    // Establish a valid buffer independently of the first-render override
    // when testing replacement and in-place notification.
    const bool baselineMatchesOuter = test == TestCase::Replace || test == TestCase::Edit;
    const Policy initial = baselineMatchesOuter ? outerStatePolicy : SoGLRenderAction::BLEND;
    auto * policy = transparency(initial);
    texture->sceneTransparencyType = policy;

    render(action, root, probe, "first render", initial, true);
    render(action, root, probe, "unchanged first render", initial, false);

    const char * stage = nullptr;
    Policy changed = SoGLRenderAction::SCREEN_DOOR;
    switch (test) {
    case TestCase::Initial:
      texture->wrapS = SoSceneTexture2::CLAMP;
      render(action, root, probe, "wrap change", initial, false);
      break;
    case TestCase::Replace:
      changed = SoGLRenderAction::DELAYED_BLEND;
      texture->sceneTransparencyType = transparency(changed);
      stage = "replace transparency node";
      break;
    case TestCase::Fbo:
    case TestCase::Edit:
      policy->value = changed;
      stage = "edit transparency node value";
      break;
    case TestCase::NullNode:
      texture->sceneTransparencyType = nullptr;
      changed = outerStatePolicy;
      stage = "inherit with NULL transparency node";
      break;
    case TestCase::InvalidNode:
      texture->sceneTransparencyType = new SoSeparator;
      changed = outerStatePolicy;
      stage = "inherit with invalid transparency node type";
      break;
    }
    if (stage) {
      render(action, root, probe, stage, changed, true);
      render(action, root, probe, "unchanged after policy change", changed, false);
    }
  }

  root->unref();
  SoContextHandler::destructingContext(cacheContext);
  checkGl("releasing Coin GL resources");
  return EXIT_SUCCESS;
}

int run(TestCase test)
{
  GlxWindow window;
  if (!window.display) return skip("No X11 display");
  if (test != TestCase::Fbo) {
    Display * display = coinOffscreenDisplay();
    if (!display) return skip("Coin could not activate an offscreen GLX context");

    // Use Coin's connection for both drawables so restoring the consumer
    // context after a GLXPixmap render stays on the same GLX connection.
    XCloseDisplay(window.display);
    window.display = display;
    window.ownsDisplay = false;
  }

  const int screen = DefaultScreen(window.display);
  int attributes[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
  window.visual = glXChooseVisual(window.display, screen, attributes);
  if (!window.visual) return skip("No compatible GLX visual");
  window.context = glXCreateContext(window.display, window.visual, nullptr, True);
  if (!window.context) fail("setup", "could not create a consumer GLX context");

  const Window parent = RootWindow(window.display, screen);
  window.colormap = XCreateColormap(window.display, parent, window.visual->visual, AllocNone);
  XSetWindowAttributes options = {};
  options.colormap = window.colormap;
  window.drawable = XCreateWindow(window.display, parent, 0, 0, 32, 32, 0,
                                  window.visual->depth, InputOutput, window.visual->visual,
                                  CWColormap, &options);
  if (!glXMakeCurrent(window.display, window.drawable, window.context)) {
    fail("setup", "could not activate the consumer GLX context");
  }
  return exercise(test, window);
}

} // namespace

int main(int argc, char ** argv)
{
  const struct {
    const char * name;
    TestCase test;
  } cases[] = {
    { "initial", TestCase::Initial },
    { "replace", TestCase::Replace },
    { "edit", TestCase::Edit },
    { "null", TestCase::NullNode },
    { "invalid", TestCase::InvalidNode },
    { "fbo", TestCase::Fbo }
  };
  if (argc == 2) {
    for (const auto & entry : cases) {
      if (std::strcmp(argv[1], entry.name) == 0) {
        SoDB::init();
        const int result = run(entry.test);
        SoDB::finish();
        return result;
      }
    }
  }
  fail("usage", "CoinSceneTexturePolicyTest initial|replace|edit|null|invalid|fbo");
}
