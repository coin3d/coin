/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
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

class SoVRMLAudioClipP;

class COIN_DLL_API SoVRMLAudioClip : public SoNode
{
  typedef SoNode inherited;
  SO_NODE_HEADER(SoVRMLAudioClip);

public:
  typedef void *open_func(const SbStringList &url, 
                            SoVRMLAudioClip *clip, void *userdataptr);
  typedef size_t read_func(void *datasource, 
                             void *buffer, int numframes, int &channels,
                             SoVRMLAudioClip *clip, void *userdataptr);
  typedef int seek_func(void *datasource, long offset, int whence,
                          SoVRMLAudioClip *clip, void *userdataptr);
  typedef long tell_func(void *datasource,
                         SoVRMLAudioClip *clip, void *userdataptr);
  typedef int close_func(void *datasource,
                         SoVRMLAudioClip *clip, void *userdataptr);
  
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
  static SbTime getDefaultPauseBetweenTracks();
  static void setDefaultIntroPause(SbTime pause);
  static SbTime getDefaultIntroPause();
  static void setDefaultSampleRate(int samplerate);
  static int getDefaultSampleRate();
  static void setDefaultTimerInterval(SbTime interval);
  static SbTime getDefaultTimerInterval();

  int getSampleRate();

  void * open(const SbStringList &url);
  size_t read(void *datasource, void *buffer, int numframes, int &channels);
  int    seek(void *datasource, long offset, int whence);
  long   tell(void *datasource);
  int    close(void *datasource);

  void setCallbacks(open_func *opencb, read_func *readcb, seek_func *seekcb,
                    tell_func *tellcb, close_func *closecb, void *userdataptr);

protected:
  virtual ~SoVRMLAudioClip();
  SoSFTime duration_changed; // eventOut
  SoSFBool isActive;         // eventOut

private:
  SoVRMLAudioClipP *pimpl;
  friend class SoVRMLAudioClipP;
};

#endif // ! COIN_SOVRMLAUDIOCLIP_H
