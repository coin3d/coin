// Exercise the actual delayed transparent-path pass in a compatibility context.
#include "CoinTest.h"
#include "DepthCountingEngine.h"
#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <Inventor/SoDB.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/misc/SoContextHandler.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoDepthBufferElement.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoDepthBuffer.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoSeparator.h>

namespace {
struct TransparentProbe {
  DepthCountingEngine * engine;
  SoDepthBuffer * depth;
  int before, opaqueVisits, transparentVisits;
  SbBool delayedWrite;
  int nodeNotifications, fieldNotifications;
  SoNodeSensor nodeSensor;
  SoFieldSensor fieldSensor;
  static void notified(void * data, SoSensor *) { ++*static_cast<int *>(data); }
  SbBool defaultBefore;
  TransparentProbe(DepthCountingEngine * e, SoDepthBuffer * d, SbBool delayed)
    : engine(e), depth(d), before(0), opaqueVisits(0), transparentVisits(0),
      defaultBefore(FALSE), delayedWrite(delayed), nodeNotifications(0), fieldNotifications(0) {
    nodeSensor.setFunction(notified);
    nodeSensor.setData(&nodeNotifications);
    nodeSensor.setPriority(0);
    nodeSensor.attach(depth);
    fieldSensor.setFunction(notified);
    fieldSensor.setData(&fieldNotifications);
    // A priority-zero field sensor would itself evaluate the dirty field.
    fieldSensor.setPriority(1);
    fieldSensor.attach(&depth->write);
  }
  ~TransparentProbe() { fieldSensor.detach(); nodeSensor.detach(); }
  static void dirty(void * data, SoAction * action) {
    if (!action->isOfType(SoGLRenderAction::getClassTypeId())) return;
    TransparentProbe * p = static_cast<TransparentProbe *>(data);
    p->engine->dirty();
    p->before = p->engine->evaluations;
    p->depth->write.setDefault(TRUE);
    p->defaultBefore = p->depth->write.isDefault();
    p->fieldSensor.unschedule();
    p->nodeNotifications = p->fieldNotifications = 0;
  }
  static void observe(void * data, SoAction * action) {
    if (!action->isOfType(SoGLRenderAction::getClassTypeId())) return;
    TransparentProbe * p = static_cast<TransparentProbe *>(data);
    SoGLRenderAction * render = static_cast<SoGLRenderAction *>(action);
    const SbBool transparent = render->isRenderingTranspPaths();
    if (transparent) ++p->transparentVisits;
    else ++p->opaqueVisits;
    const int required = !transparent && !p->depth->write.isIgnored();
    BOOST_CHECK_EQUAL(p->engine->evaluations, p->before + required);
    SbBool test, write;
    SoDepthBufferElement::DepthWriteFunction function;
    SbVec2f range;
    SoDepthBufferElement::get(action->getState(), test, write, function, range);
    const SbBool expectedWrite = transparent ? p->delayedWrite : p->depth->write.isIgnored();
    BOOST_CHECK_EQUAL(write, expectedWrite);
    GLboolean glWrite = GL_FALSE;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &glWrite);
    BOOST_CHECK_EQUAL(glWrite != GL_FALSE, expectedWrite != FALSE);
    BOOST_CHECK(p->depth->write.isConnectionEnabled());
    BOOST_CHECK_EQUAL(p->depth->write.isDefault(), required ? FALSE : p->defaultBefore);
    SoEngineOutput * output = NULL;
    BOOST_CHECK(p->depth->write.getConnectedEngine(output));
    BOOST_REQUIRE(output == &p->engine->boolean);
    BOOST_CHECK(output->isEnabled());
    BOOST_CHECK_EQUAL(p->nodeNotifications, 0);
    BOOST_CHECK_EQUAL(p->fieldNotifications, 0);
    BOOST_CHECK(!p->fieldSensor.isScheduled());
  }
};
SoCallback * callback(SoCallbackCB * function, void * data) {
  SoCallback * node = new SoCallback;
  node->setCallback(function, data);
  return node;
}
}

