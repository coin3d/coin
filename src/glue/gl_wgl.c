/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdlib.h>
#include <assert.h>

#include <Inventor/C/glue/gl.h>
#include <Inventor/C/glue/glp.h>
#include <Inventor/C/errors/debugerror.h>
#include <Inventor/C/glue/gl_wgl.h>


#ifndef HAVE_WGL

void * coin_wgl_getprocaddress(const char * fname) { return NULL; }

void * wglglue_context_create_offscreen(unsigned int width, unsigned int height) { assert(FALSE); return NULL; }
SbBool wglglue_context_make_current(void * ctx) { assert(FALSE); return FALSE; }
void wglglue_context_reinstate_previous(void * ctx) { assert(FALSE); }
void wglglue_context_destruct(void * ctx) { assert(FALSE); }

#else /* HAVE_WGL */

void *
coin_wgl_getprocaddress(const char * fname)
{
  void * ptr = wglGetProcAddress(fname);

  /* wglGetProcAddress() seems to only be able to fetch
     function-addresses for *extension* functions, not "proper" OpenGL
     (1.1+) functions. */

  if (ptr == NULL) {
    /* check for function in opengl32.dll.
     *
     * FIXME: Is it sufficient to just check opengl32.dll? pederb, 2002-07-12
     *
     * UPDATE 2002-07-12 kintel: I think not, as the function pointers
     * that should be used are dependent upon 1) the current
     * framebuffer configuration and 2) the ICD driver installed. You
     * might be lucky though...
     *
     * UPDATE 2002-09-11 mortene: should at least replace
     * "opengl32.dll" with the OpenGL library name found by the
     * configure script.
     */

    /* If this worked like dlopen() on UNIX-like systems with libdl,
     * we could just get the executable handle by passing NULL to
     * GetModuleHandle(), and then get any symbol from that. That
     * doesn't work, though, as GetProcAddress() will *only* resolve
     * symbols from the specific DLL module handle.
     */
    HINSTANCE glhandle = GetModuleHandle("opengl32.dll");

    if (!glhandle && coin_glglue_debug()) {
      cc_debugerror_postwarning("coin_wgl_getprocaddress",
                                "couldn't get hold of module handle for "
                                "\"opengl32.dll\"");
    }

    if (glhandle) {
      ptr = (void *)GetProcAddress(glhandle, fname);

      if (ptr && coin_glglue_debug()) {
        cc_debugerror_postinfo("coin_wgl_getprocaddress",
                               "wglGetProcAddress() missed \"%s\", "
                               "but found with GetProcAddress()", fname);
      }
    }
  }
  return ptr;
}


/*** WGL offscreen contexts **************************************************/

struct wglglue_contextdata {
  unsigned int width, height;

  HDC memorydc;
  HBITMAP bitmap, oldbitmap;
  HGLRC wglcontext;

  HGLRC storedcontext;
  HDC storeddc;
};

static void
wglglue_contextdata_init(struct wglglue_contextdata * c)
{
  c->memorydc = NULL;
  c->bitmap = NULL;
  c->oldbitmap = NULL;
  c->wglcontext = NULL;
  c->storedcontext = NULL;
  c->storeddc = NULL;
}

static void
wglglue_contextdata_cleanup(struct wglglue_contextdata * c)
{
  if (c->wglcontext) { (void)wglDeleteContext(c->wglcontext); }
  if (c->oldbitmap) { (void)SelectObject(c->memorydc, c->bitmap); }
  if (c->bitmap) { DeleteObject(c->bitmap); }
  if (c->memorydc) { DeleteDC(c->memorydc); }
}

