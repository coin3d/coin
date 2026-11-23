#include <Inventor/SbName.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/SoDB.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/misc/SoContextHandler.h>
#include <Inventor/misc/SoGLDriverDatabase.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/nodes/SoSceneTexture2.h>
#include <Inventor/nodes/SoSeparator.h>

#include <GL/glx.h>
#include <X11/Xlib.h>

#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace {

constexpr short textureSize = 16;
constexpr int skipStatus = 77;

enum class Backend { Fbo, Pbuffer };
using Pixel = std::array<GLubyte, 4>;

[[noreturn]] void fail(const char * message)
{
  std::fprintf(stderr, "%s\n", message);
  std::exit(EXIT_FAILURE);
}

int skip(const char * message)
{
  std::printf("Skipped: %s\n", message);
  return skipStatus;
}

void checkGl(const char * stage)
{
  const GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    std::fprintf(stderr, "%s: OpenGL error 0x%x\n", stage,
                 static_cast<unsigned>(error));
    std::exit(EXIT_FAILURE);
  }
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

bool hasPbufferConfig(Display * display, int screen)
{
  int major, minor;
  if (!glXQueryVersion(display, &major, &minor) ||
      major < 1 || (major == 1 && minor < 3)) {
    return false;
  }
  const int attributes[] = {
    GLX_DRAWABLE_TYPE, GLX_PBUFFER_BIT,
    GLX_RENDER_TYPE, GLX_RGBA_BIT,
    GLX_RED_SIZE, 8, GLX_GREEN_SIZE, 8, GLX_BLUE_SIZE, 8, GLX_ALPHA_SIZE, 8,
    GLX_DEPTH_SIZE, 24, GLX_STENCIL_SIZE, 1,
    None
  };
  int count = 0;
  GLXFBConfig * configs = glXChooseFBConfig(display, screen, attributes, &count);
  const bool supported = configs && count > 0;
  if (configs) XFree(configs);
  return supported;
}

struct Probe {
  Probe(Backend backend, GLXContext context, GLXDrawable drawable)
    : backend(backend), consumerContext(context), consumerDrawable(drawable)
  {
  }

  Backend backend;
  GLXContext consumerContext;
  GLXDrawable consumerDrawable;
  const char * stage = nullptr;
  Pixel expected = {};
  int producerVisits = 0;
  int readbacks = 0;
};

void observeProducer(void * userdata, SoAction *)
{
  auto & probe = *static_cast<Probe *>(userdata);
  const GLXContext context = glXGetCurrentContext();
  const GLXDrawable drawable = glXGetCurrentDrawable();
  if (probe.backend == Backend::Fbo) {
    GLint framebuffer = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &framebuffer);
    if (context != probe.consumerContext || drawable != probe.consumerDrawable ||
        framebuffer == 0) {
      fail("The producer did not render into an FBO of the consumer context");
    }
  }
  else {
    if (!context || context == probe.consumerContext || !drawable ||
        drawable == probe.consumerDrawable) {
      fail("The producer did not render in a separate offscreen GLX context");
    }
    if (probe.producerVisits == 0) {
      // Coin's GLX offscreen path can fall back to a GLXPixmap. GLX 1.3
      // permits selecting clobber events on a pbuffer, but not on a pixmap.
      Display * display = glXGetCurrentDisplay();
      unsigned long previous = 0;
      unsigned long selected = 0;
      glXGetSelectedEvent(display, drawable, &previous);
      glXSelectEvent(display, drawable, GLX_PBUFFER_CLOBBER_MASK);
      glXGetSelectedEvent(display, drawable, &selected);
      glXSelectEvent(display, drawable, previous);
      XSync(display, False);
      if (!(selected & GLX_PBUFFER_CLOBBER_MASK)) {
        fail("The offscreen drawable did not accept a GLX pbuffer event mask");
      }
    }
  }
  checkGl("producer traversal");
  ++probe.producerVisits;
}

