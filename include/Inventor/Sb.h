#ifndef COIN_SB_H
#define COIN_SB_H

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

#if defined(COIN_INTERNAL)
#error "Do not include Sb.h internally."
#endif // COIN_INTERNAL

// Include all header files for the basic classes.

#include <Inventor/SbBSPTree.h>
#include <Inventor/SbBasic.h>
#include <Inventor/SbBox2f.h>
#include <Inventor/SbBox2s.h>
#include <Inventor/SbBox3f.h>
#include <Inventor/SbColor.h>
#include <Inventor/SbColor4f.h>
#include <Inventor/SbCylinder.h>
#include <Inventor/SbDict.h>
#include <Inventor/SbLine.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/SbName.h>
#include <Inventor/SbPlane.h>
#include <Inventor/SbRotation.h>
#include <Inventor/SbSphere.h>
#include <Inventor/SbString.h>
#include <Inventor/SbTime.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/SbVec3s.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbVec4f.h>
#include <Inventor/SbViewVolume.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/SbXfBox3f.h>
#include <Inventor/lists/SbIntList.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/lists/SbPList.h>
#include <Inventor/lists/SbStringList.h>
#include <Inventor/projectors/SbCylinderProjector.h>
#include <Inventor/projectors/SbLineProjector.h>
#include <Inventor/projectors/SbPlaneProjector.h>
#include <Inventor/projectors/SbProjector.h>
#include <Inventor/projectors/SbProjectors.h>
#include <Inventor/projectors/SbSphereProjector.h>
#include <Inventor/projectors/SbSphereSheetProjector.h>

#endif // !COIN_SB_H
