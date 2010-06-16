#ifndef COIN_GLUE_DLP_H
#define COIN_GLUE_DLP_H

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

/* Functions internal to the library, related to the dynamic linking
   interface. */

/* ********************************************************************** */

#ifndef COIN_INTERNAL
#error this is a private header file
#endif /* ! COIN_INTERNAL */

#include <Inventor/C/basic.h>
#include <Inventor/C/glue/dl.h>

/* ********************************************************************** */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if 0 /* to get proper auto-indentation in emacs */
}
#endif /* emacs indentation */

/* ********************************************************************** */

SbBool cc_dl_available(void);

cc_libhandle cc_dl_handle_with_gl_symbols(void);

cc_libhandle cc_dl_process_handle(void);
cc_libhandle cc_dl_coin_handle(void);
cc_libhandle cc_dl_opengl_handle(void);

/* ********************************************************************** */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !COIN_GLUE_DLP_H */
