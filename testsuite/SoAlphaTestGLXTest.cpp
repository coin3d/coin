#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <Inventor/SoDB.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLLazyElement.h>
#include <Inventor/nodes/SoAlphaTest.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/misc/SoContextHandler.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <limits>

namespace {
int failures = 0;
#define CHECK(x) do { if (!(x)) { std::fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, #x); ++failures; } } while (0)
struct Context {
  Display * display; XVisualInfo * visual; GLXContext context; Window window; Colormap map;
  Context() : display(XOpenDisplay(NULL)), visual(NULL), context(NULL), window(0), map(0) {
    if (!display) return;
    int attr[] = { GLX_RGBA, GLX_RED_SIZE, 8, GLX_GREEN_SIZE, 8, GLX_BLUE_SIZE, 8, None };
    visual = glXChooseVisual(display, DefaultScreen(display), attr);
    if (!visual) return;
    context = glXCreateContext(display, visual, NULL, True);
    if (!context) return;
    map = XCreateColormap(display, RootWindow(display, visual->screen), visual->visual, AllocNone);
    XSetWindowAttributes a = {}; a.colormap = map;
    window = XCreateWindow(display, RootWindow(display, visual->screen), 0, 0, 16, 16, 0,
                           visual->depth, InputOutput, visual->visual, CWColormap, &a);
    if (!glXMakeCurrent(display, window, context)) context = NULL;
  }
  ~Context() {
    if (context) { glXMakeCurrent(display, None, NULL); glXDestroyContext(display, context); }
    if (window) XDestroyWindow(display, window);
    if (map) XFreeColormap(display, map);
    if (visual) XFree(visual);
    if (display) XCloseDisplay(display);
  }
};
struct Sample { int function; float reference; int calls; };
void sample(void * data, SoAction * a) {
  Sample * s = static_cast<Sample *>(data);
  float ref = 0;
  const int fn = SoLazyElement::getAlphaTest(a->getState(), ref);
  CHECK(fn == s->function);
  CHECK(ref == s->reference);
  ++s->calls;
}
SoCallback * observer(Sample * s) { SoCallback * n = new SoCallback; n->setCallback(sample, s); return n; }
SoAlphaTest * alpha(int function, float reference) {
  SoAlphaTest * n = new SoAlphaTest; n->function = function; n->value = reference; return n;
}
void state_case(SoGLRenderAction & action, bool ignorefunction, bool ignorevalue,
                int function, int glfunction, float reference,
                int parentfunction = SoAlphaTest::GREATER, int parentgl = GL_GREATER,
                float parentreference = 0.75f) {
  SoSeparator * root = new SoSeparator; root->ref(); root->renderCaching = SoSeparator::OFF;
  root->addChild(alpha(parentfunction, parentreference));
  SoSeparator * child = new SoSeparator; child->renderCaching = SoSeparator::OFF; root->addChild(child);
  SoAlphaTest * node = alpha(function, reference);
  node->function.setIgnored(ignorefunction); node->value.setIgnored(ignorevalue);
  // Keep a live field connection: traversal must not rewrite or disconnect it.
  SoAlphaTest * source = alpha(function, reference); source->ref();
  node->value.connectFrom(&source->value);
  const SbBool functiondefault = node->function.isDefault();
  const SbBool valuedefault = node->value.isDefault();
  child->addChild(node);
  Sample inside = {ignorefunction ? parentgl : glfunction, ignorevalue ? parentreference : reference, 0};
  Sample outside = {parentgl, parentreference, 0};
  child->addChild(observer(&inside)); root->addChild(observer(&outside));
  action.apply(root);
  CHECK(inside.calls == 1 && outside.calls == 1);
  CHECK(node->function.getValue() == function && node->value.getValue() == reference);
  CHECK(node->function.isIgnored() == ignorefunction && node->value.isIgnored() == ignorevalue);
  CHECK(node->function.isDefault() == functiondefault && node->value.isDefault() == valuedefault);
  CHECK(node->value.isConnected());
  root->unref(); source->unref();
}
void controls(SoGLRenderAction & action) {
  SoSeparator * root = new SoSeparator; root->ref(); root->renderCaching = SoSeparator::OFF;
  SoAlphaTest * defaults = new SoAlphaTest;
  CHECK(defaults->function.getValue() == SoAlphaTest::NONE); CHECK(defaults->value.getValue() == 0.5f);
  CHECK(defaults->function.isDefault() && defaults->value.isDefault());
  root->addChild(defaults); Sample s = {0, 0.5f, 0}; root->addChild(observer(&s)); action.apply(root);
  CHECK(s.calls == 1); root->unref();
  const int functions[] = {SoAlphaTest::NONE, SoAlphaTest::NEVER, SoAlphaTest::ALWAYS,
    SoAlphaTest::LESS, SoAlphaTest::LEQUAL, SoAlphaTest::EQUAL, SoAlphaTest::GEQUAL,
    SoAlphaTest::GREATER, SoAlphaTest::NOTEQUAL, 123456};
  const int glfunctions[] = {0, GL_NEVER, GL_ALWAYS, GL_LESS, GL_LEQUAL, GL_EQUAL,
    GL_GEQUAL, GL_GREATER, GL_NOTEQUAL, 0};
  const float refs[] = {-std::numeric_limits<float>::max(), -2.0f, -0.0f, 0.0f,
    0.5f, 1.0f, 2.0f, std::numeric_limits<float>::max()};
  for (unsigned f = 0; f < sizeof(functions)/sizeof(functions[0]); ++f)
    for (unsigned r = 0; r < sizeof(refs)/sizeof(refs[0]); ++r)
      state_case(action, false, false, functions[f], glfunctions[f], refs[r]);
}
void ignored(SoGLRenderAction & action) {
  for (int f = 0; f < 2; ++f) for (int v = 0; v < 2; ++v) {
    state_case(action, f != 0, v != 0, SoAlphaTest::LESS, GL_LESS, -2.0f);
    state_case(action, f != 0, v != 0, SoAlphaTest::NONE, 0, 2.0f);
    state_case(action, f != 0, v != 0, SoAlphaTest::LESS, GL_LESS, 0.5f,
               SoAlphaTest::NONE, 0, -4.0f);
    state_case(action, f != 0, v != 0, SoAlphaTest::LESS, GL_LESS, 0.5f,
               SoAlphaTest::GREATER, GL_GREATER, 1.25f);
  }
}
void quad(float a) {
  glDisable(GL_LIGHTING); glDisable(GL_DEPTH_TEST); glDisable(GL_BLEND);
  glMatrixMode(GL_PROJECTION); glLoadIdentity(); glMatrixMode(GL_MODELVIEW); glLoadIdentity();
  glColor4f(1, 1, 1, a);
  glBegin(GL_QUADS); glVertex2f(-1,-1); glVertex2f(1,-1); glVertex2f(1,1); glVertex2f(-1,1); glEnd();
}
bool pixel() { float p[4] = {}; glReadPixels(8,8,1,1,GL_RGBA,GL_FLOAT,p); return p[0] > 0.5f; }
struct Draw { float alpha; int calls; int compiled; };
void draw(void * data, SoAction * a) {
  Draw * d = static_cast<Draw *>(data); ++d->calls;
  GLint list = 0; glGetIntegerv(GL_LIST_INDEX, &list); if (list) ++d->compiled;
  SoGLLazyElement::getInstance(a->getState())->send(a->getState(), SoLazyElement::ALPHATEST_MASK);
  quad(d->alpha);
}
SoCallback * drawing(Draw * d) { SoCallback * n = new SoCallback; n->setCallback(draw, d); return n; }
bool render(SoGLRenderAction & action, SoNode * root) {
  glClearColor(0,0,0,0); glClear(GL_COLOR_BUFFER_BIT); action.apply(root); glFinish(); return pixel();
}
void rendered_controls(SoGLRenderAction & action) {
  const int functions[] = {SoAlphaTest::NONE, SoAlphaTest::NEVER, SoAlphaTest::ALWAYS,
    SoAlphaTest::LESS, SoAlphaTest::LEQUAL, SoAlphaTest::EQUAL, SoAlphaTest::GEQUAL,
    SoAlphaTest::GREATER, SoAlphaTest::NOTEQUAL, 123456};
  const float incoming[] = {0.25f, 0.5f, 0.75f};
  const bool expected[][3] = {{true,true,true}, {false,false,false}, {true,true,true},
    {true,false,false}, {true,true,false}, {false,true,false}, {false,true,true},
    {false,false,true}, {true,false,true}, {true,true,true}};
  for (unsigned f = 0; f < sizeof(functions)/sizeof(functions[0]); ++f) {
    SoSeparator * root = new SoSeparator; root->ref(); root->renderCaching = SoSeparator::OFF;
    root->addChild(alpha(functions[f],0.5f)); Draw d = {0.5f,0,0}; root->addChild(drawing(&d));
    for (int i = 0; i < 3; ++i) { d.alpha = incoming[i]; CHECK(render(action,root) == expected[f][i]); }
    root->unref();
  }
}
void cache_case(SoGLRenderAction & action, bool ignorefunction) {
  Draw d = {0.5f, 0, 0};
  SoSeparator * shared = new SoSeparator; shared->renderCaching = SoSeparator::ON;
  SoAlphaTest * partial = alpha(SoAlphaTest::LESS, 0.75f);
  partial->function.setIgnored(ignorefunction); partial->value.setIgnored(!ignorefunction);
  shared->addChild(partial); shared->addChild(drawing(&d));
  SoSeparator * roots[2];
  for (int i = 0; i < 2; ++i) {
    roots[i] = new SoSeparator; roots[i]->ref(); roots[i]->renderCaching = SoSeparator::OFF;
    // In the first parent the locally specified half differs from the
    // inherited pair. A cache must retain the dependency on the other half
    // even though updating this half writes their shared ALPHATEST_MASK.
    const int parentfunction = (i || !ignorefunction) ? SoAlphaTest::GREATER : SoAlphaTest::LESS;
    const float parentreference = (i || ignorefunction) ? 0.25f : 0.75f;
    roots[i]->addChild(alpha(parentfunction, parentreference));
    roots[i]->addChild(shared);
  }
  for (int i = 0; i < 5; ++i) CHECK(render(action, roots[0]));
  CHECK(d.compiled > 0);
  const int warmcalls = d.calls;
  CHECK(render(action, roots[0])); CHECK(d.calls == warmcalls); // Prove cache replay.
  for (int i = 0; i < 4; ++i) {
    CHECK(!render(action, roots[1]));
    CHECK(render(action, roots[0]));
  }
  // The shared child must not leave its explicit half in the surrounding state.
  Draw after = {0.5f, 0, 0}; roots[1]->addChild(drawing(&after));
  CHECK(render(action, roots[1])); // Parent GREATER .25 passes after the cached child fails.
  CHECK(after.calls == 1);
  roots[0]->unref(); roots[1]->unref();
}

// GL_ALPHA_TEST_REF queries are deliberately absent: they may clamp the query
// result even when the driver stores an unclamped reference for a float target.
int floating(SoGLRenderAction & action) {
  const char * ext = reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS));
  if (!ext || !std::strstr(ext, "GL_ARB_color_buffer_float") ||
      !std::strstr(ext, "GL_ARB_texture_float") || !std::strstr(ext, "GL_EXT_framebuffer_object")) {
    std::puts("SKIP: floating alpha test needs color_buffer_float, texture_float and framebuffer_object"); return 77;
  }
  PFNGLCLAMPCOLORARBPROC clamp = reinterpret_cast<PFNGLCLAMPCOLORARBPROC>(glXGetProcAddressARB(reinterpret_cast<const GLubyte *>("glClampColorARB")));
  if (!clamp) { std::puts("SKIP: glClampColorARB unavailable"); return 77; }
  GLuint fbo = 0, tex = 0; glGenFramebuffers(1, &fbo); glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glGenTextures(1, &tex); glBindTexture(GL_TEXTURE_2D, tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  clamp(GL_CLAMP_VERTEX_COLOR_ARB, GL_FALSE); clamp(GL_CLAMP_READ_COLOR_ARB, GL_FALSE);
  Draw d = {1.5f, 0, 0};
  SoSeparator * root = new SoSeparator; root->ref(); root->renderCaching = SoSeparator::OFF;
  SoAlphaTest * node = alpha(SoAlphaTest::LESS, 2.0f); root->addChild(node); root->addChild(drawing(&d));
  for (int target = 0; target < 3; ++target) {
    const bool floatingtarget = target != 0;
    glTexImage2D(GL_TEXTURE_2D, 0, floatingtarget ? GL_RGBA32F_ARB : GL_RGBA8,16,16,0,GL_RGBA,GL_FLOAT,NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,tex,0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
      root->unref(); glBindFramebuffer(GL_FRAMEBUFFER,0); glDeleteFramebuffers(1,&fbo); glDeleteTextures(1,&tex);
      std::puts("SKIP: requested framebuffer format unavailable"); return 77;
    }
    glDrawBuffer(GL_COLOR_ATTACHMENT0); glReadBuffer(GL_COLOR_ATTACHMENT0); glViewport(0,0,16,16);
    const bool unclamped = target == 1;
    clamp(GL_CLAMP_FRAGMENT_COLOR_ARB, unclamped ? GL_FALSE : GL_TRUE);
    // Read back an unconditional fragment to prove which color representation
    // reaches this target. Fragment clamping changes 1.5 to 1, but a float
    // target still compares against reference 2 in floating-point form.
    glClearColor(0,0,0,0); glClear(GL_COLOR_BUFFER_BIT); glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_ALWAYS, 2.0f); quad(1.5f);
    float rgba[4] = {}; glReadPixels(8,8,1,1,GL_RGBA,GL_FLOAT,rgba);
    CHECK(rgba[0] > 0.5f); CHECK(rgba[3] == (unclamped ? 1.5f : 1.0f));
    // ARB_color_buffer_float section 4.1.5 converts the reference to the
    // fragment alpha representation. Fixed-point conversion clamps it;
    // floating-point conversion preserves 2 even when fragment colors clamp.
    glClear(GL_COLOR_BUFFER_BIT); glAlphaFunc(GL_LESS, 2.0f); quad(1.5f);
    CHECK(pixel() == floatingtarget);
    glClear(GL_COLOR_BUFFER_BIT); glAlphaFunc(GL_LESS, 1.0f); quad(1.5f); CHECK(!pixel());
    node->function = SoAlphaTest::LESS; CHECK(render(action, root) == floatingtarget);
    node->function = SoAlphaTest::GREATER; CHECK(!render(action, root));
    CHECK(glGetError() == GL_NO_ERROR);
  }
  root->unref(); clamp(GL_CLAMP_VERTEX_COLOR_ARB, GL_FIXED_ONLY_ARB); clamp(GL_CLAMP_FRAGMENT_COLOR_ARB, GL_FIXED_ONLY_ARB);
  glBindFramebuffer(GL_FRAMEBUFFER,0); glDeleteFramebuffers(1,&fbo); glDeleteTextures(1,&tex);
  return failures ? 1 : 0;
}
}
int main(int argc, char ** argv) {
  Context context;
  if (!context.context) { std::puts("SKIP: no compatibility GLX context (try Xvfb with Mesa llvmpipe)"); return 77; }
  std::printf("GL renderer: %s\n", glGetString(GL_RENDERER));
  SoDB::init();
  int result = 0;
  {
    SoGLRenderAction action(SbViewportRegion(16,16)); action.setCacheContext(137);
    const char * mode = argc > 1 ? argv[1] : "controls";
    if (!std::strcmp(mode,"controls")) { controls(action); rendered_controls(action); }
    else if (!std::strcmp(mode,"ignored")) ignored(action);
    else if (!std::strcmp(mode,"cache")) { cache_case(action,true); cache_case(action,false); }
    else if (!std::strcmp(mode,"float")) result = floating(action);
    else { std::fprintf(stderr,"Unknown test mode: %s\n", mode); result = 1; }
    SoContextHandler::destructingContext(137);
  }
  SoDB::finish();
  return failures ? 1 : result;
}