void readTexture(void * userdata, SoAction *)
{
  auto & probe = *static_cast<Probe *>(userdata);
  if (glXGetCurrentContext() != probe.consumerContext ||
      glXGetCurrentDrawable() != probe.consumerDrawable) {
    fail("The consumer GLX context was not restored");
  }
  checkGl(probe.stage);

  GLint binding = 0;
  GLint width = 0;
  GLint height = 0;
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &binding);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
  checkGl("querying the consumer texture");
  if (binding == 0 || width != textureSize || height != textureSize) {
    std::fprintf(stderr, "%s: expected a %dx%d texture, got binding %d of size %dx%d\n",
                 probe.stage, textureSize, textureSize, binding, width, height);
    std::exit(EXIT_FAILURE);
  }

  std::array<GLubyte, textureSize * textureSize * 4> texels = {};
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, texels.data());
  checkGl("reading the consumer texture");
  for (int y = 0; y < textureSize; ++y) {
    for (int x = 0; x < textureSize; ++x) {
      const GLubyte * rgba = texels.data() + 4 * (y * textureSize + x);
      for (int channel = 0; channel < 4; ++channel) {
        if (std::abs(int(rgba[channel]) - int(probe.expected[channel])) > 1) {
          std::fprintf(stderr,
                       "%s: texel (%d,%d) expected RGBA (%d,%d,%d,%d), got (%d,%d,%d,%d)\n",
                       probe.stage, x, y, probe.expected[0], probe.expected[1],
                       probe.expected[2], probe.expected[3], rgba[0], rgba[1], rgba[2], rgba[3]);
          std::exit(EXIT_FAILURE);
        }
      }
    }
  }
  ++probe.readbacks;
}

void render(SoGLRenderAction & action, SoSeparator * root, Probe & probe,
            const char * stage, const Pixel & expected, bool expectProducerVisit)
{
  probe.stage = stage;
  probe.expected = expected;
  const int readbacks = probe.readbacks;
  const int producerVisits = probe.producerVisits;
  action.apply(root);
  checkGl(stage);
  if (probe.readbacks != readbacks + 1) fail("Expected one consumer readback");
  if (probe.producerVisits != producerVisits + int(expectProducerVisit)) {
    std::fprintf(stderr, "%s: unexpected number of producer traversals\n", stage);
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
    if (!display || !glXGetCurrentContext()) {
      fail("Coin offscreen activation did not make a GLX context current");
    }
    checkGl("activating the Coin offscreen context");
    cc_glglue_context_reinstate_previous(context);
  }
  cc_glglue_context_destruct(context);
  if (glXGetCurrentContext()) fail("The offscreen capability probe left a GLX context current");
  return display;
}

