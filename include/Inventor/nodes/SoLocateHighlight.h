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

#ifndef __SOLOCATEHIGHLIGHT_H__
#define __SOLOCATEHIGHLIGHT_H__

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/fields/SoSFColor.h>
#include <Inventor/fields/SoSFEnum.h>

#if defined(COIN_EXCLUDE_SOLOCATEHIGHLIGHT)
#error Configuration settings disrespected -- do not include this file!
#endif // COIN_EXCLUDE_SOLOCATEHIGHLIGHT

// *************************************************************************

class SoLocateHighlight : public SoSeparator {
  typedef SoSeparator inherited;

//$ BEGIN TEMPLATE NodeHeader( SoLocateHighlight )
private:
  static SoType classTypeId;

public:
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
  static void * createInstance(void);
public:
  static void initClass(void);
  static void cleanClass(void);

  SoLocateHighlight(void);
protected:
  virtual ~SoLocateHighlight();
//$ END TEMPLATE NodeHeader

public:
  enum Modes {
    AUTO, ON, OFF
  };

  enum Styles {
    EMISSIVE, EMISSIVE_DIFFUSE
  };

  SoSFColor color;
  SoSFEnum style;
  SoSFEnum mode;

#if !defined(COIN_EXCLUDE_SOHANDLEEVENTACTION)
  virtual void handleEvent( SoHandleEventAction * action );
#endif // !COIN_EXCLUDE_SOHANDLEEVENTACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  virtual void GLRenderBelowPath(SoGLRenderAction * action);
  virtual void GLRenderInPath(SoGLRenderAction * action);
  static void turnOffCurrentHighlight(SoGLRenderAction * action);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

protected:
#if !defined(COIN_EXCLUDE_SOACTION)
  virtual void redrawHighlighted(SoAction *act, SbBool  flag);
#endif // !COIN_EXCLUDE_SOACTION
};

#endif // !__SOLOCATEHIGHLIGHT_H__
