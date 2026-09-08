// Reproducer/regression test for an uninitialized-variable read in
// glxglue_context_create_pbuffer() (src/glue/gl_glx.cpp).
//
// fbc_cnt, the "number of FBConfigs returned" output parameter passed
// to glXChooseFBConfig(), was declared without an initializer and is
// only ever written by that call. Some GLX implementations have been
// observed to leave this output parameter unwritten on certain
// internal failure paths, in violation of the documented
// glXChooseFBConfig() contract (it's supposed to always set
// *nelements, even to 0, whenever it returns) -- an uninitialized
// read afterwards used to trip assert(fbc_cnt >= 0) nondeterministically
// (reproduced 5/5 with one particular ASan build of unrelated work
// elsewhere in this session, though not with others, confirming it
// depends on incidental stack contents rather than on any specific
// input), and in a non-debug (NDEBUG) build would have silently fed
// unpredictable garbage into the fbc_cnt == 0 fallback check and the
// fbc[0] access just below it.
//
// P2 fix, round 1 (code review): the original version of this
// reproducer drove 50 independent SoOffscreenRenderer::render() calls
// hoping to incidentally land on whatever stack layout had triggered
// the bug, then printed PASS *unconditionally* regardless of how many
// (if any) of those 50 calls actually succeeded -- so it reported
// PASS even in an environment where zero renders succeeded and the
// fixed GLX code path was never exercised at all.
//
// P2 fix, round 2 (code review): the first fix attempt replaced that
// with a deterministic-*looking* check -- extracting the "call the
// chooser, then validate its output" logic out of
// glxglue_context_create_pbuffer() into its own function,
// glxglue_choose_fbconfig() (below), so a reproducer could call it
// directly with a mock chooser reproducing the exact contract
// violation (returns NULL without writing *nelements) -- combined
// with deliberately pre-filling a same-sized stack region with a
// negative bit pattern immediately beforehand, on the theory that
// compilers commonly reuse stack slots across sibling call frames at
// the same nesting depth. That held up under GCC (5/5 runs correctly
// aborted on the pre-fix assert) but did *not* reproduce under Clang
// at the same -O1 level these reproducers are built at (0/1: read
// back as 0, not negative, despite an 16KB poisoning buffer) --
// stack-slot reuse across sibling frames is fundamentally a compiler-
// and optimization-level-specific implementation detail, not
// something safe to rely on for a test that needs to hold reliably
// across toolchains.
//
// Fixed properly this time with a tool built for exactly this
// problem: Clang's MemorySanitizer (MSan, -fsanitize=memory) tracks
// definedness at the byte level via shadow memory, so it flags a read
// of uninitialized memory unconditionally -- regardless of what
// incidental bit pattern happens to occupy that memory, no stack
// poisoning required. Confirmed directly against this exact function
// (glxglue_choose_fbconfig(), called with the same non-writing mock
// chooser as below, via a small MSan-instrumented standalone probe
// linked against an MSan-instrumented libCoin.so -- deliberately
// *not* calling SoDB::init() first, to keep the run isolated from any
// unrelated Coin subsystem noise): the pre-fix code
// (`int fbc_cnt;`) reports "MemorySanitizer: use-of-uninitialized-value"
// pinpointing gl_glx.cpp's assert(fbc_cnt >= 0) line exactly; the
// fixed code (`int fbc_cnt = 0;`) reports nothing at all. See the
// MSan build recipe in this reproducer's run.sh for how to reproduce
// that verification.
//
// This reproducer itself -- built normally, without MSan, the same
// way as every other reproducer in this tree -- keeps the
// deterministic call to glxglue_choose_fbconfig() with the
// non-writing mock chooser (still a real, useful functional-
// correctness check: confirms the fixed function doesn't crash and
// returns the documented safe values for a non-conforming chooser,
// no real X11/GLX state needed at all for this part), but no longer
// depends on any stack-content trick to do so -- that determinism now
// comes from *how* this same check is additionally verified (under
// MSan), not from anything baked into the check's own pass/fail logic.
//
// A secondary, best-effort integration check drives real
// SoOffscreenRenderer::render() calls end to end, same as in the
// original version, but is now clearly informational (not a source of
// a false PASS): it's skipped outright with no X display reachable at
// all (see somisc-fullpath-migration/repro.cpp on the sibling branch
// for the same treatment and rationale), and even with a display, is
// reported separately from -- and never able to mask a failure of --
// the deterministic check above, which is what actually gates
// PASS/FAIL for the bug this reproducer targets.

