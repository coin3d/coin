#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/Xlib.h>

#include <Inventor/SoDB.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/misc/SoContextHandler.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>

#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <string>

#ifndef GL_FRAMEBUFFER_BINDING_EXT
#define GL_FRAMEBUFFER_BINDING_EXT 0x8CA6
#endif

namespace {
void check(bool ok, const char * what)
{
  if (!ok) { std::fprintf(stderr, "%s\n", what); std::abort(); }
}

struct Probe {
  SoGLRenderAction * action;
  SoCube * cube;
  bool throwpre, throwscene, throwtransparency, throwdelayed, delaycube;
  int precalls, scenecalls, transparencycalls, delayedcalls;
  GLint transparentfbo;
  Probe() : action(NULL), cube(NULL), throwpre(false), throwscene(false),
            throwtransparency(false), throwdelayed(false), delaycube(false),
            precalls(0), scenecalls(0), transparencycalls(0), delayedcalls(0),
            transparentfbo(-1) { }
};

void pre(void * data, SoGLRenderAction *)
{
  Probe & p = *static_cast<Probe *>(data);
  ++p.precalls;
  if (p.throwpre) throw std::runtime_error("pre-render failure");
}

void scene(void * data, SoAction *)
{
  Probe & p = *static_cast<Probe *>(data);
  ++p.scenecalls;
  if (p.throwscene) throw std::runtime_error("scene failure");
}

SoGLRenderAction::AbortCode abortRender(void * data)
{
  Probe & p = *static_cast<Probe *>(data);
  if (p.action->isRenderingTranspPaths()) {
    ++p.transparencycalls;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &p.transparentfbo);
    if (p.throwtransparency) throw std::runtime_error("transparent pass failure");
  }
  if (p.action->isRenderingDelayedPaths()) {
    ++p.delayedcalls;
    if (p.throwdelayed) throw std::runtime_error("delayed pass failure");
  }
  if (p.delaycube && p.action->getCurPathTail() == p.cube &&
      !p.action->isRenderingDelayedPaths()) {
    p.delaycube = false;
    return SoGLRenderAction::DELAY;
  }
  return SoGLRenderAction::CONTINUE;
}

template <typename F>
void expectFailure(F invoke, const char * message)
{
  bool caught = false;
  try { invoke(); }
  catch (const std::runtime_error & e) { caught = std::string(e.what()) == message; }
  check(caught, "render exception did not propagate");
}
}

int main()
{
  Display * display = XOpenDisplay(NULL);
  if (!display) return 77;
  int attributes[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
  XVisualInfo * visual = glXChooseVisual(display, DefaultScreen(display), attributes);
  if (!visual) { XCloseDisplay(display); return 77; }
  GLXContext glcontext = glXCreateContext(display, visual, NULL, True);
  if (!glcontext) { XFree(visual); XCloseDisplay(display); return 77; }
  Colormap colormap = XCreateColormap(display, RootWindow(display, visual->screen),
                                     visual->visual, AllocNone);
  XSetWindowAttributes attributeswindow = {};
  attributeswindow.colormap = colormap;
  Window window = XCreateWindow(display, RootWindow(display, visual->screen),
                                0, 0, 32, 32, 0, visual->depth, InputOutput,
                                visual->visual, CWColormap, &attributeswindow);
  check(glXMakeCurrent(display, window, glcontext), "GLX context activation failed");
  SoDB::init();
  {
    Probe p;
    SoSeparator * root = new SoSeparator;
    root->ref();
    root->renderCaching = SoSeparator::OFF;
    SoCallback * callback = new SoCallback;
    callback->setCallback(scene, &p);
    root->addChild(callback);
    SoMaterial * material = new SoMaterial;
    material->transparency.setValue(0.5f);
    root->addChild(material);
    p.cube = new SoCube;
    root->addChild(p.cube);
    SoGLRenderAction action(SbViewportRegion(32, 32));
    p.action = &action;
    action.setCacheContext(919);
    action.addPreRenderCallback(pre, &p);
    action.setAbortCallback(abortRender, &p);
    const int depth = action.getState()->getDepth();
    const int refs = root->getRefCount();

    p.throwpre = true;
    expectFailure([&]() { action.apply(root); }, "pre-render failure");
    p.throwpre = false;
    check(root->getRefCount() == refs, "pre-render root reference leaked");
    check(action.getState()->getDepth() == depth, "pre-render state depth changed");
    action.apply(root);
    check(p.precalls == 2, "pre-render callback was skipped after failure");

    p.throwscene = true;
    expectFailure([&]() { action.apply(root); }, "scene failure");
    p.throwscene = false;
    check(action.getState()->getDepth() == depth, "scene state depth changed");
    action.apply(root);
    check(p.precalls == 4, "scene failure left rendering flag active");

    action.setTransparencyType(SoGLRenderAction::DELAYED_BLEND);
    p.throwtransparency = true;
    expectFailure([&]() { action.apply(root); }, "transparent pass failure");
    p.throwtransparency = false;
    check(p.transparencycalls > 0, "transparent pass was not exercised");
    check(!action.isRenderingTranspPaths(), "transparent pass flag remained active");
    action.apply(root);

    p.delaycube = true;
    p.throwdelayed = true;
    expectFailure([&]() { action.apply(root); }, "delayed pass failure");
    p.throwdelayed = false;
    check(p.delayedcalls > 0, "delayed pass was not exercised");
    check(!action.isRenderingDelayedPaths(), "delayed pass flag remained active");
    action.apply(root);

    action.setTransparencyType(SoGLRenderAction::WEIGHTED_BLEND);
    GLint beforefbo = 0, afterfbo = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &beforefbo);
    p.throwtransparency = true;
    expectFailure([&]() { action.apply(root); }, "transparent pass failure");
    p.throwtransparency = false;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &afterfbo);
    if (p.transparentfbo > 0) {
      check(afterfbo == beforefbo, "WBOIT framebuffer binding not restored");
    }
    action.apply(root);
    root->unref();
    SoContextHandler::destructingContext(919);
  }
  SoDB::finish();
  glXMakeCurrent(display, None, NULL);
  glXDestroyContext(display, glcontext);
  XDestroyWindow(display, window);
  XFreeColormap(display, colormap);
  XFree(visual);
  XCloseDisplay(display);
  return 0;
}
