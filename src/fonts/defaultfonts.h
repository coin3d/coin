#ifndef COIN_DEFAULTFONTS_H
#define COIN_DEFAULTFONTS_H

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
#endif /* ! COIN_INTERNAL */

/* ********************************************************************** */

#include <Inventor/system/inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int coin_default2dfont_get_height(float size);
int coin_default2dfont_get_width(float size);
const unsigned char * coin_default2dfont_get_data(float size);

const float ** coin_default3dfont_get_coords(void);
const int ** coin_default3dfont_get_faceidx(void);
const int ** coin_default3dfont_get_edgeidx(void);
float coin_default3dfont_get_advance(int charidx);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* COIN_DEFAULTSFONTS_H */
