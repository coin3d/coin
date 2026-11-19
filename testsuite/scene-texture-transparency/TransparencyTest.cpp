#include <Inventor/SbColor.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/SoDB.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoGLMultiTextureImageElement.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/misc/SoContextHandler.h>
#include <Inventor/misc/SoGLDriverDatabase.h>
#include <Inventor/misc/SoGLImage.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/nodes/SoSceneTexture2.h>
#include <Inventor/nodes/SoSeparator.h>

#include <GL/glx.h>
#include <X11/Xlib.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace {

constexpr short textureSize = 16;
constexpr uint32_t policyMask = SoGLImage::FORCE_TRANSPARENCY_TRUE |
                                SoGLImage::FORCE_TRANSPARENCY_FALSE |
                                SoGLImage::FORCE_ALPHA_TEST_TRUE |
                                SoGLImage::FORCE_ALPHA_TEST_FALSE;
using Pixel = std::array<GLubyte, 4>;
constexpr Pixel initialPixel = {255, 0, 255, 0};
constexpr Pixel updatedPixel = {0, 255, 0, 255};

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
    std::fprintf(stderr, "%s: OpenGL error 0x%x\n", stage, static_cast<unsigned>(error));
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
  Probe(const GlxWindow & window, const cc_glglue * glue)
    : window(window), glue(glue)
  {
  }

  const GlxWindow & window;
  const cc_glglue * glue;
  const char * stage = nullptr;
  SoSceneTexture2::TransparencyFunction policy = SoSceneTexture2::NONE;
  Pixel scenePixel = initialPixel;
  GLint textureName = 0;
  uint32_t otherFlags = 0;
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

  const Pixel & pixel = probe.scenePixel;
  GLfloat previousClearColor[4];
  glGetFloatv(GL_COLOR_CLEAR_VALUE, previousClearColor);
  glClearColor(pixel[0] / 255.0f, pixel[1] / 255.0f, pixel[2] / 255.0f, pixel[3] / 255.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(previousClearColor[0], previousClearColor[1],
               previousClearColor[2], previousClearColor[3]);
  checkGl(probe.stage);
  ++probe.producerVisits;
}

void observeTexture(void * userdata, SoAction * action)
{
  auto & probe = *static_cast<Probe *>(userdata);
  checkContext(probe);
  SoState * state = action->getState();
  SoMultiTextureImageElement::Model model;
  SbColor blendColor;
  SoGLImage * image = SoGLMultiTextureImageElement::get(state, 0, model, blendColor);
  if (!image) fail(probe.stage, "expected a published texture image on unit zero");

  const bool transparent = probe.policy != SoSceneTexture2::NONE;
  const bool alphaTest = probe.policy == SoSceneTexture2::ALPHA_TEST;
  const uint32_t expectedFlags =
    (transparent ? SoGLImage::FORCE_TRANSPARENCY_TRUE : SoGLImage::FORCE_TRANSPARENCY_FALSE) |
    (alphaTest ? SoGLImage::FORCE_ALPHA_TEST_TRUE : SoGLImage::FORCE_ALPHA_TEST_FALSE);
  const uint32_t flags = image->getFlags();
  if ((flags & policyMask) != expectedFlags) {
    std::fprintf(stderr, "%s: expected policy flags 0x%x, got 0x%x\n", probe.stage,
                 static_cast<unsigned>(expectedFlags), static_cast<unsigned>(flags & policyMask));
    std::exit(EXIT_FAILURE);
  }
  const bool styleTransparent =
    (SoShapeStyleElement::get(state)->getFlags() & SoShapeStyleElement::TRANSP_TEXTURE) != 0;
  if ((SoGLMultiTextureImageElement::hasTransparency(state) != FALSE) != transparent ||
      (image->useAlphaTest() != FALSE) != alphaTest ||
      styleTransparent != transparent) {
    fail(probe.stage, "unexpected effective transparency or alpha-test classification");
  }

  GLint framebuffer = 0;
  GLint texture = 0;
  GLint width = 0;
  GLint height = 0;
  GLint format = 0;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &framebuffer);
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &texture);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &format);
  checkGl(probe.stage);
  if (framebuffer != 0) fail(probe.stage, "the consumer framebuffer was not restored");
  if (texture == 0 || width != textureSize || height != textureSize || format != GL_RGBA8) {
    fail(probe.stage, "expected a bound 16x16 RGBA8 texture");
  }

  if (probe.consumerVisits == 0) {
    probe.textureName = texture;
    // Give preservation an unrelated flag to track in addition to the image defaults.
    image->setFlags(flags | SoGLImage::INVINCIBLE);
    probe.otherFlags = image->getFlags() & ~policyMask;
  } else {
    if (texture != probe.textureName) fail(probe.stage, "the published texture object changed");
    if ((flags & ~policyMask) != probe.otherFlags) fail(probe.stage, "unrelated image flags changed");
  }

  std::array<Pixel, textureSize * textureSize> pixels = {};
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
  checkGl(probe.stage);
  if (!std::all_of(pixels.begin(), pixels.end(),
                   [&probe](const Pixel & pixel) { return pixel == probe.scenePixel; })) {
    fail(probe.stage, "unexpected producer pixels in the published texture");
  }
  ++probe.consumerVisits;
}

