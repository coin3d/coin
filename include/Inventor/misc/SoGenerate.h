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

//
// reuseable code for generatePrimitives (OpenInventor, VRML and VRML2)
//

#ifndef COIN_SOGENERATE_H
#define COIN_SOGENERATE_H

#include <Inventor/SbBasic.h>
#include <Inventor/system/inttypes.h>

class SoShape;
class SoAction;

// flags for cone, cylinder and cube

#define SOGEN_GENERATE_SIDE       0x01
#define SOGEN_GENERATE_TOP        0x02
#define SOGEN_GENERATE_BOTTOM     0x04
#define SOGEN_MATERIAL_PER_PART   0x08

void sogen_generate_cone(const float bottomRadius,
                         const float height,
                         const int numslices,
                         const unsigned int flags,
                         SoShape * const shape,
                         SoAction * const action);


void sogen_generate_cylinder(const float radius,
                             const float height,
                             const int numslices,
                             const unsigned int flags,
                             SoShape * const shape,
                             SoAction * const action);

void sogen_generate_sphere(const float radius,
                           const int numstacks,
                           const int numslices,
                           SoShape * const shape,
                           SoAction * const action);

void sogen_generate_cube(const float width,
                         const float height,
                         const float depth,
                         const unsigned int flags,
                         SoShape * const shape,
                         SoAction * const action);


#endif // COIN_SOGENERATE_H
