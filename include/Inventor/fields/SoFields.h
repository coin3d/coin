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

#ifndef __SOFIELDS_H__
#define __SOFIELDS_H__

#if !defined(COIN_EXCLUDE_SOFIELD)
#include <Inventor/fields/SoField.h>
#endif // !COIN_EXCLUDE_SOFIELD
#if !defined(COIN_EXCLUDE_SOSFIELD)
#include <Inventor/fields/SoSField.h>
#endif // !COIN_EXCLUDE_SOSFIELD
#if !defined(COIN_EXCLUDE_SOSFBOOL)
#include <Inventor/fields/SoSFBool.h>
#endif // !COIN_EXCLUDE_SOSFBOOL
#if !defined(COIN_EXCLUDE_SOSFCOLOR)
#include <Inventor/fields/SoSFColor.h>
#endif // !COIN_EXCLUDE_SOSFCOLOR
#if !defined(COIN_EXCLUDE_SOSFENUM)
#include <Inventor/fields/SoSFEnum.h>
#endif // !COIN_EXCLUDE_SOSFENUM
#if !defined(COIN_EXCLUDE_SOSFBITMASK)
#include <Inventor/fields/SoSFBitMask.h>
#endif // !COIN_EXCLUDE_SOSFBITMASK
#if !defined(COIN_EXCLUDE_SOSFFLOAT)
#include <Inventor/fields/SoSFFloat.h>
#endif // !COIN_EXCLUDE_SOSFFLOAT
#if !defined(COIN_EXCLUDE_SOSFIMAGE)
#include <Inventor/fields/SoSFImage.h>
#endif // !COIN_EXCLUDE_SOSFIMAGE
#if !defined(COIN_EXCLUDE_SOSFINT32)
#include <Inventor/fields/SoSFInt32.h>
#endif // !COIN_EXCLUDE_SOSFINT32
#if !defined(COIN_EXCLUDE_SOSFMATRIX)
#include <Inventor/fields/SoSFMatrix.h>
#endif // !COIN_EXCLUDE_SOSFMATRIX
#if !defined(COIN_EXCLUDE_SOSFNAME)
#include <Inventor/fields/SoSFName.h>
#endif // !COIN_EXCLUDE_SOSFNAME
#if !defined(COIN_EXCLUDE_SOSFNODE)
#include <Inventor/fields/SoSFNode.h>
#endif // !COIN_EXCLUDE_SOSFNODE
#if !defined(COIN_EXCLUDE_SOSFPATH)
#include <Inventor/fields/SoSFPath.h>
#endif // !COIN_EXCLUDE_SOSFPATH
#if !defined(COIN_EXCLUDE_SOSFPLANE)
#include <Inventor/fields/SoSFPlane.h>
#endif // !COIN_EXCLUDE_SOSFPLANE
#if !defined(COIN_EXCLUDE_SOSFROTATION)
#include <Inventor/fields/SoSFRotation.h>
#endif // !COIN_EXCLUDE_SOSFROTATION
#if !defined(COIN_EXCLUDE_SOSFSHORT)
#include <Inventor/fields/SoSFShort.h>
#endif // !COIN_EXCLUDE_SOSFSHORT
#if !defined(COIN_EXCLUDE_SOSFSTRING)
#include <Inventor/fields/SoSFString.h>
#endif // !COIN_EXCLUDE_SOSFSTRING
#if !defined(COIN_EXCLUDE_SOSFTIME)
#include <Inventor/fields/SoSFTime.h>
#endif // !COIN_EXCLUDE_SOSFTIME
#if !defined(COIN_EXCLUDE_SOSFTRIGGER)
#include <Inventor/fields/SoSFTrigger.h>
#endif // !COIN_EXCLUDE_SOSFTRIGGER
#if !defined(COIN_EXCLUDE_SOSFUINT32)
#include <Inventor/fields/SoSFUInt32.h>
#endif // !COIN_EXCLUDE_SOSFUINT32
#if !defined(COIN_EXCLUDE_SOSFUSHORT)
#include <Inventor/fields/SoSFUShort.h>
#endif // !COIN_EXCLUDE_SOSFUSHORT
#if !defined(COIN_EXCLUDE_SOSFVEC2F)
#include <Inventor/fields/SoSFVec2f.h>
#endif // !COIN_EXCLUDE_SOSFVEC2F
#if !defined(COIN_EXCLUDE_SOSFVEC3F)
#include <Inventor/fields/SoSFVec3f.h>
#endif // !COIN_EXCLUDE_SOSFVEC3F
#if !defined(COIN_EXCLUDE_SOSFVEC4F)
#include <Inventor/fields/SoSFVec4f.h>
#endif // !COIN_EXCLUDE_SOSFVEC4F
#if !defined(COIN_EXCLUDE_SOSFENGINE)
#include <Inventor/fields/SoSFEngine.h>
#endif // !COIN_EXCLUDE_SOSFENGINE