void render(SoGLRenderAction & action, SoSeparator * root, Probe & probe,
            const char * stage, SoSceneTexture2::TransparencyFunction policy,
            bool expectProducerVisit)
{
  probe.stage = stage;
  probe.policy = policy;
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

int exercise(const GlxWindow & window, bool changeScene)
{
  const uint32_t cacheContext = SoGLCacheContextElement::getUniqueCacheContext();
  const cc_glglue * glue = cc_glglue_instance(cacheContext);
  checkGl("initializing Coin OpenGL glue");
  if (!SoGLDriverDatabase::isSupported(glue, SO_GL_FRAMEBUFFER_OBJECT)) {
    SoContextHandler::destructingContext(cacheContext);
    return skip("Coin FBO support is unavailable");
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
  texture->type = SoSceneTexture2::RGBA8;

  SoCallback * producer = new SoCallback;
  producer->setCallback(observeProducer, &probe);
  texture->scene = producer;
  root->addChild(texture);

  SoCallback * consumer = new SoCallback;
  consumer->setCallback(observeTexture, &probe);
  root->addChild(consumer);

  {
    SoGLRenderAction action(SbViewportRegion(32, 32));
    action.setCacheContext(cacheContext);
    action.setTransparencyType(SoGLRenderAction::NONE);

    render(action, root, probe, "initial NONE", SoSceneTexture2::NONE, true);
    if (!changeScene) {
      texture->transparencyFunction = SoSceneTexture2::ALPHA_TEST;
      render(action, root, probe, "NONE to ALPHA_TEST", SoSceneTexture2::ALPHA_TEST, false);
      render(action, root, probe, "unchanged ALPHA_TEST", SoSceneTexture2::ALPHA_TEST, false);

      texture->transparencyFunction = SoSceneTexture2::ALPHA_BLEND;
      render(action, root, probe, "ALPHA_TEST to ALPHA_BLEND", SoSceneTexture2::ALPHA_BLEND, false);

      texture->transparencyFunction = SoSceneTexture2::NONE;
      render(action, root, probe, "ALPHA_BLEND to NONE", SoSceneTexture2::NONE, false);
    } else {
      probe.scenePixel = updatedPixel;
      texture->scene.touch();
      texture->transparencyFunction = SoSceneTexture2::ALPHA_TEST;
      render(action, root, probe, "scene change before ALPHA_TEST", SoSceneTexture2::ALPHA_TEST, true);

      texture->transparencyFunction = SoSceneTexture2::ALPHA_BLEND;
      probe.scenePixel = initialPixel;
      texture->scene.touch();
      render(action, root, probe, "scene change after ALPHA_BLEND", SoSceneTexture2::ALPHA_BLEND, true);
      render(action, root, probe, "unchanged scene and ALPHA_BLEND", SoSceneTexture2::ALPHA_BLEND, false);
    }
  }

  root->unref();
  SoContextHandler::destructingContext(cacheContext);
  checkGl("releasing Coin GL resources");
  return EXIT_SUCCESS;
}

int run(bool changeScene)
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

  return exercise(window, changeScene);
}

} // namespace

int main(int argc, char ** argv)
{
  if (argc != 2 ||
      (std::strcmp(argv[1], "policy") != 0 && std::strcmp(argv[1], "scene") != 0)) {
    fail("arguments", "usage: CoinSceneTextureTransparencyTest policy|scene");
  }

  SoDB::init();
  const int status = run(std::strcmp(argv[1], "scene") == 0);
  SoDB::finish();
  return status;
}
