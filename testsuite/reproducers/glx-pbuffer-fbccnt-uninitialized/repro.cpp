// Reproducer/regression test for an uninitialized-variable read in
// glxglue_context_create_pbuffer() (src/glue/gl_glx.cpp).
//
// fbc_cnt, the "number of FBConfigs returned" output parameter passed
// to glXChooseFBConfig(), was declared without an initializer and is
// only ever written by that call. In this environment, running the
// same compiled binary repeatedly (and in particular under
// AddressSanitizer's stack redzone poisoning, which perturbs whatever
// garbage value happens to occupy that stack slot from one run/build
// to the next) sometimes leaves fbc_cnt holding a negative garbage
// value after the call returns, tripping assert(fbc_cnt >= 0) and
// aborting the process outright -- reproduced consistently (5/5) with
// one particular ASan build of the surrounding SoCallbackList
// function-pointer UB fix work, though not with others, confirming
// it depends on incidental stack contents rather than on any specific
// input. In a non-debug (NDEBUG) build the assert compiles out
// entirely and the garbage value would silently flow into the
// `fbc_cnt == 0` fallback check and the `fbc[0]` access just below.
//
// Fixed by initializing fbc_cnt = 0 at its declaration, so a
// (hypothetical, contract-violating) early return from
// glXChooseFBConfig() without writing *nelements leaves it at a safe,
// defined value instead of stack garbage.
//
// This drives many independent SoOffscreenRenderer::render() calls
// (each triggers a fresh context-creation attempt, which is what
// exercises glxglue_context_create_pbuffer() as part of probing
// hardware-accelerated offscreen rendering support) to maximize the
// chance of hitting whatever stack layout previously triggered the
// bug. All that's asserted here is that the process does not abort;
// render() itself is allowed to legitimately return FALSE depending
// on what GLX offscreen backends are available in a given
// environment (see somisc-fullpath-migration/repro.cpp for the
// COIN_GLX_PIXMAP_DIRECT_RENDERING environment variable this
// environment needs for the software-pixmap fallback to work at
// all).

#include <cstdlib>
#include <cstdio>
#include <Inventor/SoDB.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/SbViewportRegion.h>

int
main()
{
  setenv("COIN_GLX_PIXMAP_DIRECT_RENDERING", "1", 0);

  SoDB::init();

  SbViewportRegion vp(64, 64);

  SoSeparator * root = new SoSeparator;
  root->ref();
  SoPerspectiveCamera * camera = new SoPerspectiveCamera;
  root->addChild(camera);
  root->addChild(new SoDirectionalLight);
  SoCube * cube = new SoCube;
  root->addChild(cube);
  camera->viewAll(root, vp);

  const int iterations = 50;
  int successes = 0;
  for (int i = 0; i < iterations; i++) {
    SoOffscreenRenderer renderer(vp);
    if (renderer.render(root)) successes++;
  }

  root->unref();

  fprintf(stderr, "[repro] %d/%d SoOffscreenRenderer::render() calls succeeded "
                  "(process did not abort)\n", successes, iterations);
  fprintf(stderr, "[repro] PASS\n");
  return 0;
}
