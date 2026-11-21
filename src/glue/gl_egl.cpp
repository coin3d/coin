/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

/*
 *  Environment variable controls available:
 * 
 *   - COIN_EGLGLUE_NO_PBUFFERS: set to 1 to disable EGL offscreen
 *     pbuffers.
 */

#include "glue/gl_egl.h"
#include "coindefs.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <cstdlib>
#include <cstring>
#include <cassert>
#include <climits>

#include <Inventor/C/tidbits.h>
#include <Inventor/C/glue/gl.h>
#include <Inventor/C/errors/debugerror.h>
#include <Inventor/C/glue/dl.h>

#include "glue/glp.h"
#include "glue/dlp.h"

/* ********************************************************************** */

#ifndef HAVE_EGL

void * eglglue_getprocaddress(const cc_glglue * COIN_UNUSED_ARG(glue_in), const char * fname)
{
  return NULL;
}

void * eglglue_context_create_offscreen(unsigned int COIN_UNUSED_ARG(width),
                                        unsigned int COIN_UNUSED_ARG(height)) {
  return NULL;
}

SbBool eglglue_context_make_current(void * COIN_UNUSED_ARG(ctx))
{
  return FALSE;
}

void eglglue_context_reinstate_previous(void * COIN_UNUSED_ARG(ctx))
{
  assert(FALSE);
}

void eglglue_context_destruct(void * COIN_UNUSED_ARG(ctx))
{
  assert(FALSE);
}

SbBool eglglue_context_pbuffer_max(void * ctx, unsigned int * lims)
{
  assert(FALSE); return FALSE;
}

#else /* HAVE_EGL */

/* ********************************************************************** */

#include <EGL/egl.h>
#include <EGL/eglext.h>

EGLDisplay eglglue_display = EGL_NO_DISPLAY;
static EGLDisplay eglglue_surfaceless_display = EGL_NO_DISPLAY;
struct eglglue_contextdata;

#define CASE_STR( value ) case value: return #value;
const char* eglErrorString( EGLint error )
{
    switch( error )
    {
    CASE_STR( EGL_SUCCESS             )
    CASE_STR( EGL_NOT_INITIALIZED     )
    CASE_STR( EGL_BAD_ACCESS          )
    CASE_STR( EGL_BAD_ALLOC           )
    CASE_STR( EGL_BAD_ATTRIBUTE       )
    CASE_STR( EGL_BAD_CONTEXT         )
    CASE_STR( EGL_BAD_CONFIG          )
    CASE_STR( EGL_BAD_CURRENT_SURFACE )
    CASE_STR( EGL_BAD_DISPLAY         )
    CASE_STR( EGL_BAD_SURFACE         )
    CASE_STR( EGL_BAD_MATCH           )
    CASE_STR( EGL_BAD_PARAMETER       )
    CASE_STR( EGL_BAD_NATIVE_PIXMAP   )
    CASE_STR( EGL_BAD_NATIVE_WINDOW   )
    CASE_STR( EGL_CONTEXT_LOST        )
    default: return "Unknown";
    }
}
const char* eglAPIString( EGLenum api )
{
    switch( api )
    {
    CASE_STR( EGL_OPENGL_API          )
    CASE_STR( EGL_OPENGL_ES_API          )
    CASE_STR( EGL_OPENVG_API          )
    default: return "Unknown";
    }
}
#undef CASE_STR

struct eglglue_contextdata {
  EGLDisplay display;
  EGLConfig config;
  EGLContext context;
  EGLSurface surface;
  EGLDisplay previousdisplay;
  EGLContext previouscontext;
  EGLSurface previousdraw;
  EGLSurface previousread;
  EGLenum previousapi;
  EGLint bindtotexture;
};

static EGLDisplay
eglglue_get_display(void)
{
  PFNEGLGETPLATFORMDISPLAYEXTPROC eglGetPlatformDisplayEXT;

  if (eglglue_display != EGL_NO_DISPLAY) {
      return eglglue_display;
  }

  eglglue_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  if (eglglue_display != EGL_NO_DISPLAY) {
    goto found;
  }

  eglGetPlatformDisplayEXT =
      (PFNEGLGETPLATFORMDISPLAYEXTPROC)eglGetProcAddress("eglGetPlatformDisplayEXT");
  if (!eglGetPlatformDisplayEXT) {
    return EGL_NO_DISPLAY;
  }

  eglglue_display = eglGetPlatformDisplay(EGL_PLATFORM_WAYLAND_KHR, EGL_DEFAULT_DISPLAY, NULL);
  if (eglglue_display != EGL_NO_DISPLAY) {
    goto found;
  }

  eglglue_display = eglGetPlatformDisplay(EGL_PLATFORM_X11_EXT, EGL_DEFAULT_DISPLAY, NULL);

  if (eglglue_display == EGL_NO_DISPLAY) {
    cc_debugerror_post("eglglue_get_display", "Display not found.");
    return EGL_NO_DISPLAY;
  }

found:
  if (coin_glglue_debug()) {
    cc_debugerror_postinfo("eglglue_get_display",
                            "got EGLDisplay==%p",
                            eglglue_display);
  }

  return eglglue_display;
}

