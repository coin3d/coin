#ifndef COIN_SOAUDIORENDERACTION_H
#define COIN_SOAUDIORENDERACTION_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/actions/SoAction.h>
#include <Inventor/actions/SoSubAction.h>

class COIN_DLL_API SoAudioRenderAction : public SoAction {
  typedef SoAction inherited;

  SO_ACTION_HEADER(SoAudioRenderAction);

public:
  static void initClass();
  SoAudioRenderAction();
  virtual ~SoAudioRenderAction();

  static void callDoAction(SoAction *action, SoNode *node);
  static void callAudioRender(SoAction *action, SoNode *node);

  SbBool setSceneGraphHasSoundNode(SbBool flag);
  SbBool sceneGraphHasSoundNode();

protected:
  virtual void beginTraversal(SoNode *node);

private:
  class SoAudioRenderActionP *pimpl;
};

#endif // !COIN_SOAUDIORENDERACTION_H
