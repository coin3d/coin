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
#include <string.h>
#include <assert.h>

#include <Inventor/C/tidbits.h>
#include <Inventor/C/glue/gl.h>
#include <Inventor/C/glue/glp.h>
#include <Inventor/C/errors/debugerror.h>
#include <Inventor/C/glue/dl.h>
#include <Inventor/C/glue/gl_agl.h>

#ifdef HAVE_AGL
#include <AGL/agl.h>  
#include <Carbon/Carbon.h> 
#endif

#ifdef HAVE_MACH_O_DYLD_H
#include <mach-o/dyld.h>
#endif /* HAVE_MACH_O_DYLD_H */

void *
coin_agl_getprocaddress(const char * fname)
{
  cc_libhandle h = cc_dl_open(NULL);
  return cc_dl_sym(h, fname);
}

#ifndef HAVE_AGL

void * aglglue_context_create_offscreen(unsigned int width, 
                                        unsigned int height) { 
  assert(FALSE); return NULL; 
}

SbBool aglglue_context_make_current(void * ctx)
{ 
  assert(FALSE); return FALSE; 
}

void aglglue_context_reinstate_previous(void * ctx) 
{ 
  assert(FALSE); 
}

void aglglue_context_destruct(void * ctx) 
{ 
  assert(FALSE); 
}

#else /* HAVE_AGL */

#ifndef HAVE_AGL_PBUFFER

/* Dummy implementations of the pBuffer related functions. */

/* FIXME: This is a temporary hack to make things compile under
   both Mac OS 10.3 (which has pBuffer support) and Mac OS 10.2
   and earlier (which hasn't). We should rather use the GL wrapper
   infrastructure for run-time lookup of these functions.
   kyrah 20031031 
 */

typedef void * AGLPbuffer;

GLboolean aglCreatePBuffer (GLint width, GLint height, GLenum target, GLenum 
  internalFormat, long max_level, AGLPbuffer *pbuffer) { 
  return FALSE; 
} 

GLboolean aglDestroyPBuffer (AGLPbuffer pbuffer) { 
  return FALSE; 
} 

GLboolean aglSetPBuffer (AGLContext ctx, AGLPbuffer pbuffer, GLint 
  face, GLint level, GLint screen) { 
  return FALSE; 
}

#endif /* !HAVE_AGL_PBUFFER */


struct aglglue_contextdata {
  AGLDrawable drawable;
  AGLContext aglcontext;
  AGLContext storedcontext;
  AGLDrawable storeddrawable;
  AGLPixelFormat pixformat;
  Rect bounds;
  CGrafPtr savedport;
  GDHandle savedgdh;
  AGLPbuffer aglpbuffer; 
};

static SbBool
aglglue_has_pbuffer_support(void)
{
  /* Make it possible to turn off pBuffer support completely.
  Mostly relevant for debugging purposes. */
  const char * env = coin_getenv("COIN_AGLGLUE_NO_PBUFFERS");
  if (env && atoi(env) > 0) { return FALSE; }
  else { return (int)coin_agl_getprocaddress("aglDescribePBuffer"); }
}

static void
aglglue_contextdata_init(struct aglglue_contextdata * c)
{
  c->drawable = NULL;
  c->aglcontext = NULL;
  c->storedcontext = NULL;
  c->storeddrawable = NULL;
  c->pixformat = NULL;
  c->savedport = NULL;
  c->savedgdh = NULL;
  c->aglpbuffer = NULL; 
}

static void
aglglue_contextdata_cleanup(struct aglglue_contextdata * c)
{
  if (c->drawable) DisposeGWorld((GWorldPtr) c->drawable);
  if (c->aglcontext) aglDestroyContext(c->aglcontext);
  if (c->pixformat) aglDestroyPixelFormat(c->pixformat);
}

