#ifndef COIN_SOSWITCH_H
#define COIN_SOSWITCH_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/nodes/SoGroup.h>

#define SO_SWITCH_NONE (-1)
#define SO_SWITCH_INHERIT (-2)
#define SO_SWITCH_ALL (-3)

class COIN_DLL_API SoSwitch : public SoGroup {
  typedef SoGroup inherited;

  SO_NODE_HEADER(SoSwitch);

public:
  static void initClass(void);
  SoSwitch(void);
  SoSwitch(int numchildren);

  SoSFInt32 whichChild;

  virtual SbBool affectsState(void) const;

  virtual void doAction(SoAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void pick(SoPickAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void handleEvent(SoHandleEventAction * action);
  virtual void getMatrix(SoGetMatrixAction * action);
  virtual void search(SoSearchAction * action);
  virtual void write(SoWriteAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);
  virtual void audioRender(SoAudioRenderAction * action);
  virtual void notify(SoNotList *nl);

protected:
  virtual ~SoSwitch();

  void traverseChildren(SoAction * action);

private:
  class SoSwitchP *pimpl;

  void commonConstructor(void);
};

#endif // !COIN_SOSWITCH_H
