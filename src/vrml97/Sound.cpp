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
  \class SoVRMLSound SoVRMLSound.h Inventor/VRMLnodes/SoVRMLSound.h
  \brief The SoVRMLSound class is used to represent a sound source.
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