void *
aglglue_context_create_offscreen(unsigned int width, unsigned int height) 
{
  QDErr e;
  struct aglglue_contextdata * context = 
    (struct aglglue_contextdata *)malloc(sizeof(struct aglglue_contextdata));
  aglglue_contextdata_init(context);

  if (!aglglue_has_pbuffer_support()) {

    if (coin_glglue_debug()) {
      cc_debugerror_postinfo("aglglue_context_create_offscreen",
                             "Not using pBuffer.");
    }

    GLint attrib[] = {
      AGL_OFFSCREEN,
      AGL_RGBA,
      AGL_NO_RECOVERY,
      AGL_RED_SIZE, 8,
      AGL_GREEN_SIZE, 8,
      AGL_BLUE_SIZE, 8,
      AGL_DEPTH_SIZE, 24,
      AGL_STENCIL_SIZE, 1,
      AGL_NONE
    };

    context->pixformat = aglChoosePixelFormat( NULL, 0, attrib );
    if (!context->pixformat) {
      cc_debugerror_postwarning("aglglue_context_create_offscreen",
                                "Couldn't get RGBA AGL pixelformat.");
      return NULL;
    }

    context->aglcontext = aglCreateContext(context->pixformat, NULL );
    if (!context->aglcontext) {
      cc_debugerror_postwarning("aglglue_context_create_offscreen",
                                "Couldn't create AGL context.");
      aglglue_contextdata_cleanup(context);
      return NULL;
    } 

    if (coin_glglue_debug()) {
      cc_debugerror_postinfo("aglglue_create_offscreen_context",
                             "created new offscreen context == %p",
                             context->aglcontext);
    } 

    SetRect(&context->bounds, 0, 0, width, height);

    /* We have to save (and later restore) the old graphics port and 
       GHandle, since this function will probably called before the
       Mac OS X viewer is fully set up. */
    GetGWorld(&context->savedport, &context->savedgdh); 

    e = NewGWorld(&context->drawable, 32, &context->bounds, 
                        NULL /* cTable */,  NULL /*aGDevice */, 0);
    if(e != noErr) {
      cc_debugerror_postwarning("aglglue_create_offscreen_context",
                                "Error creating GWorld: %d", e);
    }

    if (!context->drawable) {
      cc_debugerror_postwarning("aglglue_create_offscreen_context",
                                "Couldn't create AGL drawable.");
    }

    SetGWorld(context->savedport, context->savedgdh);
 
  } else { /* pBuffer support available */
    if (coin_glglue_debug()) {
      cc_debugerror_postinfo("aglglue_context_create_offscreen",
                              "Using pBuffer.");
    }

    GLint attribs[] = { 
      AGL_RGBA, 
      AGL_RED_SIZE, 8, 
      AGL_ALPHA_SIZE, 8, 
      AGL_DEPTH_SIZE, 24, 
      AGL_CLOSEST_POLICY, 
      AGL_ACCELERATED, 
      AGL_NO_RECOVERY, 
      AGL_NONE 
    };
    context->pixformat = aglChoosePixelFormat (NULL, 0, attribs);
    GLenum error = aglGetError();
    if (error != AGL_NO_ERROR) {
      cc_debugerror_post("aglglue_create_offscreen_context",
                         "Couldn't create AGL Pixelformat: %s", 
                         (char *)aglErrorString(error));
      return NULL;
    }

    if (context->pixformat) {
      context->aglcontext = aglCreateContext (context->pixformat, NULL);
      GLenum error = aglGetError();
      if (error != AGL_NO_ERROR) {
        cc_debugerror_post("aglglue_context_create_offscreen",
                           "Couldn't create AGL context: %s", 
                           (char *)aglErrorString(error));
        aglglue_contextdata_cleanup(context);
        return NULL;
      }
    }

    if (context->aglcontext) {
      if (!aglCreatePBuffer (width, height, GL_TEXTURE_2D, 
        GL_RGBA, 0, &context->aglpbuffer)) {
          GLenum error = aglGetError();
        if (error != AGL_NO_ERROR) {
          cc_debugerror_post("aglglue_context_create_offscreen",
                             "Couldn't create AGL context: %s", 
                             (char *)aglErrorString(error));
          return NULL;
        }
      } 
    } 
  }

  return context;
}