#include <cstdlib>
#include <cstdio>
#include <X11/Xlib.h>
#include <Inventor/SoDB.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/SbViewportRegion.h>

// Matches glxglue_choose_fbconfig()'s actual signature in
// src/glue/gl_glx.cpp exactly (typedef names don't affect C++ name
// mangling, only the underlying types do, so these don't need to be
// named the same as the ones private to that file). Not declared in
// any public Coin header -- this is still a private implementation
// detail we're reaching into specifically for this regression test.
extern void **
glxglue_choose_fbconfig(void ** (*chooser)(Display *, int, const int *, int *),
                        Display * dpy, int screen, const int * attrs,
                        int * fbc_cnt_out);

// Reproduces the exact, real-world glXChooseFBConfig() contract
// violation this bug depended on: returns failure (NULL) without
// writing to the "number of elements" output parameter at all.
static void **
mockChooserFailsWithoutWritingCount(Display *, int, const int *, int *)
{
  return NULL;
}

// Deliberately called before SoDB::init(): this check has no
// dependency on any Coin subsystem being initialized at all, and
// running it first keeps it isolated from any unrelated noise (this
// matters most when this reproducer is compiled and run under MSan --
// see the top-of-file comment).
static SbBool
test_choose_fbconfig_deterministic(void)
{
  int fbc_cnt = -999; // overwritten by the call; sentinel to catch a no-op
  void ** fbc = glxglue_choose_fbconfig(mockChooserFailsWithoutWritingCount,
                                        NULL, 0, NULL, &fbc_cnt);

  fprintf(stderr, "[repro] glxglue_choose_fbconfig() with a non-writing mock "
                  "chooser: fbc=%p fbc_cnt=%d\n", (void *)fbc, fbc_cnt);

  if (fbc != NULL) {
    fprintf(stderr, "[repro] FAIL: expected NULL for a chooser that found nothing\n");
    return FALSE;
  }
  if (fbc_cnt != 0) {
    fprintf(stderr, "[repro] FAIL: expected fbc_cnt == 0 (safe default), got %d\n",
            fbc_cnt);
    return FALSE;
  }
  return TRUE;
}

static SbBool
haveXDisplay(void)
{
  Display * dpy = XOpenDisplay(NULL);
  if (!dpy) return FALSE;
  XCloseDisplay(dpy);
  return TRUE;
}

static void
test_offscreen_render_informational(void)
{
  if (!haveXDisplay()) {
    fprintf(stderr, "[repro] INFO: skipping SoOffscreenRenderer integration "
                    "check (no X display reachable in this environment)\n");
    return;
  }

  setenv("COIN_GLX_PIXMAP_DIRECT_RENDERING", "1", 0);

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

  fprintf(stderr, "[repro] INFO: %d/%d SoOffscreenRenderer::render() calls "
                  "succeeded (process did not abort; purely informational,\n"
                  "        does not gate PASS/FAIL -- see "
                  "test_choose_fbconfig_deterministic() for that)\n",
          successes, iterations);
}

int
main()
{
  int failures = 0;
  if (!test_choose_fbconfig_deterministic()) {
    fprintf(stderr, "[repro] FAIL: glxglue_choose_fbconfig() deterministic check\n");
    failures++;
  }

  SoDB::init();
  test_offscreen_render_informational();

  if (failures > 0) {
    fprintf(stderr, "[repro] FAIL: %d check(s) failed\n", failures);
    return 1;
  }
  fprintf(stderr, "[repro] PASS\n");
  return 0;
}
