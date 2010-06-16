#ifndef SMALLCHANGE_SWITCHBOARD_H
#define SMALLCHANGE_SWITCHBOARD_H

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

#ifdef _MSC_VER
#ifdef SWITCHBOARD_INTERNAL
#define SWITCHBOARD_API __declspec(dllexport)
#else
#define SWITCHBOARD_API __declspec(dllimport)
#endif
#else
#define SWITCHBOARD_API
#endif

#include <Inventor/nodes/SoGroup.h>
#include <Inventor/fields/SoMFBool.h>
#include <Inventor/nodes/SoSubNode.h>

class SWITCHBOARD_API Switchboard : public SoGroup {
  typedef SoGroup inherited;
  SO_NODE_HEADER(Switchboard);

public:
  static void initClass(void);
  Switchboard(void);
  Switchboard(int numchildren);

  SoMFBool enable;

  virtual void doAction(SoAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void pick(SoPickAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void handleEvent(SoHandleEventAction * action);
  virtual void getMatrix(SoGetMatrixAction * action);
  virtual void search(SoSearchAction * action);

protected:
  virtual ~Switchboard(void);

};

#endif // !SMALLCHANGE_SWITCHBOARD_H
