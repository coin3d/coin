/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

#ifndef COIN_SOSHAPE_BIGTEXTURE_H
#define COIN_SOSHAPE_BIGTEXTURE_H

class SoGLBigImage;
class SoState;
class SoPrimitiveVertex;
class SoMaterialBundle;
class SoShape;

// FIXME: these functions will also be part of the global namespace of
// the compiled Coin library (at least when compiled to a UNIX-style
// library). That should be avoided. 20020220 mortene.

void bigtexture_begin_shape(SoState * state,
                            SoGLBigImage * image,
                            const float quality);
void bigtexture_triangle(SoState * state,
                         const SoPrimitiveVertex * v1,
                         const SoPrimitiveVertex * v2,
                         const SoPrimitiveVertex * v3);
void bigtexture_end_shape(SoState * state, SoShape * shape,
                          SoMaterialBundle & mb);

#endif // !COIN_SOSHAPE_BIGTEXTURE_H
