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
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLLight);

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

