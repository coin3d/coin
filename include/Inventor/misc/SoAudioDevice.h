#ifndef COIN_SOAUDIODEVICE_H
#define COIN_SOAUDIODEVICE_H

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

#include <Inventor/SbString.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoAudioRenderAction.h>

class SoAudioDeviceP;

// *************************************************************************

class COIN_DLL_API SoAudioDevice {
public:
  static SoAudioDevice * instance();

  SbBool init(const SbString & devicetype, const SbString & devicename);

  SbBool enable();
  void disable();
  SbBool isEnabled();
  SbBool haveSound();
  void setGain(float gain);
  void mute(SbBool mute=TRUE);

private:
  SoAudioDevice();
  ~SoAudioDevice();
  SoAudioDevice(const SoAudioDevice &);
  const SoAudioDevice & operator=(const SoAudioDevice &);

  SoAudioDeviceP *pimpl;
  friend class SoAudioDeviceP;
};

#endif // !COIN_SOAUDIODEVICE_H
