/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2004 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

/*
 *  Environment variable controls available:
 * 
 *   - COIN_AGLGLUE_NO_PBUFFERS: set to 1 to force software rendering of
 *     offscreen contexts.
 */

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
#include <Inventor/C/glue/dlp.h>
#include <Inventor/C/glue/gl_agl.h>

/* ********************************************************************** */

#ifndef HAVE_AGL


SbBool aglglue_context_is_using_pbuffer(void * ctx)
{
  /* FIXME: Not sure how to handle this -- I think we should never get
     here in the first place if we don't have AGL at all. kyrah 20031123 */
  return FALSE;
}


void * aglglue_getprocaddress(const char * fname)
{
  return NULL;
}

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

/* ********************************************************************** */

#include <AGL/agl.h>  
#include <Carbon/Carbon.h> 

#ifndef HAVE_AGL_PBUFFER 

/* 
 * pBuffer functions are picked up at runtime, so the only thing
 * we need from the AGL headers is the AGLPBuffer type, which is
 * void* anyways...  
 */
typedef void * AGLPbuffer;  

#endif /* !HAVE_AGL_PBUFFER */

typedef GLboolean (* COIN_AGLCREATEPBUFFER) (GLint width, 
                                             GLint height, 
                                             GLenum target, 
                                             GLenum internalFormat, 
                                             long max_level, 
                                             AGLPbuffer *pbuffer);
typedef GLboolean (* COIN_AGLDESTROYPBUFFER) (AGLPbuffer pbuffer);
typedef GLboolean (* COIN_AGLSETPBUFFER) (AGLContext ctx, 
                                          AGLPbuffer pbuffer, 
                                          GLint face, 
                                          GLint level, 
                                          GLint screen);
typedef GLboolean (* COIN_AGLTEXIMAGEPBUFFER) (AGLContext ctx, 
                                               AGLPbuffer pbuffer, 
                                               GLint source);

static COIN_AGLCREATEPBUFFER aglglue_aglCreatePBuffer = NULL;
static COIN_AGLDESTROYPBUFFER aglglue_aglDestroyPBuffer = NULL;
static COIN_AGLSETPBUFFER aglglue_aglSetPBuffer = NULL;
static COIN_AGLTEXIMAGEPBUFFER aglglue_aglTexImagePBuffer = NULL;

struct aglglue_contextdata;
static SbBool (* aglglue_context_create)(struct aglglue_contextdata * ctx) = NULL;

static cc_libhandle glhnd = NULL;
static SbBool tried_fetching_handle = FALSE;

/* 
 * Since AGL does not have an aglGetProcAddress() as such, this is
 * simply a wrapper around cc_dl_*. Prsent here for consistency with
 * WGL and GLX implementations.
 */
void *
aglglue_getprocaddress(const char * fname)
{
  if (!tried_fetching_handle) {
    /* FIXME: never released -- one-off resource leak. 20041208 mortene.*/
    glhnd = cc_dl_handle_with_gl_symbols();
    tried_fetching_handle = TRUE;
  }

  return glhnd ? cc_dl_sym(glhnd, fname) : NULL;
}

static void
aglglue_resolve_symbols()
{
  /* FIXME: Support static binding? I don't think anybody would ever
     want to do that though... kyrah 20031114 */

  /* Resolve symbols only once... */
  if (aglglue_aglCreatePBuffer && aglglue_aglDestroyPBuffer &&
      aglglue_aglSetPBuffer && aglglue_aglTexImagePBuffer) return; 

  aglglue_aglCreatePBuffer = (COIN_AGLCREATEPBUFFER)aglglue_getprocaddress("aglCreatePBuffer");
  aglglue_aglDestroyPBuffer = (COIN_AGLDESTROYPBUFFER)aglglue_getprocaddress("aglDestroyPBuffer");
  aglglue_aglSetPBuffer = (COIN_AGLSETPBUFFER)aglglue_getprocaddress("aglSetPBuffer");
  aglglue_aglTexImagePBuffer = (COIN_AGLTEXIMAGEPBUFFER)aglglue_getprocaddress("aglTexImagePBuffer");
}


