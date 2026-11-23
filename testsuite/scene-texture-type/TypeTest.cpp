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
constexpr int skipStatus = 77;

struct TextureFormat {
  SoSceneTexture2::Type type;
  GLenum storage;
};

constexpr TextureFormat rgba8{SoSceneTexture2::RGBA8, GL_RGBA8};
constexpr TextureFormat rgba16f{SoSceneTexture2::RGBA16F, GL_RGBA16F_ARB};
constexpr TextureFormat depth{SoSceneTexture2::DEPTH, GL_DEPTH_COMPONENT};
constexpr TextureFormat rgba4{SoSceneTexture2::RGBA4, GL_RGBA4};

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

// Probe driver support independently of the SoSceneTexture2 allocation path.
bool supportsColorFormat(const cc_glglue * glue, GLenum internalFormat)
{
  GLuint texture = 0;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, textureSize, textureSize,
               0, GL_RGBA, GL_FLOAT, nullptr);
  GLint storage = 0;
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &storage);
  const GLenum allocationError = glGetError();

  GLuint framebuffer = 0;
  GLuint depthBuffer = 0;
  cc_glglue_glGenFramebuffers(glue, 1, &framebuffer);
  cc_glglue_glGenRenderbuffers(glue, 1, &depthBuffer);
  cc_glglue_glBindFramebuffer(glue, GL_FRAMEBUFFER_EXT, framebuffer);
  cc_glglue_glBindRenderbuffer(glue, GL_RENDERBUFFER_EXT, depthBuffer);
  cc_glglue_glRenderbufferStorage(glue, GL_RENDERBUFFER_EXT,
                                 GL_DEPTH_COMPONENT24, textureSize, textureSize);
  cc_glglue_glFramebufferTexture2D(glue, GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                                   GL_TEXTURE_2D, texture, 0);
  cc_glglue_glFramebufferRenderbuffer(glue, GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
                                      GL_RENDERBUFFER_EXT, depthBuffer);
  const GLenum status = cc_glglue_glCheckFramebufferStatus(glue, GL_FRAMEBUFFER_EXT);

  cc_glglue_glBindFramebuffer(glue, GL_FRAMEBUFFER_EXT, 0);
  cc_glglue_glBindRenderbuffer(glue, GL_RENDERBUFFER_EXT, 0);
  cc_glglue_glDeleteFramebuffers(glue, 1, &framebuffer);
  cc_glglue_glDeleteRenderbuffers(glue, 1, &depthBuffer);
  glBindTexture(GL_TEXTURE_2D, 0);
  glDeleteTextures(1, &texture);
  checkGl("probing driver format support");

  return allocationError == GL_NO_ERROR && storage == GLint(internalFormat) &&
         status == GL_FRAMEBUFFER_COMPLETE_EXT;
}

struct Probe {
  Probe(const GlxWindow & window, const cc_glglue * glue) : window(window), glue(glue)
  {
  }

  const GlxWindow & window;
  const cc_glglue * glue;
  const char * stage = nullptr;
  GLenum expectedStorage = 0;
  int producerVisits = 0;
  int readbacks = 0;
};

void observeProducer(void * userdata, SoAction *)
{
  auto & probe = *static_cast<Probe *>(userdata);
  GLint framebuffer = 0;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &framebuffer);
  if (glXGetCurrentContext() != probe.window.context ||
      glXGetCurrentDrawable() != probe.window.drawable || framebuffer == 0) {
    fail("The producer did not render into a consumer-context FBO");
  }
  if (cc_glglue_glCheckFramebufferStatus(probe.glue, GL_FRAMEBUFFER_EXT) !=
      GL_FRAMEBUFFER_COMPLETE_EXT) {
    fail("The producer framebuffer is incomplete");
  }
  checkGl(probe.stage);
  ++probe.producerVisits;
}

