#ifndef COIN_SOLOCATEHIGHLIGHT_H
#define COIN_SOLOCATEHIGHLIGHT_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/fields/SoSFColor.h>
#include <Inventor/fields/SoSFEnum.h>

class SoFullPath;
class SoLocateHighlightP;

class COIN_DLL_API SoLocateHighlight : public SoSeparator {
  typedef SoSeparator inherited;

  SO_NODE_HEADER(SoLocateHighlight);

public:
  static void initClass(void);
  SoLocateHighlight(void);

  enum Modes {
    AUTO, ON, OFF
  };

  enum Styles {
    EMISSIVE, EMISSIVE_DIFFUSE
  };

  SoSFColor color;
  SoSFEnum style;
  SoSFEnum mode;

  virtual void handleEvent(SoHandleEventAction * action);
  virtual void GLRenderBelowPath(SoGLRenderAction * action);
  virtual void GLRenderInPath(SoGLRenderAction * action);
  static void turnOffCurrentHighlight(SoGLRenderAction * action);

protected:
  virtual ~SoLocateHighlight();
  virtual void redrawHighlighted(SoAction * act, SbBool  flag);

private:

  static void turnoffcurrent(SoAction * action);
  void setOverride(SoGLRenderAction * action);
  static SoFullPath * currenthighlight;

  SoLocateHighlightP * pimpl;
};

#endif // !COIN_SOLOCATEHIGHLIGHT_H
