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
#include <Inventor/misc/SoAudioDevice.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/SbTime.h>
#include <stddef.h>

#include "../misc/AudioTools.h"

#if HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_THREADS
#include <Inventor/threads/SbMutex.h>
#include <Inventor/threads/SbThreadAutoLock.h>
#include <Inventor/C/threads/thread.h>
#endif

#ifdef HAVE_SOUND
#include <AL/al.h>
#include <AL/altypes.h>
#endif

#define DEBUG_AUDIO 0

class SoVRMLSoundP
{
public:
  SoVRMLSoundP(SoVRMLSound * master) : master(master) {};
  SoVRMLSound *master;

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
  SbList<ALuint> alBuffers;
#endif // HAVE_SOUND
  SoVRMLAudioClip *currentAudioClip;
  SbBool playing;
  SbBool useTimerCallback;
  SbBool endoffile;
  SbBool waitingForAudioClipToFinish;

  SoTimerSensor * timersensor;
#ifdef HAVE_THREADS
  cc_thread *workerThread;
  static SbMutex *syncmutex;
#endif
  volatile SbBool exitthread;
  volatile SbBool errorInThread;


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

#undef PRIVATE
#define PRIVATE(p) ((p)->pimpl)
#undef PUBLIC
#define PUBLIC(p) ((p)->master)

// fixme 20021006 thammer: should really do individual synchronization instead of global
#ifdef HAVE_THREADS
SbMutex *SoVRMLSoundP::syncmutex = NULL;
#endif
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
#ifdef HAVE_THREADS
  SoVRMLSoundP::syncmutex = new SbMutex;
#endif
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

  PRIVATE(this) = new SoVRMLSoundP(this);

  PRIVATE(this)->channels = 1; // because spatialize defaults to TRUE and OpenAL only spatializes mono buffers

  PRIVATE(this)->currentAudioClip = NULL;
  PRIVATE(this)->playing = FALSE;
  PRIVATE(this)->endoffile = FALSE;
  PRIVATE(this)->waitingForAudioClipToFinish = FALSE;

  PRIVATE(this)->timersensor = NULL;
#ifdef HAVE_THREADS
  PRIVATE(this)->useTimerCallback = FALSE;
#else
  PRIVATE(this)->useTimerCallback = TRUE;
#endif // HAVE_THREADS

  PRIVATE(this)->sourcesensor = new SoFieldSensor(PRIVATE(this)->sourceSensorCBWrapper, PRIVATE(this));
  PRIVATE(this)->sourcesensor->setPriority(0);
  PRIVATE(this)->sourcesensor->attach(&this->source);

#ifdef HAVE_THREADS
  PRIVATE(this)->workerThread = NULL;
#endif
  PRIVATE(this)->exitthread = FALSE;
  PRIVATE(this)->errorInThread = FALSE;
  PRIVATE(this)->audioBuffer = NULL;

  this->setBufferingProperties(SoVRMLSoundP::defaultBufferLength, SoVRMLSoundP::defaultNumBuffers,
                               SoVRMLSoundP::defaultSleepTime);

#ifdef HAVE_SOUND
  if (SoAudioDevice::instance()->haveSound()) {
    ALint  error;
    alGenSources(1, &(PRIVATE(this)->sourceId));
    if ((error = alGetError()) != AL_NO_ERROR) {
      SoDebugError::post("SoVRMLSound::SoVRMLSound",
                         "alGenSources failed. %s",
                         coin_get_openal_error(error));
      return;
    }
  }
#endif
}

/*!
  Destructor.
*/
SoVRMLSound::~SoVRMLSound(void)
{
  delete PRIVATE(this)->sourcesensor;

  if (PRIVATE(this)->currentAudioClip != NULL)
    PRIVATE(this)->currentAudioClip->unref();
  PRIVATE(this)->currentAudioClip = NULL;

  PRIVATE(this)->stopPlaying();

  if (PRIVATE(this)->audioBuffer != NULL)
    delete[] PRIVATE(this)->audioBuffer;

#ifdef HAVE_SOUND
  if (SoAudioDevice::instance()->haveSound()) {
    ALint  error;
    alDeleteSources(1, &(PRIVATE(this)->sourceId));
    if ((error = alGetError()) != AL_NO_ERROR) {
      SoDebugError::postWarning("SoVRMLSound::~SoVRMLSound",
                                "alDeleteSources() failed. %s",
                                coin_get_openal_error(error));
    }
  }
  PRIVATE(this)->deleteAlBuffers();
#endif
  delete PRIVATE(this);
}

