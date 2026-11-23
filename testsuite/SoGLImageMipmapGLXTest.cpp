#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <Inventor/SoDB.h>
#include <Inventor/SbName.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/elements/SoMultiTextureEnabledElement.h>
#include <Inventor/elements/SoGLDisplayList.h>
#include <Inventor/misc/SoGLDriverDatabase.h>
#include <Inventor/misc/SoGLImage.h>
#include <Inventor/misc/SoContextHandler.h>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

#define CHECK(e) do { if (!(e)) { std::fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, #e); std::exit(1); } } while (0)

namespace {
struct Size { int x, y, z; };
size_t count(Size s, int nc) { return size_t(s.x) * s.y * s.z * nc; }

// Coordinate-based oracle: deliberately out-of-place and independent of the
// production pointer/stride traversal. One active axis historically truncates;
// two or three active axes round to nearest (ties upward).
std::vector<unsigned char> reduce(const std::vector<unsigned char> & src,
                                  Size s, int nc, Size & next) {
  next = Size{std::max(1, s.x / 2), std::max(1, s.y / 2), std::max(1, s.z / 2)};
  const int dx = s.x > 1 ? 2 : 1, dy = s.y > 1 ? 2 : 1, dz = s.z > 1 ? 2 : 1;
  const int samples = dx * dy * dz;
  std::vector<unsigned char> out(count(next, nc));
  for (int z = 0; z < next.z; ++z)
    for (int y = 0; y < next.y; ++y)
      for (int x = 0; x < next.x; ++x)
        for (int c = 0; c < nc; ++c) {
          unsigned sum = 0;
          for (int k = 0; k < dz; ++k)
            for (int j = 0; j < dy; ++j)
              for (int i = 0; i < dx; ++i) {
                const int sx = x * dx + i, sy = y * dy + j, sz = z * dz + k;
                sum += src[((sz * s.y + sy) * s.x + sx) * nc + c];
              }
          out[((z * next.y + y) * next.x + x) * nc + c] =
            static_cast<unsigned char>((sum + (samples > 2 ? samples / 2 : 0)) / samples);
        }
  return out;
}

std::vector<unsigned char> pattern(Size s, int nc, int seed) {
  std::vector<unsigned char> out(count(s, nc));
  unsigned value = 0x1234567u + unsigned(seed) * 0x76543u;
  for (size_t i = 0; i < out.size(); ++i) {
    value = value * 1664525u + 1013904223u;
    out[i] = static_cast<unsigned char>(value >> 24);
  }
  return out;
}

// Creation may delegate the 2D chain to GL. Use a separate texture as the
// reference for that case: drivers need not round like Coin's CPU filter.
// Subimage updates always use Coin's software chain and retain the CPU oracle.
GLuint driverReference(Size s, int nc, const std::vector<unsigned char> & pixels) {
  const cc_glglue * glue = cc_glglue_instance(915);
  const bool sgis = SoGLDriverDatabase::isSupported(glue, SbName("GL_SGIS_generate_mipmap"));
  const bool generate = SoGLDriverDatabase::isSupported(glue, SbName(SO_GL_GENERATE_MIPMAP));
  if (!sgis && !generate) return 0;
  const GLenum formats[] = {0, GL_LUMINANCE, GL_LUMINANCE_ALPHA, GL_RGB, GL_RGBA};
  const GLint internal[] = {0, GL_LUMINANCE8, GL_LUMINANCE8_ALPHA8, GL_RGB8, GL_RGBA8};
  GLint saved = 0; glGetIntegerv(GL_TEXTURE_BINDING_2D, &saved);
  GLuint id = 0; glGenTextures(1, &id); glBindTexture(GL_TEXTURE_2D, id);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  if (sgis) glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
  glTexImage2D(GL_TEXTURE_2D, 0, internal[nc], s.x, s.y, 0, formats[nc], GL_UNSIGNED_BYTE, &pixels[0]);
  if (!sgis) {
    PFNGLGENERATEMIPMAPPROC func = reinterpret_cast<PFNGLGENERATEMIPMAPPROC>(
      glXGetProcAddressARB(reinterpret_cast<const GLubyte *>("glGenerateMipmap")));
    if (!func) func = reinterpret_cast<PFNGLGENERATEMIPMAPPROC>(
      glXGetProcAddressARB(reinterpret_cast<const GLubyte *>("glGenerateMipmapEXT")));
    CHECK(func); func(GL_TEXTURE_2D);
  }
  CHECK(glGetError() == GL_NO_ERROR);
  glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(saved));
  return id;
}

