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

#ifndef __SOARRAY_H__
#define __SOARRAY_H__

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFShort.h>
#include <Inventor/fields/SoSFVec3f.h>

#if defined(COIN_EXCLUDE_SOARRAY)
#error Configuration settings disrespected -- do not include this file!
#endif // COIN_EXCLUDE_SOARRAY

// *************************************************************************

class SoArray : public SoGroup {
    typedef SoGroup inherited;

  SO_NODE_HEADER(SoArray);

public:
  static void initClass(void);
  SoArray(void);

  enum Origin {
    FIRST, CENTER, LAST
  };

  SoSFEnum origin;
  SoSFShort numElements1;
  SoSFShort numElements2;
  SoSFShort numElements3;
  SoSFVec3f separation1;
  SoSFVec3f separation2;
  SoSFVec3f separation3;

  virtual SbBool affectsState(void) const;

#if !defined(COIN_EXCLUDE_SOACTION)
  virtual void doAction(SoAction * action);
#endif // !COIN_EXCLUDE_SOACTION
#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
  virtual void callback(SoCallbackAction * action);
#endif // !COIN_EXCLUDE_SOCALLBACKACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  virtual void GLRender(SoGLRenderAction * action);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOPICKACTION)
  virtual void pick(SoPickAction * action);
#endif // !COIN_EXCLUDE_SOPICKACTION
#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
#if !defined(COIN_EXCLUDE_SOHANDLEEVENTACTION)
  virtual void handleEvent(SoHandleEventAction * action);
#endif // !COIN_EXCLUDE_SOHANDLEEVENTACTION
#if !defined(COIN_EXCLUDE_SOGETMATRIXACTION)
  virtual void getMatrix(SoGetMatrixAction * action);
#endif // !COIN_EXCLUDE_SOGETMATRIXACTION
#if !defined(COIN_EXCLUDE_SOSEARCHACTION)
  virtual void search(SoSearchAction * action);
#endif // !COIN_EXCLUDE_SOSEARCHACTION
#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

protected:
  virtual ~SoArray();
};

#endif // !__SOARRAY_H__
