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


/* ---------------- Offscreen Rendering code ------------------- */

struct aglglue_contextdata {
  AGLDrawable drawable;
  AGLContext aglcontext;
  AGLContext storedcontext;
  AGLDrawable storeddrawable;
  AGLPixelFormat pixformat;
  Rect bounds;
  CGrafPtr savedport;
  GDHandle savedgdh;
};

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
  struct aglglue_contextdata context, * retctx;
  GLint attrib[] = {
    AGL_OFFSCREEN,
    AGL_RGBA,
    AGL_NO_RECOVERY,
    AGL_RED_SIZE, 8,
    AGL_GREEN_SIZE, 8,
    AGL_BLUE_SIZE, 8,
    AGL_DEPTH_SIZE, 32,
    AGL_STENCIL_SIZE, 1,
    AGL_NONE
  };

  aglglue_contextdata_init(&context);
  
  context.pixformat = aglChoosePixelFormat( NULL, 0, attrib );
  if (!context.pixformat) {
    cc_debugerror_postwarning("aglglue_context_create_offscreen",
                              "Couldn't get RGBA AGL pixelformat.");
    return NULL;
  }

  context.aglcontext = aglCreateContext(context.pixformat, NULL );
  if (!context.aglcontext) {
    cc_debugerror_postwarning("aglglue_context_create_offscreen",
                              "Couldn't create AGL context.");
    aglglue_contextdata_cleanup(&context);
    return NULL;
  } 

  if (coin_glglue_debug()) {
    cc_debugerror_postinfo("aglglue_create_offscreen_context",
                           "created new offscreen context == %p",
                           context.aglcontext);
  } 

  SetRect(&context.bounds, 0, 0, width, height);

  /* We have to save (and later restore) the old graphics port and 
     GHandle, since this function will probably called before the
     Mac OS X viewer is fully set up. */
  GetGWorld(&context.savedport, &context.savedgdh); 

  QDErr e = NewGWorld(&context.drawable, 32, &context.bounds, 
                      NULL /* cTable */,  NULL /*aGDevice */, 0);
  if(e != noErr) {
    cc_debugerror_postwarning("aglglue_create_offscreen_context",
                              "Error creating GWorld: %d", e);
  }

  if (!context.drawable) {
    cc_debugerror_postwarning("aglglue_create_offscreen_context",
                              "Couldn't create AGL drawable.");
  }

  SetGWorld(context.savedport, context.savedgdh);

  retctx = 
    (struct aglglue_contextdata *)malloc(sizeof(struct aglglue_contextdata));
  (void)memcpy(retctx, &context, sizeof(struct aglglue_contextdata));
  return retctx;
}

SbBool
aglglue_context_make_current(void * ctx)
{
  struct aglglue_contextdata * context = (struct aglglue_contextdata *)ctx;
  GLboolean r;

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

  PixMapHandle pixmap = GetGWorldPixMap((GWorldPtr)context->drawable);
  r = aglSetOffScreen(context->aglcontext, 
                      context->bounds.right - context->bounds.left, 
                      context->bounds.bottom - context->bounds.top,
                      GetPixRowBytes(pixmap), GetPixBaseAddr(pixmap));     

  if(r != GL_TRUE) {
    cc_debugerror_postwarning("aglglue_context_make_current",
                                "Error setting offscreen context");
    return FALSE;
  }

  aglSetCurrentContext(context->aglcontext);
  return TRUE;
}

void
aglglue_context_reinstate_previous(void * ctx)
{
  struct aglglue_contextdata * context = (struct aglglue_contextdata *)ctx;

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
}

void
aglglue_context_destruct(void * ctx) 
{
  struct aglglue_contextdata * context = (struct aglglue_contextdata *)ctx;

  if (coin_glglue_debug()) {
    cc_debugerror_postinfo("aglglue_context_destruct",
                           "destroy context %p", context->aglcontext);
  }

  aglglue_contextdata_cleanup(context);
  free(context);
}

#endif /* HAVE_AGL */


