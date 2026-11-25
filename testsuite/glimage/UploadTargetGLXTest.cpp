#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <Inventor/SoDB.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoGLDisplayList.h>
#include <Inventor/elements/SoMultiTextureEnabledElement.h>
#include <Inventor/misc/SoContextHandler.h>
#include <Inventor/misc/SoGLImage.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTexture3.h>
#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/elements/SoTextureQualityElement.h>
#include <Inventor/nodes/SoTextureUnit.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace {
const char * testname;
void require(bool condition, const char * text, int line) {
  if (!condition) {
    std::fprintf(stderr, "%s:%d: %s\n", testname, line, text);
    std::exit(1);
  }
}
#define CHECK(x) require((x), #x, __LINE__)
void noError(const char * phase) {
  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    std::fprintf(stderr, "%s: GL error 0x%x at %s\n", testname, error, phase);
    std::exit(1);
  }
}
void active(int unit) {
  PFNGLACTIVETEXTUREPROC fn = reinterpret_cast<PFNGLACTIVETEXTUREPROC>(
    glXGetProcAddressARB(reinterpret_cast<const GLubyte *>("glActiveTexture")));
  CHECK(fn != NULL);
  fn(GL_TEXTURE0 + unit);
}
GLenum bindingFor(GLenum target) {
  if (target == GL_TEXTURE_3D) return GL_TEXTURE_BINDING_3D;
  if (target == GL_TEXTURE_RECTANGLE_ARB) return GL_TEXTURE_BINDING_RECTANGLE_ARB;
  return GL_TEXTURE_BINDING_2D;
}
std::vector<unsigned char> pixels(int w, int h, int d, bool mipmap) {
  std::vector<unsigned char> data(w * h * (d ? d : 1) * 4);
  for (size_t i = 0; i < data.size(); ++i)
    data[i] = mipmap ? 96 : static_cast<unsigned char>(17 + (i * 37) % 211);
  return data;
}
void storage(GLenum target, int w, int h, int d, bool mipmap) {
  noError("before storage inspection (includes upload)");
  GLint name = 0;
  glGetIntegerv(bindingFor(target), &name);
  CHECK(name != 0);
  CHECK(glIsTexture(static_cast<GLuint>(name)) == GL_TRUE);
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  for (int level = 0;; ++level) {
    GLint width = -1, height = -1, depth = -1;
    glGetTexLevelParameteriv(target, level, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(target, level, GL_TEXTURE_HEIGHT, &height);
    if (d) glGetTexLevelParameteriv(target, level, GL_TEXTURE_DEPTH, &depth);
    noError("dimensions");
    CHECK(width == w);
    CHECK(height == h);
    if (d) CHECK(depth == d);
    std::vector<unsigned char> expected = pixels(w, h, d, mipmap);
    std::vector<unsigned char> actual(expected.size(), 0);
    glGetTexImage(target, level, GL_RGBA, GL_UNSIGNED_BYTE, &actual[0]);
    noError("texel readback");
    CHECK(actual == expected);
    if (!mipmap || (w == 1 && h == 1 && d <= 1)) {
      // A non-mipmapped object must not acquire levels as a side effect.
      // Rectangle textures only admit level zero.
      if (target != GL_TEXTURE_RECTANGLE_ARB) {
        glGetTexLevelParameteriv(target, level + 1, GL_TEXTURE_WIDTH, &width);
        CHECK(width == 0);
      }
      break;
    }
    w = w > 1 ? w / 2 : 1;
    h = h > 1 ? h / 2 : 1;
    if (d) d = d > 1 ? d / 2 : 1;
  }
  noError("end storage inspection");
}

struct Case {
  int depth;
  bool mipmap;
  bool rectangle;
  int mode;
  bool eager;
};
void logicalMode(SoState * state, int mode) {
  switch (mode) {
  case SoMultiTextureEnabledElement::DISABLED:
    SoMultiTextureEnabledElement::set(state, NULL, 0, FALSE); break;
  case SoMultiTextureEnabledElement::TEXTURE2D:
    SoMultiTextureEnabledElement::set(state, NULL, 0, TRUE); break;
  case SoMultiTextureEnabledElement::TEXTURE3D:
    SoMultiTextureEnabledElement::enableTexture3(state, NULL, 0); break;
  case SoMultiTextureEnabledElement::RECTANGLE:
    SoMultiTextureEnabledElement::enableRectangle(state, NULL, 0); break;
  case SoMultiTextureEnabledElement::CUBEMAP:
    SoMultiTextureEnabledElement::enableCubeMap(state, NULL, 0); break;
  }
  CHECK(SoMultiTextureEnabledElement::getMode(state, 0) == mode);
}
void direct(void * closure, SoAction * base) {
  if (!base->isOfType(SoGLRenderAction::getClassTypeId())) return;
  const Case & c = *static_cast<Case *>(closure);
  SoState * state = base->getState();
  logicalMode(state, c.mode);
  noError("before fresh direct image");
  const int w = 4, h = 4;
  std::vector<unsigned char> data = pixels(w, h, c.depth, c.mipmap);
  SoGLImage * image = new SoGLImage;
  // Quality controls the normal path; explicit flags exercise filter/wrap
  // preservation and the valid rectangle path without requesting its mips.
  if (c.rectangle) image->setFlags(SoGLImage::RECTANGLE | SoGLImage::NO_MIPMAP |
                                  SoGLImage::LINEAR_MAG_FILTER | SoGLImage::LINEAR_MIN_FILTER);
  SoState * create = c.eager ? state : NULL;
  const float quality = c.mipmap ? 1.0f : 0.4f;
  SoTextureQualityElement::set(state, quality);
  if (c.depth)
    image->setData(&data[0], SbVec3s(w,h,c.depth), 4, SoGLImage::CLAMP_TO_EDGE,
                   SoGLImage::CLAMP_TO_EDGE, SoGLImage::CLAMP_TO_EDGE, quality, 0, create);
  else
    image->setData(&data[0], SbVec2s(w,h), 4, SoGLImage::CLAMP_TO_EDGE,
                   SoGLImage::CLAMP_TO_EDGE, quality, 0, create);
  SoGLDisplayList * dl = image->getGLDisplayList(state);
  CHECK(dl != NULL);
  const GLenum target = c.depth ? GL_TEXTURE_3D :
    (c.rectangle ? GL_TEXTURE_RECTANGLE_ARB : GL_TEXTURE_2D);
  CHECK(dl->getType() == SoGLDisplayList::TEXTURE_OBJECT);
  CHECK(dl->getTextureTarget() == static_cast<int>(target));
  dl->call(state);
  storage(target, w, h, c.depth, c.mipmap);
  GLint filter = 0, wrap = 0;
  glGetTexParameteriv(target, GL_TEXTURE_MIN_FILTER, &filter);
  CHECK(filter == (c.mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR));
  glGetTexParameteriv(target, GL_TEXTURE_WRAP_S, &wrap);
  CHECK(wrap == GL_CLAMP_TO_EDGE);
  glGetTexParameteriv(target, GL_TEXTURE_WRAP_T, &wrap);
  CHECK(wrap == GL_CLAMP_TO_EDGE);
  if (c.depth) {
    glGetTexParameteriv(target, GL_TEXTURE_WRAP_R, &wrap);
    CHECK(wrap == GL_CLAMP_TO_EDGE);
  }
  noError("filter and wrap");
  image->unref(state);
}
struct Inspect {
  int unit;
  int depth;
  bool mipmap;
  int calls;
  Inspect(int u, int d, bool m) : unit(u), depth(d), mipmap(m), calls(0) {}
};
void inspect(void * closure, SoAction * base) {
  if (!base->isOfType(SoGLRenderAction::getClassTypeId())) return;
  Inspect & c = *static_cast<Inspect *>(closure);
  ++c.calls;
  active(c.unit);
  storage(c.depth ? GL_TEXTURE_3D : GL_TEXTURE_2D, 4, 4, c.depth, c.mipmap);
  active(0);
}
SoCallback * callback(SoCallbackCB * fn, void * closure) {
  SoCallback * node = new SoCallback;
  node->setCallback(fn, closure);
  return node;
}
SoTexture2 * texture2(bool mipmap) {
  SoTexture2 * node = new SoTexture2;
  std::vector<unsigned char> data = pixels(4,4,0,mipmap);
  node->image.setValue(SbVec2s(4,4), 4, &data[0]);
  return node;
}
SoTexture3 * texture3(int depth, bool mipmap) {
  SoTexture3 * node = new SoTexture3;
  std::vector<unsigned char> data = pixels(4,4,depth,mipmap);
  node->images.setValue(SbVec3s(4,4,depth), 4, &data[0]);
  return node;
}
void scene(SoGLRenderAction & action, bool transition, bool unitzero2d,
           int depth, bool mipmap) {
  SoGroup * root = new SoGroup; root->ref();
  SoComplexity * quality = new SoComplexity;
  quality->textureQuality = mipmap ? 1.0f : 0.4f;
  root->addChild(quality);
  Inspect volume(transition ? 0 : 1, depth, mipmap), flat(0,0,mipmap);
  if (transition) {
    // Both nodes and their GL resources are cold. SoTexture2's upload happens
    // during its first traversal immediately after a real SoTexture3.
    root->addChild(texture3(depth, mipmap));
    root->addChild(callback(inspect, &volume));
    root->addChild(texture2(mipmap));
    root->addChild(callback(inspect, &flat));
  }
  else {
    if (unitzero2d) {
      root->addChild(texture2(mipmap));
      root->addChild(callback(inspect, &flat));
    }
    SoTextureUnit * unit = new SoTextureUnit; unit->unit = 1;
    root->addChild(unit);
    root->addChild(texture3(depth, mipmap));
    root->addChild(callback(inspect, &volume));
  }
  action.apply(root);
  CHECK(volume.calls == 1);
  CHECK(flat.calls == ((transition || unitzero2d) ? 1 : 0));
  root->unref();
  noError("scene traversal and release");
}
}

