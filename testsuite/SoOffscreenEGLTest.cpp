#include <Inventor/SoDB.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoSeparator.h>

#include <EGL/egl.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

static bool
renderScene(bool expectRender = true)
{
  SoSeparator * root = new SoSeparator;
  root->ref();
  SoOrthographicCamera * camera = new SoOrthographicCamera;
  camera->position.setValue(0, 0, 5);
  camera->height = 4;
  root->addChild(camera);
  root->addChild(new SoCube);

  constexpr int width = 32;
  constexpr int height = 24;
  const EGLenum previousapi = eglQueryAPI();
  const EGLDisplay previousdisplay = eglGetCurrentDisplay();
  const EGLContext previouscontext = eglGetCurrentContext();
  const EGLSurface previousdraw = eglGetCurrentSurface(EGL_DRAW);
  const EGLSurface previousread = eglGetCurrentSurface(EGL_READ);
  bool passed = false;
  {
    SoOffscreenRenderer renderer(SbViewportRegion(width, height));
    renderer.setComponents(SoOffscreenRenderer::RGB);
    renderer.setBackgroundColor(SbColor(0, 0, 0));
    const bool rendered = renderer.render(root);
    if (rendered && expectRender) {
      const unsigned char * pixels = renderer.getBuffer();
      const int center = ((height / 2) * width + width / 2) * 3;
      passed = pixels && std::memcmp(pixels, pixels + center, 3) != 0;
    } else {
      passed = !rendered && !expectRender;
    }
  }
  root->unref();
  if (!passed) {
    std::fprintf(stderr, "%s\n", expectRender ? "EGL offscreen rendering did not produce the scene" :
                 "Expected EGL offscreen rendering to fail with pbuffers disabled");
  }
  const bool restored = eglQueryAPI() == previousapi &&
    eglGetCurrentDisplay() == previousdisplay &&
    eglGetCurrentContext() == previouscontext &&
    eglGetCurrentSurface(EGL_DRAW) == previousdraw &&
    eglGetCurrentSurface(EGL_READ) == previousread;
  if (!restored) std::fprintf(stderr, "EGL thread state changed after rendering\n");
  return passed && restored;
}

static int
renderAfterFailedProbe()
{
  if (setenv("COIN_EGLGLUE_NO_PBUFFERS", "1", 1) != 0) return 1;
  const bool failedAsExpected = renderScene(false);
  if (unsetenv("COIN_EGLGLUE_NO_PBUFFERS") != 0 || !failedAsExpected) return 1;

  const EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  const EGLint attribs[] = {
    EGL_SURFACE_TYPE, EGL_PBUFFER_BIT, EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT, EGL_NONE
  };
  const EGLint size[] = {EGL_WIDTH, 64, EGL_HEIGHT, 64, EGL_NONE};
  EGLConfig config;
  EGLint count;
  if (!eglInitialize(display, nullptr, nullptr) ||
      !eglChooseConfig(display, attribs, &config, 1, &count) || count < 1 ||
      !eglBindAPI(EGL_OPENGL_API)) {
    std::fprintf(stderr, "No software EGL pbuffer configuration for this test\n");
    return 77;
  }
  const EGLContext context = eglCreateContext(display, config, EGL_NO_CONTEXT, nullptr);
  const EGLSurface surface = eglCreatePbufferSurface(display, config, size);
  const bool current = context != EGL_NO_CONTEXT && surface != EGL_NO_SURFACE &&
    eglMakeCurrent(display, surface, surface, context);
  if (!current) std::fprintf(stderr, "Could not activate the application's EGL context\n");
  const bool passed = current && renderScene();
  if (current) eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
  if (surface != EGL_NO_SURFACE) eglDestroySurface(display, surface);
  if (context != EGL_NO_CONTEXT) eglDestroyContext(display, context);
  eglTerminate(display);
  return passed ? 0 : 1;
}

int
main(int argc, char ** argv)
{
  SoDB::init();
  if (eglGetCurrentContext() != EGL_NO_CONTEXT) {
    std::fprintf(stderr, "Expected the test to start without an EGL context\n");
    SoDB::finish();
    return 1;
  }
  const int result = argc > 1 && std::strcmp(argv[1], "after-failed-probe") == 0 ?
    renderAfterFailedProbe() : (renderScene() ? 0 : 1);
  SoDB::finish();
  return result;
}
