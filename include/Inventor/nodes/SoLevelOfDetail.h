#ifndef COIN_SOLEVELOFDETAIL_H
#define COIN_SOLEVELOFDETAIL_H

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
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/fields/SoMFFloat.h>

class SoLevelOfDetailP;

class COIN_DLL_API SoLevelOfDetail : public SoGroup {
  typedef SoGroup inherited;

  SO_NODE_HEADER(SoLevelOfDetail);

public:
  static void initClass(void);

  SoLevelOfDetail(void);
  SoLevelOfDetail(int numchildren);

  SoMFFloat screenArea;

  virtual void doAction(SoAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void rayPick(SoRayPickAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void audioRender(SoAudioRenderAction * action);

  virtual void notify(SoNotList * nl);

protected:
  virtual ~SoLevelOfDetail();

private:
  void commonConstructor(void);

  SoLevelOfDetailP * pimpl;
  friend class SoLevelOfDetailP;
};

#endif // !COIN_SOLEVELOFDETAIL_H
