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

#ifndef __SOLEVELOFDETAIL_H__
#define __SOLEVELOFDETAIL_H__

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/fields/SoMFFloat.h>

#if defined(COIN_EXCLUDE_SOLEVELOFDETAIL)
#error Configuration settings disrespected -- do not include this file!
#endif // COIN_EXCLUDE_SOLEVELOFDETAIL

// *************************************************************************

class SoLevelOfDetail : public SoGroup {
  typedef SoGroup inherited;

  SO_NODE_HEADER(SoLevelOfDetail);

public:
  static void initClass(void);
  SoLevelOfDetail(void);

  SoMFFloat screenArea;

#if !defined(COIN_EXCLUDE_SOACTION)
  virtual void doAction(SoAction * action);
#endif // !COIN_EXCLUDE_SOACTION
#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
  virtual void callback(SoCallbackAction * action);
#endif // !COIN_EXCLUDE_SOCALLBACKACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  virtual void GLRender(SoGLRenderAction * action);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
  virtual void rayPick(SoRayPickAction * action);
#endif // !COIN_EXCLUDE_SORAYPICKACTION

protected:
  virtual ~SoLevelOfDetail();
};

#endif // !__SOLEVELOFDETAIL_H__