static EGLDisplay
eglglue_get_surfaceless_display(void)
{
#ifdef EGL_PLATFORM_SURFACELESS_MESA
  if (eglglue_surfaceless_display != EGL_NO_DISPLAY) return eglglue_surfaceless_display;

  const char extension[] = "EGL_MESA_platform_surfaceless";
  const char * extensions = eglQueryString(EGL_NO_DISPLAY, EGL_EXTENSIONS);
  const char * match = extensions ? std::strstr(extensions, extension) : NULL;
  if (match && (match == extensions || match[-1] == ' ') &&
      (match[sizeof(extension) - 1] == ' ' || match[sizeof(extension) - 1] == '\0')) {
    eglglue_surfaceless_display =
      eglGetPlatformDisplay(EGL_PLATFORM_SURFACELESS_MESA, EGL_DEFAULT_DISPLAY, NULL);
  }
#endif
  return eglglue_surfaceless_display;
}

static SbBool
eglglue_choose_pbuffer_config(EGLDisplay display, const EGLint * attrib, EGLConfig * config)
{
  EGLint matches;
  return display != EGL_NO_DISPLAY && eglInitialize(display, NULL, NULL) &&
    eglChooseConfig(display, attrib, config, 1, &matches) && matches > 0;
}

void
eglglue_init(cc_glglue * w)
{
  w->glx.isdirect = 1;
  w->glx.serverversion = NULL;
  w->glx.servervendor = NULL;
  w->glx.serverextensions = NULL;
  w->glx.clientversion = NULL;
  w->glx.clientvendor = NULL;
  w->glx.clientextensions = NULL;
  w->glx.glxextensions = NULL;

  w->glx.glXGetCurrentDisplay = (COIN_PFNGLXGETCURRENTDISPLAYPROC)eglglue_getprocaddress(w, "eglglue_get_display");

  EGLDisplay display = eglGetCurrentDisplay();
  if (display == EGL_NO_DISPLAY) display = eglglue_get_display();
  if (eglInitialize(display, &w->glx.version.major, &w->glx.version.minor) == EGL_FALSE) {
    cc_debugerror_post("eglglue_init",
                       "Couldn't initialize EGL. %s",
                        eglErrorString(eglGetError()));
    return;
  }

  if (eglBindAPI(EGL_OPENGL_API) == EGL_FALSE) {
    cc_debugerror_post("eglglue_init",
                       "eglBindAPI(EGL_OPENGL_API) failed. %s",
                       eglErrorString(eglGetError()));
    return;
  }

  if (coin_glglue_debug()) {
    cc_debugerror_postinfo("eglglue_init",
                           "EGL version: %d.%d",
                            w->glx.version.major,
                            w->glx.version.minor);
    cc_debugerror_postinfo("eglglue_init",
                           "eglQueryString(EGL_VERSION)=='%s'",
                            eglQueryString(display, EGL_VERSION));
    cc_debugerror_postinfo("eglglue_init",
                           "eglQueryString(EGL_VENDOR)=='%s'",
                            eglQueryString(display, EGL_VENDOR));
    cc_debugerror_postinfo("eglglue_init",
                           "eglQueryString(EGL_CLIENT_APIS)=='%s'",
                            eglQueryString(display, EGL_CLIENT_APIS));
    cc_debugerror_postinfo("eglglue_init",
                           "eglQueryAPI()=='%s'",
                           eglAPIString(eglQueryAPI()));
    cc_debugerror_postinfo("eglglue_init",
                           "eglQueryString(EGL_EXTENSIONS)=='%s'",
                            eglQueryString(display, EGL_EXTENSIONS));
  }
}

static void
eglglue_contextdata_cleanup(struct eglglue_contextdata * ctx)
{
  if (ctx->context != EGL_NO_CONTEXT) eglDestroyContext(ctx->display, ctx->context);
  if (ctx->surface != EGL_NO_SURFACE) eglDestroySurface(ctx->display, ctx->surface);
  free(ctx);
}

