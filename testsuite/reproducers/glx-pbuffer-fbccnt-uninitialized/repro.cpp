// Compile the real GLX implementation into this standalone test so its
// private pbuffer entry point and GLX function pointers can be exercised
// without adding a test-only symbol to libCoin. See README.md.
#include "glue/gl_glx.cpp"

#ifndef HAVE_GLX
#error This regression test requires a Coin build with GLX enabled.
#else

static int choose_calls = 0;
static int pbuffer_calls = 0;
static int context_calls = 0;
static int mode = 0;
static char config_token;
static char context_token;

static COIN_GLXFBConfig * APIENTRY
mock_choose(Display *, int, const int *, int * count)
{
  ++choose_calls;
  if (mode == 0) return NULL; // Deliberately leave count unwritten.
  *count = mode == 1 ? 0 : 1;
  if (mode != 3) return NULL;
  COIN_GLXFBConfig * configs =
    static_cast<COIN_GLXFBConfig *>(std::malloc(sizeof(COIN_GLXFBConfig)));
  if (!configs) std::abort();
  configs[0] = &config_token;
  return configs; // The implementation releases this with XFree().
}

static COIN_GLXPbuffer APIENTRY
mock_pbuffer(Display *, COIN_GLXFBConfig config, const int *)
{
  ++pbuffer_calls;
  if (config != &config_token) std::abort();
  return 42;
}

static GLXContext APIENTRY
mock_context(Display *, COIN_GLXFBConfig config, int, GLXContext, Bool)
{
  ++context_calls;
  if (config != &config_token) std::abort();
  return reinterpret_cast<GLXContext>(&context_token);
}

int
main()
{
  // Only DefaultScreen() reads this structure. No mock accesses an X server.
  _XPrivDisplay display = static_cast<_XPrivDisplay>(std::calloc(1, sizeof(*display)));
  if (!display) return 2;
  glxglue_display = reinterpret_cast<Display *>(display);
  glxglue_glXChooseFBConfig = mock_choose;
  glxglue_glXCreatePbuffer_GLX_1_3 = mock_pbuffer;
  glxglue_glXCreateNewContext = mock_context;

  int failures = 0;
  for (mode = 0; mode < 4; ++mode) {
    choose_calls = pbuffer_calls = context_calls = 0;
    glxglue_contextdata context = {};
    context.width = context.height = 64;
    const SbBool result = glxglue_context_create_pbuffer(&context);
    const bool success = mode == 3;
    bool valid = (result == (success ? TRUE : FALSE)) && choose_calls == 1 &&
      pbuffer_calls == (success ? 1 : 0) && context_calls == (success ? 1 : 0);
    if (success) {
      valid = valid && context.fbconfig == &config_token &&
        context.glxcontext == reinterpret_cast<GLXContext>(&context_token) &&
        context.glxpixmap == 42 && context.pbuffer == TRUE &&
        context.display == glxglue_display;
    }
    std::fprintf(stderr, "case=%d result=%d chooser=%d pbuffer=%d context=%d: %s\n",
                 mode, result, choose_calls, pbuffer_calls, context_calls,
                 valid ? "PASS" : "FAIL");
    if (!valid) ++failures;
  }
  glxglue_display = NULL;
  std::free(display);
  return failures ? 1 : 0;
}
#endif // HAVE_GLX