void verify(GLenum target, Size initial, int nc,
            const std::vector<unsigned char> & input, const char * stage, GLuint reference) {
  const GLenum formats[] = {0, GL_LUMINANCE, GL_LUMINANCE_ALPHA, GL_RGB, GL_RGBA};
  Size s = initial;
  std::vector<unsigned char> expected = input;
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  for (int level = 0;; ++level) {
    GLint w = 0, h = 0, d = 1;
    glGetTexLevelParameteriv(target, level, GL_TEXTURE_WIDTH, &w);
    glGetTexLevelParameteriv(target, level, GL_TEXTURE_HEIGHT, &h);
    if (target == GL_TEXTURE_3D) glGetTexLevelParameteriv(target, level, GL_TEXTURE_DEPTH, &d);
    CHECK(glGetError() == GL_NO_ERROR);
    if (w != s.x || h != s.y || d != s.z) {
      std::fprintf(stderr, "%s %dx%dx%d nc=%d level=%d size=%dx%dx%d expected=%dx%dx%d\n",
                   stage, initial.x, initial.y, initial.z, nc, level, w,h,d, s.x,s.y,s.z);
      CHECK(false);
    }
    // Padding catches an unexpectedly large GL readback, independently of ASan.
    std::vector<unsigned char> actual(expected.size() + 32, 0xa5);
    glGetTexImage(target, level, formats[nc], GL_UNSIGNED_BYTE, &actual[0]);
    CHECK(glGetError() == GL_NO_ERROR);
    for (size_t i = 0; i < expected.size(); ++i) {
      if (actual[i] != expected[i]) {
        std::fprintf(stderr, "%s %dx%dx%d nc=%d level=%d byte=%lu actual=%u expected=%u\n",
                     stage, initial.x,initial.y,initial.z,nc,level,
                     static_cast<unsigned long>(i), unsigned(actual[i]),unsigned(expected[i]));
        CHECK(false);
      }
    }
    for (size_t i = expected.size(); i < actual.size(); ++i) CHECK(actual[i] == 0xa5);
    if (s.x == 1 && s.y == 1 && s.z == 1) {
      glGetTexLevelParameteriv(target, level + 1, GL_TEXTURE_WIDTH, &w);
      CHECK(w == 0); CHECK(glGetError() == GL_NO_ERROR);
      break;
    }
    Size next;
    expected = reduce(expected, s, nc, next); s = next;
    if (reference) {
      GLint saved = 0; glGetIntegerv(GL_TEXTURE_BINDING_2D, &saved);
      glBindTexture(GL_TEXTURE_2D, reference);
      glGetTexImage(GL_TEXTURE_2D, level + 1, formats[nc], GL_UNSIGNED_BYTE, &expected[0]);
      CHECK(glGetError() == GL_NO_ERROR);
      glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(saved));
    }
  }
}

void exercise(SoState * state, Size s, int nc, bool volume) {
  const GLenum target = volume ? GL_TEXTURE_3D : GL_TEXTURE_2D;
  if (volume) SoMultiTextureEnabledElement::enableTexture3(state, NULL);
  else SoMultiTextureEnabledElement::set(state, NULL, 0, TRUE);
  CHECK(SoMultiTextureEnabledElement::getMode(state) ==
        (volume ? SoMultiTextureEnabledElement::TEXTURE3D : SoMultiTextureEnabledElement::TEXTURE2D));
  SoGLImage * image = new SoGLImage;
  image->setFlags(0); // Force mipmaps, independent of quality environment defaults.
  unsigned int texture = 0;
  for (int update = 0; update < 3; ++update) {
    std::vector<unsigned char> pixels = pattern(s, nc, update + nc * 11);
    const std::vector<unsigned char> original = pixels;
    const GLuint reference = !volume && update == 0 ? driverReference(s, nc, pixels) : 0;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    if (volume) image->setData(&pixels[0], SbVec3s(s.x,s.y,s.z), nc,
                               SoGLImage::REPEAT, SoGLImage::REPEAT, SoGLImage::REPEAT, 1.0f, 0, state);
    else image->setData(&pixels[0], SbVec2s(s.x,s.y), nc,
                        SoGLImage::REPEAT, SoGLImage::REPEAT, 1.0f, 0, state);
    CHECK(pixels == original);
    CHECK(glGetError() == GL_NO_ERROR);
    SoGLDisplayList * dl = image->getGLDisplayList(state);
    CHECK(dl && dl->isMipMapTextureObject());
    if (update == 0) texture = dl->getFirstIndex();
    else CHECK(dl->getFirstIndex() == texture); // Exercise subimage updates of the existing object.
    dl->call(state);
    CHECK(dl->getTextureTarget() == int(target));
    verify(target, s, nc, pixels, update ? "update" : "create", reference);
    if (reference) glDeleteTextures(1, &reference);
  }
  image->unref(state);
  CHECK(glGetError() == GL_NO_ERROR);
}