void *
eglglue_context_create_offscreen(unsigned int width, unsigned int height)
{
  const char * env = coin_getenv("COIN_EGLGLUE_NO_PBUFFERS");
  if (env && atoi(env) > 0) return NULL;

  const int stencilbits = coin_glglue_stencil_bits_hack();
  const EGLint attrib[] = {
    EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
    EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
    EGL_RED_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_BLUE_SIZE, 8,
    EGL_ALPHA_SIZE, 8,
    EGL_DEPTH_SIZE, 24,
    EGL_STENCIL_SIZE, stencilbits == -1 ? 1 : stencilbits,
    EGL_NONE
  };
  EGLDisplay display = eglGetCurrentDisplay();
  if (display == EGL_NO_DISPLAY) display = eglglue_get_display();
  EGLConfig config;
  if (!eglglue_choose_pbuffer_config(display, attrib, &config)) {
    // A Wayland EGL display can offer window surfaces without any pbuffers.
    const EGLDisplay fallback = eglglue_get_surfaceless_display();
    if (fallback == display || !eglglue_choose_pbuffer_config(fallback, attrib, &config)) {
      cc_debugerror_post("eglglue_context_create_offscreen",
                         "No matching EGL pbuffer configuration.");
      return NULL;
    }
    display = fallback;
  }

  struct eglglue_contextdata * ctx =
    static_cast<struct eglglue_contextdata *>(calloc(1, sizeof(struct eglglue_contextdata)));
  if (!ctx) return NULL;
  ctx->display = display;
  ctx->config = config;
  ctx->context = EGL_NO_CONTEXT;
  ctx->previouscontext = EGL_NO_CONTEXT;
  ctx->bindtotexture = EGL_FALSE;
  eglGetConfigAttrib(display, config, EGL_BIND_TO_TEXTURE_RGBA, &ctx->bindtotexture);

  const EGLint surfaceattrib[] = {
    EGL_WIDTH, static_cast<EGLint>(width),
    EGL_HEIGHT, static_cast<EGLint>(height),
    EGL_TEXTURE_FORMAT, ctx->bindtotexture ? EGL_TEXTURE_RGBA : EGL_NO_TEXTURE,
    EGL_TEXTURE_TARGET, ctx->bindtotexture ? EGL_TEXTURE_2D : EGL_NO_TEXTURE,
    EGL_NONE
  };
  ctx->surface = eglCreatePbufferSurface(display, config, surfaceattrib);
  if (ctx->surface == EGL_NO_SURFACE) {
    cc_debugerror_post("eglglue_context_create_offscreen",
                       "Couldn't create EGL pbuffer. %s",
                       eglErrorString(eglGetError()));
    eglglue_contextdata_cleanup(ctx);
    return NULL;
  }

  const EGLenum previousapi = eglQueryAPI();
  if (!eglBindAPI(EGL_OPENGL_API)) {
    cc_debugerror_post("eglglue_context_create_offscreen",
                       "eglBindAPI(EGL_OPENGL_API) failed. %s",
                       eglErrorString(eglGetError()));
    eglglue_contextdata_cleanup(ctx);
    return NULL;
  }
  ctx->context = eglCreateContext(display, config, EGL_NO_CONTEXT, NULL);
  const EGLint error = ctx->context == EGL_NO_CONTEXT ? eglGetError() : EGL_SUCCESS;
  if (previousapi != EGL_NONE) eglBindAPI(previousapi);

  if (ctx->context == EGL_NO_CONTEXT) {
    cc_debugerror_post("eglglue_context_create_offscreen",
                       "Couldn't create EGL context. %s", eglErrorString(error));
    eglglue_contextdata_cleanup(ctx);
    return NULL;
  }
  return ctx;
}

SbBool
eglglue_context_make_current(void * ctx)
{
  struct eglglue_contextdata * context = static_cast<struct eglglue_contextdata *>(ctx);

  context->previousapi = eglQueryAPI();
  if (!eglBindAPI(EGL_OPENGL_API)) return FALSE;
  context->previousdisplay = eglGetCurrentDisplay();
  context->previouscontext = eglGetCurrentContext();
  context->previousdraw = eglGetCurrentSurface(EGL_DRAW);
  context->previousread = eglGetCurrentSurface(EGL_READ);

  if (!eglMakeCurrent(context->display, context->surface, context->surface, context->context)) {
    cc_debugerror_post("eglglue_context_make_current",
                       "eglMakeCurrent failed: %s", eglErrorString(eglGetError()));
    if (context->previousapi != EGL_NONE) eglBindAPI(context->previousapi);
    return FALSE;
  }
  return TRUE;
}

