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
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <stddef.h>

SO_NODE_SOURCE(SoVRMLSound);

// Doc in parent
void
SoVRMLSound::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLSound, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLSound::SoVRMLSound(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLSound);

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
}

/*!
  Destructor.
*/
SoVRMLSound::~SoVRMLSound(void)
{
}


// Doc in parent
void
SoVRMLSound::GLRender(SoGLRenderAction * action)
{
}
