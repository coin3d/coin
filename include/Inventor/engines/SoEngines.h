/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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

#if !defined(COIN_EXCLUDE_SOBOOLOPERATION)
#include <Inventor/engines/SoBoolOperation.h>
#endif // !COIN_EXCLUDE_SOBOOLOPERATION
#if !defined(COIN_EXCLUDE_SOCALCULATOR)
// FIXME: not made yet. 19990406 mortene.
//  #include <Inventor/engines/SoCalculator.h>
#endif // !COIN_EXCLUDE_SOCALCULATOR
#if !defined(COIN_EXCLUDE_SOCOMPOSEVEC2F)
#include <Inventor/engines/SoComposeVec2f.h>
#endif // !COIN_EXCLUDE_SOCOMPOSEVEC2F
#if !defined(COIN_EXCLUDE_SOCOMPOSEVEC3F)
#include <Inventor/engines/SoComposeVec3f.h>
#endif // !COIN_EXCLUDE_SOCOMPOSEVEC3F
#if !defined(COIN_EXCLUDE_SOCOMPOSEVEC4F)
#include <Inventor/engines/SoComposeVec4f.h>
#endif // !COIN_EXCLUDE_SOCOMPOSEVEC4F
#if !defined(COIN_EXCLUDE_SODECOMPOSEVEC2F)
#include <Inventor/engines/SoDecomposeVec2f.h>
#endif // !COIN_EXCLUDE_SODECOMPOSEVEC2F
#if !defined(COIN_EXCLUDE_SODECOMPOSEVEC3F)
#include <Inventor/engines/SoDecomposeVec3f.h>
#endif // !COIN_EXCLUDE_SODECOMPOSEVEC3F
#if !defined(COIN_EXCLUDE_SODECOMPOSEVEC4F)
#include <Inventor/engines/SoDecomposeVec4f.h>
#endif // !COIN_EXCLUDE_SODECOMPOSEVEC4F
#if !defined(COIN_EXCLUDE_SOCOMPOSEROTATION)
#include <Inventor/engines/SoComposeRotation.h>
#endif // !COIN_EXCLUDE_SOCOMPOSEROTATION
#if !defined(COIN_EXCLUDE_SOCOMPOSEROTATIONFROMTO)
// FIXME: not made yet. 19990406 mortene.
//  #include <Inventor/engines/SoComposeRotationFromTo.h>
#endif // !COIN_EXCLUDE_SOCOMPOSEROTATIONFROMTO
#if !defined(COIN_EXCLUDE_SODECOMPOSEROTATION)
#include <Inventor/engines/SoDecomposeRotation.h>
#endif // !COIN_EXCLUDE_SODECOMPOSEROTATION
#if !defined(COIN_EXCLUDE_SOCOMPOSEMATRIX)
#include <Inventor/engines/SoComposeMatrix.h>
#endif // !COIN_EXCLUDE_SOCOMPOSEMATRIX
#if !defined(COIN_EXCLUDE_SODECOMPOSEMATRIX)
#include <Inventor/engines/SoDecomposeMatrix.h>
#endif // !COIN_EXCLUDE_SODECOMPOSEMATRIX
#if !defined(COIN_EXCLUDE_SOCOMPUTEBOUNDINGBOX)
// FIXME: not made yet. 19990406 mortene.
//  #include <Inventor/engines/SoComputeBoundingBox.h>
#endif // !COIN_EXCLUDE_SOCOMPUTEBOUNDINGBOX
#if !defined(COIN_EXCLUDE_SOCONCATENATE)
#include <Inventor/engines/SoConcatenate.h>
#endif // !COIN_EXCLUDE_SOCONCATENATE
#if !defined(COIN_EXCLUDE_SOCOUNTER)
#include <Inventor/engines/SoCounter.h>
#endif // !COIN_EXCLUDE_SOCOUNTER
#if !defined(COIN_EXCLUDE_SOELAPSEDTIME)
#include <Inventor/engines/SoElapsedTime.h>
#endif // !COIN_EXCLUDE_SOELAPSEDTIME
#if !defined(COIN_EXCLUDE_SOFIELDCONVERTER)
#include <Inventor/engines/SoFieldConverter.h>
#endif // !COIN_EXCLUDE_SOFIELDCONVERTER
#if !defined(COIN_EXCLUDE_SOCONVERTALL)
#include <Inventor/engines/SoConvertAll.h>
#endif // !COIN_EXCLUDE_SOCONVERTALL
#if !defined(COIN_EXCLUDE_SOGATE)
#include <Inventor/engines/SoGate.h>
#endif // !COIN_EXCLUDE_SOGATE
#if !defined(COIN_EXCLUDE_SOINTERPOLATE)
// FIXME: why not SoInterpolate.h? 19990407 mortene.
#include <Inventor/engines/SoInterpolateAbs.h>
#endif // !COIN_EXCLUDE_SOINTERPOLATE
#if !defined(COIN_EXCLUDE_SOINTERPOLATEFLOAT)
#include <Inventor/engines/SoInterpolateFloat.h>
#endif // !COIN_EXCLUDE_SOINTERPOLATEFLOAT
#if !defined(COIN_EXCLUDE_SOINTERPOLATEROTATION)
#include <Inventor/engines/SoInterpolateRotation.h>
#endif // !COIN_EXCLUDE_SOINTERPOLATEROTATION
#if !defined(COIN_EXCLUDE_SOINTERPOLATEVEC2F)
#include <Inventor/engines/SoInterpolateVec2f.h>
#endif // !COIN_EXCLUDE_SOINTERPOLATEVEC2F
#if !defined(COIN_EXCLUDE_SOINTERPOLATEVEC3F)
#include <Inventor/engines/SoInterpolateVec3f.h>
#endif // !COIN_EXCLUDE_SOINTERPOLATEVEC3F
#if !defined(COIN_EXCLUDE_SOINTERPOLATEVEC4F)
#include <Inventor/engines/SoInterpolateVec4f.h>
#endif // !COIN_EXCLUDE_SOINTERPOLATEVEC4F
#if !defined(COIN_EXCLUDE_SOONOFF)
#include <Inventor/engines/SoOnOff.h>
#endif // !COIN_EXCLUDE_SOONOFF
#if !defined(COIN_EXCLUDE_SOONESHOT)
#include <Inventor/engines/SoOneShot.h>
#endif // !COIN_EXCLUDE_SOONESHOT
#if !defined(COIN_EXCLUDE_SOSELECTONE)
#include <Inventor/engines/SoSelectOne.h>
#endif // !COIN_EXCLUDE_SOSELECTONE
#if !defined(COIN_EXCLUDE_SOTIMECOUNTER)
// FIXME: not made yet. 19990406 mortene.
//  #include <Inventor/engines/SoTimeCounter.h>
#endif // !COIN_EXCLUDE_SOTIMECOUNTER
#if !defined(COIN_EXCLUDE_SOTRANSFORMVEC3F)
// FIXME: not made yet. 19990406 mortene.
//  #include <Inventor/engines/SoTransformVec3f.h>
#endif // !COIN_EXCLUDE_SOTRANSFORMVEC3F
#if !defined(COIN_EXCLUDE_SOTRIGGERANY)
#include <Inventor/engines/SoTriggerAny.h>
#endif // !COIN_EXCLUDE_SOTRIGGERANY

#endif // !__SOENGINES_H__