void *
wglglue_context_create_offscreen(unsigned int width, unsigned int height)
{
  /* FIXME: on WGL drivers with p-buffer support, that should be used
     instead of a standard offscreen WGL context, as it would render
     much faster (due to hardware acceleration). 20030310 mortene. */

  struct wglglue_contextdata context, * retctx;

  wglglue_contextdata_init(&context);

  context.width = width;
  context.height = height;

  context.memorydc = CreateCompatibleDC(NULL);
  if (context.memorydc == NULL) {
    DWORD dwError = GetLastError();
    cc_debugerror_postwarning("wglglue_context_create_offscreen",
                              "CreateCompatibleDC(NULL) failed with "
                              "error code %d.", dwError);
    return NULL;
  }
  
  /* make a bitmap to draw to */
  {
    BITMAPINFO bmi;
    void * pvbits;

    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 24;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = 0;
    bmi.bmiHeader.biXPelsPerMeter = 0;
    bmi.bmiHeader.biYPelsPerMeter = 0;
    bmi.bmiHeader.biClrUsed  = 0;
    bmi.bmiHeader.biClrImportant = 0;
    bmi.bmiColors[0].rgbBlue = 0;
    bmi.bmiColors[0].rgbGreen = 0;
    bmi.bmiColors[0].rgbRed = 0;
    bmi.bmiColors[0].rgbReserved = 0;
  
    context.bitmap = CreateDIBSection(context.memorydc, &bmi, DIB_RGB_COLORS,
                                      &pvbits, NULL, 0);
    if (context.bitmap == NULL) {
      DWORD dwError = GetLastError();
      cc_debugerror_postwarning("wglglue_context_create_offscreen",
                                "CreateDIBSection() failed with error "
                                "code %d.", dwError);
      wglglue_contextdata_cleanup(&context);
      return NULL;
    }
  }
  
  context.oldbitmap = (HBITMAP)
    SelectObject(context.memorydc, context.bitmap);
  if (context.oldbitmap == NULL) {
    DWORD dwError = GetLastError();
    cc_debugerror_postwarning("wglglue_context_create_offscreen",
                              "SelectObject() failed with error code %d.",
                              dwError);
    wglglue_contextdata_cleanup(&context);
    return NULL;
  }
  
  {
    BOOL ret;
    int pixformat;
    PIXELFORMATDESCRIPTOR pfd = {
      sizeof(PIXELFORMATDESCRIPTOR),   /* size of this pfd */
      1,                     /* version number */
      PFD_DRAW_TO_BITMAP |   /* support bitmap */
      PFD_SUPPORT_OPENGL,    /* support OpenGL */
      PFD_TYPE_RGBA,         /* RGBA type */
      24,                    /* 24-bit color depth */
      0, 0, 0, 0, 0, 0,      /* color bits ignored */
      0,                     /* no alpha buffer */
      0,                     /* shift bit ignored */
      0,                     /* no accumulation buffer */
      0, 0, 0, 0,            /* accum bits ignored */
      32,                    /* 32-bit z-buffer */
      0,                     /* no stencil buffer */
      0,                     /* no auxiliary buffer */
      PFD_MAIN_PLANE,        /* main layer */
      0,                     /* reserved */
      0, 0, 0                /* layer masks ignored */
    };
  
 
    /* get the best available match of pixel format for the device context */
    pixformat = ChoosePixelFormat(context.memorydc, &pfd);
    if (pixformat == 0) {
      DWORD dwError = GetLastError();
      cc_debugerror_postwarning("wglglue_context_create_offscreen",
                                "ChoosePixelFormat() failed with "
                                "error code %d.", dwError);
      wglglue_contextdata_cleanup(&context);
      return NULL;
    }
  
    /* make that the pixel format of the device context */
    ret = SetPixelFormat(context.memorydc, pixformat, &pfd);
    if (!ret) {
      DWORD dwError = GetLastError();
      cc_debugerror_postwarning("wglglue_context_create_offscreen",
                                "SetPixelFormat() failed with error code %d.",
                                dwError);
      wglglue_contextdata_cleanup(&context);
      return NULL;
    }
  }
  
  context.wglcontext = wglCreateContext(context.memorydc);
  if (context.wglcontext == NULL) {
    DWORD dwError = GetLastError();
    cc_debugerror_postwarning("wglglue_context_create_offscreen",
                              "wglCreateContext() failed with error code %d.",
                              dwError);
    wglglue_contextdata_cleanup(&context);
    return NULL;
  }

  retctx = (struct wglglue_contextdata *)malloc(sizeof(struct wglglue_contextdata));
  (void)memcpy(retctx, &context, sizeof(struct wglglue_contextdata));
  return retctx;
}

SbBool
wglglue_context_make_current(void * ctx)
{
  struct wglglue_contextdata * context = (struct wglglue_contextdata *)ctx;

  context->storedcontext = wglGetCurrentContext();
  if (context->storedcontext) { context->storeddc = wglGetCurrentDC(); }
  return wglMakeCurrent(context->memorydc, context->wglcontext) ? TRUE : FALSE;
}

void
wglglue_context_reinstate_previous(void * ctx)
{
  /* The previous context is stored and reset to make it possible to
     use an SoOffscreenRenderer from for instance an SoCallback node
     callback during SoGLRenderAction traversal, without the need for
     any extra book-keeping on the application side. */
  
  struct wglglue_contextdata * context = (struct wglglue_contextdata *)ctx;

  if (context->storedcontext && context->storeddc) {
    (void)wglMakeCurrent(context->storeddc, context->storedcontext);
    context->storedcontext = NULL;
  }
  else {
    (void)wglMakeCurrent(NULL, NULL);
  }
}

void
wglglue_context_destruct(void * ctx)
{
  /* FIXME: needs to call into the (as of yet unimplemented)
     "destructing GL context" handler. 20030310 mortene. */

  struct wglglue_contextdata * context = (struct wglglue_contextdata *)ctx;

  if (coin_glglue_debug()) {
    cc_debugerror_postinfo("wglglue_context_destruct",
                           "destroy context %p", context->wglcontext);
  }

  wglglue_contextdata_cleanup(context);
  free(context);
}

#endif /* HAVE_WGL */
