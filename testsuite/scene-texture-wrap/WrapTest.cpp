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

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace {

constexpr short textureSize = 16;

[[noreturn]] void fail(const char * stage, const char * message)
{
  std::fprintf(stderr, "%s: %s\n", stage, message);
  std::exit(EXIT_FAILURE);
}

int skip(const char * message)
{
  std::printf("Skipped: %s\n", message);
  return 77;
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
    if (display) XCloseDisplay(display);
  }
};

struct Probe {
  Probe(const GlxWindow & window, const cc_glglue * glue, bool testS)
    : window(window), glue(glue), testS(testS)
  {
  }

  const GlxWindow & window;
  const cc_glglue * glue;
  const bool testS;
  const char * stage = nullptr;
  GLint expectedWrap = GL_REPEAT;
  GLint texture = 0;
  int producerVisits = 0;
  int consumerVisits = 0;
};

void checkContext(const Probe & probe)
{
  if (glXGetCurrentContext() != probe.window.context ||
      glXGetCurrentDrawable() != probe.window.drawable) {
    fail(probe.stage, "expected the consumer GLX context and drawable");
  }
}

void observeProducer(void * userdata, SoAction *)
{
  auto & probe = *static_cast<Probe *>(userdata);
  checkContext(probe);
  GLint framebuffer = 0;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &framebuffer);
  if (framebuffer == 0 ||
      cc_glglue_glCheckFramebufferStatus(probe.glue, GL_FRAMEBUFFER_EXT) !=
        GL_FRAMEBUFFER_COMPLETE_EXT) {
    fail(probe.stage, "expected a complete producer framebuffer");
  }
  checkGl(probe.stage);
  ++probe.producerVisits;
}

void observeTexture(void * userdata, SoAction *)
{
  auto & probe = *static_cast<Probe *>(userdata);
  checkContext(probe);

  GLint framebuffer = 0;
  GLint texture = 0;
  GLint width = 0;
  GLint height = 0;
  GLint storage = 0;
  GLint wrapS = 0;
  GLint wrapT = 0;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &framebuffer);
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &texture);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &storage);
  glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, &wrapS);
  glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, &wrapT);
  checkGl(probe.stage);

  if (framebuffer != 0) fail(probe.stage, "the consumer framebuffer was not restored");
  if (texture == 0 || width != textureSize || height != textureSize || storage != GL_RGBA8) {
    fail(probe.stage, "expected a bound 16x16 RGBA8 texture");
  }
  if (probe.texture == 0) probe.texture = texture;
  if (texture != probe.texture) fail(probe.stage, "the published texture object changed");

  // A storage allocation can discard the image while retaining the texture name.
  GLubyte pixels[textureSize * textureSize * 4] = {};
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
  checkGl(probe.stage);
  const GLubyte expectedPixel[] = {255, 0, 255, 255};
  if (std::memcmp(pixels, expectedPixel, sizeof(expectedPixel)) != 0) {
    fail(probe.stage, "the published texture lost the producer's clear color");
  }

  const GLint expectedS = probe.testS ? probe.expectedWrap : GL_REPEAT;
  const GLint expectedT = probe.testS ? GL_REPEAT : probe.expectedWrap;
  if (wrapS != expectedS || wrapT != expectedT) {
    std::fprintf(stderr, "%s: expected S=0x%x T=0x%x, got S=0x%x T=0x%x\n",
                 probe.stage, unsigned(expectedS), unsigned(expectedT),
                 unsigned(wrapS), unsigned(wrapT));
    std::exit(EXIT_FAILURE);
  }
  ++probe.consumerVisits;
}

void render(SoGLRenderAction & action, SoSeparator * root, Probe & probe,
            const char * stage, GLint expectedWrap, bool expectProducerVisit)
{
  probe.stage = stage;
  probe.expectedWrap = expectedWrap;
  const int consumerVisits = probe.consumerVisits;
  const int producerVisits = probe.producerVisits;
  action.apply(root);
  checkGl(stage);
  if (probe.consumerVisits != consumerVisits + 1) {
    fail(stage, "expected one consumer texture observation");
  }
  if (probe.producerVisits != producerVisits + int(expectProducerVisit)) {
    fail(stage, "unexpected number of producer traversals");
  }
}

