#ifndef COIN_SOPICK_H
#define COIN_SOPICK_H

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

//
// reusable code for picking
//

#include <Inventor/SbBasic.h>
#include <Inventor/system/inttypes.h>

class SoShape;
class SoRayPickAction;

// flags for cone, cylinder and cube

#define SOPICK_SIDES      0x01
#define SOPICK_TOP        0x02
#define SOPICK_BOTTOM     0x04
#define SOPICK_MATERIAL_PER_PART   0x08

void sopick_pick_cone(const float bottomRadius,
                      const float height,
                      const unsigned int flags,
                      SoShape * const shape,
                      SoRayPickAction * const action);


void sopick_pick_cylinder(const float radius,
                          const float height,
                          const unsigned int flags,
                          SoShape * const shape,
                          SoRayPickAction * const action);

void sopick_pick_sphere(const float radius,
                        SoRayPickAction * const action);

void sopick_pick_cube(const float width,
                      const float height,
                      const float depth,
                      const unsigned int flags,
                      SoShape * const shape,
                      SoRayPickAction * const action);

#endif // !COIN_SOPICK_H