#if !defined(COIN_EXCLUDE_SOMFIELD)
#include <Inventor/fields/SoMField.h>
#endif // !COIN_EXCLUDE_SOMFIELD
#if !defined(COIN_EXCLUDE_SOMFBOOL)
#include <Inventor/fields/SoMFBool.h>
#endif // !COIN_EXCLUDE_SOMFBOOL
#if !defined(COIN_EXCLUDE_SOMFCOLOR)
#include <Inventor/fields/SoMFColor.h>
#endif // !COIN_EXCLUDE_SOMFCOLOR
#if !defined(COIN_EXCLUDE_SOMFENGINE)
#include <Inventor/fields/SoMFEngine.h>
#endif // !COIN_EXCLUDE_SOMFENGINE
#if !defined(COIN_EXCLUDE_SOMFENUM)
#include <Inventor/fields/SoMFEnum.h>
#endif // !COIN_EXCLUDE_SOMFENUM
#if !defined(COIN_EXCLUDE_SOMFBITMASK)
#include <Inventor/fields/SoMFBitMask.h>
#endif // !COIN_EXCLUDE_SOMFBITMASK
#if !defined(COIN_EXCLUDE_SOMFFLOAT)
#include <Inventor/fields/SoMFFloat.h>
#endif // !COIN_EXCLUDE_SOMFFLOAT
#if !defined(COIN_EXCLUDE_SOMFINT32)
#include <Inventor/fields/SoMFInt32.h>
#endif // !COIN_EXCLUDE_SOMFINT32
#if !defined(COIN_EXCLUDE_SOMFMATRIX)
#include <Inventor/fields/SoMFMatrix.h>
#endif // !COIN_EXCLUDE_SOMFMATRIX
#if !defined(COIN_EXCLUDE_SOMFNAME)
#include <Inventor/fields/SoMFName.h>
#endif // !COIN_EXCLUDE_SOMFNAME
#if !defined(COIN_EXCLUDE_SOMFNODE)
#include <Inventor/fields/SoMFNode.h>
#endif // !COIN_EXCLUDE_SOMFNODE
#if !defined(COIN_EXCLUDE_SOMFPATH)
#include <Inventor/fields/SoMFPath.h>
#endif // !COIN_EXCLUDE_SOMFPATH
#if !defined(COIN_EXCLUDE_SOMFPLANE)
#include <Inventor/fields/SoMFPlane.h>
#endif // !COIN_EXCLUDE_SOMFPLANE
#if !defined(COIN_EXCLUDE_SOMFROTATION)
#include <Inventor/fields/SoMFRotation.h>
#endif // !COIN_EXCLUDE_SOMFROTATION
#if !defined(COIN_EXCLUDE_SOMFSHORT)
#include <Inventor/fields/SoMFShort.h>
#endif // !COIN_EXCLUDE_SOMFSHORT
#if !defined(COIN_EXCLUDE_SOMFSTRING)
#include <Inventor/fields/SoMFString.h>
#endif // !COIN_EXCLUDE_SOMFSTRING
#if !defined(COIN_EXCLUDE_SOMFTIME)
#include <Inventor/fields/SoMFTime.h>
#endif // !COIN_EXCLUDE_SOMFTIME
#if !defined(COIN_EXCLUDE_SOMFUINT32)
#include <Inventor/fields/SoMFUInt32.h>
#endif // !COIN_EXCLUDE_SOMFUINT32
#if !defined(COIN_EXCLUDE_SOMFUSHORT)
#include <Inventor/fields/SoMFUShort.h>
#endif // !COIN_EXCLUDE_SOMFUSHORT
#if !defined(COIN_EXCLUDE_SOMFVEC2F)
#include <Inventor/fields/SoMFVec2f.h>
#endif // !COIN_EXCLUDE_SOMFVEC2F
#if !defined(COIN_EXCLUDE_SOMFVEC3F)
#include <Inventor/fields/SoMFVec3f.h>
#endif // !COIN_EXCLUDE_SOMFVEC3F
#if !defined(COIN_EXCLUDE_SOMFVEC4F)
#include <Inventor/fields/SoMFVec4f.h>
#endif // !COIN_EXCLUDE_SOMFVEC4F

#endif // !__SOFIELDS_H__
