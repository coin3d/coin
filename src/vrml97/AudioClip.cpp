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
#include <Inventor/actions/SoAudioRenderAction.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/threads/SbMutex.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/SoInput.h>
#include <string.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <../misc/simage_wrapper.h>

class SoVRMLAudioClipP 
{
public:
  SoVRMLAudioClipP(SoVRMLAudioClip * interfaceptr) : ifacep(interfaceptr) {};
  SoVRMLAudioClip *ifacep;

  static void urlSensorCBWrapper(void *, SoSensor *);
  void urlSensorCB(SoSensor *);

  static void loopSensorCBWrapper(void *, SoSensor *);
  void loopSensorCB(SoSensor *);

  static void startTimeSensorCBWrapper(void *, SoSensor *);
  void startTimeSensorCB(SoSensor *);

  static void stopTimeSensorCBWrapper(void *, SoSensor *);
  void stopTimeSensorCB(SoSensor *);

  void * internalFillBuffer(int frameoffset, void *buffer, 
                            int numframes, int &channels);

  SoVRMLAudioClip::FillBufferCallback *fillBufferCallback;
  static void * internalFillBufferWrapper(int frameoffset, void *buffer, 
                                          int numframes, int &channels, 
                                          void *userdata);

  void loadUrl(void); 
  void unloadUrl(void);

  void startPlaying();
  void stopPlaying();
  
  void *fillBufferCallbackUserData;

  SoFieldSensor * urlsensor;
  SoFieldSensor * loopsensor;
  SoFieldSensor * startTimeSensor;
  SoFieldSensor * stopTimeSensor;

  static SbStringList subdirectories;
  static SbTime pauseBetweenTracks;
  static SbTime introPause;
  static int defaultSampleRate;

  int sampleRate;

  SbTime currentPause;

  SbBool openFile(int playlistIndex);
  SbBool openFile(const char *filename);
  void closeFile();

  s_stream * stream;

  int channels;
  int samplerate;
  int bitspersample;

  SbList<SbString> playlist;
  SbBool playlistDirty;
  int currentPlaylistIndex;

  SbBool loop;
  SbBool endOfFile;

  SbMutex syncmutex;

  SbTime actualStartTime;
  int totalNumberOfFramesToPlay;
};

SbStringList SoVRMLAudioClipP::subdirectories = SbStringList();
SbTime SoVRMLAudioClipP::pauseBetweenTracks = 2.0f;
SbTime SoVRMLAudioClipP::introPause = 0.0f;
int SoVRMLAudioClipP::defaultSampleRate = 44100;


#undef THIS
#define THIS this->sovrmlaudioclip_impl

#undef ITHIS
#define ITHIS this->ifacep

SO_NODE_SOURCE(SoVRMLAudioClip);

// Doc in parent
void
SoVRMLAudioClip::initClass(void) // static
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLAudioClip, SO_VRML97_NODE_TYPE);
  SoAudioRenderAction::addMethod(SoVRMLAudioClip::getClassTypeId(),
                                 SoNode::audioRenderS);
}

