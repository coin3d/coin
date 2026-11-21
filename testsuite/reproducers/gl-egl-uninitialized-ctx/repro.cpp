// Reproducer for a -Wuninitialized bug in eglglue_context_create_offscreen()
// (src/glue/gl_egl.cpp): the function used to read ctx->width / ctx->height
// to build its surface attribute array before ctx was assigned by
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

  // With the shim enabled, verify the dimensions passed to EGL.
  const char * outpath = getenv("EGL_SHIM_OUTFILE");
  if (outpath) {
    bool verified = false;
    FILE * f = fopen(outpath, "r");
    if (f) {
      int calls; long w, h;
      if (std::fscanf(f, "%d %ld %ld", &calls, &w, &h) == 3 && calls > 0) {
        std::fprintf(stderr, "[repro] shim captured EGL_WIDTH=%ld EGL_HEIGHT=%ld (expected %u %u)\n",
                     w, h, W, H);
        if (w != (long) W || h != (long) H) {
          std::fprintf(stderr, "[repro] FAIL: attrib list carried wrong width/height\n");
          fclose(f);
          if (ctx) eglglue_context_destruct(ctx);
          return 1;
        }
        verified = true;
      }
      fclose(f);
    }
    if (!verified) {
      std::fprintf(stderr, "[repro] INCONCLUSIVE: EGL_SHIM_OUTFILE was set but the shim "
                           "never recorded a call -- the intercepted entry point was never "
                           "reached in this environment, so the width/height fix was NOT "
                           "actually verified either way\n");
      if (ctx) eglglue_context_destruct(ctx);
      return 2;
    }
    if (ctx) eglglue_context_destruct(ctx);
    std::fprintf(stderr, "[repro] PASS\n");
    return 0;
  }

  // No shim requested (e.g. the plain "does it still crash" run without
  // LD_PRELOAD): ctx==NULL here means eglglue_context_create_offscreen()
  // didn't fully succeed (commonly an environment limitation, like no
  // real EGL-capable GPU), which the uninitialized-pointer bug this
  // reproducer targets says nothing about one way or the other -- that
  // bug is about *whether the surface_attrib[] construction crashes*,
  // which either already happened (we'd have crashed, not returned) or
  // didn't reach that code at all.
  if (!ctx) {
    std::fprintf(stderr, "[repro] INCONCLUSIVE: no context was created (no real "
                         "EGL-capable GPU in this environment?) -- not a pass or a fail\n");
    return 2;
  }

  eglglue_context_destruct(ctx);
  std::fprintf(stderr, "[repro] PASS\n");
  return 0;
}
