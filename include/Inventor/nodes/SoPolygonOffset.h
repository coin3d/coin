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

#ifndef __SOPOLYGONOFFSET_H__
#define __SOPOLYGONOFFSET_H__

#include <Inventor/nodes/SoNode.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFBitMask.h>
#include <Inventor/fields/SoSFBool.h>

#if defined(COIN_EXCLUDE_SOPOLYGONOFFSET)
#error Configuration settings disrespected -- do not include this file!
#endif // COIN_EXCLUDE_SOPOLYGONOFFSET

#include <Inventor/elements/SoPolygonOffsetElement.h>

// *************************************************************************

class SoPolygonOffset : public SoNode {
  typedef SoNode inherited;
  
//$ BEGIN TEMPLATE NodeHeader( SoPolygonOffset )
private:
  static SoType classTypeId;

public:
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
  static void * createInstance(void);
public:
  static void initClass(void);
  static void cleanClass(void);

  SoPolygonOffset(void);
protected:
  virtual ~SoPolygonOffset();
//$ END TEMPLATE NodeHeader

public:
  enum Style {
    FILLED = SoPolygonOffsetElement::FILLED, 
    LINES = SoPolygonOffsetElement::LINES, 
    POINTS = SoPolygonOffsetElement::POINTS
  };
  
  SoSFFloat factor;
  SoSFFloat units;
  SoSFBitMask styles;
  SoSFBool on;

#if !defined(COIN_EXCLUDE_SOACTION)
  virtual void doAction(SoAction * action);
#endif // !COIN_EXCLUDE_SOACTION
#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
  virtual void callback(SoCallbackAction * action);
#endif // !COIN_EXCLUDE_SOCALLBACKACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  virtual void GLRender(SoGLRenderAction * action);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
};

#endif // !__SOPOLYGONOFFSET_H__
