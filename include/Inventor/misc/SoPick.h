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

//
// reusable code for picking
//

#ifndef COIN_SOPICK_H
#define COIN_SOPICK_H

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


#endif // COIN_SOPICK_H
