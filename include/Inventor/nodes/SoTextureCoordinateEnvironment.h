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

#ifndef __SOTEXTURECOORDINATEENVIRONMENT_H__
#define __SOTEXTURECOORDINATEENVIRONMENT_H__

#include <Inventor/nodes/SoTextureCoordinateFunction.h>
#include <Inventor/SbVec4f.h>

#if defined(COIN_EXCLUDE_SOTEXTURECOORDINATEENVIRONMENT)
#error Configuration settings disrespected -- do not include this file!
#endif // COIN_EXCLUDE_SOTEXTURECOORDINATEENVIRONMENT

// *************************************************************************

class SoTextureCoordinateEnvironment : public SoTextureCoordinateFunction {
  typedef SoTextureCoordinateFunction inherited;

//$ BEGIN TEMPLATE NodeHeader(SoTextureCoordinateEnvironment)
private:
  static SoType classTypeId;

public:
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
  static void * createInstance(void);
public:
  static void initClass(void);
  static void cleanClass(void);

  SoTextureCoordinateEnvironment(void);
protected:
  virtual ~SoTextureCoordinateEnvironment();
//$ END TEMPLATE NodeHeader

public:

#if !defined(COIN_EXCLUDE_SOACTION)
  void doAction(SoAction * action);
#endif // !COIN_EXCLUDE_SOACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  void GLRender(SoGLRenderAction * action);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOPICKACTION)
  void pick(SoPickAction * action);
#endif // !COIN_EXCLUDE_SOPICKACTION
#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
  void callback(SoCallbackAction * action);
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

private:
  static const SbVec4f &generate(void * userdata,
				 const SbVec3f & p,
				 const SbVec3f & n);
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  static void handleTexgen(void *data);
#endif // ! COIN_EXCLUDE_SOGLRENDERACTION
};

#endif // !__SOTEXTURECOORDINATEENVIRONMENT_H__