void SoVRMLSound::setDefaultBufferingProperties(int bufferLength, int numBuffers, SbTime sleepTime)
{
  SoVRMLSoundP::defaultBufferLength = bufferLength;
  SoVRMLSoundP::defaultNumBuffers = numBuffers;
  SoVRMLSoundP::defaultSleepTime = sleepTime;
}

void SoVRMLSound::setBufferingProperties(int bufferLength, int numBuffers, SbTime sleepTime)
{
#ifdef HAVE_THREADS
  SbThreadAutoLock autoLock(SoVRMLSoundP::syncmutex);
#endif 
  PRIVATE(this)->numBuffers = numBuffers;
  PRIVATE(this)->sleepTime = sleepTime;

  if (PRIVATE(this)->bufferLength == bufferLength) 
    return;

  PRIVATE(this)->bufferLength = bufferLength;
  delete[] PRIVATE(this)->audioBuffer;
  PRIVATE(this)->audioBuffer = new int16_t[PRIVATE(this)->bufferLength * 2];
}

void SoVRMLSound::getBufferingProperties(int &bufferLength, int &numBuffers, SbTime &sleepTime)
{
#ifdef HAVE_THREADS
  SbThreadAutoLock autoLock(SoVRMLSoundP::syncmutex);
#endif
  bufferLength = PRIVATE(this)->bufferLength;
  numBuffers = PRIVATE(this)->numBuffers;
  sleepTime = PRIVATE(this)->sleepTime;
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
#ifdef HAVE_THREADS
  SbThreadAutoLock autoLock(SoVRMLSoundP::syncmutex);
#endif
  if (!SoAudioDevice::instance()->haveSound())
    return;

  if (PRIVATE(this)->currentAudioClip == NULL)
    return;

  SoSFBool * isActiveField = (SoSFBool *)PRIVATE(this)->currentAudioClip->getField("isActive");
  SbBool isactive = isActiveField->getValue();

  if ( (!PRIVATE(this)->playing) && (!isactive) )
    return;

  if (PRIVATE(this)->errorInThread) {
#ifdef HAVE_THREADS
    SoVRMLSoundP::syncmutex->unlock();
#endif
    PRIVATE(this)->stopPlaying();
#ifdef HAVE_THREADS
    SoVRMLSoundP::syncmutex->lock();
#endif
    return;
  }

  if ( PRIVATE(this)->playing && (!isactive) ) {
    if (coin_debug_audio())
      SoDebugError::postInfo("SoVRMLSound::audioRender",
                             "PRIVATE(this)->playing && (!isactive). Calling stopPlaying()");
#ifdef HAVE_THREADS
      SoVRMLSoundP::syncmutex->unlock();
#endif
      PRIVATE(this)->stopPlaying();
#ifdef HAVE_THREADS
      SoVRMLSoundP::syncmutex->lock();
#endif
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
  fprintf(stderr, "(%0.2f, %0.2f, %0.2f)-----------------------\n", x, y, z);
#endif // debug

  SbVec3f2ALfloat3(alfloat3, worldpos);

  // Position ...
  alSourcefv(PRIVATE(this)->sourceId, AL_POSITION, alfloat3);
  if ((error = alGetError()) != AL_NO_ERROR) {
    SoDebugError::postWarning("SoVRMLSound::audioRender",
                              "alSourcefv(,AL_POSITION,) failed. %s",
                              coin_get_openal_error(error));
    return;
  }

#if 0
  // 20021007 thammer note: if we ever want to implement velocity (supported by OpenAL)
  // then this is how it should be done
  // get alfloat3 from PRIVATE(this)->velocity
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
  float gain = this->intensity.getValue();
  gain = (gain > 0.0f) ? ((gain < 1.0f) ? gain : 1.0f) : 0.0f;
  alSourcef(PRIVATE(this)->sourceId,AL_GAIN, gain);
  if ((error = alGetError()) != AL_NO_ERROR) {
    SoDebugError::postWarning("SoVRMLSound::audioRender",
                              "alSourcef(,AL_GAIN,) failed. %s",
                              coin_get_openal_error(error));
    return;
  }

  /*
    FIXME: for non-spatialized sources, the source's position should
    be set to AL_RELATIVE, and position to 0. 2002-10-31 thammer.
   */

  // Distance attenuation
  /* FIXME: If minFront = maxFront = 0.0f, we interpret this as no distance attenuation.
     This is a violation of the VRML97 spec, but we keep it like this because it is
     so very very useful. It is very strange that the VRML97 spec doesn't specify a 
     way to bypass distance attenuation. 
     If the user was able to specify some kind of MAX_FLOAT value as a parameter, we
     wouldn't have to make this a special case, and we wouldn't have to violate the
     VRML97 spec. Investigate this further. 2002-11-07 thammer.
   */

  /*
    FIXME: move these to a sensor instead. 2002-11-07 thammer.
   */
  if ((this->maxBack.getValue() == 0.0) && (this->maxFront.getValue() == 0.0)) {
    /* Note: On some systems, it might not be possible to disable distance attenuation.
       This has been experienced by thammer on WindowsXP using Creaitve Labs Extigy,
       driver version 5.12.01.0038.
       On the same system, using another soundcard (DellInspiron 8200's built-in 
       soundcard), distance attenuation was disabled, as it should be.
       This difference is probably due to poor DirectSound3D drivers for the Extigy.
       2002-11-07 thammer.
      
     */
    alSourcef(PRIVATE(this)->sourceId, AL_ROLLOFF_FACTOR, 0.0f); // no distance attenuation
    if ((error = alGetError()) != AL_NO_ERROR) {
      SoDebugError::postWarning("SoVRMLSound::audioRender",
                                "alSourcef(,AL_ROLLOF_FACTOR,) failed. %s",
                                coin_get_openal_error(error));
      return;
    }
    alSourcef(PRIVATE(this)->sourceId, AL_MIN_GAIN, gain); // no distance attenuation
    if ((error = alGetError()) != AL_NO_ERROR) {
      SoDebugError::postWarning("SoVRMLSound::audioRender",
                                "alSourcef(,AL_ROLLOF_FACTOR,) failed. %s",
                                coin_get_openal_error(error));
      return;
    }
  } else {
    alSourcef(PRIVATE(this)->sourceId, AL_ROLLOFF_FACTOR, 1.0f); // default
    if ((error = alGetError()) != AL_NO_ERROR) {
      SoDebugError::postWarning("SoVRMLSound::audioRender",
                                "alSourcef(,AL_ROLLOF_FACTOR,) failed. %s",
                                coin_get_openal_error(error));
      return;
    }
    alSourcef(PRIVATE(this)->sourceId, AL_MIN_GAIN, 0.0f); // default
    if ((error = alGetError()) != AL_NO_ERROR) {
      SoDebugError::postWarning("SoVRMLSound::audioRender",
                                "alSourcef(,AL_ROLLOF_FACTOR,) failed. %s",
                                coin_get_openal_error(error));
      return;
    }
  }

  // Spatialization

  int newchannels = this->spatialize.getValue() ? 1 : 2;
  if (PRIVATE(this)->channels != newchannels) {
    if (PRIVATE(this)->playing) {
#ifdef HAVE_THREADS
      SoVRMLSoundP::syncmutex->unlock();
#endif
      PRIVATE(this)->stopPlaying();
#ifdef HAVE_THREADS
      SoVRMLSoundP::syncmutex->lock();
#endif
    }

    PRIVATE(this)->channels = newchannels;
  }

  if ( (!PRIVATE(this)->playing) && isactive )  {
    if (coin_debug_audio())
      SoDebugError::postInfo("SoVRMLSound::audioRender",
                             "(!PRIVATE(this)->playing) && isactive. Calling startPlaying()");
    PRIVATE(this)->startPlaying();
  }
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
  if (SoAudioDevice::instance()->haveSound()) {
    while (this->alBuffers.getLength() > 0) 
      {
        ALuint bufferid = this->alBuffers.pop();
        alDeleteBuffers(1, &bufferid);
      }
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
  /* FIXME: An application using Coin might crash when shutdown because
     a SoAudioClip node might have been deleted (even though it is ref'ed).
     Using this->currentAudioClip in the fillThread is then undefined...
     20021104 thammer

  */
  while (!this->exitthread) {
    this->fillBuffers();
#ifdef HAVE_THREADS
    cc_sleep(this->workerThreadSleepTime.getValue());
#endif
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
    fprintf(stderr, "sound:stop[.");
  #endif // debug

  if (!SoAudioDevice::instance()->haveSound())
    return FALSE;

  if (!this->playing)
    return TRUE;

  #if COIN_DEBUG && DEBUG_AUDIO // debug
    fprintf(stderr, ".");
  #endif // debug

  ALint error;

  // stop timersensor
  if (this->timersensor) {
    if (this->timersensor->isScheduled()) 
      this->timersensor->unschedule();
    delete this->timersensor;
    this->timersensor = NULL;
  }

  /* FIXME: joining with workerThread will normally cause a lag of
     sleepTime. This should be fixed in some way. 20021107 thammer.
  */

  // stop thread
#ifdef HAVE_THREADS
  if (this->workerThread!=NULL) {
    this->exitthread = TRUE;
    void *retval = NULL;
    cc_thread_join(this->workerThread, &retval);
    cc_thread_destruct(this->workerThread);
    this->workerThread = NULL;
  }
#endif // HAVE_THREADS

  #if COIN_DEBUG && DEBUG_AUDIO // debug
    fprintf(stderr, ".");
  #endif // debug

  this->errorInThread = FALSE;

  SbBool retval = TRUE;


  alSourceStop(this->sourceId);
  if ((error = alGetError()) != AL_NO_ERROR) {
    SoDebugError::postWarning("SoVRMLSound::stopPlaying",
                              "alSourceStop failed. %s",
                              coin_get_openal_error(error));
    retval= FALSE;
  }

  /* Note: Rewinding will make sure state is AL_INITIAL, not just AL_STOPPED.
     This lets us give the user a warning if the source stopped playing
     because a buffer underrun occured. See fillBuffers().
     2002-11-07 thammer.
  */

  alSourceRewind(this->sourceId);
  if ((error = alGetError()) != AL_NO_ERROR) {
    SoDebugError::postWarning("SoVRMLSound::stopPlaying",
                              "alSourceRewind failed. %s",
                              coin_get_openal_error(error));
    retval= FALSE;
  }

  /* FIXME: improve use of alGetSourcei[v] to avoid the #ifdef _WIN32. Look at how it is done in SoAudioRender.
     20021106 thammer.
   */
  ALint      processed;
  ALint      queued;

#ifdef _WIN32
  alGetSourcei(this->sourceId, AL_BUFFERS_QUEUED, &queued);
#else
  alGetSourceiv(this->sourceId, AL_BUFFERS_QUEUED, &queued);
#endif
#ifdef _WIN32
  alGetSourcei(this->sourceId, AL_BUFFERS_PROCESSED, &processed);
#else
  alGetSourceiv(this->sourceId, AL_BUFFERS_PROCESSED, &processed);
#endif

  /* Note: if the sound was stopped after the thread reported to the audioclip that all
     buffer were played, queued and processed should both be 0. If the sound was stopped
     for any other reason (for instance because the url or startTime/stopTime of the
     audioclip changed, processed and/or queued could be != 0.
     20021106 thammer
    
  */
  if (coin_debug_audio())
    SoDebugError::postInfo("SoVRMLSound::stopPlaying",
                           "Queued: %d, Processed: %d.",
                           queued, processed);

  ALuint *removedBuffers = new ALuint[processed];
  alSourceUnqueueBuffers(this->sourceId, processed, removedBuffers);
  delete[] removedBuffers;
  if ((error = alGetError()) != AL_NO_ERROR) {
    SoDebugError::postWarning("SoVRMLSoundP::stopPlaying",
                              "alSourceUnqueueBuffers failed. %s",
                              coin_get_openal_error(error));
    retval = FALSE;
  }

  this->deleteAlBuffers();

  this->playing = FALSE;

  #if COIN_DEBUG && DEBUG_AUDIO
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
  #if COIN_DEBUG && DEBUG_AUDIO
    printf("sound:start[.");
  #endif // debug

  if (!SoAudioDevice::instance()->haveSound())
    return FALSE;

  if (this->playing)
    return TRUE;

  #if COIN_DEBUG && DEBUG_AUDIO
    printf(".");
  #endif // debug

  ALint error;

  assert(this->alBuffers.getLength() == 0);

  alSourcei(this->sourceId,AL_LOOPING, FALSE);
  if ((error = alGetError()) != AL_NO_ERROR) {
    SoDebugError::postWarning("SoVRMLSoundP::startPlaying",
                              "alSourcei(,AL_LOOPING,) failed. %s",
                              coin_get_openal_error(error));
    return FALSE;
  }

  this->playing = TRUE;
  this->endoffile = FALSE;
  this->waitingForAudioClipToFinish = FALSE;

  // Start timer or thread
  if (this->useTimerCallback) {
    // stop previous timer
    if (this->timersensor) {
      if (this->timersensor->isScheduled()) 
        this->timersensor->unschedule();
      delete this->timersensor;
      this->timersensor = NULL;
    }
    this->errorInThread = FALSE;
    // start new timer
    this->timersensor = new SoTimerSensor(timercb, this);
    this->timersensor->setInterval(this->sleepTime);
    this->timersensor->schedule();
  }
  else {
    // stop existing thread, start new thread
#ifdef HAVE_THREADS
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
#endif // HAVE_THREADS
  }

  #if COIN_DEBUG && DEBUG_AUDIO
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
#ifdef HAVE_THREADS
  SbThreadAutoLock autoLock(SoVRMLSoundP::syncmutex);
#endif

  if (this->waitingForAudioClipToFinish) {
    if (coin_debug_audio())
      SoDebugError::postInfo("SoVRMLSound::fillBuffers",
                             "this->waitingForAudioClipToFinish == TRUE, returning.");
    return;
  }

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
  fprintf(stderr, "Processed: %d, Queued: %d\n", processed, queued);
#endif // debug

  ALuint bufferid = 0;
  ALint error;
  void *ret;

#if 0
  // 20021021 thammer, kept for debugging purposes
  if (queued<=0) {
    // no buffers were queued, so there's nothing to do
    // this should only happen after audioclip::fillBuffer() returns NULL to indicate an EOF,
    // and sound::fillBuffers() does not queue new buffers after that
    #if COIN_DEBUG && 1 // debug
      fprintf(stderr, "No more buffers queued (we're probably stopping soon)\n");
    #endif // debug
    return; 
  }
#endif

  if (this->endoffile) {
#if COIN_DEBUG && DEBUG_AUDIO
    fprintf(stderr, "sound, eof\n");
#endif
    if (processed > 0) {
#if COIN_DEBUG && DEBUG_AUDIO
      fprintf(stderr, "sound, processed = %d\n", processed);
#endif
      assert(queued > 0);
      ALuint *removedBuffers = new ALuint[processed];
      alSourceUnqueueBuffers(this->sourceId, processed, removedBuffers);
      delete[] removedBuffers;
    } else if (queued == 0) {
#if COIN_DEBUG && DEBUG_AUDIO
      fprintf(stderr, "sound, queued = %d\n", queued);
#endif
      this->waitingForAudioClipToFinish = TRUE;
      // inform currentAudioClip() that the last buffer has been played,
      // so it can decide if it would like to stop playing
      int channels;
      ret = this->currentAudioClip->fillBuffer(0, NULL, 0, channels);
      assert (ret == NULL); // or else the AudioClip isn't performing as it should
    }
  } else {
    while (((processed > 0) || (queued<this->numBuffers)) && !this->endoffile)  {
      // FIXME: perhaps we should reread processed in the while loop too. 
      // This might make buffer underruns less frequent. 20021007 thammer

      if (queued < this->numBuffers) {
        alGenBuffers(1, &bufferid);
        if ((error = alGetError()) != AL_NO_ERROR) {
          SoDebugError::post("SoVRMLSoundP::fillBuffers",
                             "alGenBuffers failed. %s",
                             coin_get_openal_error(error));
          this->errorInThread = TRUE;
          return;
        }
        this->alBuffers.push(bufferid);
      } else {
      // unqueue one buffer
        alSourceUnqueueBuffers(this->sourceId, 1, &bufferid);
        if ((error = alGetError()) != AL_NO_ERROR) {
          SoDebugError::post("SoVRMLSound::fillBuffers",
                             "alSourceUnqueueBuffers failed. "
                             "Queued: %d, Processed: %d."
                             "OpenAL error: %s.",
                             queued, processed,
                             coin_get_openal_error(error));
          this->errorInThread = TRUE;
          return;
        }
      }
      // fill buffer

      int frameoffset, channels;
      frameoffset=0;
      ret = this->currentAudioClip->fillBuffer(frameoffset, this->audioBuffer, 
                                               this->bufferLength, channels);

      if ( (channels==1) && (this->channels==2) )
        mono2stereo(this->audioBuffer, this->bufferLength);
      else if ( (channels==2) && (this->channels==1) )
        stereo2mono(this->audioBuffer, this->bufferLength);

      // copy buffer data to the newly generated or unqueued openal buffer

      ALenum  alformat = 0;;
      alformat = getALSampleFormat(this->channels, 16);

      alBufferData(bufferid, alformat, this->audioBuffer, 
                   this->bufferLength * sizeof(int16_t) * this->channels, 
                   this->currentAudioClip->getSampleRate());

      if ((error = alGetError()) != AL_NO_ERROR) {
        SoDebugError::post("SoVRMLSound::fillBuffers",
                           "alBufferData(buffer=%d, format=%d, data=%p, "
                           "size=%d, freq=%d) failed. "
                           "Queued: %d, Processed: %d. "
                           "OpenAL error: %s",
                           bufferid, alformat, this->audioBuffer, 
                           this->bufferLength * sizeof(int16_t) * this->channels,
                           this->currentAudioClip->getSampleRate(),
                           queued, processed,
                           coin_get_openal_error(error));
        this->errorInThread = TRUE;
        return;
      }

      // Queue buffer
      alSourceQueueBuffers(this->sourceId, 1, &bufferid);
      if ((error = alGetError()) != AL_NO_ERROR) {
        SoDebugError::post("SoVRMLSound::fillBuffers",
                           "alSourceQueueBuffers failed. "
                           "Queued: %d, Processed: %d."
                           "OpenAL error: %s.",
                           queued, processed,
                           coin_get_openal_error(error));
        this->errorInThread = TRUE;
        return;
      }

      if (ret == NULL) {
        this->endoffile = true; // AudioClip has reached EOF (or an error), so we shouldn't fill any more buffers
#if COIN_DEBUG && DEBUG_AUDIO
        fprintf(stderr, "sound, ret == true\n");
#endif
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
        if ( (state == AL_STOPPED) || (state == AL_INITIAL) ) {
          alSourcePlay(this->sourceId);
          if ((error = alGetError()) != AL_NO_ERROR) {
            SoDebugError::post("SoVRMLSoundP::fillBuffers",
                               "alSourcePlay(sid=%d) failed: %s",
                               this->sourceId,
                               coin_get_openal_error(error));
            this->errorInThread = TRUE;
            return;
          }
          if (state == AL_STOPPED)
            SoDebugError::postWarning("SoVRMLSoundP::fillBuffers",
                                      "Buffer underrun. The audio source had to be restarted. "
                                      "Queued: %d, Processed: %d. "
                                      "Try increasing buffer size (current: %d frames), "
                                      "and/or increasing number of buffers (current: %d buffers), "
                                      "and/or decreasing sleeptime (current: %0.3fs)", 
                                      queued, processed,
                                      this->bufferLength, this->numBuffers, this->sleepTime.getValue());
          else if (coin_debug_audio())
            SoDebugError::postInfo("SoVRMLSoundP::fillBuffers",
                                   "Source had not been started (state==AL_INITIAL). "
                                   "The audio source has been started. "
                                   "Queued: %d, Processed: %d. "
                                   "Current buffer size: %d frames, "
                                   "current number of buffers: %d buffers, "
                                   "current sleeptime: %0.3fs", 
                                   queued, processed,
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
      if (queued < this->numBuffers)
        queued++;
      else
        processed--;
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
    fprintf(stderr, "(S)");
  #endif // debug

#ifdef HAVE_THREADS
  SbThreadAutoLock autoLock(SoVRMLSoundP::syncmutex);
#endif
  if (!SoAudioDevice::instance()->haveSound())
    return;

  SoNode *node = (SoNode *)PUBLIC(this)->source.getValue();

  if (!node->isOfType(SoVRMLAudioClip::getClassTypeId())) {
    SoDebugError::postWarning("SoVRMLSoundP::sourceSensorCB",
                              "Unknown source node type");
    if (this->currentAudioClip != NULL) {
      this->currentAudioClip->unref();
#ifdef HAVE_THREADS
      SoVRMLSoundP::syncmutex->unlock();
#endif
      this->stopPlaying();
#ifdef HAVE_THREADS
      SoVRMLSoundP::syncmutex->lock();
#endif
    }
    this->currentAudioClip = NULL;
    return;
  }

  SoVRMLAudioClip *audioClip = (SoVRMLAudioClip *)node;
  if (audioClip != this->currentAudioClip) {
    if (this->currentAudioClip != NULL) {
      this->currentAudioClip->unref();
      this->currentAudioClip = NULL;
      if (coin_debug_audio())
        SoDebugError::postInfo("SoVRMLSound::sourceSensorCB",
                               "audioClip != this->currentAudioClip. Calling stopPlaying()");
#ifdef HAVE_THREADS
      SoVRMLSoundP::syncmutex->unlock();
#endif
      this->stopPlaying();
#ifdef HAVE_THREADS
      SoVRMLSoundP::syncmutex->lock();
#endif
    }
    if (audioClip!=NULL)
      audioClip->ref();
    this->currentAudioClip = audioClip;
  }

  if (this->currentAudioClip == NULL)
    return;

  /* Note: According to the OpenAL 1.0 spec, the legal range for pitch is [0, 1].
     However, both the Win32 implementation and the linux implementation supports
     the range [0, 2]. The Mac implementation supports the range [0, infinite>.
     Testing shows that Creative Labs' binary-only OpenAL implementations also
     supports the range [0, 2]. Since it is very useful to be able to increase
     the pitch above unity, and since the VRML97 spec specifies the range to be
     [0, infinite>, we will allow the range to be within [0, 2], and clamp
     outside this range. 2002-11-07 thammer.

     Update: It turns out that CreativeLabs' Win32 binary release of OpenAL will 
     crash if pitch == 0.0. For that reason, we will clamp at 0.01. The range
     supported is thus [0.01..2.0]. 2002-11-07 thammer.
  */
  ALint error;
  float pitch = this->currentAudioClip->pitch.getValue();
  pitch = (pitch >= 0.01f) ? ( (pitch<=2.0f) ? pitch : 2.0f ) : 0.01f;
  alSourcef(this->sourceId, AL_PITCH, pitch);
  if ((error = alGetError()) != AL_NO_ERROR)
  {
    SoDebugError::postWarning("SoVRMLSoundP::sourceSensorCB",
                              "alSourcef(,AL_PITCH,) failed. %s",
                              coin_get_openal_error(error));
    return;
  }

  SoSFBool * isActiveField = (SoSFBool *)this->currentAudioClip->getField("isActive");
  SbBool isactive = isActiveField->getValue();
  if ( this->playing && (!isactive) ) {
    /* FIXME: #ifdef COIN_EXTRA_DEBUG all these debugging messages */
    if (coin_debug_audio())
      SoDebugError::postInfo("SoVRMLSound::sourceSensorCB",
                             "this->playing && (!isactive). Calling stopPlaying()");
#ifdef HAVE_THREADS
      SoVRMLSoundP::syncmutex->unlock();
#endif
      this->stopPlaying();
#ifdef HAVE_THREADS
      SoVRMLSoundP::syncmutex->lock();
#endif
  }
}
