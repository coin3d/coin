/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOSHAPE_BIGTEXTURE_H
#define COIN_SOSHAPE_BIGTEXTURE_H

class SoGLBigImage;
class SoState;
class SoPrimitiveVertex;
class SoMaterialBundle;

int bigtexture_init(SoState * state, 
                    SoGLBigImage * image,
                    const float quality);
void bigtexture_subinit(SoState * state, const int idx);
void bigtexture_triangle(SoState * state,
                         SoMaterialBundle & mb,
                         const SoPrimitiveVertex * v1,
                         const SoPrimitiveVertex * v2,
                         const SoPrimitiveVertex * v3);


#endif // COIN_SOSHAPE_BIGTEXTURE_H
