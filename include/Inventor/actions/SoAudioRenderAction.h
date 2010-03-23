#ifndef COIN_SOAUDIORENDERACTION_H
#define COIN_SOAUDIORENDERACTION_H

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

#include <Inventor/actions/SoAction.h>
#include <Inventor/actions/SoSubAction.h>
#include <Inventor/tools/SbLazyPimplPtr.h>

class SoAudioRenderActionP;

class COIN_DLL_API SoAudioRenderAction : public SoAction {
  typedef SoAction inherited;

  SO_ACTION_HEADER(SoAudioRenderAction);

public:
  static void initClass(void);

  SoAudioRenderAction(void);
  virtual ~SoAudioRenderAction(void);

  static void callDoAction(SoAction *action, SoNode *node);
  static void callAudioRender(SoAction *action, SoNode *node);

protected:
  virtual void beginTraversal(SoNode *node);

private:
  SbLazyPimplPtr<SoAudioRenderActionP> pimpl;

  // NOT IMPLEMENTED:
  SoAudioRenderAction(const SoAudioRenderAction & rhs);
  SoAudioRenderAction & operator = (const SoAudioRenderAction & rhs);
}; // SoAudioRenderAction

#endif // !COIN_SOAUDIORENDERACTION_H