int exercise(Backend backend, const GlxWindow & window)
{
  const uint32_t cacheContext = SoGLCacheContextElement::getUniqueCacheContext();
  const cc_glglue * glue = cc_glglue_instance(cacheContext);
  const bool fboSupported = SoGLDriverDatabase::isSupported(glue, SO_GL_FRAMEBUFFER_OBJECT);
  if (backend == Backend::Fbo && !fboSupported) {
    SoContextHandler::destructingContext(cacheContext);
    return skip("The GL context has no FBO support in Coin");
  }
  if (backend == Backend::Pbuffer && fboSupported) {
    fail("COIN_DONT_USE_FBO=1 did not disable Coin FBO support");
  }
  checkGl("initializing the OpenGL context");

  Probe probe{backend, window.context, window.drawable};
  SoSeparator * root = new SoSeparator;
  root->ref();
  root->renderCaching = SoSeparator::OFF;

  SoComplexity * complexity = new SoComplexity;
  complexity->textureQuality = 0.25f;
  root->addChild(complexity);

  SoSceneTexture2 * texture = new SoSceneTexture2;
  texture->size.setValue(textureSize, textureSize);
  texture->type = SoSceneTexture2::RGBA8;
  SoSeparator * producer = new SoSeparator;
  producer->renderCaching = SoSeparator::OFF;
  // This observer issues no rendering commands; the texture is all clear color.
  SoCallback * observer = new SoCallback;
  observer->setCallback(observeProducer, &probe);
  producer->addChild(observer);
  texture->scene = producer;
  root->addChild(texture);

  SoCallback * consumer = new SoCallback;
  consumer->setCallback(readTexture, &probe);
  root->addChild(consumer);

  {
    SoGLRenderAction action(SbViewportRegion(32, 32));
    action.setCacheContext(cacheContext);
    action.setTransparencyType(SoGLRenderAction::NONE);
    auto setBackground = [texture](const Pixel & color) {
      texture->backgroundColor.setValue(color[0] / 255.0f, color[1] / 255.0f,
                                        color[2] / 255.0f, color[3] / 255.0f);
    };

    const Pixel initial{{31, 101, 233, 71}};
    setBackground(initial);
    render(action, root, probe, "initial render", initial, true);
    render(action, root, probe, "unchanged initial render", initial, false);

    const Pixel alphaOnly{{31, 101, 233, 211}};
    setBackground(alphaOnly);
    render(action, root, probe, "alpha-only change", alphaOnly, true);

    const Pixel rgbOnly{{197, 41, 83, 211}};
    setBackground(rgbOnly);
    render(action, root, probe, "consecutive RGB-only change", rgbOnly, true);

    const Pixel transparent{{0, 255, 0, 0}};
    setBackground(transparent);
    render(action, root, probe, "consecutive transparent change", transparent, true);
    render(action, root, probe, "unchanged transparent render", transparent, false);
  }

  root->unref();
  SoContextHandler::destructingContext(cacheContext);
  checkGl("releasing Coin GL resources");
  std::printf("Verified RGBA texture updates using the %s producer\n",
              backend == Backend::Fbo ? "FBO" : "GLX pbuffer");
  return EXIT_SUCCESS;
}

int run(Backend backend)
{
  GlxWindow window;
  if (!window.display) return skip("No X11 display");
  if (backend == Backend::Pbuffer) {
    if (!hasPbufferConfig(window.display, DefaultScreen(window.display))) {
      return skip("No GLX 1.3 RGBA8 pbuffer configuration with depth and stencil");
    }
    Display * display = coinOffscreenDisplay();
    if (!display) return skip("Coin could not create and activate an offscreen GLX context");

    // Use Coin's connection for both drawables so that switching between the
    // consumer window and the producer pbuffer stays on one GLX connection.
    XCloseDisplay(window.display);
    window.display = display;
    window.ownsDisplay = false;
  }
  const int screen = DefaultScreen(window.display);
  int attributes[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, None};
  window.visual = glXChooseVisual(window.display, screen, attributes);
  if (!window.visual) return skip("No compatible GLX visual");

  window.context = glXCreateContext(window.display, window.visual, nullptr, True);
  if (!window.context) fail("Could not create a context for the selected GLX visual");
  const Window parent = RootWindow(window.display, screen);
  window.colormap = XCreateColormap(window.display, parent, window.visual->visual, AllocNone);
  XSetWindowAttributes options = {};
  options.colormap = window.colormap;
  window.drawable = XCreateWindow(window.display, parent, 0, 0, 32, 32, 0,
                                  window.visual->depth, InputOutput, window.visual->visual,
                                  CWColormap, &options);
  if (!glXMakeCurrent(window.display, window.drawable, window.context)) {
    fail("Could not activate the consumer GLX context");
  }

  return exercise(backend, window);
}

} // namespace

int main(int argc, char ** argv)
{
  if (argc != 2 || (std::strcmp(argv[1], "fbo") != 0 &&
                    std::strcmp(argv[1], "pbuffer") != 0)) {
    fail("Usage: CoinSceneTextureBackgroundTest fbo|pbuffer");
  }
  const Backend backend = std::strcmp(argv[1], "fbo") == 0 ? Backend::Fbo : Backend::Pbuffer;
  SoDB::init();
  const int status = run(backend);
  SoDB::finish();
  return status;
}