static SbBool
aglglue_get_pbuffer_enable(void)
{
  /* Make it possible to turn off pBuffer support completely.
     Mostly relevant for debugging purposes. */
  const char * env = coin_getenv("COIN_AGLGLUE_NO_PBUFFERS");
  if (env && atoi(env) > 0) { 
    return FALSE; 
  } else { 
    aglglue_resolve_symbols();
    return (aglglue_aglCreatePBuffer && aglglue_aglDestroyPBuffer && 
            aglglue_aglSetPBuffer && aglglue_aglTexImagePBuffer);
  }
}


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
  unsigned int width, height; 
  SbBool pbufferisbound;
};


static struct aglglue_contextdata *
aglglue_contextdata_init(unsigned int width, unsigned int height)
{
  struct aglglue_contextdata * ctx;
  ctx = (struct aglglue_contextdata *)malloc(sizeof(struct aglglue_contextdata));

  ctx->drawable = NULL;
  ctx->aglcontext = NULL;
  ctx->storedcontext = NULL;
  ctx->storeddrawable = NULL;
  ctx->pixformat = NULL;
  ctx->savedport = NULL;
  ctx->savedgdh = NULL;
  ctx->aglpbuffer = NULL; 
  ctx->width = width;
  ctx->height = height;
  ctx->pbufferisbound = FALSE;
  return ctx;
}

static void
aglglue_contextdata_cleanup(struct aglglue_contextdata * ctx)
{
  if (ctx->drawable) DisposeGWorld((GWorldPtr)ctx->drawable);
  if (ctx->aglcontext) aglDestroyContext(ctx->aglcontext);
  if (ctx->pixformat) aglDestroyPixelFormat(ctx->pixformat);
  if (ctx->aglpbuffer) aglglue_aglDestroyPBuffer(ctx->aglpbuffer); 
  free(ctx);
}

static SbBool
aglglue_context_create_software(struct aglglue_contextdata * ctx)
{
  GLint attrib[] = {
    AGL_OFFSCREEN,
    AGL_RGBA,
    AGL_NO_RECOVERY,
    AGL_RED_SIZE, 8,
    AGL_GREEN_SIZE, 8,
    AGL_BLUE_SIZE, 8,
    AGL_ALPHA_SIZE, 8,
    AGL_DEPTH_SIZE, 24,
    AGL_STENCIL_SIZE, 1,
    AGL_NONE
  };

  if (coin_glglue_debug()) {
    cc_debugerror_postinfo("aglglue_context_create_software",
                           "Creating software buffer.");
  }
    
  ctx->pixformat = aglChoosePixelFormat( NULL, 0, attrib );
  if (!ctx->pixformat) {
    cc_debugerror_postwarning("aglglue_context_create_software",
                              "Couldn't get RGBA AGL pixelformat.");
    return FALSE;
  }
  
  ctx->aglcontext = aglCreateContext(ctx->pixformat, NULL );
  if (!ctx->aglcontext) {
    cc_debugerror_postwarning("aglglue_context_create_software",
                              "Couldn't create AGL context.");
    aglglue_contextdata_cleanup(ctx);
    return FALSE;
  } 
  
  if (coin_glglue_debug()) {
    cc_debugerror_postinfo("aglglue_context_create_software",
                           "created new software offscreen context == %p",
                           ctx->aglcontext);
  } 
  
  SetRect(&ctx->bounds, 0, 0, ctx->width, ctx->height);
  
  /* We have to save (and later restore) the old graphics port and 
     GHandle, since this function will probably called before the
     Mac OS X viewer is fully set up. */
  GetGWorld(&ctx->savedport, &ctx->savedgdh); 
  
  {
    QDErr e = NewGWorld(&ctx->drawable, 32, &ctx->bounds, NULL, NULL, 0);
    if (e != noErr) {
      cc_debugerror_postwarning("aglglue_context_create_software",
                                "Error creating GWorld: %d", e);
      return FALSE;
    }
  }
  
  if (!ctx->drawable) {
    cc_debugerror_postwarning("aglglue_context_create_software",
                              "Couldn't create AGL drawable.");
    return FALSE;
  }
  
  SetGWorld(ctx->savedport, ctx->savedgdh);
  return TRUE;
} 