BOOST_AUTO_TEST_CASE(depth_transparent_write_engine_is_lazy) {
  const SoGLRenderAction::TransparencyType modes[] = {
    SoGLRenderAction::DELAYED_BLEND, SoGLRenderAction::SORTED_OBJECT_BLEND
  };
  for (int mode = 0; mode != 2; ++mode) {
    for (int delayed = 0; delayed != 2; ++delayed) {
    for (int ignored = 0; ignored != 2; ++ignored) {
      SoSeparator * root = new SoSeparator;
      root->ref();
      root->renderCaching = SoSeparator::OFF;
      root->renderCulling = SoSeparator::OFF;
      SoOrthographicCamera * camera = new SoOrthographicCamera;
      camera->position.setValue(0, 0, 5);
      camera->nearDistance = 1;
      camera->farDistance = 10;
      root->addChild(camera);
      SoMaterial * material = new SoMaterial;
      material->transparency = .5f;
      root->addChild(material);
      SoDepthBuffer * parent = new SoDepthBuffer;
      parent->write = TRUE;
      root->addChild(parent);
      SoDepthBuffer * depth = new SoDepthBuffer;
      DepthCountingEngine * engine = new DepthCountingEngine;
      engine->ref();
      engine->booleanValue = FALSE;
      BOOST_REQUIRE(depth->write.connectFrom(&engine->boolean));
      depth->write.getValue(); // Prime, then dirty before each actual node visit.
      depth->write.setIgnored(ignored);
      depth->write.setDefault(TRUE);
      TransparentProbe probe(engine, depth, delayed);
      root->addChild(callback(TransparentProbe::dirty, &probe));
      root->addChild(depth);
      root->addChild(callback(TransparentProbe::observe, &probe));
      root->addChild(new SoCube);
      {
        SoGLRenderAction action(SbViewportRegion(16, 16));
        action.setCacheContext(87);
        action.setTransparencyType(modes[mode]);
        action.setDelayedObjDepthWrite(delayed);
        action.apply(root);
        action.apply(root);
      }
      BOOST_CHECK(probe.opaqueVisits >= 2);
      BOOST_CHECK(probe.transparentVisits >= 2);
      BOOST_CHECK_EQUAL(depth->write.isIgnored(), ignored != 0);
      probe.fieldSensor.detach();
      probe.nodeSensor.detach();
      root->unref();
      engine->unref();
    }
    }
  }
}

int main(int argc, char ** argv) {
  Display * display = XOpenDisplay(NULL);
  if (!display) { std::puts("SKIP: no X display"); return 77; }
  int attributes[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
  XVisualInfo * visual = glXChooseVisual(display, DefaultScreen(display), attributes);
  if (!visual) { XCloseDisplay(display); return 77; }
  GLXContext context = glXCreateContext(display, visual, NULL, True);
  if (!context) { XFree(visual); XCloseDisplay(display); return 77; }
  Colormap colormap = XCreateColormap(display, RootWindow(display, visual->screen),
                                     visual->visual, AllocNone);
  XSetWindowAttributes attributesWindow = {};
  attributesWindow.colormap = colormap;
  Window window = XCreateWindow(display, RootWindow(display, visual->screen),
    0, 0, 16, 16, 0, visual->depth, InputOutput, visual->visual,
    CWColormap, &attributesWindow);
  if (!glXMakeCurrent(display, window, context)) return 77;
  SoDB::init();
  DepthCountingEngine::initClass();
  const int result = CoinTest::run_all(argc, argv);
  SoContextHandler::destructingContext(87);
  SoDB::finish();
  glXMakeCurrent(display, None, NULL);
  glXDestroyContext(display, context);
  XDestroyWindow(display, window);
  XFreeColormap(display, colormap);
  XFree(visual);
  XCloseDisplay(display);
  return result;
}
