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
#include <Inventor/C/tidbits.h>

#ifdef HAVE_WIN32_API
/* Conditional inclusion, as the functions in win32api.h will not be
   implemented unless the Win32 API is available. */
#include <Inventor/C/glue/win32api.h>
#endif /* HAVE_WIN32_API */

/* ********************************************************************** */

#ifndef HAVE_WGL

void * coin_wgl_getprocaddress(const char * fname) { return NULL; }

void * wglglue_context_create_offscreen(unsigned int width, unsigned int height) { assert(FALSE); return NULL; }
SbBool wglglue_context_make_current(void * ctx) { assert(FALSE); return FALSE; }
void wglglue_context_reinstate_previous(void * ctx) { assert(FALSE); }
void wglglue_context_destruct(void * ctx) { assert(FALSE); }

/* ********************************************************************** */

#else /* HAVE_WGL */

struct wglglue_contextdata;
static SbBool (* wglglue_context_create)(struct wglglue_contextdata * context, SbBool warnonerrors) = NULL;

/* ********************************************************************** */

/* Declared in the same manner as in the OpenGL ARB document for the
   pbuffer extension, but with a different name to avoid clashes upon
   static binding.

   Note that I couldn't find any documentation on the DECLARE_HANDLE
   thinga-majingy, so I would appreciate it if someone could confirm
   that this is correct. mortene.
*/
DECLARE_HANDLE(WGLGLUE_HPBUFFER);

/* The following are from either the WGL_ARB_pbuffer or the
   WGL_EXT_pbuffer extensions: */

typedef WGLGLUE_HPBUFFER (WINAPI * COIN_PFNWGLCREATEPBUFFERPROC)(HDC hDC,
                                                               int iPixelFormat,
                                                               int iWidth,
                                                               int iHeight,
                                                               const int * piAttribList);
typedef HDC (WINAPI * COIN_PFNWGLGETPBUFFERDCPROC)(WGLGLUE_HPBUFFER hPbuffer);
typedef int (WINAPI * COIN_PFNWGLRELEASEPBUFFERDCPROC)(WGLGLUE_HPBUFFER hPbuffer,
                                                          HDC hDC);
typedef BOOL (WINAPI * COIN_PFNWGLDESTROYPBUFFERPROC)(WGLGLUE_HPBUFFER hPbuffer);
typedef BOOL (WINAPI * COIN_PFNWGLQUERYPBUFFERPROC)(WGLGLUE_HPBUFFER hPbuffer,
                                                       int iAttribute,
                                                       int * piValue);

typedef BOOL (WINAPI * COIN_PFNWGLBINDTEXIMAGEARBPROC)(WGLGLUE_HPBUFFER hPbuffer, int iBuffer);
typedef BOOL (WINAPI * COIN_PFNWGLRELEASETEXIMAGEARBPROC)(WGLGLUE_HPBUFFER hPbuffer, int iBuffer);
/* typedef BOOL (WINAPI * COIN_PFNWGLSETPBUFFERATTRIBARBPROC)(WGLGLUE_HPBUFFER hPbuffer, const int * piAttribList); */

static COIN_PFNWGLCREATEPBUFFERPROC wglglue_wglCreatePbuffer = NULL;
static COIN_PFNWGLGETPBUFFERDCPROC wglglue_wglGetPbufferDC = NULL;
static COIN_PFNWGLRELEASEPBUFFERDCPROC wglglue_wglReleasePbufferDC = NULL;
static COIN_PFNWGLDESTROYPBUFFERPROC wglglue_wglDestroyPbuffer = NULL;
static COIN_PFNWGLQUERYPBUFFERPROC wglglue_wglQueryPbuffer = NULL;
static COIN_PFNWGLBINDTEXIMAGEARBPROC wglglue_wglBindTexImageARB = NULL;
static COIN_PFNWGLBINDTEXIMAGEARBPROC wglglue_wglReleaseTexImageARB = NULL;
/* static COIN_PFNWGLSETPBUFFERATTRIBARBPROC wglglue_wglSetPBufferAttrib = NULL; */


