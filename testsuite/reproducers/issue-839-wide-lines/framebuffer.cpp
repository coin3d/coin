#include <Inventor/SoDB.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoScale.h>

#include <EGL/egl.h>
#include <GL/gl.h>
#include <algorithm>
#include <cstdio>
#include <vector>

namespace {

constexpr short surfaceSize = 256;

struct Context {
  EGLDisplay display = EGL_NO_DISPLAY;
  EGLSurface surface = EGL_NO_SURFACE;
  EGLContext context = EGL_NO_CONTEXT;

  bool init()
  {
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display == EGL_NO_DISPLAY || !eglInitialize(display, nullptr, nullptr) ||
        !eglBindAPI(EGL_OPENGL_API)) return false;

    const EGLint configAttributes[] = {
      EGL_SURFACE_TYPE, EGL_PBUFFER_BIT, EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
      EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_DEPTH_SIZE, 24, EGL_NONE
    };
    EGLConfig config;
    EGLint count;
    if (!eglChooseConfig(display, configAttributes, &config, 1, &count) || count == 0) return false;
    const EGLint surfaceAttributes[] = { EGL_WIDTH, surfaceSize, EGL_HEIGHT, surfaceSize, EGL_NONE };
    surface = eglCreatePbufferSurface(display, config, surfaceAttributes);
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, nullptr);
    return surface != EGL_NO_SURFACE && context != EGL_NO_CONTEXT &&
      eglMakeCurrent(display, surface, surface, context);
  }

  ~Context()
  {
    if (display == EGL_NO_DISPLAY) return;
    eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    if (context != EGL_NO_CONTEXT) eglDestroyContext(display, context);
    if (surface != EGL_NO_SURFACE) eglDestroySurface(display, surface);
    eglTerminate(display);
  }
};

bool measure(SoGLRenderAction & action, bool indexed, bool perspective)
{
  auto * root = new SoSeparator;
  root->ref();
  root->renderCaching = SoSeparator::ON;
  SoCamera * camera = perspective ? static_cast<SoCamera *>(new SoPerspectiveCamera)
                                  : static_cast<SoCamera *>(new SoOrthographicCamera);
  camera->position.setValue(0, 0, 4);
  camera->nearDistance = 1;
  camera->farDistance = 10;
  root->addChild(camera);
  auto * lighting = new SoLightModel;
  lighting->model = SoLightModel::BASE_COLOR;
  root->addChild(lighting);
  auto * style = new SoDrawStyle;
  root->addChild(style);
  auto * scale = new SoScale;
  scale->scaleFactor.setValue(1.1f, 0.75f, 1.0f);
  root->addChild(scale);
  auto * coords = new SoCoordinate3;
  coords->point.set1Value(0, -0.65f, 0, 0);
  coords->point.set1Value(1, 0.65f, 0, perspective ? 1.0f : 0.0f);
  root->addChild(coords);
  if (indexed) {
    auto * lines = new SoIndexedLineSet;
    const int32_t indices[] = { 0, 1, -1 };
    lines->coordIndex.setValues(0, 3, indices);
    root->addChild(lines);
  }
  else {
    auto * lines = new SoLineSet;
    lines->numVertices.set1Value(0, 2);
    root->addChild(lines);
  }

  const int widths[] = { 0, 7, 10, 64 };
  const short heights[] = { surfaceSize, 160 };
  bool passed = true;
  for (int requested : widths) {
    style->lineWidth = float(requested);
    for (short height : heights) {
      action.setViewportRegion(SbViewportRegion(surfaceSize, height));
      glViewport(0, 0, surfaceSize, height);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      action.apply(root);

      std::vector<GLubyte> pixels(4 * height);
      glReadPixels(surfaceSize / 2, 0, 1, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
      int measured = 0;
      for (int y = 0; y < height; ++y) {
        if (pixels[4 * y] > 127) ++measured;
      }
      const GLenum error = glGetError();
      const int expected = std::max(1, requested);
      std::printf("%s %s %dx%d requested=%d measured=%d glerror=0x%x\n",
                  indexed ? "SoIndexedLineSet" : "SoLineSet",
                  perspective ? "perspective" : "orthographic",
                  surfaceSize, height, requested, measured, unsigned(error));
      if (measured != expected || error != GL_NO_ERROR) passed = false;
    }
  }
  root->unref();
  return passed;
}

} // namespace

int main()
{
  Context context;
  if (!context.init()) {
    std::printf("SKIP: no EGL OpenGL pbuffer (error 0x%x)\n", unsigned(eglGetError()));
    return 77;
  }

  GLfloat range[2] = {};
  glGetFloatv(GL_LINE_WIDTH_RANGE, range);
  std::printf("GL renderer: %s, simulated line width range [%g, %g]\n",
              reinterpret_cast<const char *>(glGetString(GL_RENDERER)), range[0], range[1]);
  if (range[1] != 7.0f) return 1;

  SoDB::init();
  bool passed = true;
  {
    SoGLRenderAction action(SbViewportRegion(surfaceSize, surfaceSize));
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    passed &= measure(action, false, false);
    passed &= measure(action, true, false);
    passed &= measure(action, false, true);
    passed &= measure(action, true, true);
  }
  SoDB::finish();
  return passed ? 0 : 1;
}
