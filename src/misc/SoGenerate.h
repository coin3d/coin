#ifndef COIN_SOGENERATE_H
#define COIN_SOGENERATE_H

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
#endif /* !COIN_INTERNAL */

//
// reuseable code for generatePrimitives (OpenInventor, VRML and VRML2)
//

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

#endif // !COIN_SOGENERATE_H