/*!
  Constructor.
*/
SoVRMLAudioClip::SoVRMLAudioClip(void)
{
  SO_VRMLNODE_INTERNAL_CONSTRUCTOR(SoVRMLAudioClip);

  SO_VRMLNODE_ADD_EXPOSED_FIELD(description, (""));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(loop, (FALSE));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(pitch, (1.0f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(startTime, (0.0f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(stopTime, (0.0f));
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(url);

  SO_VRMLNODE_ADD_EVENT_OUT(duration_changed);
  SO_VRMLNODE_ADD_EVENT_OUT(isActive);

  this->isActive.setValue(FALSE);

  THIS = new SoVRMLAudioClipP(this);

  THIS->urlsensor = new SoFieldSensor(THIS->urlSensorCBWrapper, THIS);
  THIS->urlsensor->setPriority(0);
  THIS->urlsensor->attach(&this->url);

  THIS->loopsensor = new SoFieldSensor(THIS->loopSensorCBWrapper, THIS);
  THIS->loopsensor->setPriority(0);
  THIS->loopsensor->attach(&this->loop);

  THIS->startTimeSensor = new SoFieldSensor(THIS->startTimeSensorCBWrapper, 
                                            THIS);
  THIS->startTimeSensor->setPriority(0);
  THIS->startTimeSensor->attach(&this->startTime);

  THIS->stopTimeSensor = new SoFieldSensor(THIS->stopTimeSensorCBWrapper, 
                                           THIS);
  THIS->stopTimeSensor->setPriority(0);
  THIS->stopTimeSensor->attach(&this->stopTime);

  THIS->loop = FALSE;
  THIS->endOfFile = FALSE;

  THIS->stream = NULL;

  THIS->channels = 0;
  THIS->bitspersample = 0;

  THIS->currentPlaylistIndex = 0;
  THIS->playlistDirty = FALSE;

  THIS->sampleRate = SoVRMLAudioClipP::defaultSampleRate;

  this->setFillBufferCallback(THIS->internalFillBufferWrapper, THIS);

  THIS->actualStartTime = 0.0f;
  THIS->totalNumberOfFramesToPlay = 0;
}

/*!
  Destructor.
*/
SoVRMLAudioClip::~SoVRMLAudioClip() 
{
  THIS->unloadUrl();

  delete THIS->urlsensor;
  delete THIS->loopsensor;
  delete THIS->startTimeSensor;
  delete THIS->stopTimeSensor;
  delete THIS;
}

void SoVRMLAudioClip::setDefaultSampleRate(int samplerate)
{
  SoVRMLAudioClipP::defaultSampleRate = samplerate;
}

void SoVRMLAudioClip::setDefaultPauseBetweenTracks(SbTime pause)
{
  // fixme: use SbTime instead. use both default and node-specific. 20021007 thammer.
  SoVRMLAudioClipP::pauseBetweenTracks = pause;
};

void SoVRMLAudioClip::setDefaultIntroPause(SbTime pause)
{
  SoVRMLAudioClipP::introPause = pause;
}

int SoVRMLAudioClip::getSampleRate()
{
  return THIS->sampleRate;
}

int SoVRMLAudioClip::getCurrentFrameOffset()
{
  /* fixme 20021007 thammer. Implement this. It is needed to support more than one 
     sound node connected to each audioclip.
  */  
  return 0;
}

void SoVRMLAudioClip::setFillBufferCallback(FillBufferCallback *callback, void *userdata)
{
  THIS->fillBufferCallback = callback;
  THIS->fillBufferCallbackUserData = userdata;
}

void * SoVRMLAudioClip::fillBuffer(int frameoffset, void *buffer, int numframes, int &channels)
{
  assert (THIS->fillBufferCallback != NULL);

  SbMutexAutoLock autoLock(&THIS->syncmutex);
  if (THIS->actualStartTime == 0.0f)
    THIS->actualStartTime = SbTime::getTimeOfDay();
  void *ret;
  ret = THIS->fillBufferCallback(frameoffset, buffer, numframes, channels, 
                                  THIS->fillBufferCallbackUserData);
  if (ret != NULL) {
    THIS->totalNumberOfFramesToPlay += numframes;
  }

  return ret;
}

void SoVRMLAudioClip::setSubdirectories(const SbList<SbString> &subdirectories)
{
  int i;
  for (i = 0; i < SoVRMLAudioClipP::subdirectories.getLength(); i++) {
    delete SoVRMLAudioClipP::subdirectories[i];
  }
  for (i = 0; i < subdirectories.getLength(); i++) {
    SoVRMLAudioClipP::subdirectories.append(new SbString(subdirectories[i]));
  }
}

const SbStringList & SoVRMLAudioClip::getSubdirectories()
{
  return SoVRMLAudioClipP::subdirectories;
}

void SoVRMLAudioClip::audioRender(SoAudioRenderAction *action)
{
  SbMutexAutoLock autoLock(&THIS->syncmutex);

  SbTime now = SbTime::getTimeOfDay();
  SbTime start = this->startTime.getValue();
  SbTime stop = this->stopTime.getValue();

#if COIN_DEBUG && DEBUG_AUDIO // debug
  SbString start_str = start.format("%D %h %m %s");
  SbString stop_str = stop.format("%D %h %m %s");
  SbString now_str = now.format("%D %h %m %s");
#endif // debug

  // 20021007 thammer removed: if ( (now<start) || ( (now>=stop) && (stop>start)) )

  if ( (now>=stop) && (stop>start) ) 
  {
    // we shouldn't be playing now
    if  (this->isActive.getValue())
      THIS->stopPlaying();
    return; 
  }

  // ( (now<stop) || (stop<=start) )

  if (THIS->endOfFile == TRUE) {
    if  (this->isActive.getValue()) {
      // FIXME: perhaps add some additional slack, the size of one buffer? 20021008 thammer.
      if ( (now-THIS->actualStartTime) > 
           ((float)THIS->totalNumberOfFramesToPlay / (float)SoVRMLAudioClipP::defaultSampleRate) )
        // we have played through the clip once, so we shouldn't play anymore
        THIS->stopPlaying(); 
    }
    return; 
  }

  if (now>=start) {
    if (!this->isActive.getValue())
      THIS->startPlaying();
  }
}

void SoVRMLAudioClipP::startPlaying()
{
#ifdef DEBUG_AUDIO
  printf("ac:start\n");
#endif
  this->currentPause = SoVRMLAudioClipP::introPause;
  this->currentPlaylistIndex = 0;
  this->endOfFile = FALSE;
  ITHIS->isActive.setValue(TRUE);
  this->actualStartTime = 0.0f; // will be set in fillBuffers
  this->totalNumberOfFramesToPlay = 0; // will be increased in fillBuffers
}

void SoVRMLAudioClipP::stopPlaying()
{
#ifdef DEBUG_AUDIO
  printf("ac:stop\n");
#endif
  ITHIS->isActive.setValue(FALSE);
  this->closeFile();
}

void * SoVRMLAudioClipP::internalFillBufferWrapper(int frameoffset, void *buffer, int numframes, 
                                               int &channels, void *userdata)
{
  SoVRMLAudioClipP *pthis = (SoVRMLAudioClipP *)userdata;
  return pthis->internalFillBuffer(frameoffset, buffer, numframes, channels);
}

void * SoVRMLAudioClipP::internalFillBuffer(int frameoffset, void *buffer, int numframes, int &channels)
{
  // 20021007 thammer note:
  // this method might be called from a thread different from the thread which created the
  // "main" Coin thread.

  /* FIXME: We should really support different sampling rates and bitspersample. 
     I think it should be the AudioClip's
     responsibility to resample if necessary. 20021007 thammer.
  */

  /* FIXME: Opening a file might take some CPU time, so we should perhaps try doing this
     in non-critical places. Such as when url changes. Perhaps we should even open
     multiple files when url changes. This _might_ improve the current problem we have
     with possible stuttering at the beginning of playing a buffer...
     20021007 thammer.
  */

  if (! (simage_wrapper()->available &&
         simage_wrapper()->versionMatchesAtLeast(1,4,0) &&
         simage_wrapper()->s_stream_get_buffer) ) {
    // without simage, we can't deliver
    this->endOfFile = TRUE;
    int outputsize = numframes * 1 * sizeof(int16_t);
    memset(buffer, 0, outputsize);
    channels=1;
    return NULL;
  }


  if (currentPause>0.0) {
    // deliver a zero'ed, mono buffer
    int outputsize = numframes * 1 * sizeof(int16_t);
    memset(buffer, 0, outputsize);
    // for (int i=0; i<outputsize; i++)
    //   ((char *)buffer)[i] = 0;
    currentPause -= (double)numframes / (double)SoVRMLAudioClipP::defaultSampleRate;
    channels = 1;
    return buffer;
  }

  if (this->playlistDirty) {
    this->playlistDirty = FALSE;
    this->closeFile();
    this->currentPlaylistIndex = 0;
  }

  if ( (!this->endOfFile) && (this->stream==NULL) ) {
    if ( this->loop && (this->currentPlaylistIndex >= this->playlist.getLength()) ) 
      this->currentPlaylistIndex = 0;
    int startindex = this->currentPlaylistIndex;
    SbBool ret = FALSE;
    while ( (!ret) && (this->currentPlaylistIndex < this->playlist.getLength()) ) {
      ret = openFile(this->currentPlaylistIndex);
      if (!ret) {
        this->currentPlaylistIndex++;
        if ( this->loop && (this->currentPlaylistIndex >= this->playlist.getLength()) &&
             (this->currentPlaylistIndex != startindex) ) 
          this->currentPlaylistIndex = 0;
      }
    } 

    if (!ret)
      this->endOfFile = TRUE;
  }

  if (this->endOfFile) {
    // deliver a zero'ed, mono buffer
    int outputsize = numframes * 1 * sizeof(int16_t);
    memset(buffer, 0, outputsize);
    channels=1;
    return NULL;
  }

  assert(this->stream!=NULL);

  assert(bitspersample == sizeof(int16_t) * 8);

  int inputsize = numframes * this->channels * this->bitspersample / 8;

  int numread = inputsize;
  
  simage_wrapper()->s_stream_get_buffer(this->stream, buffer, &numread, NULL);

  if (numread != inputsize) {
    closeFile();
    // fill rest of buffer with zeros
    for (int i=numread; i<inputsize; i++)
      ((char *)buffer)[i] = 0;

    this->currentPlaylistIndex++;
    if ( (this->currentPlaylistIndex<this->playlist.getLength()) && this->loop )
      this->currentPause = SoVRMLAudioClipP::pauseBetweenTracks;
  }

  channels = this->channels;
  return buffer;
}

void SoVRMLAudioClipP::loadUrl()
{
  SbMutexAutoLock autoLock(&this->syncmutex);

  this->unloadUrl();

  for (int i=0; i<ITHIS->url.getNum(); i++) {
    const char * str = ITHIS->url[i].getString();
    if ( (str == NULL) || (strlen(str)==0) )
      continue; // ignore empty url

    SbString filename = SoInput::searchForFile(SbString(str), SoInput::getDirectories(), 
      SoVRMLAudioClip::getSubdirectories());

    if (filename.getLength() <= 0) {
      SoDebugError::postWarning("SoVRMLAudioClipStreaming::loadUrl(index)",
                                "File not found: '%s'",
                                filename.getString());
      continue; // ignore invalid file
    }

    this->playlist.append(filename);
  }
}

void SoVRMLAudioClipP::unloadUrl()
{
  this->playlistDirty = TRUE;
  this->playlist.truncate(0);
  this->closeFile();
}

//
// called when filename changes
//
void
SoVRMLAudioClipP::urlSensorCBWrapper(void * data, SoSensor *)
{
  SoVRMLAudioClipP * thisp = (SoVRMLAudioClipP*) data;
  thisp->urlSensorCB(NULL);
}

//
// called when filename changes
//
void
SoVRMLAudioClipP::urlSensorCB(SoSensor *)
{
  this->loadUrl();
}

//
// called when loop changes
//
void
SoVRMLAudioClipP::loopSensorCBWrapper(void * data, SoSensor *)
{
  SoVRMLAudioClipP * thisp = (SoVRMLAudioClipP*) data;
  thisp->loopSensorCB(NULL);
}

//
// called when loop changes
//
void
SoVRMLAudioClipP::loopSensorCB(SoSensor *)
{
  SbMutexAutoLock autoLock(&this->syncmutex);
  this->loop = ITHIS->loop.getValue();
}

//
// called when startTime changes
//
void
SoVRMLAudioClipP::startTimeSensorCBWrapper(void * data, SoSensor *)
{
  SoVRMLAudioClipP * thisp = (SoVRMLAudioClipP*) data;
  thisp->startTimeSensorCB(NULL);
}

//
// called when startTime changes
//
void
SoVRMLAudioClipP::startTimeSensorCB(SoSensor *)
{
  SbMutexAutoLock autoLock(&this->syncmutex);

  SbTime now = SbTime::getTimeOfDay();
  SbTime start = ITHIS->startTime.getValue();

  if (now>=start) {
    if (!ITHIS->isActive.getValue())
      this->startPlaying();
  }
}

//
// called when stopTime changes
//
void
SoVRMLAudioClipP::stopTimeSensorCBWrapper(void * data, SoSensor *)
{
  SoVRMLAudioClipP * thisp = (SoVRMLAudioClipP*) data;
  thisp->stopTimeSensorCB(NULL);
}

//
// called when stopTime changes
//
void
SoVRMLAudioClipP::stopTimeSensorCB(SoSensor *)
{
  SbMutexAutoLock autoLock(&this->syncmutex);

  SbTime now = SbTime::getTimeOfDay();
  SbTime start = ITHIS->startTime.getValue();
  SbTime stop = ITHIS->stopTime.getValue();

  if ( (now>=stop) && (stop>start) ) 
  {
    // we shouldn't be playing now
    if  (ITHIS->isActive.getValue()) 
      this->stopPlaying();
    return; 
  }
}

SbBool SoVRMLAudioClipP::openFile(int playlistIndex)
{
  assert ( (playlistIndex<this->playlist.getLength()) && (playlistIndex>=0) );

  return this->openFile(this->playlist[playlistIndex].getString());
}

SbBool SoVRMLAudioClipP::openFile(const char *filename)
{
  closeFile();

  if (! (simage_wrapper()->available &&
         simage_wrapper()->versionMatchesAtLeast(1,4,0) &&
         simage_wrapper()->s_stream_open &&
         simage_wrapper()->s_params_get) ) {
    SoDebugError::postWarning("SoVRMLAudioClipP::openFile",
                              "This function needs simage v1.4.0 or newer"
                              " to read audio files. Couldn't open file '%s'",
                              filename);
    return FALSE;
  }

  this->stream = simage_wrapper()->s_stream_open(filename, NULL);
  if (this->stream == NULL) {
    SoDebugError::postWarning("SoVRMLAudioClipP::openFile",
                              "Couldn't open file '%s'",
                              filename);
    return FALSE;
  }

  s_params * params;
  params = simage_wrapper()->s_stream_params(stream);

  this->channels = 0;
  this->bitspersample = 16;
  this->samplerate = 0;
  if (params != NULL) {
    simage_wrapper()->s_params_get(params,
                 "channels", S_INTEGER_PARAM_TYPE, &this->channels, NULL);
    simage_wrapper()->s_params_get(params,
                 "samplerate", S_INTEGER_PARAM_TYPE, &this->samplerate, NULL);
  }

  #ifdef DEBUG_AUDIO
  printf("Wave file '%s' opened successfully\n", filename);
  #endif

  return TRUE; // OK
}

void SoVRMLAudioClipP::closeFile()
{
  if (this->stream != NULL) {
    if (! (simage_wrapper()->available &&
           simage_wrapper()->versionMatchesAtLeast(1,4,0) &&
           simage_wrapper()->s_stream_close &&
           simage_wrapper()->s_stream_destroy) ) {
      return;
    } else {
      simage_wrapper()->s_stream_close(this->stream);
      simage_wrapper()->s_stream_destroy(this->stream);
    }
    this->stream = NULL;
  }
}