int main(int argc, char ** argv) {
  if (argc != 2) return 1;
  testname = argv[1];
  Display * display = XOpenDisplay(NULL);
  if (!display) { std::puts("SKIP: no X display"); return 77; }
  int attributes[] = { GLX_RGBA, GLX_DEPTH_SIZE, 16, None };
  XVisualInfo * visual = glXChooseVisual(display, DefaultScreen(display), attributes);
  if (!visual) { std::puts("SKIP: no GLX visual"); XCloseDisplay(display); return 77; }
  GLXContext context = glXCreateContext(display, visual, NULL, True);
  if (!context) { std::puts("SKIP: no GLX context"); XFree(visual); XCloseDisplay(display); return 77; }
  Colormap colormap = XCreateColormap(display, RootWindow(display,visual->screen), visual->visual, AllocNone);
  XSetWindowAttributes attr = {}; attr.colormap = colormap;
  Window window = XCreateWindow(display, RootWindow(display,visual->screen), 0,0,16,16,0,
                                visual->depth, InputOutput,visual->visual,CWColormap,&attr);
  if (!glXMakeCurrent(display,window,context)) {
    std::puts("SKIP: GLX context cannot be made current");
    glXDestroyContext(display,context);
    XDestroyWindow(display,window); XFreeColormap(display,colormap);
    XFree(visual); XCloseDisplay(display);
    return 77;
  }
  int major = 0, minor = 0;
  const char * version = reinterpret_cast<const char *>(glGetString(GL_VERSION));
  if (version) std::sscanf(version, "%d.%d", &major, &minor);
  const char * extensions = reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS));
  if (major < 2 || !extensions) {
    std::puts("SKIP: OpenGL 2 compatibility context required"); return 77;
  }
  const bool rectangle = std::strstr(testname,"rectangle") != NULL;
  const bool needsrectangle = rectangle ||
    std::strstr(testname,"moderect") != NULL;
  if (needsrectangle && !std::strstr(extensions,"GL_ARB_texture_rectangle") &&
      !std::strstr(extensions,"GL_EXT_texture_rectangle") &&
      !std::strstr(extensions,"GL_NV_texture_rectangle")) {
    std::puts("SKIP: texture rectangle unsupported"); return 77;
  }
  GLint units = 0; glGetIntegerv(GL_MAX_TEXTURE_UNITS,&units);
  if (std::strstr(testname,"unit1") && units < 2) {
    std::puts("SKIP: two texture units required"); return 77;
  }
  std::printf("GL renderer: %s; version: %s\n",glGetString(GL_RENDERER),version);
  noError("context initialization");
  SoDB::init();
  const uint32_t contextid = SoGLCacheContextElement::getUniqueCacheContext();
  {
    SoGLRenderAction action(SbViewportRegion(16,16));
    action.setCacheContext(contextid);
    const bool mipmap = std::strstr(testname,"mipmap") != NULL;
    const int depth = std::strstr(testname,"depth1") ? 1 : 4;
    if (std::strncmp(testname,"direct-",7) == 0) {
      int mode = SoMultiTextureEnabledElement::DISABLED;
      if (std::strstr(testname,"mode2d")) mode = SoMultiTextureEnabledElement::TEXTURE2D;
      if (std::strstr(testname,"mode3d")) mode = SoMultiTextureEnabledElement::TEXTURE3D;
      if (std::strstr(testname,"modecube")) mode = SoMultiTextureEnabledElement::CUBEMAP;
      if (std::strstr(testname,"moderect")) mode = SoMultiTextureEnabledElement::RECTANGLE;
      Case c = { std::strstr(testname,"-2d-") ? 0 : depth, mipmap, rectangle,
                 mode, std::strstr(testname,"eager") != NULL };
      SoGroup * root = new SoGroup; root->ref();
      root->addChild(callback(direct,&c));
      action.apply(root);
      root->unref();
    }
    else if (std::strncmp(testname,"transition-",11) == 0)
      scene(action,true,false,depth,mipmap);
    else if (std::strncmp(testname,"unit1-",6) == 0)
      scene(action,false,std::strstr(testname,"zero2d") != NULL,depth,mipmap);
    else CHECK(false);
    noError("final action");
  }
  SoContextHandler::destructingContext(contextid);
  SoDB::finish();
  glXMakeCurrent(display,None,NULL);
  glXDestroyContext(display,context);
  XDestroyWindow(display,window); XFreeColormap(display,colormap);
  XFree(visual); XCloseDisplay(display);
  std::printf("PASS: %s\n",testname);
  return 0;
}
