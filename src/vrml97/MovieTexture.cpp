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
  \class SoVRMLMovieTexture SoVRMLMovieTexture.h Inventor/VRMLnodes/SoVRMLMovieTexture.h
  \brief The SoVRMLMovieTexture class is used the map movies onto geometry.
  \ingroup VRMLnodes
  
  \WEB3DCOPYRIGHT

  \verbatim
  MovieTexture {
    exposedField SFBool   loop             FALSE
    exposedField SFFloat  speed            1.0      # (-inf, inf)
    exposedField SFTime   startTime        0        # (-inf, inf)
    exposedField SFTime   stopTime         0        # (-inf, inf)
    exposedField MFString url              []
    field        SFBool   repeatS          TRUE
    field        SFBool   repeatT          TRUE
    eventOut     SFTime   duration_changed
    eventOut     SFBool   isActive
  }
  \endverbatim

  The MovieTexture node defines a time dependent texture map
  (contained in a movie file) and parameters for controlling the movie
  and the texture mapping. A MovieTexture node can also be used as the
  source of sound data for a Sound node. In this special case, the
  MovieTexture node is not used for rendering.  

  Texture maps are defined in a 2D coordinate system (s, t) that
  ranges from 0.0 to 1.0 in both directions. The bottom edge of the
  image corresponds to the S-axis of the texture map, and left edge of
  the image corresponds to the T-axis of the texture map. The
  lower-left pixel of the image corresponds to s=0.0, t=0.0, and the
  top-right pixel of the image corresponds to s=1.0, t=1.0. Figure
  6.12 depicts the texture map coordinate system of the MovieTexture.

  <center>
  <img src="http://www.web3d.org/technicalinfo/specifications/vrml97/Images/ImageTexture.gif">
  Figure 6.12 -- MovieTexture node coordinate system 
  </center>

  The url field that defines the movie data shall support
  MPEG1-Systems (audio and video) or MPEG1-Video (video-only) movie
  file formats 2 Details on the url field can be found in 4.5, VRML
  and the World Wide Web
  (http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.5).
  MovieTexture nodes can be referenced by an Appearance node's texture
  field (as a movie texture) and by a Sound node's source field (as an
  audio source only).  See 4.6.11, Texture maps
  (http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.11),
  for a general description of texture maps.  4.14, Lighting model
  (http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.14),
  contains details on lighting equations and the interaction between
  textures, materials, and geometries.  

  As soon as the movie is loaded, a duration_changed eventOut is
  sent. This indicates the duration of the movie in seconds. This
  eventOut value can be read (for instance, by a Script node) to
  determine the duration of a movie. A value of "-1" implies the movie
  has not yet loaded or the value is unavailable for some reason.  

  The loop, startTime, and stopTime exposedFields and the isActive
  eventOut, and their effects on the MovieTexture node, are discussed
  in detail in the 4.6.9, Time-dependent nodes, section
  (http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.9).

  The cycle of a MovieTexture node is the length of time in seconds
  for one playing of the movie at the specified speed.  The speed
  exposedField indicates how fast the movie shall be played.  A speed
  of 2 indicates the movie plays twice as fast. The duration_changed
  output is not affected by the speed exposedField. set_speed events
  are ignored while the movie is playing. A negative speed implies
  that the movie will play backwards.  

  If a MovieTexture node is inactive when the movie is first loaded,
  frame 0 of the movie texture is displayed if speed is non-negative
  or the last frame of the movie texture is shown if speed is negative
  (see 4.11.3, Discrete and continuous changes).  A MovieTexture node
  shall display frame 0 if speed = 0. For positive values of speed, an
  active MovieTexture node displays the frame at movie time t as
  follows (i.e., in the movie's local time system with frame 0 at time
  0 with speed = 1): 
  
  \verbatim
    t = (now - startTime) modulo (duration/speed)
  \endverbatim

  If speed is negative, the MovieTexture node displays the frame at movie
  time:
  
  \verbatim
    t = duration - ((now - startTime) modulo |duration/speed|)
  \endverbatim

  When a MovieTexture node becomes inactive, the frame corresponding
  to the time at which the MovieTexture became inactive will remain as
  the texture.  

*/

/*!
  \var SoSFBool SoVRMLMovieTexture::loop
  Specifies whether movie should be looped or not. Default value is FALSE.
*/

/*!
  \var SoSFFloat SoVRMLMovieTexture::speed
  Specifies the relative speed. Default value is 1.0.
*/

/*!
  \var SoSFTime SoVRMLMovieTexture::startTime
  Specifies the start time.
*/

/*!
  \var SoSFTime SoVRMLMovieTexture::stopTime
  Specifies the stop time.
*/

/*!
  \var SoMFString SoVRMLMovieTexture::url
  The filename URL.
*/

/*!
  \var SoSFTime SoVRMLMovieTexture::duration_changed
  An eventOut that is signaled after the node is loaded or inserted in the scene graph.
*/

/*!
  \var SoSFBool SoVRMLMovieTexture::isActive
  An eventOut that is signaled when the movie begins/finishes playing.
*/

#include <Inventor/VRMLnodes/SoVRMLMovieTexture.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/nodes/SoSubNodeP.h>

SO_NODE_SOURCE(SoVRMLMovieTexture);

// Doc in parent
void
SoVRMLMovieTexture::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLMovieTexture, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLMovieTexture::SoVRMLMovieTexture(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLMovieTexture);

  SO_VRMLNODE_ADD_EXPOSED_FIELD(loop, (FALSE));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(speed, (1.0f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(startTime, (SbTime::zero()));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(stopTime, (SbTime::zero()));
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(url);

  SO_VRMLNODE_ADD_EVENT_OUT(duration_changed);
  SO_VRMLNODE_ADD_EVENT_OUT(isActive);
}

/*!
  Destructor.
*/
SoVRMLMovieTexture::~SoVRMLMovieTexture()
{
}

// Doc in parent
void
SoVRMLMovieTexture::GLRender(SoGLRenderAction * action)
{
}
