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

#ifndef __SOTEXTURECOORDINATEDEFAULT_H__
#define __SOTEXTURECOORDINATEDEFAULT_H__

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoTextureCoordinateFunction.h>

#if defined(COIN_EXCLUDE_SOTEXTURECOORDINATEDEFAULT)
#error Configuration settings disrespected -- do not include this file!
#endif // COIN_EXCLUDE_SOTEXTURECOORDINATEDEFAULT

// *************************************************************************

class SoTextureCoordinateDefault : public SoTextureCoordinateFunction {
  typedef SoTextureCoordinateFunction inherited;

  SO_NODE_HEADER(SoTextureCoordinateDefault);

public:
  static void initClass(void);
  SoTextureCoordinateDefault(void);


#if !defined(COIN_EXCLUDE_SOACTION)
  virtual void doAction(SoAction *action);
#endif // !COIN_EXCLUDE_SOACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  virtual void GLRender(SoGLRenderAction * action);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
  virtual void callback(SoCallbackAction * action);
#endif // COIN_EXCLUDE_SOCALLBACKACTION
#if !defined(COIN_EXCLUDE_SOPICKACTION)
  virtual void pick(SoPickAction * action);
#endif // !COIN_EXCLUDE_SOPICKACTION

protected:
  virtual ~SoTextureCoordinateDefault();
};

#endif // !__SOTEXTURECOORDINATEDEFAULT_H__
