#ifndef COIN_GLUE_INTERNAL_GLX_H
#define COIN_GLUE_INTERNAL_GLX_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#ifndef COIN_INTERNAL
#error this is a private header file
#endif

#include <Inventor/C/glue/gl.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void glxglue_init(cc_glglue * w);

void * glxglue_getprocaddress(const cc_glglue * w, const char * fname);
int glxglue_ext_supported(const cc_glglue * w, const char * extension);

void * glxglue_context_create_offscreen(unsigned int width, unsigned int height);
SbBool glxglue_context_make_current(void * ctx);
void glxglue_context_reinstate_previous(void * ctx);
void glxglue_context_destruct(void * ctx);

SbBool glxglue_context_pbuffer_max(void * ctx, unsigned int * lims);

void glxglue_cleanup(void);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !COIN_GLUE_INTERNAL_GLX_H */