/* The following is from either the WGL_ARB_pixel_format or the
   WGL_EXT_pixel_format extensions: */

typedef BOOL (WINAPI * COIN_PFNWGLCHOOSEPIXELFORMATPROC)(HDC hdc,
                                                         const int * piAttribIList,
                                                         const FLOAT * pfAttribFList,
                                                         UINT nMaxFormats,
                                                         int * piFormats,
                                                         UINT * nNumFormats);

static COIN_PFNWGLCHOOSEPIXELFORMATPROC wglglue_wglChoosePixelFormat = NULL;

/* The function for finding extension strings is itself an extension
   string. */

typedef const char * (WINAPI * COIN_PFNWGLGETEXTENSIONSSTRING)(HDC hDC);

static COIN_PFNWGLGETEXTENSIONSSTRING wglglue_wglGetExtensionsString = NULL;

/* ********************************************************************** */

#ifdef HAVE_DYNAMIC_LINKING
#define PROC(_func_) cc_glglue_getprocaddress(SO__QUOTE(_func_))

/* The OpenGL library's WGL part which we dynamically pick up symbols
   from /could/ have these defined. For the code below which tries to
   dynamically resolve the methods, we will assume that they are
   defined. By doing this little "trick", can we use the same code
   below for resolving stuff dynamically as we need anyway to resolve
   in a static manner. */

#define WGL_ARB_pixel_format 1
#define WGL_EXT_pixel_format 1

#define WGL_ARB_pbuffer 1
#define WGL_EXT_pbuffer 1

#define WGL_ARB_extensions_string 1
#define WGL_EXT_extensions_string 1

#define WGL_ARB_render_texture 1
#else /* static binding */

#define PROC(_func_) (&_func_)

#endif /* static binding */

/* ********************************************************************** */

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

/*** WGL offscreen contexts ***********************************************/

struct wglglue_contextdata {
  unsigned int width, height;

  HDC memorydc;
  HWND pbufferwnd;
  SbBool didcreatememorydc;
  SbBool shouldreleasememorydc;
  HBITMAP bitmap, oldbitmap;
  HGLRC wglcontext;

  HGLRC storedcontext;
  HDC storeddc;

  WGLGLUE_HPBUFFER hpbuffer;
  SbBool noappglcontextavail;
  SbBool pbufferisbound;
};

static SbBool
wglglue_pbuffer_symbols_resolved(void)
{
  return (wglglue_wglCreatePbuffer && wglglue_wglGetPbufferDC &&
          wglglue_wglReleasePbufferDC && wglglue_wglDestroyPbuffer &&
          wglglue_wglQueryPbuffer);
}

static SbBool
wglglue_ext_supported(struct wglglue_contextdata * context, const char * reqext)
{
  /*
   * wgl extensions are not necessarily listed in the string returned
   * by glGetString(GL_EXTENSIONS), see e.g.
   *
   *   http://www.gamedev.net/reference/articles/article1929.asp
   *
   * ..so we try to get hold of wglGetExtensionsString[ARB|EXT]() and
   * use that aswell.
   */
  static SbBool attemptedresolved = FALSE;

  if (!attemptedresolved) {
    attemptedresolved = TRUE;
#ifdef WGL_ARB_extensions_string
    wglglue_wglGetExtensionsString = (COIN_PFNWGLGETEXTENSIONSSTRING)PROC(wglGetExtensionsStringARB);
#endif /* WGL_ARB_extensions_string */
#ifdef WGL_EXT_extensions_string
    if (!wglglue_wglGetExtensionsString) {
      wglglue_wglGetExtensionsString = (COIN_PFNWGLGETEXTENSIONSSTRING)PROC(wglGetExtensionsStringEXT);
    }
#endif /* WGL_EXT_extensions_string */
  }
  
  if (wglglue_wglGetExtensionsString) {
    const char * wglext = wglglue_wglGetExtensionsString(context->memorydc);
    if (coin_glglue_extension_available(wglext, reqext)) { return TRUE; }
  }

  if (coin_glglue_extension_available(glGetString(GL_EXTENSIONS), reqext)) { return TRUE; }

  return FALSE;
}

