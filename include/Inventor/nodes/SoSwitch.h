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

#ifndef __SOSWITCH_H__
#define __SOSWITCH_H__

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/nodes/SoGroup.h>

#if defined(COIN_EXCLUDE_SOSWITCH)
#error Configuration settings disrespected -- do not include this file!
#endif // COIN_EXCLUDE_SOSWITCH

#define SO_SWITCH_NONE (-1)
#define SO_SWITCH_INHERIT (-2)
#define SO_SWITCH_ALL (-3)

// *************************************************************************

class SoSwitch : public SoGroup {
  typedef SoGroup inherited;

  SO_NODE_HEADER(SoSwitch);

public:
  static void initClass(void);
  SoSwitch(void);

  SoSFInt32 whichChild;

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
#if !defined(COIN_EXCLUDE_SOWRITEACTION)
  virtual void write(SoWriteAction * action);
#endif // !COIN_EXCLUDE_SOWRITEACTION
#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

protected:
  virtual ~SoSwitch();

#if !defined(COIN_EXCLUDE_SOACTION)
  void traverseChildren(SoAction * action);
#endif // !COIN_EXCLUDE_SOACTION
};

#endif // !__SOSWITCH_H__
