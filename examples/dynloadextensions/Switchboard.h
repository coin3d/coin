#ifndef SMALLCHANGE_SWITCHBOARD_H
#define SMALLCHANGE_SWITCHBOARD_H

/**************************************************************************\
 *
 *  This file is part of the SmallChange extension library for Coin.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use SmallChange with software that is incompatible
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
