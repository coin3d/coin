/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

#ifndef __SB_H__
#define __SB_H__

#if defined(__SOLIB_INTERNAL__)
#error "Do not include Sb.h internally."
#endif // __SOLIB_INTERNAL__

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

#endif // !__SB_H__
