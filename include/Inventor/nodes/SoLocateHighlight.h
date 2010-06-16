#ifndef COIN_SOLOCATEHIGHLIGHT_H
#define COIN_SOLOCATEHIGHLIGHT_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
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

  SoLocateHighlightP * pimpl;
};

#endif // !COIN_SOLOCATEHIGHLIGHT_H
