#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <cstring>
#include <Inventor/SoSceneManager.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/sensors/SoSensorManager.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoBumpMap.h>
#include <Inventor/nodes/SoCube.h>
#include "TestAdapter.h"
#include "shapenodes/soshape_bumprender.cpp"

namespace {
GLboolean isProgram(GLuint id) {
  PFNGLISPROGRAMARBPROC func = (PFNGLISPROGRAMARBPROC) glXGetProcAddressARB((const GLubyte *) "glIsProgramARB");
  CHECK(func); return func(id);
}
void bindProgram(GLenum target, GLuint id) {
  PFNGLBINDPROGRAMARBPROC func = (PFNGLBINDPROGRAMARBPROC) glXGetProcAddressARB((const GLubyte *) "glBindProgramARB");
  CHECK(func); func(target, id);
}
void getProgramiv(GLenum target, GLenum pname, GLint * value) {
  PFNGLGETPROGRAMIVARBPROC func = (PFNGLGETPROGRAMIVARBPROC) glXGetProcAddressARB((const GLubyte *) "glGetProgramivARB");
  CHECK(func); func(target, pname, value);
}

struct GLContext {
  Display * display;
  XVisualInfo * visual;
  GLXContext context;
  Colormap colormap;
  Window window;
  GLContext(Display * d, XVisualInfo * v) : display(d), visual(v) {
    context = glXCreateContext(d, v, NULL, True); CHECK(context);
    colormap = XCreateColormap(d, RootWindow(d, v->screen), v->visual, AllocNone);
    XSetWindowAttributes attr = {}; attr.colormap = colormap;
    window = XCreateWindow(d, RootWindow(d, v->screen), 0, 0, 16, 16, 0,
                            v->depth, InputOutput, v->visual, CWColormap, &attr);
  }
  void current(uint32_t id) {
    CHECK(glXMakeCurrent(display, window, context)); BumpTestCacheContext::current() = id;
  }
  ~GLContext() {
    glXMakeCurrent(display, None, NULL); glXDestroyContext(display, context);
    XDestroyWindow(display, window); XFreeColormap(display, colormap);
  }
};
void check_lengths(const cc_glglue * glue, const CoinBumpTestRenderer::spec_programidx & p) {
  const GLenum targets[] = { GL_FRAGMENT_PROGRAM_ARB, GL_VERTEX_PROGRAM_ARB, GL_VERTEX_PROGRAM_ARB };
  const GLuint ids[] = { p.fragment, p.dirlight, p.pointlight };
  for (int i = 0; i < 3; ++i) {
    CHECK(isProgram(ids[i])); bindProgram(targets[i], ids[i]);
    GLint length = 0; cc_glglue_glGetProgramiv(glue, targets[i], GL_PROGRAM_LENGTH_ARB, &length);
    CHECK(length > 0);
  }
  bindProgram(GL_FRAGMENT_PROGRAM_ARB, 0); bindProgram(GL_VERTEX_PROGRAM_ARB, 0);
  CHECK(glGetError() == GL_NO_ERROR);
}
struct ListTrace {
  ListTrace() : listed(0) { }
  int listed;
};
void trace_list(void * closure, SoAction * action) {
  if (action->isOfType(SoGLRenderAction::getClassTypeId())) {
    GLint list = 0; glGetIntegerv(GL_LIST_INDEX, &list);
    if (list != 0) ++((ListTrace *) closure)->listed;
  }
}
void redraw_scene(void * closure, SoSceneManager * manager) {
  ++*(int *) closure;
  manager->render();
}
void read_pixel(GLubyte * pixel) { glReadPixels(8, 8, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel); }
void drain_redraws() {
  // Process the viewer's queued redraw, not another explicit action.apply().
  for (int i = 0; i < 4 && SoDB::getSensorManager()->isDelaySensorPending(); ++i) {
    SoDB::getSensorManager()->processDelayQueue(TRUE);
  }
}
void integration_scene(GLContext & a, GLContext & b) {
  SoSeparator * root = new SoSeparator; root->ref();
  root->renderCaching = SoSeparator::OFF;
  SoOrthographicCamera * camera = new SoOrthographicCamera;
  camera->position.setValue(0, 0, 5); camera->height = 3;
  camera->nearDistance = 1; camera->farDistance = 10; root->addChild(camera);
  root->addChild(new SoDirectionalLight);
  SoSeparator * scene = new SoSeparator;
  scene->renderCaching = SoSeparator::ON;
  root->addChild(scene);
  SoMaterial * material = new SoMaterial;
  material->diffuseColor.setValue(0.1f, 0.1f, 0.1f);
  material->specularColor.setValue(0.8f, 0.8f, 0.8f); scene->addChild(material);
  SoBumpMap * bump = new SoBumpMap;
  const unsigned char pixels[] = {128,128,255,128,128,255,128,128,255,128,128,255};
  bump->image.setValue(SbVec2s(2,2), 3, pixels); scene->addChild(bump);
  ListTrace trace;
  SoCallback * callback = new SoCallback;
  callback->setCallback(trace_list, &trace); scene->addChild(callback);
  scene->addChild(new SoCube);
  {
    SoGLRenderAction action(SbViewportRegion(16,16));
    SoSceneManager manager;
    manager.setGLRenderAction(&action); manager.setViewportRegion(SbViewportRegion(16,16));
    manager.setRedrawPriority(0); // Must not reenter an action inside its display list.
    manager.setSceneGraph(root); manager.setBackgroundColor(SbColor(0,0,0));
    int redraws = 0;
    manager.setRenderCallback(redraw_scene, &redraws); manager.activate();
    a.current(73); action.setCacheContext(73);
    manager.render(); manager.render(); manager.render(); drain_redraws();
    CHECK(trace.listed > 0);
    GLubyte expected[3]; read_pixel(expected); CHECK(expected[0] > 0);
    GLint fragmenta = 0, vertexa = 0;
    getProgramiv(GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_BINDING_ARB, &fragmenta);
    getProgramiv(GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_BINDING_ARB, &vertexa);
    CHECK(fragmenta != 0 && vertexa != 0);

    // The separator is already warm, but B has no programs. Its first visit
    // really compiles a list; then the scene-manager notification must render
    // again without any user input, initializing outside the list.
    b.current(74); action.setCacheContext(74);
    const int listed = trace.listed, before = redraws;
    manager.render(); CHECK(trace.listed > listed);
    drain_redraws(); CHECK(redraws > before);
    GLubyte actual[3]; read_pixel(actual);
    for (int i = 0; i < 3; ++i) CHECK(std::abs((int) actual[i] - (int) expected[i]) <= 2);
    GLint fragmentb = 0, vertexb = 0;
    getProgramiv(GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_BINDING_ARB, &fragmentb);
    getProgramiv(GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_BINDING_ARB, &vertexb);
    CHECK(fragmentb != 0 && vertexb != 0);
    GLint length = 0;
    getProgramiv(GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_LENGTH_ARB, &length); CHECK(length > 0);
    getProgramiv(GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_LENGTH_ARB, &length); CHECK(length > 0);
    const int settled = redraws; manager.render(); drain_redraws(); CHECK(redraws == settled);
    manager.deactivate(); manager.setSceneGraph(NULL); root->unref();
    SoSeparator * empty = new SoSeparator; empty->ref();
    action.apply(empty); empty->unref();
    CHECK(!isProgram((GLuint) fragmentb) && !isProgram((GLuint) vertexb));
    SoContextHandler::destructingContext(74);
    a.current(73); SoContextHandler::destructingContext(73);
    CHECK(!isProgram((GLuint) fragmenta) && !isProgram((GLuint) vertexa));
  }
  std::puts("PASS: real deferred initialization in a second context, automatic redraw and cleanup.");
}

}
int main() {
  Display * display = XOpenDisplay(NULL);
  if (!display) { std::puts("SKIP: no X display"); return 77; }
  int attr[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
  XVisualInfo * visual = glXChooseVisual(display, DefaultScreen(display), attr);
  if (!visual) { XCloseDisplay(display); return 77; }
  SoDB::init();
  {
    GLContext a(display, visual), b(display, visual); a.current(71);
    const char * extensions = (const char *) glGetString(GL_EXTENSIONS);
    if (!extensions || !std::strstr(extensions, "GL_ARB_fragment_program") ||
        !std::strstr(extensions, "GL_ARB_vertex_program")) return 77;
    std::printf("GL renderer: %s\n", glGetString(GL_RENDERER));
    const cc_glglue * gluea = cc_glglue_instance(71);
    CoinBumpTestRenderer * renderer = new CoinBumpTestRenderer;
    CoinBumpTestRenderer::spec_programidx pa, pb;
    CHECK(renderer->ensurePrograms(gluea, NULL, pa)); check_lengths(gluea, pa);

    // Both default bindings and pre-existing valid bindings must defer uploads.
    for (int prebound = 0; prebound < 2; ++prebound) {
      CoinBumpTestRenderer deferred;
      if (prebound) {
        bindProgram(GL_FRAGMENT_PROGRAM_ARB, pa.fragment);
        bindProgram(GL_VERTEX_PROGRAM_ARB, pa.dirlight);
      }
      GLuint list = glGenLists(1); glNewList(list, GL_COMPILE);
      CoinBumpTestRenderer::spec_programidx p;
      CHECK(!deferred.ensurePrograms(gluea, NULL, p));
      CHECK(!deferred.ensurePrograms(gluea, NULL, p));
      glEndList(); CHECK(glGetError() == GL_NO_ERROR);
      glCallList(list); CHECK(glGetError() == GL_NO_ERROR);
      CHECK(deferred.programcache->contexts[71].spec.fragment == 0);
      BumpTestCacheContext::flush(71);
      CHECK(deferred.ensurePrograms(gluea, NULL, p));
      GLint binding = 0; getProgramiv(GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_BINDING_ARB, &binding);
      CHECK(binding == (prebound ? (GLint) pa.fragment : 0));
      getProgramiv(GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_BINDING_ARB, &binding);
      CHECK(binding == (prebound ? (GLint) pa.dirlight : 0));
      check_lengths(gluea, p); glDeleteLists(list, 1);
    }
    BumpTestCacheContext::flush(71);
    b.current(72); const cc_glglue * glueb = cc_glglue_instance(72);
    CHECK(renderer->ensurePrograms(glueb, NULL, pb)); check_lengths(glueb, pb);
    a.current(71); CoinBumpTestRenderer::spec_programidx reused;
    CHECK(renderer->ensurePrograms(gluea, NULL, reused)); CHECK(reused.fragment == pa.fragment);
    SoContextHandler::destructingContext(71);
    CHECK(!isProgram(pa.fragment) && !isProgram(pa.dirlight) && !isProgram(pa.pointlight));
    b.current(72); delete renderer; BumpTestCacheContext::flush(72);
    CHECK(!isProgram(pb.fragment) && !isProgram(pb.dirlight) && !isProgram(pb.pointlight));
    SoContextHandler::destructingContext(72);
    CHECK(CoinBumpTestRenderer::ProgramCache::registry().entries.empty());
    CHECK(BumpTestCacheContext::queue().empty());
    integration_scene(a, b);
  }
  SoDB::finish(); XFree(visual); XCloseDisplay(display);
  std::puts("Bump GLX display-list and two-context lifecycle tests passed.");
}
