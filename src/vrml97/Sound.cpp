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
  \class SoVRMLSound SoVRMLSound.h Inventor/VRMLnodes/SoVRMLSound.h
  \brief The SoVRMLSound class is used to represent a sound source.
  \ingroup VRMLnodes

  \WEB3DCOPYRIGHT

  \verbatim
  Sound {
    exposedField SFVec3f  direction     0 0 1   # (-inf, inf)
    exposedField SFFloat  intensity     1       # [0,1]
    exposedField SFVec3f  location      0 0 0   # (-inf, inf)
    exposedField SFFloat  maxBack       10      # [0,inf)
    exposedField SFFloat  maxFront      10      # [0,inf)
    exposedField SFFloat  minBack       1       # [0,inf)
    exposedField SFFloat  minFront      1       # [0,inf)
    exposedField SFFloat  priority      0       # [0,1]
    exposedField SFNode   source        NULL
    field        SFBool   spatialize    TRUE
  }
  \endverbatim

  The Sound node specifies the spatial presentation of a sound in a
  VRML scene. The sound is located at a point in the local coordinate
  system and emits sound in an elliptical pattern (defined by two
  ellipsoids). The ellipsoids are oriented in a direction specified by
  the direction field. The shape of the ellipsoids may be modified to
  provide more or less directional focus from the location of the
  sound.
  
  The source field specifies the sound source for the Sound node. If
  the source field is not specified, the Sound node will not emit
  audio. The source field shall specify either an AudioClip node or a
  MovieTexture node. If a MovieTexture node is specified as the sound
  source, the MovieTexture shall refer to a movie format that supports
  sound (e.g., MPEG1-Systems).

  The intensity field adjusts the loudness (decibels) of the sound
  emitted by the Sound node (note: this is different from the
  traditional definition of intensity with respect to sound).

  The intensity field has a value that ranges from 0.0 to 1.0 and specifies
  a factor which shall be used to scale the normalized sample data of the
  sound source during playback. A Sound node with an intensity of 1.0 shall
  emit audio at its maximum loudness (before attenuation), and a Sound node
  with an intensity of 0.0 shall emit no audio. Between these values, the
  loudness should increase linearly from a -20 dB change approaching an intensity
  of 0.0 to a 0 dB change at an intensity of 1.0.

  The priority field provides a hint for the browser to choose which
  sounds to play when there are more active Sound nodes than can be
  played at once due to either limited system resources or system
  load. 7.3.4, Sound priority, attenuation, and spatialization
  (<http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#7.3.4>),
  describes a recommended algorithm for determining which sounds to
  play under such circumstances. The priority field ranges from 0.0 to
  1.0, with 1.0 being the highest priority and 0.0 the lowest
  priority.

  The location field determines the location of the sound emitter in
  the local coordinate system. A Sound node's output is audible only
  if it is part of the traversed scene. Sound nodes that are descended
  from LOD, Switch, or any grouping or prototype node that disables
  traversal (i.e., drawing) of its children are not audible unless
  they are traversed. If a Sound node is disabled by a Switch or LOD
  node, and later it becomes part of the traversal again, the sound
  shall resume where it would have been had it been playing
  continuously.

  The Sound node has an inner ellipsoid that defines a volume of space
  in which the maximum level of the sound is audible. Within this
  ellipsoid, the normalized sample data is scaled by the intensity
  field and there is no attenuation. The inner ellipsoid is defined by
  extending the direction vector through the location.

  The minBack and minFront fields specify distances behind and in
  front of the location along the direction vector respectively. The
  inner ellipsoid has one of its foci at location (the second focus is
  implicit) and intersects the direction vector at minBack and
  minFront.  The Sound node has an outer ellipsoid that defines a
  volume of space that bounds the audibility of the sound. No sound
  can be heard outside of this outer ellipsoid.

  The outer ellipsoid is defined by extending the direction vector
  through the location. The maxBack and maxFront fields specify
  distances behind and in front of the location along the direction
  vector respectively. The outer ellipsoid has one of its foci at
  location (the second focus is implicit) and intersects the direction
  vector at maxBack and maxFront.  The minFront, maxFront, minBack,
  and maxBack fields are defined in local coordinates, and shall be
  greater than or equal to zero. The minBack field shall be less than
  or equal to maxBack, and minFront shall be less than or equal to
  maxFront.  The ellipsoid parameters are specified in the local
  coordinate system but the ellipsoids' geometry is affected by
  ancestors' transformations.

  Between the two ellipsoids, there shall be a linear attenuation ramp
  in loudness, from 0 dB at the minimum ellipsoid to -20 dB at the
  maximum ellipsoid: attenuation = -20 × (d' / d") where d' is the
  distance along the location-to-viewer vector, measured from the
  transformed minimum ellipsoid boundary to the viewer, and d" is the
  distance along the location-to-viewer vector from the transformed
  minimum ellipsoid boundary to the transformed maximum ellipsoid
  boundary (see Figure 6.14).

  <center>
  <img src="http://www.web3d.org/technicalinfo/specifications/vrml97/Images/Sound.gif">
  Figure 6.14 -- Sound node geometry 
  </center>

  The spatialize field specifies if the sound is perceived as being
  directionally located relative to the viewer. If the spatialize
  field is TRUE and the viewer is located between the transformed
  inner and outer ellipsoids, the viewer's direction and the relative
  location of the Sound node should be taken into account during
  playback. Details outlining the minimum required spatialization
  functionality can be found in 7.3.4, Sound priority, attenuation,
  and spatialization
  (<http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#7.3.4>),
  If the spatialize field is FALSE, then directional effects are
  ignored, but the ellipsoid dimensions and intensity will still
  affect the loudness of the sound.  If the sound source is
  multi-channel (e.g., stereo), then the source should retain its
  channel separation during playback.

*/

/*!
  \var SoSFNode SoVRMLSound::source
  The audio stream. Either an SoVRMLAudioClip or an SoVRMLMovieTexture.
*/

/*!
  \var SoSFFloat SoVRMLSound::intensity
  Sound intensity. A value from 0 to 1. Default value is 1.
*/

/*!
  \var SoSFFloat SoVRMLSound::priority
  Browser hint for how important the sound is. A value from 0 to 1. Default value is 0.
*/

/*!
  \var SoSFVec3f SoVRMLSound::location
  The sound position. Default value is (0, 0, 0).
*/

/*!
  \var SoSFVec3f SoVRMLSound::direction
  Sound direction. Default value is (0, 0, 1).
*/

/*!
  \var SoSFFloat SoVRMLSound::minFront
  Inner ellipse front value. Default value is 1.
*/

/*!
  \var SoSFFloat SoVRMLSound::maxFront
  Outer ellipse front value. Default value is 10.
*/

/*!
  \var SoSFFloat SoVRMLSound::minBack
  Inner ellipse back value. Default value is 1.
*/

/*!
  \var SoSFFloat SoVRMLSound::maxBack
  Outer ellips back value. Default value is 10.
*/

/*!
  \var SoSFBool SoVRMLSound::spatialize
  TRUE is sound should be spatialized with respect to the viewer. Default value is TRUE.
*/

#include <Inventor/VRMLnodes/SoVRMLSound.h>
#include <Inventor/VRMLnodes/SoVRMLAudioClip.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/actions/SoAudioRenderAction.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoTimerSensor.h>
#include <Inventor/threads/SbMutex.h>
#include <Inventor/C/threads/thread.h>
#include <Inventor/SbTime.h>
#include <stddef.h>

#include "../misc/AudioTools.h"

#if HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_SOUND
#include <AL/al.h>
#include <AL/altypes.h>
#endif

class SoVRMLSoundP
{
public:
  SoVRMLSoundP(SoVRMLSound * interfaceptr) : ifacep(interfaceptr) {};
  SoVRMLSound *ifacep;

  static void sourceSensorCBWrapper(void *, SoSensor *);
  void sourceSensorCB(SoSensor *);

  SbBool stopPlaying();
  SbBool startPlaying();

  static void timercb(void * data, SoSensor *);

  static void * threadCallbackWrapper(void *userdata);
  void * threadCallback();
  void fillBuffers();

  void deleteAlBuffers();

  SoFieldSensor * sourcesensor;
#ifdef HAVE_SOUND
  ALuint sourceId;
  ALuint *alBuffers;
#endif // HAVE_SOUND
  SoVRMLAudioClip *currentAudioClip;
  SbBool playing;
  SbBool useTimerCallback;

  SoTimerSensor * timersensor;
  cc_thread *workerThread;
  volatile SbBool exitthread;
  volatile SbBool errorInThread;

  static SbMutex *syncmutex;

  int16_t *audioBuffer;
  int channels;
  SbTime sleepTime;
  SbTime workerThreadSleepTime;

  static int defaultBufferLength;
  static int defaultNumBuffers;
  static SbTime defaultSleepTime;

  int bufferLength; // bytesize = bufferLength*bitspersample/8*channels
  int numBuffers;

};

#undef THIS
#define THIS this->sovrmlsound_impl

#undef ITHIS
#define ITHIS this->ifacep


// fixme 20021006 thammer: should really do individual synchronization instead of global
SbMutex *SoVRMLSoundP::syncmutex = NULL;
int SoVRMLSoundP::defaultBufferLength = 44100/10;
int SoVRMLSoundP::defaultNumBuffers = 5;
SbTime SoVRMLSoundP::defaultSleepTime = 0.100; // 100ms

SO_NODE_SOURCE(SoVRMLSound);

// Doc in parent
void
SoVRMLSound::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLSound, SO_VRML97_NODE_TYPE);
  SoAudioRenderAction::addMethod(SoVRMLSound::getClassTypeId(),
                                 SoNode::audioRenderS);
  SoVRMLSoundP::syncmutex = new SbMutex;
}

/*!
  Constructor.
*/
SoVRMLSound::SoVRMLSound(void)
{
  SO_VRMLNODE_INTERNAL_CONSTRUCTOR(SoVRMLSound);

  SO_VRMLNODE_ADD_EXPOSED_FIELD(source, (NULL));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(intensity, (1.0f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(priority, (0.0f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(location, (0.0f, 0.0f, 0.0f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(direction, (0.0f, 0.0f, 1.0f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(minFront, (1.0f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(maxFront, (10.0f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(minBack, (1.0f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(maxBack, (10.0f));

  SO_VRMLNODE_ADD_FIELD(spatialize, (TRUE));

  THIS = new SoVRMLSoundP(this);

  THIS->channels = 1; // because spatialize defaults to TRUE and OpenAL only spatializes mono buffers

  THIS->currentAudioClip = NULL;
  THIS->playing = FALSE;

  THIS->timersensor = NULL;
  THIS->useTimerCallback = TRUE;

#ifdef HAVE_SOUND
  ALint  error;

  alGenSources(1, &(THIS->sourceId));
  if ((error = alGetError()) != AL_NO_ERROR) {
    SoDebugError::post("SoVRMLSound::SoVRMLSound",
                       "alGenSources failed. %s",
                       coin_get_openal_error(error));
    return;
  }
  THIS->alBuffers = NULL;
#endif

  THIS->sourcesensor = new SoFieldSensor(THIS->sourceSensorCBWrapper, THIS);
  THIS->sourcesensor->setPriority(0);
  THIS->sourcesensor->attach(&this->source);

  THIS->workerThread = NULL;
  THIS->exitthread = FALSE;
  THIS->errorInThread = FALSE;
  THIS->audioBuffer = NULL;

  this->setBufferingProperties(SoVRMLSoundP::defaultBufferLength, SoVRMLSoundP::defaultNumBuffers,
                               SoVRMLSoundP::defaultSleepTime);
}

/*!
  Destructor.
*/
SoVRMLSound::~SoVRMLSound(void)
{
  delete THIS->sourcesensor;

  if (THIS->currentAudioClip != NULL)
    THIS->currentAudioClip->unref();
  THIS->currentAudioClip = NULL;

  THIS->stopPlaying();

  if (THIS->audioBuffer != NULL)
    delete[] THIS->audioBuffer;

#ifdef HAVE_SOUND
  ALint  error;
  alDeleteSources(1, &(THIS->sourceId));
  if ((error = alGetError()) != AL_NO_ERROR) {
    SoDebugError::postWarning("SoVRMLSound::~SoVRMLSound",
                              "alDeleteSources() failed. %s",
                              coin_get_openal_error(error));
  }
  THIS->deleteAlBuffers();
#endif
  delete THIS;
}

void SoVRMLSound::setDefaultBufferingProperties(int bufferLength, int numBuffers, SbTime sleepTime)
{
  SoVRMLSoundP::defaultBufferLength = bufferLength;
  SoVRMLSoundP::defaultNumBuffers = numBuffers;
  SoVRMLSoundP::defaultSleepTime = sleepTime;
}

void SoVRMLSound::setBufferingProperties(int bufferLength, int numBuffers, SbTime sleepTime)
{
  SbMutexAutoLock autoLock(SoVRMLSoundP::syncmutex);
 
  THIS->numBuffers = numBuffers;
  THIS->sleepTime = sleepTime;

  if (THIS->bufferLength == bufferLength) 
    return;

  THIS->bufferLength = bufferLength;
  delete[] THIS->audioBuffer;
  THIS->audioBuffer = new int16_t[THIS->bufferLength * 2];
}

void SoVRMLSound::getBufferingProperties(int &bufferLength, int &numBuffers, SbTime &sleepTime)
{
  SbMutexAutoLock autoLock(SoVRMLSoundP::syncmutex);

  bufferLength = THIS->bufferLength;
  numBuffers = THIS->numBuffers;
  sleepTime = THIS->sleepTime;
}

#ifdef HAVE_SOUND
static inline
void SbVec3f2ALfloat3(ALfloat *dest, const SbVec3f &source)
{
  source.getValue(dest[0], dest[1], dest[2]);
}
#endif // HAVE_SOUND

void SoVRMLSound::audioRender(SoAudioRenderAction *action)
{
#ifdef HAVE_SOUND
  SbMutexAutoLock autoLock(SoVRMLSoundP::syncmutex);

  if (THIS->currentAudioClip == NULL)
    return;

  SoSFBool * isActiveField = (SoSFBool *)THIS->currentAudioClip->getField("isActive");
  SbBool isactive = isActiveField->getValue();

  if ( (!THIS->playing) && (!isactive) )
    return;

  if (THIS->errorInThread) {
    THIS->stopPlaying();
    return;
  }

  if ( THIS->playing && (!isactive) ) {
    THIS->stopPlaying();
    return;
  }

  // if we got here then we're either allready playing, or we should be

  ALint error;
  SbVec3f pos, worldpos;
  ALfloat alfloat3[3];

  pos = this->location.getValue();
  SoModelMatrixElement::get(action->getState()).multVecMatrix(pos, worldpos); 

#if COIN_DEBUG && 0 // debug
  float x, y, z;
  worldpos.getValue(x, y, z);
  printf("(%0.2f, %0.2f, %0.2f)-----------------------\n", x, y, z);
#endif // debug

  SbVec3f2ALfloat3(alfloat3, worldpos);

  // Position ...
  alSourcefv(THIS->sourceId, AL_POSITION, alfloat3);
  if ((error = alGetError()) != AL_NO_ERROR) {
    SoDebugError::postWarning("SoVRMLSound::audioRender",
                              "alSourcefv(,AL_POSITION,) failed. %s",
                              coin_get_openal_error(error));
    return;
  }

#if 0
  // 20021007 thammer note: if we ever want to implement velocity (supported by OpenAL)
  // then this is how it should be done
  // get alfloat3 from THIS->velocity
  SbVec3f2ALfloat3(alfloat3, velocity.getValue());

  alSourcefv(this->sourceId, AL_VELOCITY, alfloat3);
  if ((error = alGetError()) != AL_NO_ERROR) {
    SoDebugError::postWarning("SoVRMLSound::GLRender",
                              "alSourcefv(,AL_VELOCITY,) failed. %s",
                              coin_get_openal_error(error));
    return;
  }
#endif

  // Gain / intensity
  alSourcef(THIS->sourceId,AL_GAIN, this->intensity.getValue());
  if ((error = alGetError()) != AL_NO_ERROR) {
    SoDebugError::postWarning("SoVRMLSound::audioRender",
                              "alSourcef(,AL_GAIN,) failed. %s",
                              coin_get_openal_error(error));
    return;
  }

  int newchannels = this->spatialize.getValue() ? 1 : 2;
  if (THIS->channels != newchannels) {
    if (THIS->playing)
      THIS->stopPlaying();

    THIS->channels = newchannels;
  }

  if ( (!THIS->playing) && isactive ) 
    THIS->startPlaying();
#endif // HAVE_SOUND
}

static void
mono2stereo(short int *buffer, int length) 
{
  // assumes that buffersize = length * sizeof(short int) * 2
  for (int i=length-1; i>=0; i--) {
    buffer[i*2] = buffer[i*2+1] = buffer[i];
  }
}

static void
stereo2mono(short int *buffer, int length) 
{
  // assumes that buffersize = length * sizeof(short int) * 2
  for (int i=0; i<length; i++) {
    buffer[i] = ((long int)buffer[i*2] + (long int)buffer[i*2+1]) / 2;
  }
}

#ifdef HAVE_SOUND
static ALenum
getALSampleFormat(int channels, int bitspersample)
{
  ALenum  alformat = 0;;

  if ( (channels==1) && (bitspersample==8) )
    alformat = AL_FORMAT_MONO8;
  else if ( (channels==1) && (bitspersample==16) )
    alformat = AL_FORMAT_MONO16;
  else if ( (channels==2) && (bitspersample==8) )
    alformat = AL_FORMAT_STEREO8;
  else if ( (channels==2) && (bitspersample==16) )
    alformat = AL_FORMAT_STEREO16;

  return alformat;
}
#endif // HAVE_SOUND

void SoVRMLSoundP::deleteAlBuffers()
{
#ifdef HAVE_SOUND
  if (this->alBuffers != NULL) {
    alDeleteBuffers(this->numBuffers, this->alBuffers);
    delete [] this->alBuffers;
    this->alBuffers = NULL;
  }
#endif
}

void *SoVRMLSoundP::threadCallbackWrapper(void *userdata)
{
  SoVRMLSoundP *thisp = (SoVRMLSoundP *)userdata;
  return thisp->threadCallback();
}

void *SoVRMLSoundP::threadCallback()
{
  while (!this->exitthread) {
    this->fillBuffers();
    cc_sleep(this->workerThreadSleepTime.getValue());
  }
  return NULL;
}

void
SoVRMLSoundP::timercb(void * data, SoSensor * s)
{
  SoVRMLSoundP * thisp = (SoVRMLSoundP*) data;
  thisp->fillBuffers();
}

SbBool SoVRMLSoundP::stopPlaying()
{
#ifdef HAVE_SOUND
  #if COIN_DEBUG && DEBUG_AUDIO // debug
    printf("sound:stop[.");
  #endif // debug

  if (!this->playing)
    return TRUE;

  #if COIN_DEBUG && DEBUG_AUDIO // debug
    printf(".");
  #endif // debug

  ALint error;

  // stop timersensor
  if (this->timersensor) {
    if (this->timersensor->isScheduled()) 
      this->timersensor->unschedule();
    delete this->timersensor;
    this->timersensor = NULL;
  }

  // stop thread
  if (this->workerThread!=NULL) {
    this->exitthread = TRUE;
    void *retval = NULL;
    cc_thread_join(this->workerThread, &retval);
    cc_thread_destruct(this->workerThread);
    this->workerThread = NULL;
  }

  #if COIN_DEBUG && DEBUG_AUDIO // debug
    printf(".");
  #endif // debug

  this->errorInThread = FALSE;

  SbBool retval = TRUE;

  // wait until all buffers have been played
  ALint      processed;

#if 0
  // this will (busy) wait for all remaining buffers to play
  // 20021008 thammer
  SbBool done = FALSE;
  while (!done) {
    ALint      state;
    #ifdef _WIN32
      alGetSourcei(this->sourceId, AL_SOURCE_STATE, &state);
    #else
      alGetSourceiv(this->sourceId, AL_SOURCE_STATE, &state);
    #endif

    #ifdef _WIN32
      alGetSourcei(this->sourceId, AL_BUFFERS_PROCESSED, &processed);
    #else
      alGetSourceiv(this->sourceId, AL_BUFFERS_PROCESSED, &processed);
    #endif

    ALint      queued;
    #ifdef _WIN32
      alGetSourcei(this->sourceId, AL_BUFFERS_QUEUED, &queued);
    #else
      alGetSourceiv(this->sourceId, AL_BUFFERS_QUEUED, &queued);
    #endif

    #if COIN_DEBUG && DEBUG_AUDIO // debug
      printf("State: %d, Processed: %d, Queued: %d\n", state, processed, queued);
    #endif // debug

    // if (state == AL_PLAYING)
    if (processed<queued)
      cc_sleep(this->workerThreadSleepTime.getValue());
    else
      done = TRUE;
  }
#endif

  alSourceStop(this->sourceId);
  if ((error = alGetError()) != AL_NO_ERROR) {
    SoDebugError::postWarning("SoVRMLSound::stopPlaying",
                              "alSourceStop failed. %s",
                              coin_get_openal_error(error));
    retval= FALSE;
  }

  #ifdef _WIN32
    alGetSourcei(this->sourceId, AL_BUFFERS_PROCESSED, &processed);
  #else
    alGetSourceiv(this->sourceId, AL_BUFFERS_PROCESSED, &processed);
  #endif

  // alSourceUnqueueBuffers(this->sourceId, this->numBuffers, this->alBuffers);
  alSourceUnqueueBuffers(this->sourceId, processed, this->alBuffers);
  if ((error = alGetError()) != AL_NO_ERROR) {
    SoDebugError::postWarning("SoVRMLSoundP::stopPlaying",
                              "alSourceUnqueueBuffers failed. %s",
                              coin_get_openal_error(error));
    retval = FALSE;
  }

  this->deleteAlBuffers();

  this->playing = FALSE;

  #if COIN_DEBUG && DEBUG_AUDIO // debug
    printf(".]\n");
  #endif // debug

  return retval;
#else
  return FALSE;
#endif // HAVE_SOUND
}

SbBool SoVRMLSoundP::startPlaying()
{
#ifdef HAVE_SOUND
  #if COIN_DEBUG && DEBUG_AUDIO // debug
    printf("sound:start[.");
  #endif // debug

  if (this->playing)
    return TRUE;

  #if COIN_DEBUG && DEBUG_AUDIO // debug
    printf(".");
  #endif // debug

  ALint error;

  // Create new buffers

  this->alBuffers = new ALuint[this->numBuffers];
  alGenBuffers(this->numBuffers, this->alBuffers);
  if ((error = alGetError()) != AL_NO_ERROR) {
    SoDebugError::postWarning("SoVRMLSoundP::startPlaying",
                              "alGenBuffers failed. %s",
                              coin_get_openal_error(error));
    return FALSE;
  }

  // Fill buffer with data

  ALenum  alformat = 0;;
  alformat = getALSampleFormat(this->channels, 16);

  int bufferno;
  void *ret;
  for (bufferno = 0; bufferno < this->numBuffers; bufferno++) {
    // FIXME: must fill buffer without risking deadlocks. Investigate. 20021007 thammer.
    int frameoffset, channels;
    frameoffset=0;
    ret = this->currentAudioClip->fillBuffer(frameoffset, this->audioBuffer, 
      this->bufferLength, channels);

  #if COIN_DEBUG && DEBUG_AUDIO // debug
    printf(".");
  #endif // debug

    if (ret == NULL) {
      break; // eof before done filling buffers
    }

    if ( (channels==1) && (this->channels==2) )
      mono2stereo(this->audioBuffer, this->bufferLength);
    else if ( (channels==2) && (this->channels==1) )
      stereo2mono(this->audioBuffer, this->bufferLength);
    
    alBufferData(this->alBuffers[bufferno], alformat, this->audioBuffer, 
      this->bufferLength * sizeof(int16_t) * this->channels, 
      this->currentAudioClip->getSampleRate());
    if ((error = alGetError()) != AL_NO_ERROR) {
      SoDebugError::postWarning("SoVRMLSoundP::startPlaying",
                                "alBufferData failed. %s",
                                coin_get_openal_error(error));
      return FALSE;
    }
  }


  // Queue the buffers on the source
  
  //alSourceQueueBuffers(this->sourceId, this->numBuffers, this->alBuffers);
  alSourceQueueBuffers(this->sourceId, bufferno, this->alBuffers);
  if ((error = alGetError()) != AL_NO_ERROR) {
    SoDebugError::postWarning("SoVRMLSoundP::startPlaying",
                              "alSourceQueueBuffers failed. %s",
                              coin_get_openal_error(error));
    return FALSE;
  }

  alSourcei(this->sourceId,AL_LOOPING, FALSE);
  if ((error = alGetError()) != AL_NO_ERROR) {
    SoDebugError::postWarning("SoVRMLSoundP::startPlaying",
                              "alSourcei(,AL_LOOPING,) failed. %s",
                              coin_get_openal_error(error));
    return FALSE;
  }

  // Start timer or thread
  if (this->useTimerCallback) {
    // stop previous timer
    if (this->timersensor) {
      if (this->timersensor->isScheduled()) 
        this->timersensor->unschedule();
      delete this->timersensor;
      this->timersensor = NULL;
    }
    // start new timer
    this->timersensor = new SoTimerSensor(timercb, this);
    this->timersensor->setInterval(this->sleepTime);
    this->timersensor->schedule();
  }
  else {
    // stop existing thread, start new thread
    if (this->workerThread!=NULL) {
      this->exitthread = TRUE;
      void *retval = NULL;
      cc_thread_join(this->workerThread, &retval);
      cc_thread_destruct(this->workerThread);
      this->workerThread = NULL;
    }

    this->workerThreadSleepTime = this->sleepTime;
    this->errorInThread = FALSE;
    this->exitthread = FALSE;
    this->workerThread = cc_thread_construct(this->threadCallbackWrapper, this);
  }

  alSourcePlay(this->sourceId);
  if ((error = alGetError()) != AL_NO_ERROR) {
    SoDebugError::postWarning("SoVRMLSoundP::StartPlaying",
                              "alSourcePlay(sid=%d) failed: %s",
                              this->sourceId,
                              coin_get_openal_error(error));
    return FALSE;
  }

  this->playing = TRUE;

  #if COIN_DEBUG && DEBUG_AUDIO // debug
    printf(".]\n");
  #endif // debug

  return TRUE;
#else
  return FALSE;
#endif // HAVE_SOUND
}

void SoVRMLSoundP::fillBuffers()
{
#ifdef HAVE_SOUND
  SbMutexAutoLock autoLock(SoVRMLSoundP::syncmutex);

  ALint      processed;

  // Get status
#ifdef _WIN32
  alGetSourcei(this->sourceId, AL_BUFFERS_PROCESSED, &processed);
#else
  alGetSourceiv(this->sourceId, AL_BUFFERS_PROCESSED, &processed);
#endif

  ALint      queued;
#ifdef _WIN32
  alGetSourcei(this->sourceId, AL_BUFFERS_QUEUED, &queued);
#else
  alGetSourceiv(this->sourceId, AL_BUFFERS_QUEUED, &queued);
#endif

#if COIN_DEBUG && DEBUG_AUDIO // debug
  printf("Processed: %d, Queued: %d\n", processed, queued);
#endif // debug

  ALuint BufferID;
  ALint error;
  void *ret;

/*
  if (queued<=0) {
    // no buffers were queued, so there's nothing to do
    // this should only happen after audioclip::fillBuffer() returns NULL to indicate an EOF,
    // and sound::fillBuffers() does not queue new buffers after that
    #if COIN_DEBUG && 1 // debug
      printf("No more buffers queued (we're probably stopping soon)\n");
    #endif // debug
    return; 
  }
*/

  while (processed>0) {

    // FIXME: perhaps we should reread processed in the while loop too. 
    // This might make buffer underruns less frequent. 20021007 thammer

    // unqueue one buffer

    alSourceUnqueueBuffers(this->sourceId, 1, &BufferID);
    if ((error = alGetError()) != AL_NO_ERROR) {
      SoDebugError::post("SoVRMLSound::fillBuffers",
                         "alSourceUnqueueBuffers failed. %s",
                         coin_get_openal_error(error));
      this->errorInThread = TRUE;
      return;
    }

    // fill buffer

    int frameoffset, channels;
    frameoffset=0;
    ret = this->currentAudioClip->fillBuffer(frameoffset, this->audioBuffer, 
      this->bufferLength, channels);

/*    if (ret == NULL)
      return; // AudioClip has reached EOF (or an error), so we shouldn't fill any more buffers
*/
    if ( (channels==1) && (this->channels==2) )
      mono2stereo(this->audioBuffer, this->bufferLength);
    else if ( (channels==2) && (this->channels==1) )
      stereo2mono(this->audioBuffer, this->bufferLength);

    // send buffer to OpenAL

    ALenum  alformat = 0;;
    alformat = getALSampleFormat(this->channels, 16);

    alBufferData(BufferID, alformat, this->audioBuffer, 
                 this->bufferLength * sizeof(int16_t) * this->channels, 
                 this->currentAudioClip->getSampleRate());

    if ((error = alGetError()) != AL_NO_ERROR) {
      SoDebugError::post("SoVRMLSound::fillBuffers",
                         "alBufferData(buffer=%d, format=%d, data=%p, "
                         "size=%d, freq=%d) failed: %s",
                         BufferID, alformat, this->audioBuffer, 
                         this->bufferLength * sizeof(int16_t) * this->channels,
                         this->currentAudioClip->getSampleRate(),
                         coin_get_openal_error(error));
      this->errorInThread = TRUE;
      return;
    }

    // Queue buffer
    alSourceQueueBuffers(this->sourceId, 1, &BufferID);
    if ((error = alGetError()) != AL_NO_ERROR) {
      SoDebugError::post("SoVRMLSound::fillBuffers",
                         "alSourceQueueBuffers failed. %s",
                         coin_get_openal_error(error));
      this->errorInThread = TRUE;
      return;
    }

    processed--;
  }

  // Check to see if we're still playing
  // if not, make sure to start over again.
  // If we're not playing, it's because the buffers have not been filled up 
  // (unqueued, filled, queued) fast enough, so the source has played the last 
  // buffer in the queue and changed state from playing to stopped.

  ALint      state;
#ifdef _WIN32
  alGetSourcei(this->sourceId, AL_SOURCE_STATE, &state);
#else
  alGetSourceiv(this->sourceId, AL_SOURCE_STATE, &state);
#endif
  if (state != AL_PLAYING) {
    if (state == AL_STOPPED) {
      alSourcePlay(this->sourceId);
      if ((error = alGetError()) != AL_NO_ERROR) {
        SoDebugError::post("SoVRMLSoundP::fillBuffers",
                           "alSourcePlay(sid=%d) failed: %s",
                           this->sourceId,
                           coin_get_openal_error(error));
        this->errorInThread = TRUE;
        return;
      }
      SoDebugError::postWarning("SoVRMLSoundP::fillBuffers",
                                "Buffer underrun. The audio source had to be restarted. "
                                "Try increasing buffer size (current: %d frames), "
                                "and/or increasing number of buffers (current: %d buffers), "
                                "and/or decreasing sleeptime (current: %0.3fs)", 
                                this->bufferLength, this->numBuffers, this->sleepTime.getValue());
    }
    else {
      char statestr[20];
      switch (state) {
      case AL_INITIAL : sprintf(statestr, "initial"); break;
      case AL_PLAYING : sprintf(statestr, "playing"); break;
      case AL_PAUSED : sprintf(statestr, "paused"); break;
      case AL_STOPPED : sprintf(statestr, "stopped"); break;
      default : sprintf(statestr, "unknown"); break;
      };
      // 20021007 thammer fixme: deal with this properly!
#if COIN_DEBUG && DEBUG_AUDIO // debug
      fprintf(stderr, "state == %s. Don't know what to do about it...\n", statestr);
#endif
    }
  }
#endif // HAVE_SOUND
}

//
// called when source changes
//
void
SoVRMLSoundP::sourceSensorCBWrapper(void * data, SoSensor *)
{
  SoVRMLSoundP * thisp = (SoVRMLSoundP*) data;
  thisp->sourceSensorCB(NULL);
}

//
// called when source changes
//
void
SoVRMLSoundP::sourceSensorCB(SoSensor *)
{
  #if COIN_DEBUG && DEBUG_AUDIO // debug
    printf("(S)");
  #endif // debug

  SbMutexAutoLock autoLock(SoVRMLSoundP::syncmutex);

  SoNode *node = (SoNode *)ITHIS->source.getValue();

  if (!node->isOfType(SoVRMLAudioClip::getClassTypeId())) {
    SoDebugError::postWarning("SoVRMLSoundP::sourceSensorCB",
                              "Unknown source node type");
    if (this->currentAudioClip != NULL) {
      this->currentAudioClip->unref();
      this->stopPlaying();
    }
    this->currentAudioClip = NULL;
    return;
  }

  SoVRMLAudioClip *audioClip = (SoVRMLAudioClip *)node;
  if (audioClip != this->currentAudioClip) {
    if (this->currentAudioClip != NULL) {
      this->currentAudioClip->unref();
      this->currentAudioClip = NULL;
      this->stopPlaying();
    }
    if (audioClip!=NULL)
      audioClip->ref();
    this->currentAudioClip = audioClip;
  }

  if (this->currentAudioClip == NULL)
    return;

#if 0
  // FIXME: support pitch. To do this, AC must be modified so that the calculation
  // of duration (see AC::audioRender, close to ac.endOfFile) must be improved.
  // 20021008 thammer
  ALint error;
  alSourcef(this->sourceId, AL_PITCH, this->currentAudioClip->pitch.getValue());
  if ((error = alGetError()) != AL_NO_ERROR)
  {
    SoDebugError::postWarning("SoVRMLSoundP::sourceSensorCB",
                              "alSourcef(,AL_PITCH,) failed. %s",
                              coin_get_openal_error(error));
    return;
  }
#endif // pitch support

  // 20021008 thammer: added this block. perhaps we should also start playing here?
  SoSFBool * isActiveField = (SoSFBool *)this->currentAudioClip->getField("isActive");
  SbBool isactive = isActiveField->getValue();
  if ( this->playing && (!isactive) ) {
    this->stopPlaying();
  }

}
