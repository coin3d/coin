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
  \class SoVRMLLight SoVRMLLight.h Inventor/VRMLnodes/SoVRMLLight.h
  \brief The SoVRMLLight class is a superclass for VRML light nodes.
*/

/*!
  \var SoSFBool SoVRMLLight::on
  Enable/disable light. Default value is TRUE.
*/

/*!
  \var SoSFFloat SoVRMLLight::intensity
  The light intensity. A value from 0 to 1. Default value is 1.
*/

/*!
  \var SoSFColor SoVRMLLight::color
  The light color. Default value is (1, 1, 1).
*/

/*!
  \var SoSFFloat SoVRMLLight::ambientIntensity
  The light ambient intensity. A value from 0 to 1. Default value is 0.
*/

#include <Inventor/VRMLnodes/SoVRMLLight.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>

SO_NODE_ABSTRACT_SOURCE(SoVRMLLight);

// Doc in parent
void
SoVRMLLight::initClass(void)
{
  SO_NODE_INTERNAL_INIT_ABSTRACT_CLASS(SoVRMLLight, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLLight::SoVRMLLight(void)
{
  SO_VRMLNODE_INTERNAL_CONSTRUCTOR(SoVRMLLight);

  SO_VRMLNODE_ADD_FIELD(on, (TRUE));
  SO_VRMLNODE_ADD_FIELD(intensity, (1.0f));
  SO_VRMLNODE_ADD_FIELD(color, (1.0f, 1.0f, 1.0f));
  SO_VRMLNODE_ADD_FIELD(ambientIntensity, (0.0f));
}

/*!
  Destructor.
*/
SoVRMLLight::~SoVRMLLight()
{
}

// Doc in parent
void
SoVRMLLight::GLRender(SoGLRenderAction * action)
{
}

