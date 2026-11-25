// Public traversal/state regression tests for SoTexture3 image overrides.
#ifdef COIN_TEXTURE3_GLX
#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#endif
#include <Inventor/SoDB.h>
#ifdef COIN_TEXTURE3_GLX
#include <Inventor/misc/SoContextHandler.h>
#include <Inventor/misc/SoGLDriverDatabase.h>
#endif
#include <Inventor/SbImage.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/elements/SoGLMultiTextureImageElement.h>
#include <Inventor/elements/SoMultiTextureEnabledElement.h>
#include <Inventor/elements/SoMultiTextureImageElement.h>
#include <Inventor/elements/SoTextureOverrideElement.h>
#include <Inventor/elements/SoTextureQualityElement.h>
#include <Inventor/misc/SoGLImage.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTexture3.h>
#include <Inventor/nodes/SoTextureUnit.h>
#include <cstdio>
#include <cstring>
#include <deque>

namespace {
int failures = 0;
int observations = 0;
void check(bool ok, const char * label, const char * condition) {
  if (!ok) { ++failures; std::fprintf(stderr, "FAIL [%s]: %s\n", label, condition); }
}
#define CHECK_AT(ok, label) check((ok), (label), #ok)

// Count virtual dispatch as well as inspecting the resulting base-class state.
class DerivedTexture3 : public SoTexture3 {
  SO_NODE_HEADER(DerivedTexture3);
public:
  static void initClass() { SO_NODE_INIT_CLASS(DerivedTexture3, SoTexture3, "Texture3"); }
  DerivedTexture3() : glcalls(0), cbcalls(0) { SO_NODE_CONSTRUCTOR(DerivedTexture3); }
  void GLRender(SoGLRenderAction * a) override { ++glcalls; SoTexture3::GLRender(a); }
  void callback(SoCallbackAction * a) override { ++cbcalls; SoTexture3::callback(a); }
  int glcalls, cbcalls;
protected:
  ~DerivedTexture3() override { }
};
SO_NODE_SOURCE(DerivedTexture3);

struct Expected {
  const char * label;
  int unit, value;
  bool imageoverride, qualityoverride, enabled;
};
void observe(void * data, SoAction * action) {
  const Expected & e = *static_cast<Expected *>(data);
  SoState * state = action->getState();
  ++observations;
  CHECK_AT(!!SoTextureOverrideElement::getImageOverride(state) == e.imageoverride, e.label);
  CHECK_AT(!!SoTextureOverrideElement::getQualityOverride(state) == e.qualityoverride, e.label);
  const bool rendering = action->isOfType(SoGLRenderAction::getClassTypeId());
  SbVec3s size(0,0,0);
  int nc = 0;
  const unsigned char * bytes = NULL;
  if (rendering) {
    SoMultiTextureImageElement::Model model;
    SbColor blend;
    SoGLImage * image = SoGLMultiTextureImageElement::get(state, e.unit, model, blend);
    CHECK_AT((image != NULL) == (e.value >= 0), e.label);
    if (image && image->getImage()) bytes = image->getImage()->getValue(size, nc);
    CHECK_AT(SoMultiTextureEnabledElement::getMode(state, e.unit) ==
             (e.enabled ? SoMultiTextureEnabledElement::TEXTURE3D :
                          SoMultiTextureEnabledElement::DISABLED), e.label);
  }
  else {
    bytes = SoMultiTextureImageElement::getImage(state, e.unit, size, nc);
  }
  if (e.value >= 0) {
    CHECK_AT(bytes && size == SbVec3s(2,2,2) && nc == 3, e.label);
    if (bytes && size == SbVec3s(2,2,2) && nc == 3) {
      for (int i = 0; i < 24; ++i) CHECK_AT(bytes[i] == e.value, e.label);
    }
  }
  else CHECK_AT(size == SbVec3s(0,0,0), e.label);
#ifdef COIN_TEXTURE3_GLX
  if (rendering) {
    GLint active = 0;
    glGetIntegerv(GL_ACTIVE_TEXTURE, &active);
    glActiveTexture(GL_TEXTURE0 + e.unit);
    CHECK_AT(!!glIsEnabled(GL_TEXTURE_3D) == e.enabled, e.label);
    if (e.value >= 0 && e.enabled) {
      GLint binding = 0, width = 0, height = 0, depth = 0;
      glGetIntegerv(GL_TEXTURE_BINDING_3D, &binding);
      CHECK_AT(binding != 0, e.label);
      glGetTexLevelParameteriv(GL_TEXTURE_3D, 0, GL_TEXTURE_WIDTH, &width);
      glGetTexLevelParameteriv(GL_TEXTURE_3D, 0, GL_TEXTURE_HEIGHT, &height);
      glGetTexLevelParameteriv(GL_TEXTURE_3D, 0, GL_TEXTURE_DEPTH, &depth);
      CHECK_AT(width == 2 && height == 2 && depth == 2, e.label);
      // Read only the checked size: a wrong target must not overflow the buffer.
      if (width == 2 && height == 2 && depth == 2) {
        unsigned char pixels[24] = {};
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glGetTexImage(GL_TEXTURE_3D, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
        for (int i = 0; i < 24; ++i) CHECK_AT(pixels[i] == e.value, e.label);
      }
    }
    glActiveTexture(active);
    CHECK_AT(glGetError() == GL_NO_ERROR, e.label);
  }
#endif
}

struct Quality { float quality; bool flag; };
void quality(void * data, SoAction * action) {
  Quality & q = *static_cast<Quality *>(data);
  SoState * state = action->getState();
  SoTextureOverrideElement::setQualityOverride(state, q.flag);
  // Ray picking does not enable this element. Never read or set it there.
  if (state->isElementEnabled(SoTextureQualityElement::getClassStackIndex()))
    SoTextureQualityElement::set(state, q.quality);
}
struct Scene {
  Scene() : root(new SoSeparator) { root->ref(); root->renderCaching = SoSeparator::OFF; }
  ~Scene() { root->unref(); }
  SoSeparator * root;
  std::deque<Expected> expected;
  std::deque<Quality> qualities;
  void unit(int u, SoSeparator * parent = NULL) {
    SoTextureUnit * n = new SoTextureUnit; n->unit = u;
    (parent ? parent : root)->addChild(n);
  }
  SoTexture3 * texture(int value, bool over = false, SoSeparator * parent = NULL,
                       SoTexture3 * n = NULL) {
    if (!n) n = new SoTexture3;
    if (value >= 0) {
      unsigned char bytes[24]; std::memset(bytes, value, sizeof(bytes));
      n->images.setValue(SbVec3s(2,2,2), 3, bytes);
    }
    n->setOverride(over);
    (parent ? parent : root)->addChild(n);
    return n;
  }
  void probe(const char * label, int u, int value, bool over, bool qflag = false,
             bool enabled = true, SoSeparator * parent = NULL) {
    expected.push_back(Expected{label,u,value,over,qflag,enabled && value >= 0});
    SoCallback * cb = new SoCallback; cb->setCallback(observe, &expected.back());
    (parent ? parent : root)->addChild(cb);
  }
  void setquality(float q, bool flag, SoSeparator * parent = NULL) {
    qualities.push_back(Quality{q,flag});
    SoCallback * cb = new SoCallback; cb->setCallback(quality, &qualities.back());
    (parent ? parent : root)->addChild(cb);
  }
};

void plain(SoAction & action) {
  Scene s;
  s.texture(30); s.probe("plain zero",0,30,false);
  s.unit(1); s.texture(60); s.probe("plain one",1,60,false);
  s.texture(90); s.probe("plain replacement",1,90,false);
  s.texture(-1); s.probe("plain empty",1,-1,false);
  s.texture(100); s.probe("plain reenable",1,100,false);
  s.probe("plain zero preserved",0,30,false);
  action.apply(s.root);
}
void lockedzero(SoAction & action) {
  Scene s;
  s.texture(30,true); s.probe("first override",0,30,true);
  s.texture(60); s.probe("blocked ordinary",0,30,true);
  s.texture(90,true); s.probe("blocked second override",0,30,true);
  s.texture(-1); s.probe("blocked empty",0,30,true);
  s.setquality(0.0f,true); s.texture(100);
  s.probe("blocked zero quality leaves image and mode",0,30,true,true);
  action.apply(s.root);
}
void crossunit(SoAction & action) {
  Scene s;
  // A volume on zero is deliberate: SoGLImage's existing upload-target choice
  // consults zero's mode. This test isolates the override guard from that bug.
  s.texture(30,true); s.unit(1);
  s.texture(60,true); s.probe("locked zero permits one override",1,60,true);
  s.texture(90); s.probe("one override cannot lock one",1,90,true);
  s.texture(-1); s.probe("locked zero permits one empty",1,-1,true);
  s.setquality(0.0f,true); s.texture(100);
  s.probe("locked zero permits one zero quality",1,100,true,true,false);
  s.setquality(0.75f,false); s.texture(110);
  s.probe("locked zero permits one reenable",1,110,true);
  s.unit(0); s.texture(120); s.probe("one cannot clear zero lock",0,30,true);
  action.apply(s.root);
}
void nonzerooverride(SoAction & action) {
  Scene s;
  s.texture(30); s.unit(1); s.texture(60,true);
  s.probe("one override does not set global flag",1,60,false);
  s.unit(0); s.texture(90); s.probe("one override does not block zero",0,90,false);
  s.unit(1); s.texture(100); s.probe("one remains replaceable",1,100,false);
  s.texture(-1,true); s.probe("empty one override does not set global flag",1,-1,false);
  s.unit(0); s.texture(110); s.probe("empty one override does not block zero",0,110,false);
  action.apply(s.root);
}
void separators(SoAction & action) {
  Scene s;
  s.texture(30);
  SoSeparator * inner = new SoSeparator; inner->renderCaching = SoSeparator::OFF;
  s.root->addChild(inner);
  s.texture(60,true,inner); s.unit(1,inner); s.texture(90,false,inner);
  s.probe("inner cross unit",1,90,true,false,true,inner);
  s.probe("pop restores zero",0,30,false);
  s.probe("pop restores one",1,-1,false);
  s.unit(0); s.texture(100,true);
  SoSeparator * nested = new SoSeparator; nested->renderCaching = SoSeparator::OFF;
  s.root->addChild(nested); s.unit(1,nested); s.texture(110,true,nested);
  s.probe("nested inherits zero lock",1,110,true,false,true,nested);
  s.probe("pop preserves inherited lock",0,100,true);
  s.probe("pop removes nested one image",1,-1,true);
  action.apply(s.root);
}
void qualityflags(SoAction & action) {
  Scene s;
  s.setquality(0.75f,true); s.texture(30);
  s.probe("quality override does not guard image",0,30,false,true);
  s.texture(60); s.probe("quality override permits replacement",0,60,false,true);
  // Warm this very resource while zero has 3D mode. Its later quality-zero
  // visit must test disabling, without exercising the separate upload bug.
  SoSeparator * warm = new SoSeparator; warm->renderCaching = SoSeparator::OFF;
  s.root->addChild(warm);
  SoTexture3 * zeroquality = s.texture(90,true,warm);
  s.probe("warm zero quality resource",0,90,true,true,true,warm);
  s.setquality(0.0f,true); s.root->addChild(zeroquality);
  s.probe("zero quality override installs image",0,90,true,true,false);
  s.texture(-1); s.probe("zero quality override still guards image",0,90,true,true,false);
  action.apply(s.root);
  Scene empty;
  empty.texture(-1,true); empty.texture(110);
  empty.probe("empty override guards zero",0,-1,true);
  action.apply(empty.root);
}
void dispatch(SoAction & action) {
  Scene s;
  s.texture(30,true); s.unit(1);
  DerivedTexture3 * n = new DerivedTexture3;
  s.texture(60,false,NULL,n); s.probe("derived virtual dispatch",1,60,true);
  action.apply(s.root);
  const bool gl = action.isOfType(SoGLRenderAction::getClassTypeId());
  CHECK_AT((gl ? n->glcalls : n->cbcalls) == 1, "virtual dispatch count");
}
void reuse(SoAction & action) {
  { Scene s; s.texture(30,true); s.probe("reuse first isolated root",0,30,true); action.apply(s.root); }
  // SoSeparator supplies explicit push/pop isolation for the reused action.
  { Scene s; s.texture(60); s.probe("reuse second isolated root",0,60,false); action.apply(s.root); }
}
void raypick() {
  Scene s;
  s.texture(30,true); s.unit(1); s.texture(60,true);
  // SoTexture3 inherits the no-op pick method. Preserve that existing contract;
  // callback and rayPick are intentionally not asserted to have equal state.
  s.probe("rayPick unchanged zero",0,-1,false);
  s.probe("rayPick unchanged one",1,-1,false);
  SoRayPickAction action(SbViewportRegion(16,16));
  action.setRay(SbVec3f(0,0,1), SbVec3f(0,0,-1));
  action.apply(s.root);
}
void run(SoAction & action, const char * name) {
  if (!std::strcmp(name,"plain")) plain(action);
  else if (!std::strcmp(name,"locked-zero")) lockedzero(action);
  else if (!std::strcmp(name,"cross-unit")) crossunit(action);
  else if (!std::strcmp(name,"nonzero-override")) nonzerooverride(action);
  else if (!std::strcmp(name,"separators")) separators(action);
  else if (!std::strcmp(name,"quality-flags")) qualityflags(action);
  else if (!std::strcmp(name,"dispatch")) dispatch(action);
  else if (!std::strcmp(name,"reuse")) reuse(action);
  else { std::fprintf(stderr,"Unknown case: %s\n",name); ++failures; }
}
}

int main(int argc, char ** argv) {
  if (argc != 2) { std::fprintf(stderr,"Expected one case name\n"); return 1; }
#ifdef COIN_TEXTURE3_GLX
  Display * d = XOpenDisplay(NULL);
  if (!d) { std::puts("SKIP: no X display for real GLX context"); return 77; }
  int attributes[] = { GLX_RGBA, GLX_DEPTH_SIZE, 16, None };
  XVisualInfo * visual = glXChooseVisual(d, DefaultScreen(d), attributes);
  if (!visual) { std::puts("SKIP: no GLX RGBA visual"); XCloseDisplay(d); return 77; }
  GLXContext context = glXCreateContext(d,visual,NULL,True);
  if (!context) { std::puts("SKIP: cannot create GLX context"); XFree(visual); XCloseDisplay(d); return 77; }
  Colormap colormap = XCreateColormap(d,RootWindow(d,visual->screen),visual->visual,AllocNone);
  XSetWindowAttributes attr = {}; attr.colormap = colormap;
  Window window = XCreateWindow(d,RootWindow(d,visual->screen),0,0,16,16,0,
                               visual->depth,InputOutput,visual->visual,CWColormap,&attr);
  if (!glXMakeCurrent(d,window,context)) { std::puts("SKIP: cannot make GLX context current"); return 77; }
  GLint units = 0, max3d = 0;
  glGetIntegerv(GL_MAX_TEXTURE_UNITS,&units); glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE,&max3d);
  if (glGetError() != GL_NO_ERROR || units < 2 || max3d < 2) {
    std::puts("SKIP: requires 3D textures and at least two fixed-function texture units"); return 77;
  }
  std::printf("Real GLX renderer: %s; version: %s; texture units: %d\n",
              glGetString(GL_RENDERER),glGetString(GL_VERSION),units);
#endif
  SoDB::init(); DerivedTexture3::initClass();
#ifdef COIN_TEXTURE3_GLX
  if (!SoGLDriverDatabase::isSupported(cc_glglue_instance(951), SO_GL_3D_TEXTURES) ||
      !SoGLDriverDatabase::isSupported(cc_glglue_instance(951), SO_GL_MULTITEXTURE)) {
    std::puts("SKIP: Coin driver database disables 3D textures or multitexture"); return 77;
  }
  { SoGLRenderAction action(SbViewportRegion(16,16)); action.setCacheContext(951); run(action,argv[1]); }
  SoContextHandler::destructingContext(951);
#else
  if (!std::strcmp(argv[1],"raypick")) raypick();
  else { SoCallbackAction action; run(action,argv[1]); }
#endif
  SoDB::finish();
#ifdef COIN_TEXTURE3_GLX
  glXMakeCurrent(d,None,NULL); glXDestroyContext(d,context);
  XDestroyWindow(d,window); XFreeColormap(d,colormap); XFree(visual); XCloseDisplay(d);
#endif
  std::printf("SoTexture3 %s: %d observations, %d failures\n",argv[1],observations,failures);
  return failures ? 1 : 0;
}
