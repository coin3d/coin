#ifndef COIN_GLUE_DLP_H
#define COIN_GLUE_DLP_H

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

/* Functions internal to the library, related to the dynamic linking
   interface. */

#ifndef COIN_INTERNAL
#error this is a private header file
#endif /* ! COIN_INTERNAL */

#include <Inventor/C/basic.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if 0 /* to get proper auto-indentation in emacs */
}
#endif /* emacs indentation */

/* Move the function from dlp.h to dl.h (and add COIN_DLL_API) for
   next minor release. (Remember to make the same change for the HEAD
   (i.e. to become 3.0) branch aswell. */
#if COIN_MINOR_VERSION < 1
#error The cc_dl_available() function should be added to the API properly from Coin 2.2.
#endif

SbBool cc_dl_available(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !COIN_GLUE_DLP_H */
