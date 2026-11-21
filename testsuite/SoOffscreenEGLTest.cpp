#include <Inventor/SoDB.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoSeparator.h>

#include <EGL/egl.h>

#include <cstdio>
#include <cstring>

int
main()
{
  SoDB::init();
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
  bool passed = false;
  {
    SoOffscreenRenderer renderer(SbViewportRegion(width, height));
    renderer.setComponents(SoOffscreenRenderer::RGB);
    renderer.setBackgroundColor(SbColor(0, 0, 0));
    if (renderer.render(root)) {
      const unsigned char * pixels = renderer.getBuffer();
      const int center = ((height / 2) * width + width / 2) * 3;
      passed = pixels && std::memcmp(pixels, pixels + center, 3) != 0;
    }
  }
  root->unref();
  if (!passed) std::fprintf(stderr, "EGL offscreen rendering did not produce the scene\n");
  const bool restored = eglQueryAPI() == previousapi &&
    eglGetCurrentContext() == EGL_NO_CONTEXT;
  if (!restored) std::fprintf(stderr, "EGL thread state changed after rendering\n");
  SoDB::finish();
  return passed && restored ? 0 : 1;
}
