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