static SbBool
aglglue_context_create_pbuffer(struct aglglue_contextdata * ctx)
{
  /* FIXME: before we get here, we should have checked the requested
     dimensions in the context struct versus GLX_MAX_PBUFFER_WIDTH,
     GLX_MAX_PBUFFER_HEIGHT and GLX_MAX_PBUFFER_PIXELS
     somewhere. Copied from gl_glx.c by kyrah 20031114, originally 
     mentioned by mortene 20030811. 
  
     Update kyrah 20040714: The way to query maximum pBuffer size
     on AGL is (according to Geoff Stahl of Apple) GL_MAX_VIEWPORT_DIMS,
     but this won't get us very far, since the numbers reported
     reflect the theoretical maximum size. We'll in any case have
     to try and allocate a pBuffer to see if we can actually get
     so much VRAM...
  */

  GLenum error;
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

  /* Note that unlike in WGL, where the ability to bind a pBuffer
     as a texture has to be specified when creating the pixel
     format, AGL works in a different way: We can assume that 
     when pBuffers are supported, it is also possible to bind
     the pBuffer as a texture (i.e. call aglTexImagePBuffer).
   */

  if (coin_glglue_debug()) {
    cc_debugerror_postinfo("aglglue_context_create_pbuffer",
                           "Creating pBuffer.");
  }

  ctx->pixformat = aglChoosePixelFormat (NULL, 0, attribs);
  error = aglGetError();
  if (error != AGL_NO_ERROR) {
    cc_debugerror_post("aglglue_context_create_pbuffer",
                       "Couldn't create AGL Pixelformat: %s", 
                       (char *)aglErrorString(error));
    return FALSE;
  }
  
  if (ctx->pixformat) {
    ctx->aglcontext = aglCreateContext (ctx->pixformat, NULL);
    error = aglGetError();
    if (error != AGL_NO_ERROR) {
      cc_debugerror_post("aglglue_context_create_pbuffer",
                         "Couldn't create AGL context: %s", 
                         (char *)aglErrorString(error));
      aglglue_contextdata_cleanup(ctx);
      return FALSE;
    }
  }
  
  if (ctx->aglcontext) {

    /* The dimension of AGL pBuffers creating with the GL_TEXTURE_2D
       flag must be both squared and powers of two; else we have to
       use GL_TEXTURE_RECTANGLE_EXT. (It would of course be ok to use
       the rectangle extension in all cases, but drivers may optimize
       for the square, pow2 case.)  
       Note that it is not necessary to check for the availability of
       GL_TEXTURE_RECTANGLE_EXT - we can always assume this extension
       to be present when pBuffers are supported.
    */
    GLenum target = GL_TEXTURE_2D;
    if (!coin_is_power_of_two(ctx->width)  ||
        !coin_is_power_of_two(ctx->height) ||
        ctx->width != ctx->height) {
      target = GL_TEXTURE_RECTANGLE_EXT;
    }

    if (!aglglue_aglCreatePBuffer (ctx->width, ctx->height, target, 
                                   GL_RGBA, 0, &ctx->aglpbuffer)) {
      GLenum error = aglGetError();
      if (error != AGL_NO_ERROR) {
        cc_debugerror_post("aglglue_context_create_pbuffer",
                           "Couldn't create AGL pBuffer: %s", 
                           (char *)aglErrorString(error));
        return FALSE;
      }
    } 
  } 
  return TRUE;
}


