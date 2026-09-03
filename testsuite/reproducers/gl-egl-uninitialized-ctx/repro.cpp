// Reproducer for a -Wuninitialized bug in eglglue_context_create_offscreen()
// (src/glue/gl_egl.cpp): the function used to read ctx->width / ctx->height
// to build its EGLAttrib surface_attrib[] array before ctx was assigned by
// eglglue_contextdata_init(), dereferencing an indeterminate pointer.
//
// Calls the real, exported glue entry point directly (extern "C", not
// hidden), bypassing Coin's GLX/EGL runtime auto-selection (COIN_EGL env
// var / gl.cpp::check_egl()) entirely, so this is independent of which
// backend a given build would pick at runtime.
//
// See run.sh in this directory for how to build and run this against a
// given libCoin build.

#include <EGL/egl.h>
#include <cstdio>
#include <cstdlib>

extern "C" void * eglglue_context_create_offscreen(unsigned int width, unsigned int height);
extern "C" void   eglglue_context_destruct(void * ctx);

int main()
{
  const unsigned int W = 800, H = 600;

  std::fprintf(stderr, "[repro] calling eglglue_context_create_offscreen(%u, %u)\n", W, H);
  std::fflush(stderr);

  void * ctx = eglglue_context_create_offscreen(W, H);

  std::fprintf(stderr, "[repro] returned ctx=%p (did not crash)\n", ctx);

  // When run under LD_PRELOAD=./egl_shim.so, cross-check the width/height
  // that actually reached the EGL attribute list against what was
  // requested -- this is what would have been garbage (read through the
  // uninitialized ctx pointer) before the fix.
  const char * outpath = getenv("EGL_SHIM_OUTFILE");
  if (outpath) {
    FILE * f = fopen(outpath, "r");
    if (f) {
      int calls; long w, h;
      if (fscanf(f, "%d %ld %ld", &calls, &w, &h) == 3) {
        std::fprintf(stderr, "[repro] shim captured EGL_WIDTH=%ld EGL_HEIGHT=%ld (expected %u %u)\n",
                     w, h, W, H);
        if (w != (long) W || h != (long) H) {
          std::fprintf(stderr, "[repro] FAIL: attrib list carried wrong width/height\n");
          fclose(f);
          return 1;
        }
      }
      fclose(f);
    }
  }

  if (ctx) eglglue_context_destruct(ctx);
  std::fprintf(stderr, "[repro] PASS\n");
  return 0;
}
