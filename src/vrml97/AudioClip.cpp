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

/*!
  \class SoVRMLAudioClip SoVRMLAudioClip.h Inventor/VRMLnodes/SoVRMLAudioClip.h
  \brief The SoVRMLAudioClip class is used to load and store audio data.
  \ingroup VRMLnodes

  \WEB3DCOPYRIGHT

  \verbatim
  AudioClip {
    exposedField   SFString description      ""
    exposedField   SFBool   loop             FALSE
    exposedField   SFFloat  pitch            1.0        # (0, inf)
    exposedField   SFTime   startTime        0          # (-inf, inf)
    exposedField   SFTime   stopTime         0          # (-inf, inf)
    exposedField   MFString url              []
    eventOut       SFTime   duration_changed
    eventOut       SFBool   isActive
  }
  \endverbatim

  An AudioClip node specifies audio data that can be referenced by
  Sound nodes.  The description field specifies a textual description
  of the audio source. A browser is not required to display the
  description field but may choose to do so in addition to playing the
  sound.  The url field specifies the URL from which the sound is
  loaded.  Browsers shall support at least the wavefile format in
  uncompressed PCM format (see
  http://www.web3d.org/technicalinfo/specifications/vrml97/part1/bibliography.html#[WAV]).
  It is recommended that browsers also support the MIDI file type 1
  sound format 
  (see http://www.web3d.org/technicalinfo/specifications/vrml97/part1/references.html#[MIDI]); 
  MIDI files are presumed to use the
  General MIDI patch set. Subclause 4.5, VRML and the World Wide Web
  (<http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.5>),
  contains details on the url field. The results are undefined when no
  URLs refer to supported data types.
  
  The loop, startTime, and stopTime exposedFields and the isActive
  eventOut, and their effects on the AudioClip node, are discussed in
  detail in 4.6.9, Time-dependent nodes
  (<http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.9>).
  The "cycle" of an AudioClip is the length of time in seconds for one
  playing of the audio at the specified pitch.  The pitch field
  specifies a multiplier for the rate at which sampled sound is
  played. Values for the pitch field shall be greater than
  zero. Changing the pitch field affects both the pitch and playback
  speed of a sound. A set_pitch event to an active AudioClip is
  ignored and no pitch_changed eventOut is generated. If pitch is set
  to 2.0, the sound shall be played one octave higher than normal and
  played twice as fast. For a sampled sound, the pitch field alters
  the sampling rate at which the sound is played. The proper
  implementation of pitch control for MIDI (or other note sequence
  sound clips) is to multiply the tempo of the playback by the pitch
  value and adjust the MIDI Coarse Tune and Fine Tune controls to
  achieve the proper pitch change.  A duration_changed event is sent
  whenever there is a new value for the "normal" duration of the
  clip. Typically, this will only occur when the current url in use
  changes and the sound data has been loaded, indicating that the clip
  is playing a different sound source.  The duration is the length of
  time in seconds for one cycle of the audio for a pitch set to
  1.0. Changing the pitch field will not trigger a duration_changed
  event. A duration value of "-1" implies that the sound data has not
  yet loaded or the value is unavailable for some reason. A
  duration_changed event shall be generated if the AudioClip node is
  loaded when the VRML file is read or the AudioClip node is added to
  the scene graph.  The isActive eventOut may be used by other nodes
  to determine if the clip is currently active. If an AudioClip is
  active, it shall be playing the sound corresponding to the sound
  time (i.e., in the sound's local time system with sample 0 at time
  0): 
  \verbatim
  t = (now - startTime) modulo (duration / pitch)
  \endverbatim
*/

/*!
  \var SoSFString SoVRMLAudioClip::description
  Description of the audio clip. Default value is an empty string.
*/

/*!
  \var SoSFBool SoVRMLAudioClip::loop
  Specifies whether sound should be looped. Is FALSE by default.
*/

/*!
  \var SoSFFloat SoVRMLAudioClip::pitch
  Specifies the pitch. The default value is 1.0.

  Alters the sampling rate at which the sound is played. A pitch of
  2.0 means that the sound should be played twice as fast and one
  octave higher than normal.  
*/

/*!
  \var SoSFTime SoVRMLAudioClip::startTime
  Specifies the start time. Default value is 0.
*/
/*!
  \var SoSFTime SoVRMLAudioClip::stopTime
  Specifies the stop time. Default value is 0.
*/

/*!
  \var SoMFString SoVRMLAudioClip::url
  The audio data URL.
*/

/*!
  \var SoVRMLAudioClip::duration_changed
  An eventOut sent when a new sound starts playing. 
*/

/*!
  \var SoVRMLAudioClip::isActive
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

