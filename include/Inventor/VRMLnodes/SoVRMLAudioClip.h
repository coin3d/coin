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

#ifndef COIN_SOVRMLAUDIOCLIP_H
#define COIN_SOVRMLAUDIOCLIP_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/lists/SbStringList.h>
#include <Inventor/SbTime.h>

class COIN_DLL_API SoVRMLAudioClip : public SoNode
{
  typedef SoNode inherited;
  SO_NODE_HEADER(SoVRMLAudioClip);

public:
  typedef void *FillBufferCallback(int frameoffset, void *buffer, 
                                   int numframes, int &channels, 
                                   void * userdataptr);

  static void initClass(void);
  SoVRMLAudioClip(void);

  SoSFString description;
  SoSFBool loop;
  SoSFFloat pitch;
  SoSFTime startTime;
  SoSFTime stopTime;
  SoMFString url;

  static void  setSubdirectories(const SbList<SbString> &subdirectories);
  static const SbStringList & getSubdirectories();
  static void setDefaultPauseBetweenTracks(SbTime pause);
  static void setDefaultIntroPause(SbTime pause);
  static void setDefaultSampleRate(int samplerate);

  int getSampleRate();
  int getCurrentFrameOffset();
  void *fillBuffer(int frameoffset, void *buffer, int numframes, 
                   int &channels);

  void setFillBufferCallback(FillBufferCallback *callback, 
                             void *userdata=NULL);

  virtual void audioRender(class SoAudioRenderAction *action);

protected:
  virtual ~SoVRMLAudioClip();
  SoSFTime duration_changed; // eventOut
  SoSFBool isActive;         // eventOut

private:
  class SoVRMLAudioClipP *sovrmlaudioclip_impl;
  friend class SoVRMLAudioClipP;
};

#endif // ! COIN_SOVRMLAUDIOCLIP_H
