#ifndef COIN_GLUE_INTERNAL_AGL_H
#define COIN_GLUE_INTERNAL_AGL_H

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

#include <Inventor/SbBasic.h> // SbBool

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


void * aglglue_getprocaddress(const char * fname);
void * aglglue_context_create_offscreen(unsigned int width,
                                        unsigned int height);
SbBool aglglue_context_make_current(void * ctx);
void aglglue_context_reinstate_previous(void * ctx);
void aglglue_context_destruct(void * ctx);
void aglglue_context_bind_pbuffer(void * ctx);
void aglglue_context_release_pbuffer(void * ctx);
SbBool aglglue_context_pbuffer_is_bound(void * ctx);
SbBool aglglue_context_can_render_to_texture(void * ctx);
void aglglue_cleanup(void);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !COIN_GLUE_INTERNAL_AGL_H */