static SbBool
wglglue_resolve_symbols(struct wglglue_contextdata * context)
{
  /* short circuit out if symbols have already been resolved */
  if (wglglue_pbuffer_symbols_resolved()) { return TRUE; }

  /* we need a(ny) current context to resolve symbols */
  if (!wglglue_context_make_current(context)) { return FALSE; }

  /* attempt to resolve the symbols */

#ifdef WGL_ARB_pixel_format
  if (wglglue_ext_supported(context, "WGL_ARB_pixel_format")) {
    wglglue_wglChoosePixelFormat = (COIN_PFNWGLCHOOSEPIXELFORMATPROC)PROC(wglChoosePixelFormatARB);
  }
#endif /* WGL_ARB_pixel_format */

#ifdef WGL_EXT_pixel_format
  if (wglglue_ext_supported(context, "WGL_EXT_pixel_format")) {
    wglglue_wglChoosePixelFormat = (COIN_PFNWGLCHOOSEPIXELFORMATPROC)PROC(wglChoosePixelFormatEXT);
  }
#endif /* WGL_EXT_pixel_format */

#ifdef WGL_ARB_pbuffer
  if (wglglue_wglChoosePixelFormat && /* <- WGL_*_pbuffer depends on WGL_*_pixel_format */
      wglglue_ext_supported(context, "WGL_ARB_pbuffer")) {
    wglglue_wglCreatePbuffer = (COIN_PFNWGLCREATEPBUFFERPROC)PROC(wglCreatePbufferARB);
    wglglue_wglGetPbufferDC = (COIN_PFNWGLGETPBUFFERDCPROC)PROC(wglGetPbufferDCARB);
    wglglue_wglReleasePbufferDC = (COIN_PFNWGLRELEASEPBUFFERDCPROC)PROC(wglReleasePbufferDCARB);
    wglglue_wglDestroyPbuffer = (COIN_PFNWGLDESTROYPBUFFERPROC)PROC(wglDestroyPbufferARB);
    wglglue_wglQueryPbuffer = (COIN_PFNWGLQUERYPBUFFERPROC)PROC(wglQueryPbufferARB);
  }
#endif /* WGL_ARB_pbuffer */

#ifdef WGL_EXT_pbuffer
  if (!wglglue_pbuffer_symbols_resolved() &&
      wglglue_wglChoosePixelFormat && /* <- WGL_*_pbuffer depends on WGL_*_pixel_format */
      wglglue_ext_supported(context, "WGL_EXT_pbuffer")) {
    wglglue_wglCreatePbuffer = (COIN_PFNWGLCREATEPBUFFERPROC)PROC(wglCreatePbufferEXT);
    wglglue_wglGetPbufferDC = (COIN_PFNWGLGETPBUFFERDCPROC)PROC(wglGetPbufferDCEXT);
    wglglue_wglReleasePbufferDC = (COIN_PFNWGLRELEASEPBUFFERDCPROC)PROC(wglReleasePbufferDCEXT);
    wglglue_wglDestroyPbuffer = (COIN_PFNWGLDESTROYPBUFFERPROC)PROC(wglDestroyPbufferEXT);
    wglglue_wglQueryPbuffer = (COIN_PFNWGLQUERYPBUFFERPROC)PROC(wglQueryPbufferEXT);
  }
#endif /* WGL_EXT_pbuffer */

#ifdef WGL_ARB_render_texture
  if (wglglue_ext_supported(context, "WGL_ARB_render_texture")) {
    wglglue_wglBindTexImageARB =  (COIN_PFNWGLBINDTEXIMAGEARBPROC) PROC(wglBindTexImageARB);
    wglglue_wglReleaseTexImageARB = (COIN_PFNWGLBINDTEXIMAGEARBPROC) PROC(wglReleaseTexImageARB);
  }
#endif /* WGL_ARB_render_texture */
  wglglue_context_reinstate_previous(context);

  return wglglue_pbuffer_symbols_resolved();
}

