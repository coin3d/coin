/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoVRMLAudioClip SoVRMLAudioClip.h Inventor/VRMLnodes/SoVRMLAudioClip.h
  \brief The SoVRMLAudioClip class is used to load and store audio data.

  Browsers should support at least the uncompressed PCM wavfile format,
  but it is supported that browsers also support the MIDI file type 1
  sound format.
*/

/*!
  \var SoVRMLAudioClip::SoSFString description
*/

/*!
  \var SoVRMLAudioClip::SoSFBool loop
  Specifies whether sound should be looped. Is FALSE by default.
*/

/*!
  \var SoVRMLAudioClip::SoSFFloat pitch
  Specifies the pitch. The default value is 1.0.

  Alters the sampling rate at which the sound is played. A pitch of
  2.0 means that the sound should be played twice as fast and one
  octave higher than normal.  
*/

/*!
  \var SoVRMLAudioClip::SoSFTime startTime
  Specifies the start time. Default value is 0.
*/
/*!
  \var SoVRMLAudioClip::SoSFTime stopTime
  Specifies the stop time. Default value is 0.
*/

/*!
  \var SoVRMLAudioClip::SoMFString url
  The audio data URL.
*/

/*!
  \var SoVRMLAudioClip::SoSFTime duration_changed
  An eventOut sent when a new sound starts playing. 
*/

/*!
  \var SoVRMLAudioClip::SoSFBool isActive
  This eventOut is sent when the sound starts/stops playing.
*/

#include <Inventor/VRMLnodes/SoVRMLAudioClip.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/nodes/SoSubNodeP.h>

SO_NODE_SOURCE(SoVRMLAudioClip);

// Doc in parent
void
SoVRMLAudioClip::initClass(void) // static
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLAudioClip, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLAudioClip::SoVRMLAudioClip(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLAudioClip);

  SO_VRMLNODE_ADD_EXPOSED_FIELD(description, (""));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(loop, (FALSE));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(pitch, (1.0f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(startTime, (0.0f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(stopTime, (0.0f));
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(url);

  SO_VRMLNODE_ADD_EVENT_OUT(duration_changed);
  SO_VRMLNODE_ADD_EVENT_OUT(isActive);
}

/*!
  Destructor.
*/
SoVRMLAudioClip::~SoVRMLAudioClip() 
{
}

// Doc in parent
void
SoVRMLAudioClip::GLRender(SoGLRenderAction * action)
{
}

