#ifndef COIN_DEFAULTFONTS_H
#define COIN_DEFAULTFONTS_H

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

#ifndef COIN_INTERNAL
#error this is a private header file
#endif /* !COIN_INTERNAL */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

const int * coin_default2dfont_get_isolatin1_mapping(void);
const unsigned char * coin_default2dfont_get_data(void);
int coin_default2dfont_get_size(void);

const float ** coin_default3dfont_get_coords(void);
const int ** coin_default3dfont_get_faceidx(void);
const int ** coin_default3dfont_get_edgeidx(void);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* COIN_DEFAULTSFONTS_H */
