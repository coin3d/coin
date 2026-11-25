#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <Inventor/SoDB.h>
#include <Inventor/C/glue/gl.h>
#include <Inventor/misc/SoContextHandler.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoCube.h>
#include "Texture2OverrideTestSupport.h"
#include <cstdio>
#include <cstdlib>

namespace {
void pixels(SoGLRenderAction & action) {
  using namespace Texture2OverrideTest;
  for (int empty = 0; empty < 2; ++empty) {
    Scene scene;
    SoOrthographicCamera * camera = new SoOrthographicCamera;
    camera->position.setValue(0,0,5); camera->height = 3;
    camera->nearDistance = 1; camera->farDistance = 10;
    scene.root->addChild(camera);
    SoLightModel * light = new SoLightModel; light->model = SoLightModel::BASE_COLOR;
    scene.root->addChild(light);
    SoMaterial * material = new SoMaterial; material->diffuseColor.setValue(1,1,1);
    scene.root->addChild(material);
    texture(scene.root, empty ? -1 : 80, true);
    texture(scene.root, 160); scene.root->addChild(new SoCube);
    glViewport(0,0,16,16); glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    action.apply(scene.root); glFinish();
    unsigned char pixel[3] = {0,0,0};
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(8,8,1,1,GL_RGB,GL_UNSIGNED_BYTE,pixel);
    const int expected[] = {empty ? 255 : 80, empty ? 255 : 20, empty ? 255 : 30};
    for (int i = 0; i < 3; ++i)
      require(std::abs(int(pixel[i]) - expected[i]) <= 4,
              "rendered pixel does not reflect winning texture/empty override");
    require(glGetError() == GL_NO_ERROR, "OpenGL error rendering texture override");
  }
}
}
int main() {
  Display * display = XOpenDisplay(NULL);
  if (!display) { std::puts("SKIP: Texture2 GLRender requires an X display"); return 77; }
  int attributes[] = {GLX_RGBA, GLX_DEPTH_SIZE, 16, None};
  XVisualInfo * visual = glXChooseVisual(display, DefaultScreen(display), attributes);
  if (!visual) { std::puts("SKIP: no compatible GLX visual"); XCloseDisplay(display); return 77; }
  GLXContext context = glXCreateContext(display, visual, NULL, True);
  if (!context) { std::puts("SKIP: cannot create GLX context"); XFree(visual); XCloseDisplay(display); return 77; }
  Colormap colormap = XCreateColormap(display, RootWindow(display, visual->screen), visual->visual, AllocNone);
  XSetWindowAttributes attributesWindow = {}; attributesWindow.colormap = colormap;
  Window window = XCreateWindow(display, RootWindow(display, visual->screen), 0,0,16,16,0,
                                visual->depth, InputOutput, visual->visual, CWColormap, &attributesWindow);
  if (!glXMakeCurrent(display, window, context)) {
    std::puts("SKIP: cannot make GLX context current");
    XDestroyWindow(display, window); XFreeColormap(display, colormap);
    glXDestroyContext(display, context); XFree(visual); XCloseDisplay(display); return 77;
  }
  int result = 0;
  SoDB::init();
  try {
    std::printf("Texture2 GL renderer: %s\n", glGetString(GL_RENDERER));
    SoGLRenderAction action(SbViewportRegion(16,16)); action.setCacheContext(901);
    const cc_glglue * glue = cc_glglue_instance(901);
    const bool multi = cc_glglue_max_texture_units(glue) >= 2;
    Texture2OverrideTest::run(action, multi, false);
    if (!multi) std::puts("SKIP: Texture2 nonzero GL units require at least two texture units");
    pixels(action);
    std::puts("PASS: Texture2 GLRender image/state, protection and pixels");
  }
  catch (const std::exception & error) {
    std::fprintf(stderr, "Texture2 GLRender failed: %s\n", error.what()); result = 1;
  }
  SoContextHandler::destructingContext(901);
  SoDB::finish();
  glXMakeCurrent(display, None, NULL); XDestroyWindow(display, window);
  XFreeColormap(display, colormap); glXDestroyContext(display, context);
  XFree(visual); XCloseDisplay(display); return result;
}
