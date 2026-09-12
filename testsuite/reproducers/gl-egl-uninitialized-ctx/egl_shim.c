/*
 * LD_PRELOAD shim that intercepts the EGL platform-surface creation
 * entry points called from eglglue_context_create_offscreen()
 * (src/glue/gl_egl.cpp) and records the EGL_WIDTH / EGL_HEIGHT values
 * found in the attrib_list argument, before returning a fake surface
 * handle (a real native window/pixmap is not available in this
 * headless reproducer, and is irrelevant to the bug under test).
 *
 * This isolates the bug (wrong values reaching the EGL attribute
 * list) from whatever a given driver does with a NULL native window,
 * which can fail for unrelated reasons.
 */
#define _GNU_SOURCE
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

static long captured_width  = -1;
static long captured_height = -1;
static int  captured_calls  = 0;

static void
capture_attribs(const EGLAttrib * attrib_list)
{
  captured_calls++;
  if (!attrib_list) return;
  for (int i = 0; attrib_list[i] != EGL_NONE; i += 2) {
    if (attrib_list[i] == EGL_WIDTH)  captured_width  = (long) attrib_list[i + 1];
    if (attrib_list[i] == EGL_HEIGHT) captured_height = (long) attrib_list[i + 1];
  }
}

static void
report(void)
{
  const char * outpath = getenv("EGL_SHIM_OUTFILE");
  fprintf(stderr, "[egl_shim] calls=%d captured EGL_WIDTH=%ld EGL_HEIGHT=%ld\n",
          captured_calls, captured_width, captured_height);
  if (outpath) {
    FILE * f = fopen(outpath, "w");
    if (f) {
      fprintf(f, "%d %ld %ld\n", captured_calls, captured_width, captured_height);
      fclose(f);
    }
  }
}

EGLSurface
eglCreatePlatformWindowSurface(EGLDisplay dpy, EGLConfig config,
                               void * native_window, const EGLAttrib * attrib_list)
{
  (void) dpy; (void) config; (void) native_window;
  capture_attribs(attrib_list);
  report();
  return (EGLSurface) 0x1; /* fake non-null handle so gl_egl.cpp proceeds */
}

EGLSurface
eglCreatePlatformPixmapSurface(EGLDisplay dpy, EGLConfig config,
                               void * native_pixmap, const EGLAttrib * attrib_list)
{
  (void) dpy; (void) config; (void) native_pixmap;
  capture_attribs(attrib_list);
  report();
  return (EGLSurface) 0x1;
}