void run(void *, SoAction * action) {
  CHECK(action->isOfType(SoGLRenderAction::getClassTypeId()));
  SoState * state = action->getState();
  const Size volumes[] = {
    {1,8,4}, {8,1,4}, {8,4,1}, // initial planes, all axis orientations
    {1,1,16}, {1,16,1}, {16,1,1}, // initial lines, all orientations
    {1,1,1}, {2,2,2}, {8,8,8},
    {2,8,4}, {8,2,4}, {8,4,2}, // become planes and then lines in-place
    {2,2,16}, {2,16,2}, {16,2,2},
    {16,8,4}, {4,16,8}, {8,4,16}
  };
  // A large prior volume poisons the reusable scratch buffer with other texels;
  // degenerate outputs cannot pass by accidentally reading zero-filled memory.
  for (int nc = 1; nc <= 4; ++nc) {
    exercise(state, Size{16,16,16}, nc, true);
    for (size_t i = 0; i < sizeof(volumes)/sizeof(volumes[0]); ++i)
      exercise(state, volumes[i], nc, true);
    const Size flat[] = {{8,8,1}, {16,4,1}, {4,16,1}, {1,16,1}, {16,1,1}, {1,1,1}};
    for (size_t i = 0; i < sizeof(flat)/sizeof(flat[0]); ++i)
      exercise(state, flat[i], nc, false);
  }
}
}

int main() {
  // Options must precede SoDB initialization. This executable is process isolated.
  setenv("COIN_TEX2_USE_GLTEXSUBIMAGE", "1", 1);
  setenv("COIN_TEX2_USE_SGIS_GENERATE_MIPMAP", "0", 1);
  setenv("COIN_GLERROR_DEBUGGING", "0", 1); // Keep errors visible to this test's checks.
  Display * display = XOpenDisplay(NULL);
  if (!display) { std::puts("SKIP: no X display"); return 77; }
  int attrs[] = {GLX_RGBA, GLX_DEPTH_SIZE, 16, None};
  XVisualInfo * visual = glXChooseVisual(display, DefaultScreen(display), attrs);
  if (!visual) { XCloseDisplay(display); std::puts("SKIP: no GLX RGBA visual"); return 77; }
  GLXContext context = glXCreateContext(display, visual, NULL, True);
  if (!context) { XFree(visual); XCloseDisplay(display); std::puts("SKIP: no GLX context"); return 77; }
  Colormap colormap = XCreateColormap(display, RootWindow(display, visual->screen), visual->visual, AllocNone);
  XSetWindowAttributes wa = {}; wa.colormap = colormap;
  Window window = XCreateWindow(display, RootWindow(display, visual->screen), 0,0,16,16,0,
                                visual->depth, InputOutput, visual->visual, CWColormap, &wa);
  if (!glXMakeCurrent(display, window, context)) {
    glXDestroyContext(display, context);
    XDestroyWindow(display, window); XFreeColormap(display, colormap);
    XFree(visual); XCloseDisplay(display);
    std::puts("SKIP: GLX context cannot be made current"); return 77;
  }
  GLint max3d = 0; glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &max3d);
  if (glGetError() != GL_NO_ERROR || max3d < 16) {
    std::puts("SKIP: 3D textures unavailable"); return 77;
  }
  SoDB::init();
  {
    SoGLRenderAction action(SbViewportRegion(16,16));
    action.setCacheContext(915);
    SoSeparator * root = new SoSeparator; root->ref(); root->renderCaching = SoSeparator::OFF;
    SoCallback * callback = new SoCallback; callback->setCallback(run, NULL); root->addChild(callback);
    action.apply(root);
    root->unref();
    SoContextHandler::destructingContext(915);
  }
  SoDB::finish();
  glXMakeCurrent(display, None, NULL); glXDestroyContext(display, context);
  XDestroyWindow(display, window); XFreeColormap(display, colormap);
  XFree(visual); XCloseDisplay(display);
  std::puts("PASS: 3D degenerate mipmaps and 2D controls, all components, create and update");
  return 0;
}