SbBool
aglglue_context_make_current(void * ctx)
{
  struct aglglue_contextdata * context = (struct aglglue_contextdata *)ctx;

  if (!context->aglpbuffer) {
    GLboolean r;
    PixMapHandle pixmap;

    if (context->aglcontext) {
      context->storedcontext = aglGetCurrentContext();
      context->storeddrawable = aglGetDrawable(context->storedcontext);
    }

    if (coin_glglue_debug()) {
      cc_debugerror_postinfo("aglglue_make_context_current",
                             "store current status first => context==%p, "
                             "drawable==%p",
                             context->storedcontext,
                             context->storeddrawable);
    }

    pixmap = GetGWorldPixMap((GWorldPtr)context->drawable);
    r = aglSetOffScreen(context->aglcontext, 
                        context->bounds.right - context->bounds.left, 
                        context->bounds.bottom - context->bounds.top,
                        GetPixRowBytes(pixmap), GetPixBaseAddr(pixmap));     

    if(r != GL_TRUE) {
      cc_debugerror_post("aglglue_context_make_current",
                         "Error setting offscreen context");
      return FALSE;
    }

    aglSetCurrentContext(context->aglcontext);
    return TRUE;

  } else { /* pBuffer support available */

    context->storedcontext = aglGetCurrentContext();
    if (!aglSetCurrentContext (context->aglcontext)) {
      GLenum error = aglGetError();
      if (error != AGL_NO_ERROR) {
        cc_debugerror_post("aglglue_context_make_current",
                           "Error setting offscreen context: %s", 
                           (char *)aglErrorString(error));
      }
    }

     GLint vs = aglGetVirtualScreen (context->aglcontext);
    if (!aglSetPBuffer (context->aglcontext, context->aglpbuffer, 0, 0, vs)) {
      GLenum error = aglGetError();
      if (error != AGL_NO_ERROR) {
        cc_debugerror_post("aglglue_context_make_current",
                           "aglSetPBbuffer failed: %s", 
                           (char *)aglErrorString(error));
      }
      return FALSE;
    }

    if (coin_glglue_debug()) {
      cc_debugerror_postinfo("aglglue_context_make_current", 
                             "Pbuffer Context (0x%X) Renderer: %s\n",
                             context->aglcontext, glGetString (GL_RENDERER));    
    }
    return TRUE;
  }
}

void
aglglue_context_reinstate_previous(void * ctx)
{
  struct aglglue_contextdata * context = (struct aglglue_contextdata *)ctx;

  if (!context->aglpbuffer) {

    if (coin_glglue_debug()) {
      cc_debugerror_postinfo("aglglue_context_reinstate_previous",
                             "releasing context");
    }

    if (context->storedcontext && context->storeddrawable) {

      if (coin_glglue_debug()) {
        cc_debugerror_postinfo("aglglue_context_reinstate_previous",
                               "restoring context %p to be current "
                               "(drawable==%p)",
                               context->storedcontext,
                               context->storeddrawable);
      }

      aglSetDrawable(context->storedcontext, context->storeddrawable);
      aglSetCurrentContext(context->storedcontext);
    }
  } else { /* pBuffer support available */
    if (context->storedcontext) aglSetCurrentContext(context->storedcontext);
    else aglSetCurrentContext(NULL);
  } 
}

void
aglglue_context_destruct(void * ctx) 
{
  struct aglglue_contextdata * context = (struct aglglue_contextdata *)ctx;
  if (coin_glglue_debug()) {
    cc_debugerror_postinfo("aglglue_context_destruct",
                           "Destroying context %p", context->aglcontext);
  }
  if (context->aglpbuffer) {
    aglDestroyPBuffer(context->aglpbuffer);
  }
  aglglue_contextdata_cleanup(context);
  free(context);
}

#endif /* HAVE_AGL */