static struct wglglue_contextdata *
wglglue_contextdata_init(unsigned int width, unsigned int height)
{
  struct wglglue_contextdata * context;

  context = (struct wglglue_contextdata *)malloc(sizeof(struct wglglue_contextdata));

  context->width = width;
  context->height = height;
  context->memorydc = NULL;
  context->pbufferwnd = NULL;
  context->didcreatememorydc = FALSE;
  context->shouldreleasememorydc = FALSE;
  context->bitmap = NULL;
  context->hpbuffer = NULL;
  context->oldbitmap = NULL;
  context->wglcontext = NULL;
  context->storedcontext = NULL;
  context->storeddc = NULL;
  context->noappglcontextavail = FALSE;
  context->pbufferisbound = FALSE;

  return context;
}

static void
wglglue_contextdata_cleanup(struct wglglue_contextdata * ctx)
{
  if (ctx == NULL) { return; }

  /* FIXME: the error handling below can and should be simplified, by
     implementing and using excpetion catching wrappers from
     glue/win32api. 20031124 mortene. */

  if (ctx->wglcontext && ctx->noappglcontextavail) {
    const BOOL r = wglDeleteContext(ctx->wglcontext);
    if (!r) {
      cc_win32_print_error("wglglue_contextdata_cleanup",
                           "wglDeleteContext", GetLastError());
    }
  }
  if (ctx->oldbitmap) {
    const HGDIOBJ o = SelectObject(ctx->memorydc, ctx->oldbitmap);
    if (!o) {
      cc_win32_print_error("wglglue_contextdata_cleanup",
                           "SelectObject", GetLastError());
    }
  }
  if (ctx->bitmap) {
    const BOOL r = DeleteObject(ctx->bitmap);
    if (!r) {
      cc_win32_print_error("wglglue_contextdata_cleanup",
                           "DeleteObject", GetLastError());
    }
  }
  if (ctx->hpbuffer) {
    {
      const int r = wglglue_wglReleasePbufferDC(ctx->hpbuffer, ctx->memorydc);
      if (!r) {
        cc_win32_print_error("wglglue_contextdata_cleanup",
                             "wglReleasePbufferDC", GetLastError());
      }
    }
    {
      const BOOL r = wglglue_wglDestroyPbuffer(ctx->hpbuffer);
      if (!r) {
        cc_win32_print_error("wglglue_contextdata_cleanup",
                             "wglDestroyPbuffer", GetLastError());
      }
    }
  }
  if (ctx->memorydc) {
    if (ctx->didcreatememorydc) { 
      const BOOL r = DeleteDC(ctx->memorydc);
      if (!r) {
        cc_win32_print_error("wglglue_contextdata_cleanup",
                             "DeleteDC", GetLastError());
      }
    }
    else if (ctx->shouldreleasememorydc && ctx->pbufferwnd) {
      int ok = ReleaseDC(ctx->pbufferwnd, ctx->memorydc);
      if (!ok) {
        cc_win32_print_error("wglglue_contextdata_cleanup",
                             "ReleaseDC", GetLastError());
      }
    }
  }
  if (ctx->pbufferwnd) {
    BOOL r = DestroyWindow(ctx->pbufferwnd);
    if (!r) {
      cc_win32_print_error("wglglue_contextdata_cleanup",
                           "DestroyWindow", GetLastError());
    }
  }

  free(ctx);
}

