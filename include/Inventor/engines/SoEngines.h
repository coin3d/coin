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

#ifndef __SOENGINES_H__
#define __SOENGINES_H__

#include <Inventor/engines/SoBoolOperation.h>
// FIXME: not made yet. 19990406 mortene.
//  #include <Inventor/engines/SoCalculator.h>
#include <Inventor/engines/SoComposeVec2f.h>
#include <Inventor/engines/SoComposeVec3f.h>
#include <Inventor/engines/SoComposeVec4f.h>
#include <Inventor/engines/SoDecomposeVec2f.h>
#include <Inventor/engines/SoDecomposeVec3f.h>
#include <Inventor/engines/SoDecomposeVec4f.h>
#include <Inventor/engines/SoComposeRotation.h>
// FIXME: not made yet. 19990406 mortene.
//  #include <Inventor/engines/SoComposeRotationFromTo.h>
#include <Inventor/engines/SoDecomposeRotation.h>
#include <Inventor/engines/SoComposeMatrix.h>
#include <Inventor/engines/SoDecomposeMatrix.h>
// FIXME: not made yet. 19990406 mortene.
//  #include <Inventor/engines/SoComputeBoundingBox.h>
#include <Inventor/engines/SoConcatenate.h>
#include <Inventor/engines/SoCounter.h>
#include <Inventor/engines/SoElapsedTime.h>
#include <Inventor/engines/SoFieldConverter.h>
#include <Inventor/engines/SoConvertAll.h>
#include <Inventor/engines/SoGate.h>
// FIXME: why not SoInterpolate.h? 19990407 mortene.
#include <Inventor/engines/SoInterpolateAbs.h>
#include <Inventor/engines/SoInterpolateFloat.h>
#include <Inventor/engines/SoInterpolateRotation.h>
#include <Inventor/engines/SoInterpolateVec2f.h>
#include <Inventor/engines/SoInterpolateVec3f.h>
#include <Inventor/engines/SoInterpolateVec4f.h>
#include <Inventor/engines/SoOnOff.h>
#include <Inventor/engines/SoOneShot.h>
#include <Inventor/engines/SoSelectOne.h>
// FIXME: not made yet. 19990406 mortene.
//  #include <Inventor/engines/SoTimeCounter.h>
// FIXME: not made yet. 19990406 mortene.
//  #include <Inventor/engines/SoTransformVec3f.h>
#include <Inventor/engines/SoTriggerAny.h>

#endif // !__SOENGINES_H__
