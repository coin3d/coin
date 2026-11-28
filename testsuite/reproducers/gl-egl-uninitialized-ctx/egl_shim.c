/* Record the width and height while forwarding real pbuffer creation. */
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
capture_attribs(const EGLint * attrib_list)
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
eglCreatePbufferSurface(EGLDisplay dpy, EGLConfig config, const EGLint * attrib_list)
{
  PFNEGLCREATEPBUFFERSURFACEPROC create =
    (PFNEGLCREATEPBUFFERSURFACEPROC)dlsym(RTLD_NEXT, "eglCreatePbufferSurface");
  capture_attribs(attrib_list);
  report();
  return create ? create(dpy, config, attrib_list) : EGL_NO_SURFACE;
}
