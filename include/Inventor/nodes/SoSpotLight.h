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

#ifndef __SOSPOTLIGHT_H__
#define __SOSPOTLIGHT_H__

#include <Inventor/nodes/SoLight.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFFloat.h>

#if defined(COIN_EXCLUDE_SOSPOTLIGHT)
#error Configuration settings disrespected -- do not include this file!
#endif // COIN_EXCLUDE_SOSPOTLIGHT

// *************************************************************************

class SoSpotLight : public SoLight {
  typedef SoLight inherited;

//$ BEGIN TEMPLATE NodeHeader( SoSpotLight )
private:
  static SoType classTypeId;

public:
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
  static void * createInstance(void);
public:
  static void initClass(void);
  static void cleanClass(void);

  SoSpotLight(void);
protected:
  virtual ~SoSpotLight();
//$ END TEMPLATE NodeHeader

public:
  SoSFVec3f location;
  SoSFVec3f direction;
  SoSFFloat dropOffRate;
  SoSFFloat cutOffAngle;

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  virtual void GLRender(SoGLRenderAction * action);
#endif 
};

#endif // !__SOSPOTLIGHT_H__