void readTexture(void * userdata, SoAction *)
{
  auto & probe = *static_cast<Probe *>(userdata);
  if (glXGetCurrentContext() != probe.window.context ||
      glXGetCurrentDrawable() != probe.window.drawable) {
    fail("The consumer GLX context was not restored");
  }

  GLint framebuffer = 0;
  GLint texture = 0;
  GLint width = 0;
  GLint height = 0;
  GLint storage = 0;
  GLint depthBits = 0;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &framebuffer);
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &texture);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &storage);
  if (probe.expectedStorage == GL_DEPTH_COMPONENT) {
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_DEPTH_SIZE, &depthBits);
  }
  checkGl(probe.stage);

  if (framebuffer != 0) fail("The consumer framebuffer was not restored");
  if (texture == 0 || width != textureSize || height != textureSize) {
    std::fprintf(stderr, "%s: expected a bound %dx%d texture, got binding %d of size %dx%d\n",
                 probe.stage, textureSize, textureSize, texture, width, height);
    std::exit(EXIT_FAILURE);
  }
  const bool matches = probe.expectedStorage == GL_DEPTH_COMPONENT ?
    depthBits > 0 : storage == GLint(probe.expectedStorage);
  if (!matches) {
    std::fprintf(stderr, "%s: expected storage 0x%x, got 0x%x (%d depth bits queried)\n",
                 probe.stage, unsigned(probe.expectedStorage), unsigned(storage), depthBits);
    std::exit(EXIT_FAILURE);
  }
  ++probe.readbacks;
}

void render(SoGLRenderAction & action, SoSeparator * root, Probe & probe,
            const char * stage, GLenum expectedStorage, bool expectProducerVisit)
{
  probe.stage = stage;
  probe.expectedStorage = expectedStorage;
  const int readbacks = probe.readbacks;
  const int producerVisits = probe.producerVisits;
  action.apply(root);
  checkGl(stage);
  if (probe.readbacks != readbacks + 1) fail("Expected one consumer texture observation");
  if (probe.producerVisits != producerVisits + int(expectProducerVisit)) {
    std::fprintf(stderr, "%s: unexpected number of producer traversals\n", stage);
    std::exit(EXIT_FAILURE);
  }
}

int exercise(const GlxWindow & window, const TextureFormat & requested)
{
  const uint32_t cacheContext = SoGLCacheContextElement::getUniqueCacheContext();
  const cc_glglue * glue = cc_glglue_instance(cacheContext);
  checkGl("initializing Coin OpenGL glue");
  if (!SoGLDriverDatabase::isSupported(glue, SO_GL_FRAMEBUFFER_OBJECT)) {
    SoContextHandler::destructingContext(cacheContext);
    return skip("Coin FBO support is unavailable");
  }
  if (requested.type != SoSceneTexture2::DEPTH && !supportsColorFormat(glue, requested.storage)) {
    SoContextHandler::destructingContext(cacheContext);
    return skip("The requested color format is unavailable for framebuffer rendering");
  }

  Probe probe{window, glue};
  SoSeparator * root = new SoSeparator;
  root->ref();
  root->renderCaching = SoSeparator::OFF;

  SoComplexity * complexity = new SoComplexity;
  complexity->textureQuality = 0.25f;
  root->addChild(complexity);

  SoSceneTexture2 * texture = new SoSceneTexture2;
  texture->size.setValue(textureSize, textureSize);
  texture->type = rgba8.type;
  SoSeparator * producer = new SoSeparator;
  producer->renderCaching = SoSeparator::OFF;
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
    render(action, root, probe, "initial RGBA8", rgba8.storage, true);
    render(action, root, probe, "unchanged RGBA8", rgba8.storage, false);

    texture->type = requested.type;
    render(action, root, probe, "first traversal after changing type", requested.storage, true);
    render(action, root, probe, "unchanged requested type", requested.storage, false);

    texture->type = rgba8.type;
    render(action, root, probe, "first traversal after restoring RGBA8", rgba8.storage, true);
    render(action, root, probe, "unchanged restored RGBA8", rgba8.storage, false);
  }

  root->unref();
  SoContextHandler::destructingContext(cacheContext);
  checkGl("releasing Coin GL resources");
  return EXIT_SUCCESS;
}

int run(const TextureFormat & requested)
{
  GlxWindow window;
  if (!window.display) return skip("No X11 display");
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

  return exercise(window, requested);
}

} // namespace

int main(int argc, char ** argv)
{
  const TextureFormat * requested = nullptr;
  if (argc == 2) {
    if (std::strcmp(argv[1], "rgba16f") == 0) requested = &rgba16f;
    if (std::strcmp(argv[1], "depth") == 0) requested = &depth;
    if (std::strcmp(argv[1], "rgba4") == 0) requested = &rgba4;
  }
  if (!requested) fail("Usage: CoinSceneTextureTypeTest rgba16f|depth|rgba4");

  SoDB::init();
  const int status = run(*requested);
  SoDB::finish();
  return status;
}
