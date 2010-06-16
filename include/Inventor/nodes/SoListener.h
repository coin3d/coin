#ifndef COIN_SOLISTENER_H
#define COIN_SOLISTENER_H

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
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/fields/SoSFFloat.h>

class SoListenerP;

class COIN_DLL_API SoListener : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoListener);
  friend class SoAudioRenderAction;

public:
  static void initClass();
  SoListener();

  SoSFVec3f position;
  SoSFRotation orientation;
  SoSFFloat gain;
  SoSFVec3f dopplerVelocity;
  SoSFFloat dopplerFactor;

protected:
  virtual void audioRender(class SoAudioRenderAction *action);
  virtual ~SoListener();

private:
  SoListenerP *pimpl;
  friend class SoListenerP;
};

#endif // COIN_SOLISTENER_H
