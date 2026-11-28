#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <Inventor/SoDB.h>
#include <Inventor/SbColor4f.h>
#include <Inventor/SoRenderManager.h>
#include <Inventor/SoPath.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoDepthBufferElement.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoDepthBuffer.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/misc/SoState.h>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <string>

namespace {
int failures = 0;
const char * casename = "";
void check(bool ok, const char * expression, int line) {
  if (!ok) { std::fprintf(stderr, "%s:%d: %s\n", casename, line, expression); ++failures; }
}
#define CHECK(x) check(!!(x), #x, __LINE__)
const int size = 32;
const uint32_t contextid = 1881;
struct Context {
  Display * display;
  XVisualInfo * visual;
  GLXContext context;
  Colormap colormap;
  Window window;
  Context() : display(XOpenDisplay(NULL)), visual(NULL), context(NULL), colormap(0), window(0) {
    if (!display) return;
    int attributes[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, None};
    visual = glXChooseVisual(display, DefaultScreen(display), attributes);
    if (!visual) return;
    context = glXCreateContext(display, visual, NULL, True);
    if (!context) return;
    colormap = XCreateColormap(display, RootWindow(display, visual->screen), visual->visual, AllocNone);
    XSetWindowAttributes attr = {}; attr.colormap = colormap;
    window = XCreateWindow(display, RootWindow(display, visual->screen), 0, 0, size, size, 0,
                           visual->depth, InputOutput, visual->visual, CWColormap, &attr);
    if (!glXMakeCurrent(display, window, context)) { window = 0; return; }
    glViewport(0, 0, size, size);
  }
  ~Context() {
    if (context) { glXMakeCurrent(display, None, NULL); glXDestroyContext(display, context); }
    if (window) XDestroyWindow(display, window);
    if (colormap) XFreeColormap(display, colormap);
    if (visual) XFree(visual);
    if (display) XCloseDisplay(display);
  }
  bool valid() const { return window != 0; }
};
void seed(bool enabled, float depth = .1f) {
  glDisable(GL_SCISSOR_TEST); glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glDepthMask(GL_TRUE); glClearColor(1, 0, 0, 1); glClearDepth(depth);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDepthFunc(GL_LEQUAL); glDepthRange(0, 1);
  if (enabled) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
}
void pixel(bool green, bool blended = false) {
  unsigned char rgb[3] = {}; glReadPixels(size/2, size/2, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, rgb);
  if (blended && green) { CHECK(rgb[0] >= 120 && rgb[0] <= 136); CHECK(rgb[1] >= 120 && rgb[1] <= 136); }
  else { CHECK(rgb[0] == (green ? 0 : 255)); CHECK(rgb[1] == (green ? 255 : 0)); }
  CHECK(rgb[2] == 0);
}
float depthpixel() {
  float value = 0; glReadPixels(size/2, size/2, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &value); return value;
}
void quad(float alpha = 1) {
  glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
  glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
  glDisable(GL_LIGHTING); glDisable(GL_TEXTURE_2D);
  glColor4f(0, 1, 0, alpha);
  glBegin(GL_QUADS);
  glVertex3f(-1,-1,0); glVertex3f(1,-1,0); glVertex3f(1,1,0); glVertex3f(-1,1,0);
  glEnd(); glPopMatrix(); glMatrixMode(GL_PROJECTION); glPopMatrix(); glMatrixMode(GL_MODELVIEW);
}
struct Probe {
  bool test, write;
  SoDepthBufferElement::DepthWriteFunction function;
  float nearval, farval;
  int visits, transparentvisits, delayedvisits, listed;
  int throws = 0, thrownPhase = -1;
  bool retainPath = false;
  SoPath *retainedPath = nullptr;
  bool draw, transparent, delayed, throwing, inspect;
  Probe(bool t = false) : test(t), write(true), function(SoDepthBufferElement::LEQUAL),
    nearval(.2f), farval(.8f), visits(0), transparentvisits(0), delayedvisits(0), listed(0),
    draw(true), transparent(false), delayed(false), throwing(false), inspect(true) {}
};
void callback(void * data, SoAction * generic) {
  if (!generic->isOfType(SoGLRenderAction::getClassTypeId())) return;
  SoGLRenderAction * action = static_cast<SoGLRenderAction *>(generic);
  Probe & p = *static_cast<Probe *>(data);
  if (p.delayed && !action->isRenderingDelayedPaths()) { action->addDelayedPath(action->getCurPath()->copy()); return; }
  if (p.transparent && action->handleTransparency(TRUE)) return;
  ++p.visits;
  if (p.retainPath && !p.retainedPath) {
    p.retainedPath = action->getCurPath()->copy();
    p.retainedPath->ref();
  }
  if (action->isRenderingTranspPaths()) ++p.transparentvisits;
  if (action->isRenderingDelayedPaths()) ++p.delayedvisits;
  GLint list = 0; glGetIntegerv(GL_LIST_INDEX, &list); if (list) ++p.listed;
  if (p.inspect) {
    const bool expectedwrite = action->isRenderingTranspPaths() ? p.write && action->getDelayedObjDepthWrite() : p.write;
    CHECK(!!SoDepthBufferElement::getTestEnable(action->getState()) == p.test);
    CHECK(!!glIsEnabled(GL_DEPTH_TEST) == p.test);
    CHECK(!!SoDepthBufferElement::getWriteEnable(action->getState()) == expectedwrite);
    GLboolean write = GL_FALSE; glGetBooleanv(GL_DEPTH_WRITEMASK, &write); CHECK(!!write == expectedwrite);
    CHECK(SoDepthBufferElement::getFunction(action->getState()) == p.function);
    GLint function = 0; glGetIntegerv(GL_DEPTH_FUNC, &function);
    CHECK(function == (p.function == SoDepthBufferElement::ALWAYS ? GL_ALWAYS : GL_LEQUAL));
    SbVec2f range = SoDepthBufferElement::getRange(action->getState());
    CHECK(std::fabs(range[0] - p.nearval) < 1e-5f); CHECK(std::fabs(range[1] - p.farval) < 1e-5f);
    GLdouble glrange[2]; glGetDoublev(GL_DEPTH_RANGE, glrange);
    CHECK(std::fabs(glrange[0] - p.nearval) < 1e-5); CHECK(std::fabs(glrange[1] - p.farval) < 1e-5);
  }
#ifdef COIN_SUPER_TEST_EXCEPTIONS
  if (p.throwing) {
    ++p.throws;
    p.thrownPhase = action->isRenderingDelayedPaths() ? 1 :
      action->isRenderingTranspPaths() ? 2 : 0;
    // Enqueue/handleTransparency and state inspection above must finish
    // before the exceptional branch can execute in its requested pass.
    CHECK(p.thrownPhase == (p.delayed ? 1 : p.transparent ? 2 : 0));
    throw std::runtime_error("intentional traversal failure");
  }
#endif
  if (p.draw) {
    if (p.transparent) { glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); }
    else glDisable(GL_BLEND);
    quad(p.transparent ? .5f : 1.0f);
  }
}
SoCallback * node(Probe & p) { SoCallback * n = new SoCallback; n->setCallback(callback, &p); return n; }
SoDepthBuffer * inheritedRange() {
  SoDepthBuffer * d = new SoDepthBuffer;
  d->test.setIgnored(TRUE); d->write.setIgnored(TRUE); d->function.setIgnored(TRUE);
  d->range.setValue(.2f, .8f); return d;
}
SoSeparator * scene(Probe & p, int explicitTest = -1) {
  SoSeparator * root = new SoSeparator; root->ref(); root->renderCaching = SoSeparator::OFF;
  SoDepthBuffer * d = inheritedRange();
  if (explicitTest >= 0) { d->test.setIgnored(FALSE); d->test = explicitTest ? TRUE : FALSE; }
  root->addChild(d);
  SoSeparator * branch = new SoSeparator; branch->renderCaching = SoSeparator::OFF;
  branch->addChild(node(p)); root->addChild(branch); return root;
}
void setup(SoGLRenderAction & action, SoRenderManager & manager) {
  action.setCacheContext(contextid); manager.setGLRenderAction(&action);
  manager.setViewportRegion(SbViewportRegion(size, size));
}
void plain_after(SoGLRenderAction & action) {
  Probe regular(true); regular.nearval = 0; regular.farval = 1; regular.draw = false;
  SoSeparator * root = new SoSeparator; root->ref(); root->renderCaching = SoSeparator::OFF;
  root->addChild(node(regular)); action.apply(root); CHECK(regular.visits == 1); root->unref();
}
void defaults(int flag, int explicitTest) {
  Probe p(explicitTest < 0 ? !!flag : !!explicitTest);
  SoSeparator * root = scene(p, explicitTest);
  Probe entry(!!flag); entry.draw = false; entry.nearval = 0; entry.farval = 1;
  root->insertChild(node(entry), 0);
  SoDepthBuffer * child = static_cast<SoDepthBuffer *>(root->getChild(1));
  SoGLRenderAction action(SbViewportRegion(size, size)); SoRenderManager manager; setup(action, manager);
  {
    SoRenderManager::Superimposition overlay(root, TRUE, &manager, flag);
    for (int initial = 0; initial < 2; ++initial) {
      seed(!!initial); overlay.render(&action);
      CHECK(!!glIsEnabled(GL_DEPTH_TEST) == !!initial);
      pixel(!p.test); CHECK(std::fabs(depthpixel() - .1f) < 1e-5f);
    }
    CHECK(p.visits == 2); CHECK(entry.visits == 2);
    CHECK(child->test.isIgnored() == (explicitTest < 0));
  }
  plain_after(action); root->unref();
}
void native_tuple() {
  Probe p(false); p.write = false; p.function = SoDepthBufferElement::ALWAYS;
  SoSeparator * root = scene(p);
  SoDepthBuffer * prefix = new SoDepthBuffer;
  prefix->test.setIgnored(TRUE); prefix->write = FALSE; prefix->function = SoDepthBuffer::ALWAYS;
  prefix->range.setValue(.3f,.7f); root->insertChild(prefix, 0);
  SoGLRenderAction action(SbViewportRegion(size,size)); SoRenderManager manager; setup(action,manager);
  { SoRenderManager::Superimposition overlay(root, TRUE, &manager, 0);
    seed(true); overlay.render(&action); pixel(true); CHECK(std::fabs(depthpixel()-.1f)<1e-5f); }
  root->unref();
}
void passes(bool test, bool writes) {
  Probe transparent(test), delayed(test); transparent.transparent = true; delayed.delayed = true;
  delayed.draw = false;
  SoSeparator * root = scene(transparent);
  SoSeparator * branch = new SoSeparator; branch->renderCaching = SoSeparator::OFF;
  branch->addChild(node(delayed)); root->addChild(branch);
  SoGLRenderAction action(SbViewportRegion(size,size)); SoRenderManager manager; setup(action,manager);
  action.setTransparencyType(SoGLRenderAction::SCREEN_DOOR); action.setDelayedObjDepthWrite(writes);
  { SoRenderManager::Superimposition overlay(root, TRUE, &manager, test ? 1 : 0);
    overlay.setTransparencyType(SoGLRenderAction::DELAYED_BLEND);
    seed(true); overlay.render(&action);
    CHECK(action.getTransparencyType() == SoGLRenderAction::SCREEN_DOOR);
    CHECK(transparent.transparentvisits == 1); CHECK(delayed.delayedvisits == 1);
    pixel(!test, true); CHECK(std::fabs(depthpixel()-.1f)<1e-5f);
  }
  plain_after(action); root->unref();
}
void clear_disabled() {
  Probe p(true); p.draw = false; SoSeparator * root = scene(p);
  SoGLRenderAction action(SbViewportRegion(size,size)); SoRenderManager manager; setup(action,manager);
  { SoRenderManager::Superimposition overlay(root, FALSE, &manager, 3);
    overlay.setTransparencyType(SoGLRenderAction::DELAYED_BLEND);
    action.setTransparencyType(SoGLRenderAction::SCREEN_DOOR);
    seed(false); overlay.render(&action, TRUE); pixel(false);
    CHECK(std::fabs(depthpixel()-.1f)<1e-5f); CHECK(!glIsEnabled(GL_DEPTH_TEST)); CHECK(p.visits == 0);
    CHECK(action.getTransparencyType() == SoGLRenderAction::SCREEN_DOOR);
    overlay.setEnabled(TRUE); glClearColor(0,0,0,1); glClearDepth(.75);
    overlay.render(&action, TRUE);
    unsigned char rgb[3]; glReadPixels(size/2,size/2,1,1,GL_RGB,GL_UNSIGNED_BYTE,rgb);
    CHECK(rgb[0] == 0 && rgb[1] == 0 && rgb[2] == 0);
    CHECK(std::fabs(depthpixel()-.75f)<1e-5f); CHECK(p.visits == 1); CHECK(!glIsEnabled(GL_DEPTH_TEST));
  }
  root->unref();
}
void cache_shared() {
  Probe p(false); p.inspect = false;
  SoSeparator * root = new SoSeparator; root->ref(); root->renderCaching = SoSeparator::OFF;
  SoSeparator * cached = new SoSeparator; cached->renderCaching = SoSeparator::ON; root->addChild(cached);
  cached->addChild(inheritedRange()); cached->addChild(node(p));
  SoGLRenderAction action(SbViewportRegion(size,size)); SoRenderManager manager; setup(action,manager);
  { SoRenderManager::Superimposition off(root,TRUE,&manager,0), on(root,TRUE,&manager,1);
    for (int i=0;i<6;++i) { seed(true); off.render(&action); pixel(true); }
    CHECK(p.listed > 0); int visits = p.visits;
    seed(true); off.render(&action); pixel(true); CHECK(p.visits == visits);
    for (int i=0;i<4;++i) { seed(false); on.render(&action); pixel(false); }
    for (int i=0;i<4;++i) { seed(true); off.render(&action); pixel(true); }
    CHECK(root->getNumChildren() == 1); CHECK(cached->getNumChildren() == 2);
    CHECK(static_cast<SoDepthBuffer *>(cached->getChild(0))->test.isIgnored());
  }
  root->unref();
}
void manager_flow(bool background) {
  Probe p(false); SoSeparator * root = scene(p);
  SoSeparator * empty = new SoSeparator; empty->ref();
  SoGLRenderAction action(SbViewportRegion(size,size)); SoRenderManager manager; setup(action,manager);
  manager.setSceneGraph(empty);
  manager.addSuperimposition(root, background ? SoRenderManager::Superimposition::BACKGROUND : 0);
  seed(true); manager.render(&action,TRUE,FALSE,FALSE); pixel(true); CHECK(p.visits == 1);
  manager.setSceneGraph(NULL); root->unref(); empty->unref();
}
struct AbortProbe {
  SoGLRenderAction *action = nullptr;
  SoNode *target = nullptr;
  bool enabled = true;
  int phase = 0, calls = 0, stops = 0;
  static SoGLRenderAction::AbortCode callback(void *data) {
    auto &p = *static_cast<AbortProbe *>(data);
    ++p.calls;
    const bool phase = p.phase == 0 ||
      (p.phase == 1 && p.action->isRenderingDelayedPaths()) ||
      (p.phase == 2 && p.action->isRenderingTranspPaths());
    if (p.enabled && phase && p.action->getCurPathTail() == p.target) {
      ++p.stops;
      return SoGLRenderAction::ABORT;
    }
    return SoGLRenderAction::CONTINUE;
  }
};
void callbackRestored(SoGLRenderAction &action, AbortProbe &abort) {
  SoGLRenderAction::SoGLRenderAbortCB *func = nullptr;
  void *data = nullptr;
  action.getAbortCallback(func, data);
  CHECK(func == AbortProbe::callback); CHECK(data == &abort);
}
void abort_reuse(int phase) {
  Probe p(false); p.delayed = phase == 1; p.transparent = phase == 2;
  SoSeparator *root = scene(p);
  SoGLRenderAction action(SbViewportRegion(size,size));
  SoRenderManager manager; setup(action,manager);
  AbortProbe abort; abort.action = &action; abort.phase = phase;
  abort.target = static_cast<SoSeparator *>(root->getChild(1))->getChild(0);
  action.setAbortCallback(AbortProbe::callback, &abort);
  action.setTransparencyType(SoGLRenderAction::SCREEN_DOOR);
  const int stack = action.getState()->getDepth();
  const int refs = root->getRefCount();
  {
    SoRenderManager::Superimposition overlay(root,TRUE,&manager,0);
    overlay.setTransparencyType(SoGLRenderAction::DELAYED_BLEND);
    for (int initial = 0; initial < 2; ++initial) {
      abort.enabled = true;
      const int stopped = abort.stops;
      seed(!!initial); overlay.render(&action);
      CHECK(abort.stops == stopped + 1);
      CHECK(!!glIsEnabled(GL_DEPTH_TEST) == !!initial);
      CHECK(action.getTransparencyType() == SoGLRenderAction::SCREEN_DOOR);
      CHECK(action.getState()->getDepth() == stack);
      CHECK(root->getRefCount() == refs + 1);
      callbackRestored(action,abort);
      abort.enabled = false;
      const int visits = p.visits;
      seed(!!initial); overlay.render(&action);
      CHECK(p.visits == visits + 1);
      CHECK(!!glIsEnabled(GL_DEPTH_TEST) == !!initial);
      CHECK(action.getTransparencyType() == SoGLRenderAction::SCREEN_DOOR);
      pixel(true, p.transparent);
      CHECK(std::fabs(depthpixel()-.1f)<1e-5f);
      callbackRestored(action,abort);
    }
  }
  CHECK(root->getRefCount() == refs);
  callbackRestored(action,abort);
  action.setAbortCallback(nullptr,nullptr);
  plain_after(action); root->unref();
}
void pre_count(void *data, SoGLRenderAction *) { ++*static_cast<int *>(data); }
void callback_lifetime() {
  SoGLRenderAction action(SbViewportRegion(size,size));
  SoRenderManager manager; setup(action,manager);
  int pre = 0;
  action.addPreRenderCallback(pre_count,&pre);
  for (int iteration=0; iteration<6; ++iteration) {
    Probe p(false); p.draw=false;
    SoSeparator *root=scene(p);
    {
      SoRenderManager::Superimposition overlay(root,TRUE,&manager,0);
      seed(true); overlay.render(&action); CHECK(p.visits==1);
    }
    root->unref();
    const int before=pre;
    // Uses the same action after the overlay and all overlay-owned graph
    // storage have gone away. Temporary callbacks must not run here.
    plain_after(action); CHECK(pre==before+1);
  }
  action.removePreRenderCallback(pre_count,&pre);
  const int before=pre;
  plain_after(action); CHECK(pre==before);
}
void retained_path_lifetime() {
  Probe p(false); p.retainPath=true;
  SoSeparator *root=scene(p);
  SoGLRenderAction action(SbViewportRegion(size,size));
  SoRenderManager manager; setup(action,manager);
  action.setTransparencyType(SoGLRenderAction::SCREEN_DOOR);
  {
    SoRenderManager::Superimposition overlay(root,TRUE,&manager,0);
    overlay.setTransparencyType(SoGLRenderAction::DELAYED_BLEND);
    seed(true); overlay.render(&action);
    CHECK(p.visits==1); CHECK(p.retainedPath!=nullptr);
    CHECK(glIsEnabled(GL_DEPTH_TEST));
    CHECK(action.getTransparencyType()==SoGLRenderAction::SCREEN_DOOR);
    pixel(true); CHECK(std::fabs(depthpixel()-.1f)<1e-5f);
  }
  // Only the public retained path now owns the traversed graph. Its next
  // traversal must use ordinary action defaults after the overlay scope.
  root->unref();
  p.test=true;
  if (p.retainedPath) {
    for (int iteration=0; iteration<2; ++iteration) {
      const int visits=p.visits;
      seed(true); action.apply(p.retainedPath);
      CHECK(p.visits==visits+1);
      CHECK(glIsEnabled(GL_DEPTH_TEST));
      CHECK(action.getTransparencyType()==SoGLRenderAction::SCREEN_DOOR);
      pixel(false); CHECK(std::fabs(depthpixel()-.1f)<1e-5f);
    }
    p.retainedPath->unref(); p.retainedPath=nullptr;
  }
  plain_after(action);
}
void clear_color() {
  Probe p(false); p.draw=false; SoSeparator *root=scene(p);
  SoGLRenderAction action(SbViewportRegion(size,size));
  SoRenderManager manager; setup(action,manager);
  manager.setBackgroundColor(SbColor4f(0,0,1,1));
  // Clear only color: an overlay without CLEAR_ZBUFFER must retain depth.
  {
    SoRenderManager::Superimposition overlay(root,TRUE,&manager,0);
    seed(true); glClearColor(0,0,1,1); glClearDepth(.75);
    overlay.render(&action,TRUE);
    unsigned char rgb[3]={}; glReadPixels(size/2,size/2,1,1,GL_RGB,GL_UNSIGNED_BYTE,rgb);
    CHECK(rgb[0]==0 && rgb[1]==0 && rgb[2]==255);
    CHECK(std::fabs(depthpixel()-.1f)<1e-5f);
    CHECK(glIsEnabled(GL_DEPTH_TEST)); CHECK(p.visits==1);
  }
  root->unref();
}
#ifdef COIN_SUPER_TEST_EXCEPTIONS
void exception_cleanup(int phase) {
  Probe p(false); p.delayed = phase == 1; p.transparent = phase == 2;
  SoSeparator * root = scene(p);
  SoGLRenderAction action(SbViewportRegion(size,size)); SoRenderManager manager; setup(action,manager);
  AbortProbe abort; abort.action=&action; abort.enabled=false;
  action.setAbortCallback(AbortProbe::callback,&abort);
  int pre=0; action.addPreRenderCallback(pre_count,&pre);
  action.setTransparencyType(SoGLRenderAction::SCREEN_DOOR);
  const int stack=action.getState()->getDepth(), refs=root->getRefCount();
  {
    SoRenderManager::Superimposition overlay(root,TRUE,&manager,0);
    overlay.setTransparencyType(SoGLRenderAction::DELAYED_BLEND);
    for (int initial=0; initial<2; ++initial) {
      p.throwing=true; seed(!!initial);
      const int thrown=p.throws;
      p.thrownPhase=-1;
      bool caught=false;
      try { overlay.render(&action); }
      catch (const std::runtime_error &e) { caught=std::string(e.what())=="intentional traversal failure"; }
      CHECK(caught); CHECK(p.throws==thrown+1); CHECK(p.thrownPhase==phase);
      CHECK(!!glIsEnabled(GL_DEPTH_TEST)==!!initial);
      CHECK(action.getTransparencyType()==SoGLRenderAction::SCREEN_DOOR);
      CHECK(action.getState()->getDepth()==stack); CHECK(root->getRefCount()==refs+1);
      CHECK(!action.isRenderingTranspPaths()); CHECK(!action.isRenderingDelayedPaths());
      callbackRestored(action,abort);
      p.throwing=false;
      const int visits=p.visits;
      seed(!!initial); overlay.render(&action);
      CHECK(p.visits==visits+1); CHECK(!!glIsEnabled(GL_DEPTH_TEST)==!!initial);
      CHECK(action.getTransparencyType()==SoGLRenderAction::SCREEN_DOOR);
      pixel(true,p.transparent); CHECK(std::fabs(depthpixel()-.1f)<1e-5f);
      callbackRestored(action,abort);
    }
  }
  CHECK(root->getRefCount() == refs);
  root->unref();
  const int before=pre; plain_after(action); CHECK(pre==before+1);
  callbackRestored(action,abort);
  action.setAbortCallback(nullptr,nullptr);
  action.removePreRenderCallback(pre_count,&pre);
  plain_after(action); CHECK(pre==before+1);
}
#endif
}
int main(int argc, char ** argv) {
  Context context; if (!context.valid()) { std::puts("SKIP: native GLX context unavailable"); return 77; }
  SoDB::init(); std::printf("GL renderer: %s\n",glGetString(GL_RENDERER));
  if (argc != 2) return 2;
  casename = argv[1]; std::string name(casename);
  if (name == "off") defaults(0,-1);
  else if (name == "on") defaults(1,-1);
  else if (name == "explicit_on") defaults(0,1);
  else if (name == "explicit_off") defaults(1,0);
  else if (name == "native_tuple") native_tuple();
  else if (name == "transparent_off") passes(false,false);
  else if (name == "transparent_on") passes(true,false);
  else if (name == "transparent_writes") passes(false,true);
  else if (name == "clear_disabled") clear_disabled();
  else if (name == "cache_shared") cache_shared();
  else if (name == "foreground") manager_flow(false);
  else if (name == "background") manager_flow(true);
  else if (name == "abort_reuse") abort_reuse(0);
  else if (name == "delayed_abort_reuse") abort_reuse(1);
  else if (name == "transparent_abort_reuse") abort_reuse(2);
  else if (name == "callback_lifetime") callback_lifetime();
  else if (name == "retained_path_lifetime") retained_path_lifetime();
  else if (name == "clear_color") clear_color();
#ifdef COIN_SUPER_TEST_EXCEPTIONS
  else if (name == "exception_cleanup") exception_cleanup(0);
  else if (name == "delayed_exception_reuse") exception_cleanup(1);
  else if (name == "transparent_exception_reuse") exception_cleanup(2);
#endif
  else return 2;
  CHECK(glGetError() == GL_NO_ERROR);
  return failures ? 1 : 0;
}
