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

#ifndef __SOENVIRONMENT_H__
#define __SOENVIRONMENT_H__

#include <Inventor/nodes/SoNode.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFColor.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFEnum.h>

#if defined(COIN_EXCLUDE_SOENVIRONMENT)
#error Configuration settings disrespected -- do not include this file!
#endif // COIN_EXCLUDE_SOENVIRONMENT

#include <Inventor/elements/SoEnvironmentElement.h>

// *************************************************************************

class SoEnvironment : public SoNode {
  typedef SoNode inherited;

//$ BEGIN TEMPLATE NodeHeader( SoEnvironment )
private:
  static SoType classTypeId;

public:
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
  static void * createInstance(void);
public:
  static void initClass(void);
  static void cleanClass(void);

  SoEnvironment(void);
protected:
  virtual ~SoEnvironment();
//$ END TEMPLATE NodeHeader

public:
  enum FogType {
    NONE = SoEnvironmentElement::NONE, 
    HAZE = SoEnvironmentElement::HAZE, 
    FOG = SoEnvironmentElement::FOG, 
    SMOKE = SoEnvironmentElement::SMOKE
  };

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  virtual void GLRender(SoGLRenderAction * action);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
  virtual void callback(SoCallbackAction * action);
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

  SoSFFloat ambientIntensity;
  SoSFColor ambientColor;
  SoSFVec3f attenuation;
  SoSFEnum fogType;
  SoSFColor fogColor;
  SoSFFloat fogVisibility;
};

#endif // !__SOENVIRONMENT_H__