int exercise(const GlxWindow & window, bool testS)
{
  const uint32_t cacheContext = SoGLCacheContextElement::getUniqueCacheContext();
  const cc_glglue * glue = cc_glglue_instance(cacheContext);
  checkGl("initializing Coin OpenGL glue");
  if (!SoGLDriverDatabase::isSupported(glue, SO_GL_FRAMEBUFFER_OBJECT)) {
    SoContextHandler::destructingContext(cacheContext);
    return skip("Coin FBO support is unavailable");
  }

  Probe probe{window, glue, testS};
  SoSeparator * root = new SoSeparator;
  root->ref();
  root->renderCaching = SoSeparator::OFF;

  SoComplexity * complexity = new SoComplexity;
  complexity->textureQuality = 0.25f;
  root->addChild(complexity);

  SoSceneTexture2 * texture = new SoSceneTexture2;
  texture->size.setValue(textureSize, textureSize);
  texture->type = SoSceneTexture2::RGBA8;
  texture->backgroundColor.setValue(1.0f, 0.0f, 1.0f, 1.0f);
  texture->wrapS = SoSceneTexture2::REPEAT;
  texture->wrapT = SoSceneTexture2::REPEAT;

  SoSeparator * producer = new SoSeparator;
  producer->renderCaching = SoSeparator::OFF;
  SoCallback * producerCallback = new SoCallback;
  producerCallback->setCallback(observeProducer, &probe);
  producer->addChild(producerCallback);
  texture->scene = producer;
  root->addChild(texture);

  SoCallback * consumerCallback = new SoCallback;
  consumerCallback->setCallback(observeTexture, &probe);
  root->addChild(consumerCallback);

  {
    SoGLRenderAction action(SbViewportRegion(32, 32));
    action.setCacheContext(cacheContext);
    action.setTransparencyType(SoGLRenderAction::NONE);

    render(action, root, probe, "initial RGBA8", GL_REPEAT, true);
    render(action, root, probe, "unchanged RGBA8", GL_REPEAT, false);

    SoSFEnum & wrap = testS ? texture->wrapS : texture->wrapT;
    wrap = SoSceneTexture2::CLAMP;
    render(action, root, probe, "first traversal after setting CLAMP", GL_CLAMP, false);
    render(action, root, probe, "unchanged CLAMP", GL_CLAMP, false);

    wrap = SoSceneTexture2::REPEAT;
    render(action, root, probe, "first traversal after restoring REPEAT", GL_REPEAT, false);

    texture->scene.touch();
    wrap = SoSceneTexture2::CLAMP;
    render(action, root, probe, "scene invalidated before wrap change", GL_CLAMP, true);

    wrap = SoSceneTexture2::REPEAT;
    texture->scene.touch();
    render(action, root, probe, "scene invalidated after wrap change", GL_REPEAT, true);
  }

  root->unref();
  SoContextHandler::destructingContext(cacheContext);
  checkGl("releasing Coin GL resources");
  return EXIT_SUCCESS;
}

int run(bool testS)
{
  GlxWindow window;
  if (!window.display) return skip("No X11 display");
  const int screen = DefaultScreen(window.display);
  int attributes[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, None};
  window.visual = glXChooseVisual(window.display, screen, attributes);
  if (!window.visual) return skip("No compatible GLX visual");

  window.context = glXCreateContext(window.display, window.visual, nullptr, True);
  if (!window.context) fail("GLX setup", "could not create a context for the selected visual");
  const Window parent = RootWindow(window.display, screen);
  window.colormap = XCreateColormap(window.display, parent, window.visual->visual, AllocNone);
  XSetWindowAttributes options = {};
  options.colormap = window.colormap;
  window.drawable = XCreateWindow(window.display, parent, 0, 0, 32, 32, 0,
                                  window.visual->depth, InputOutput, window.visual->visual,
                                  CWColormap, &options);
  if (!glXMakeCurrent(window.display, window.drawable, window.context)) {
    fail("GLX setup", "could not activate the consumer context");
  }

  return exercise(window, testS);
}

} // namespace

int main(int argc, char ** argv)
{
  if (argc != 2 || (std::strcmp(argv[1], "S") != 0 && std::strcmp(argv[1], "T") != 0)) {
    fail("arguments", "usage: CoinSceneTextureWrapTest S|T");
  }

  SoDB::init();
  const int status = run(std::strcmp(argv[1], "S") == 0);
  SoDB::finish();
  return status;
}