void
eglglue_context_reinstate_previous(void * ctx)
{
  struct eglglue_contextdata * context = static_cast<struct eglglue_contextdata *>(ctx);
  EGLDisplay display = context->previouscontext == EGL_NO_CONTEXT ?
    context->display : context->previousdisplay;
  if (!eglMakeCurrent(display, context->previousdraw, context->previousread,
                      context->previouscontext)) {
    cc_debugerror_post("eglglue_context_reinstate_previous",
                       "eglMakeCurrent failed: %s", eglErrorString(eglGetError()));
  }
  if (context->previousapi != EGL_NONE) eglBindAPI(context->previousapi);
}

void
eglglue_context_destruct(void * ctx)
{
  eglglue_contextdata_cleanup(static_cast<struct eglglue_contextdata *>(ctx));
}

void
eglglue_context_bind_pbuffer(void * ctx)
{
  struct eglglue_contextdata * context = (struct eglglue_contextdata *)ctx;

  if (eglBindTexImage(context->display, context->surface, EGL_BACK_BUFFER) == EGL_FALSE) {
    cc_debugerror_post("eglglue_context_bind_pbuffer()"
                       "after binding pbuffer: %s",
                       eglErrorString(eglGetError()));
  }
}

void
eglglue_context_release_pbuffer(void * ctx)
{
  struct eglglue_contextdata * context = (struct eglglue_contextdata *)ctx;

  if (eglReleaseTexImage(context->display, context->surface, EGL_BACK_BUFFER) == EGL_FALSE) {
    cc_debugerror_post("eglglue_context_release_pbuffer()"
                       "releasing pbuffer: %s",
                       eglErrorString(eglGetError()));
  }
}

SbBool
eglglue_context_pbuffer_is_bound(void * ctx)
{
  struct eglglue_contextdata * context = (struct eglglue_contextdata *)ctx;
  GLint buffer = EGL_NONE;

  if(eglQueryContext(context->display, context->context, EGL_RENDER_BUFFER, &buffer) == EGL_FALSE) {
    cc_debugerror_post("eglglue_context_pbuffer_is_bound()"
                       "after query pbuffer: %s",
                       eglErrorString(eglGetError()));
  }
  return buffer == EGL_BACK_BUFFER;
}

SbBool
eglglue_context_can_render_to_texture(void * ctx)
{
  struct eglglue_contextdata * context = (struct eglglue_contextdata *)ctx;
  return context->bindtotexture == EGL_TRUE;
}

SbBool
eglglue_context_pbuffer_max(void * ctx, unsigned int * lims)
{
  int attribval, i;
  const int attribs[] = {
    EGL_MAX_PBUFFER_WIDTH, EGL_MAX_PBUFFER_HEIGHT, EGL_MAX_PBUFFER_PIXELS
  };
  struct eglglue_contextdata * context = (struct eglglue_contextdata *)ctx;

  if (context->surface == EGL_NO_SURFACE) { return FALSE; }

  for (i = 0; i < 3; i++) {
    if(eglGetConfigAttrib(context->display, context->config, attribs[i], &attribval) == EGL_FALSE) {
      cc_debugerror_post("eglglue_context_pbuffer_max",
                         "eglGetConfigAttrib() failed, "
                         "returned error code %s",
                         eglErrorString(eglGetError()));
      return FALSE;
    }
    assert(attribval >= 0);
    lims[i] = (unsigned int)attribval;
  }
  // Mesa can report no separate pixel-count limit.
  if (lims[2] == 0) lims[2] = UINT_MAX;
  return TRUE;
}

void *
eglglue_getprocaddress(const cc_glglue * COIN_UNUSED_ARG(glue_in), const char * fname)
{
  return (void *)eglGetProcAddress(fname);
}

void
eglglue_cleanup(void)
{
  if (eglglue_surfaceless_display != EGL_NO_DISPLAY &&
      eglglue_surfaceless_display != eglglue_display) {
    eglTerminate(eglglue_surfaceless_display);
  }
  if (eglglue_display != EGL_NO_DISPLAY) eglTerminate(eglglue_display);
  eglglue_surfaceless_display = EGL_NO_DISPLAY;
  eglglue_display = EGL_NO_DISPLAY;
}

#endif /* HAVE_EGL */