void *
aglglue_context_create_offscreen(unsigned int width, unsigned int height)
{
  struct aglglue_contextdata * ctx;
  SbBool ok, pbuffer = FALSE, ispbuffer = FALSE;

  ctx = aglglue_contextdata_init(width, height);
  if (!ctx) return NULL;

  /* Use cached function pointer for pBuffer vs SW context creation... */
  if (aglglue_context_create != NULL) {

    ispbuffer = (aglglue_context_create == aglglue_context_create_pbuffer);

    /* Try to open a pBuffer context. If that fails, fall back to software. */
    if (aglglue_context_create(ctx)) { return ctx; }
    aglglue_contextdata_cleanup(ctx);

    if (ispbuffer) { 
      if (coin_glglue_debug()) {     
        cc_debugerror_postinfo("aglglue_context_create_offscreen",     
                              "pBuffer failed. Trying software ");
      }
      ctx = aglglue_contextdata_init(width, height);
      assert(ctx);
      if (aglglue_context_create_software(ctx)) { return ctx; } 
      aglglue_contextdata_cleanup(ctx);
    }
    return NULL;
  }

  /* ... but the first time around, we have to figure out. */
  pbuffer = aglglue_get_pbuffer_enable();

  if (coin_glglue_debug()) {
    cc_debugerror_postinfo("aglglue_context_create_offscreen",
                           "PBuffer offscreen rendering is %ssupported "
                           "by the OpenGL driver", pbuffer ? "" : "NOT ");
  }
  
  if (pbuffer && aglglue_context_create_pbuffer(ctx)) {
    aglglue_context_create = aglglue_context_create_pbuffer;
    return ctx;
  } else if (aglglue_context_create_software(ctx)) {
    aglglue_context_create = aglglue_context_create_software;
    return ctx;
  } else {
    aglglue_contextdata_cleanup(ctx);
    return NULL;
  }
}


SbBool
aglglue_context_make_current(void * ctx)
{
  struct aglglue_contextdata * context = (struct aglglue_contextdata *)ctx;
  GLint vs;

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

    vs = aglGetVirtualScreen (context->aglcontext);
    if (!aglglue_aglSetPBuffer (context->aglcontext, context->aglpbuffer, 0, 0, vs)) {
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
  /* FIXME: needs to call into the (as of yet unimplemented)
     C wrapper around the SoContextHandler. 20030310 mortene. */

  struct aglglue_contextdata * context = (struct aglglue_contextdata *)ctx;

  if (coin_glglue_debug()) {
    cc_debugerror_postinfo("aglglue_context_destruct",
                           "Destroying context %p", context->aglcontext);
  }
  aglglue_contextdata_cleanup(context);
}

void 
aglglue_context_bind_pbuffer(void * ctx)
{
  struct aglglue_contextdata * context = (struct aglglue_contextdata *)ctx;
  GLenum error = aglGetError();  
  if (error != AGL_NO_ERROR) {    
    cc_debugerror_post("aglglue_context_bind_pbuffer()"                       
                       "before binding pbuffer: %s",                        
                       (char *)aglErrorString(error)); 
  }
  
  /* FIXME: I don't think this needs to be here. pederb, 2003-11-27 */ 
  /* aglglue_context_reinstate_previous(context); */

  aglglue_aglTexImagePBuffer (context->storedcontext, context->aglpbuffer, GL_FRONT);
  context->pbufferisbound = TRUE;
  error = aglGetError();  
  if (error != AGL_NO_ERROR) {    
    cc_debugerror_post("aglglue_context_bind_pbuffer()"                       
                       "after binding pbuffer: %s",                        
                       (char *)aglErrorString(error)); 
  }
}

void 
aglglue_context_release_pbuffer(void * ctx)
{
  /* FIXME: implement, pederb 2003-11-25 */
}

SbBool 
aglglue_context_pbuffer_is_bound(void * ctx)
{
  struct aglglue_contextdata * context = (struct aglglue_contextdata *)ctx;
  return (context->pbufferisbound);
}

SbBool 
aglglue_context_can_render_to_texture(void * ctx)
{
  struct aglglue_contextdata * context = (struct aglglue_contextdata *)ctx;
  return context->aglpbuffer != NULL;
}


#endif /* HAVE_AGL */


