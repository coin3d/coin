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

#ifndef COIN_SOVRMLSOUND_H
#define COIN_SOVRMLSOUND_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/SbTime.h>

class COIN_DLL_API SoVRMLSound : public SoNode
{
  typedef SoNode inherited;
  SO_NODE_HEADER(SoVRMLSound);

public:
  static void initClass(void);
  SoVRMLSound(void);

  SoSFNode source;
  SoSFFloat intensity;
  SoSFFloat priority;
  SoSFVec3f location;
  SoSFVec3f direction;
  SoSFFloat minFront;
  SoSFFloat maxFront;
  SoSFFloat minBack;
  SoSFFloat maxBack;
  SoSFBool spatialize;

  static void setDefaultBufferingProperties(int bufferLength, int numBuffers, SbTime sleepTime);
  void setBufferingProperties(int bufferLength, int numBuffers, SbTime sleepTime);
  void getBufferingProperties(int &bufferLength, int &numBuffers, SbTime &sleepTime);

  virtual void audioRender(SoAudioRenderAction *action);

protected:
  virtual ~SoVRMLSound(void);

private:
  class SoVRMLSoundP *sovrmlsound_impl;
  friend class SoVRMLSoundP;
};

#endif // ! COIN_SOVRMLSOUND_H
