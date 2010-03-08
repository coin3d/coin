#ifndef COIN_3DSLOADER_H
#define COIN_3DSLOADER_H


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

#include <Inventor/C/basic.h>  // for M_PI

class SoInput;
class SoSeparator;

SbBool coin_3ds_read_file(SoInput * in, SoSeparator *& root,
                          int appendNormals = 2,
                          float creaseAngle = 25.f/180.f*M_PI,
                          SbBool loadMaterials = TRUE,
                          SbBool loadTextures = TRUE,
                          SbBool loadObjNames = FALSE,
                          SbBool indexedTriSet = FALSE,
                          SbBool centerModel = FALSE,
                          float modelSize = 0.0f);

#endif // !COIN_3DSLOADER_H