static SbBool 
wglglue_context_create_context(struct wglglue_contextdata * ctx, DWORD bitWin)
{
  struct wglglue_contextdata * context = (struct wglglue_contextdata *)ctx;
  int pixelformat;

  PIXELFORMATDESCRIPTOR pfd = {
    sizeof(PIXELFORMATDESCRIPTOR),   /* size of this pfd */
    1,                     /* version number */
    bitWin |               /* support bitmap or window */
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
  pixelformat = ChoosePixelFormat(context->memorydc, &pfd);
  if (pixelformat == 0) {
    DWORD dwError = GetLastError();
    cc_debugerror_postwarning("wglglue_context_create_context",
                              "ChoosePixelFormat() failed with "
                              "error code %d.", dwError);
    return FALSE;
  }
  
  /* make that the pixel format of the device context */
  if (!SetPixelFormat(context->memorydc, pixelformat, &pfd)) {
    DWORD dwError = GetLastError();
    cc_debugerror_postwarning("wglglue_context_create_context",
                              "SetPixelFormat() failed with error code %d.",
                              dwError);
    return FALSE;
  }

  context->wglcontext = wglCreateContext(context->memorydc);
  if (context->wglcontext == NULL) {
    DWORD dwError = GetLastError();
    cc_debugerror_postwarning("wglglue_context_create_context",
                              "wglCreateContext() failed with error code %d.",
                              dwError);
    return FALSE;
  }

  context->wglcontext = wglCreateContext(context->memorydc);
  if (context->wglcontext == NULL) {
    DWORD dwError = GetLastError();
    cc_debugerror_postwarning("wglglue_context_create_context",
                              "wglCreateContext() failed with error code %d.",
                              dwError);
    return FALSE;
  }

  return TRUE;
}

static SbBool
wglglue_context_create_software(struct wglglue_contextdata * ctx, SbBool warnonerrors)
{
  struct wglglue_contextdata * context = (struct wglglue_contextdata *)ctx;
 
  if (coin_glglue_debug()) {
    cc_debugerror_postwarning("wglglue_context_create_software",
                              "creating software buffer");
  }

  context->memorydc = CreateCompatibleDC(NULL);
  context->didcreatememorydc = TRUE;
  context->shouldreleasememorydc = FALSE;
  if (context->memorydc == NULL) {
    if (warnonerrors || coin_glglue_debug()) {
      DWORD dwError = GetLastError();
      cc_debugerror_postwarning("wglglue_context_create_software",
                                "CreateCompatibleDC(NULL) failed with "
                                "error code %d.", dwError);
    }
    return FALSE;
  }
  
  /* make a bitmap to draw to */
  {
    BITMAPINFO bmi;
    void * pvbits;

    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = context->width;
    bmi.bmiHeader.biHeight = context->height;
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
  
    context->bitmap = CreateDIBSection(context->memorydc, &bmi, DIB_RGB_COLORS,
                                      &pvbits, NULL, 0);
    if (context->bitmap == NULL) {
      if (warnonerrors || coin_glglue_debug()) {
        DWORD dwError = GetLastError();
        cc_debugerror_postwarning("wglglue_context_create_software",
                                  "CreateDIBSection() failed with error "
                                  "code %d.", dwError);
      }
      return FALSE;
    }
  }
  
  context->oldbitmap = (HBITMAP)
    SelectObject(context->memorydc, context->bitmap);
  if (context->oldbitmap == NULL) {
    if (warnonerrors || coin_glglue_debug()) {
      DWORD dwError = GetLastError();
      cc_debugerror_postwarning("wglglue_context_create_software",
                                "SelectObject() failed with error code %d.",
                                dwError);
    }
    return FALSE;
  }
  
  if (!(wglglue_context_create_context(context, PFD_DRAW_TO_BITMAP))) {
    return FALSE;
  }
  
  if (coin_glglue_debug()) {
    cc_debugerror_postwarning("wglglue_context_create_softwarae",
                              "success creating software buffer");
  }
  
  return TRUE;
}

static SbBool 
wglglue_context_create_pbuffer(struct wglglue_contextdata * ctx, SbBool warnonerrors)
{
  struct wglglue_contextdata * context = (struct wglglue_contextdata *)ctx;

  if (coin_glglue_debug()) {
    cc_debugerror_postwarning("wglglue_context_create_pbuffere",
                              "creating pbuffer");
  }
  
  if ((context->memorydc = wglGetCurrentDC())) { 
    context->shouldreleasememorydc = FALSE;
    context->didcreatememorydc = FALSE;
    context->wglcontext = wglGetCurrentContext();
  }
  else { context->noappglcontextavail = TRUE; }

  if (context->noappglcontextavail) {
    static int didregister = 0;
    if (!didregister) {
      WNDCLASS wc;
      didregister = 1;
      
      wc.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
      wc.lpfnWndProc    = DefWindowProc;
      wc.cbClsExtra     = 0;
      wc.cbWndExtra     = 0;
      wc.hInstance      = GetModuleHandle(NULL);
      wc.hIcon          = NULL;
      wc.hCursor        = NULL;
      wc.hbrBackground  = NULL;
      wc.lpszMenuName   = NULL;
      wc.lpszClassName  = "coin_gl_wgl";
      
      if (!RegisterClass(&wc)) {
        DWORD dwError = GetLastError();
        cc_debugerror_postwarning("wglglue_context_create_pbuffer",
                                  "RegisterClass(&wc) failed with "
                                  "error code %d.", dwError);
        return FALSE;
      }
      /* FIXME: unregister at app exit? pederb, 2003-12-15 */
    }
    
    {
      HWND hWnd;
      HINSTANCE hInstance = GetModuleHandle(NULL);
      
      if (!(hWnd = CreateWindow(
                     "coin_gl_wgl",   /* class name */
                     "coin_gl_wgl",   /* window title */
                     0,               /* selected window style */
                     0, 0,            /* window position */
                     context->width,  /* calculate adjusted window width */
                     context->height, /* calculate adjusted window height */
                     NULL,            /* no parent window */
                     NULL,            /* no menu */
                     hInstance,       /* Instance */
                     NULL)))          /* don't pass anything to WM_CREATE */
      {
        DWORD dwError = GetLastError();
        cc_debugerror_postwarning("wglglue_context_create_pbuffer",
                                  "CreateWindow(...) failed with "
                                  "error code %d.", dwError);
        return FALSE;
      }
      
      context->pbufferwnd = hWnd;
      context->memorydc = GetDC(hWnd);
      context->shouldreleasememorydc = TRUE;
      context->didcreatememorydc = FALSE;
      if (context->memorydc == NULL) {
        DWORD dwError = GetLastError();
        cc_debugerror_postwarning("wglglue_context_create_pbuffer",
                                  "GetDC(hWnd) failed with "
                                  "error code %d.", dwError);
        return FALSE;
      }
    }

    if (!(wglglue_context_create_context(context, PFD_DRAW_TO_WINDOW))) {
      return FALSE;
    }
  }    

  {
    SbBool pbuffer = wglglue_resolve_symbols(context);

    if (coin_glglue_debug()) {    
      cc_debugerror_postinfo("wglglue_context_create_pbuffer",
                             "PBuffer offscreen rendering is %ssupported "
                             "by the OpenGL driver", pbuffer ? "" : "NOT ");
    }
    
    if (!pbuffer) {
      return FALSE;
    }
  }

  {
    GLint pixformat;
    
    unsigned int numFormats;
    const float fAttribList[] = {0};
    
    const int attrs[] = {
      WGL_DRAW_TO_PBUFFER_ARB, 1,
      WGL_BIND_TO_TEXTURE_RGBA_ARB, 1,
      WGL_COLOR_BITS_ARB, 32,
      WGL_ALPHA_BITS_ARB, 8,
      WGL_DEPTH_BITS_ARB, 24,
      0, 0 /* FIXME: do I need both? pederb, 2003-11-28 */
    };

    const int pbufferflags[] = {
      WGL_TEXTURE_FORMAT_ARB, WGL_TEXTURE_RGBA_ARB,
      WGL_TEXTURE_TARGET_ARB, WGL_TEXTURE_2D_ARB,
      0, 0 /* FIXME: do I need both? pederb, 2003-11-28 */
    };

    /* choose pixel format */
    if (!wglglue_wglChoosePixelFormat(context->memorydc, attrs, fAttribList, 1,
                                      &pixformat, &numFormats)) {
      if (warnonerrors || coin_glglue_debug()) {
        cc_debugerror_postwarning("wglglue_context_create_pbuffer",
                                  "wglChoosePixelFormat() failed");
      }
      return FALSE;
    }
        
    /* create the pbuffer */
    context->hpbuffer = wglglue_wglCreatePbuffer(context->memorydc, 
                                                 pixformat,
                                                 context->width,                   
                                                 context->height, 
                                                 pbufferflags);
    if (!context->hpbuffer) {
      if (warnonerrors || coin_glglue_debug()) {
        cc_debugerror_postwarning("wglglue_context_create_pbuffer",
                                  "wglCreatePbuffer(..., %d, %d, %d, ...) failed",
                                  pixformat, context->width, context->height);
      }
      return FALSE;
    }

    /* delete/release device context and window in case we created it
       ourselves */
    if (context->memorydc) {
      if (context->didcreatememorydc) { 
        BOOL r = DeleteDC(context->memorydc); 
        if (!r) {
          cc_win32_print_error("wglglue_context_create_pbuffer",
                               "DeleteDC", GetLastError());
        }
      }
      else if (context->shouldreleasememorydc && context->pbufferwnd) {
        BOOL r = ReleaseDC(context->pbufferwnd, context->memorydc);
        if (!r) {
          cc_win32_print_error("wglglue_context_create_pbuffer",
                               "ReleaseDC", GetLastError());
        }        
      }
    }
    if (context->pbufferwnd) {
      BOOL r = DestroyWindow(context->pbufferwnd);
      if (!r) {
        cc_win32_print_error("wglglue_context_create_pbuffer",
                             "DestroyWindow", GetLastError());
      } 
      context->pbufferwnd = NULL;
    }

    context->memorydc = wglglue_wglGetPbufferDC(context->hpbuffer);
    context->didcreatememorydc = FALSE;
    context->shouldreleasememorydc = FALSE;
    if (!context->memorydc) {
      if (warnonerrors || coin_glglue_debug()) {
        cc_debugerror_postwarning("wglglue_context_create_pbuffer",
                                  "Couldn't create pbuffer's device context.");
      }
      return FALSE;
    }

    /* delete wgl context in case we created it ourselves */
    if (context->noappglcontextavail) { 
      BOOL r = wglDeleteContext(context->wglcontext); 
      if (!r) {
        if (warnonerrors || coin_glglue_debug()) {
          cc_debugerror_postwarning("wglglue_context_create_pbuffer",
                                    "Couldn't create pbuffer's device context.");
        }
      }
    }
    context->wglcontext = wglCreateContext(context->memorydc);
    if (!context->wglcontext) {
      if (warnonerrors || coin_glglue_debug()) {
        cc_debugerror_postwarning("wglglue_context_create_pbuffer",
                                  "Couldn't create rendering context for the pbuffer.");
      }
      return FALSE;
    }
    
    /* set and output the actual pBuffer dimensions */
    if (!wglglue_wglQueryPbuffer(context->hpbuffer, 
                                    WGL_PBUFFER_WIDTH_ARB, 
                                    &(context->width))) {
      if (warnonerrors || coin_glglue_debug()) {
        cc_debugerror_postwarning("wglglue_context_create_pbuffer",
                                  "Couldn't query the pbuffer width.");
      }
      return FALSE;
    }

    if (!wglglue_wglQueryPbuffer(context->hpbuffer,
                                    WGL_PBUFFER_HEIGHT_ARB,
                                    &(context->height))) {
      if (warnonerrors || coin_glglue_debug()) {
        cc_debugerror_postwarning("wglglue_context_create_pbuffer",
                                  "Couldn't query the pbuffer height.");
      }
      return FALSE;
    }
  }

  if (coin_glglue_debug()) {
    cc_debugerror_postwarning("wglglue_context_create_pbuffere",
                              "success creating pbuffer");
  }

  return TRUE;
}

/* ********************************************************************** */

/* Create and return a handle to an offscreen OpenGL buffer.

   Where p-buffer support is available that will be used instead of a
   standard offscreen WGL context, as it should render much faster
   (due to hardware acceleration).

   See:
     http://www.oss.sgi.com/projects/ogl-sample/registry/ARB/wgl_pbuffer.txt
   Or the older version:
     http://www.oss.sgi.com/projects/ogl-sample/registry/EXT/wgl_pbuffer.txt
*/
void *
wglglue_context_create_offscreen(unsigned int width, unsigned int height)
{
  struct wglglue_contextdata * swctx, * pbctx;
  SbBool ispbuffer; 

  if (coin_glglue_debug()) {
    cc_debugerror_postinfo("wglglue_context_create_offscreen",
                           "method called ");
  }

  swctx = wglglue_contextdata_init(width, height);
  assert(swctx);

  if (wglglue_context_create != NULL) {
    
    ispbuffer = wglglue_context_create == wglglue_context_create_pbuffer;
    
    /* don't warn if we fail to open a pbuffer context. we will try software  */
    if (wglglue_context_create(swctx, !ispbuffer)) { return swctx; }
    wglglue_contextdata_cleanup(swctx);
    
    /* fall back to a software context */ 
    if (ispbuffer) {
      if (coin_glglue_debug()) {
        cc_debugerror_postinfo("wglglue_context_create_offscreen",
                               "pbuffer failed. Trying software ");
      }
      swctx = wglglue_contextdata_init(width, height);
      assert(swctx);
      if (wglglue_context_create_software(swctx, TRUE)) { return swctx; }
      wglglue_contextdata_cleanup(swctx);
    }
    return NULL;
  }

  /* As there could possibly be no valid wgl context at this moment,
     we have to first make a context and set it current to be able
     to query pbuffer extension availability. */
  
  if (!wglglue_context_create_software(swctx, TRUE)) {
    wglglue_contextdata_cleanup(swctx);
    return NULL;
  }

  /* ok, so we can at least use a non-pbuffer offscreen context */
  wglglue_context_create = wglglue_context_create_software;

  /* developer or user can force pbuffer support off with this envvar */
  {
    const char * env = coin_getenv("COIN_WGLGLUE_NO_PBUFFERS");
    if (env && atoi(env) > 0) { return swctx; }
  }

  /* next, check if pbuffer support is available in the OpenGL
     library image */

  pbctx = wglglue_contextdata_init(width, height);
  assert(pbctx);

  /* attempt to create a pbuffer */
  if (!wglglue_context_create_pbuffer(pbctx, FALSE)) {
    wglglue_contextdata_cleanup(pbctx);
    return swctx;
  }

  /* pbuffers are really supported, kill the software offscreen
     context and use the pbuffer-enabled one */
  wglglue_contextdata_cleanup(swctx);

  wglglue_context_create = wglglue_context_create_pbuffer;

  return pbctx;
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
  else { (void)wglMakeCurrent(NULL, NULL); }
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
}

void 
wglglue_context_bind_pbuffer(void * ctx)
{
  struct wglglue_contextdata * context = (struct wglglue_contextdata *)ctx;
  assert(wglglue_wglBindTexImageARB != NULL);
  
  /* FIXME: check return value. pederb, 2003--11-27 */
  (void) wglglue_wglBindTexImageARB(context->hpbuffer, WGL_FRONT_LEFT_ARB);
}

void 
wglglue_context_release_pbuffer(void * ctx)
{
  struct wglglue_contextdata * context = (struct wglglue_contextdata *)ctx;
  assert(wglglue_wglReleaseTexImageARB != NULL);
  
  /* FIXME: check return value. pederb, 2003--11-27 */
  (void) wglglue_wglReleaseTexImageARB(context->hpbuffer, WGL_FRONT_LEFT_ARB);
}

SbBool 
wglglue_context_pbuffer_is_bound(void * ctx)
{
  struct wglglue_contextdata * context = (struct wglglue_contextdata *)ctx;
  return context->pbufferisbound;
}

SbBool 
wglglue_context_can_render_to_texture(void * ctx)
{
  struct wglglue_contextdata * context = (struct wglglue_contextdata *)ctx;
  return context->hpbuffer != NULL;
}

#endif /* HAVE_WGL */

/* ********************************************************************** */
