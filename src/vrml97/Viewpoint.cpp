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
  \class SoVRMLViewpoint SoVRMLViewpoint.h Inventor/VRMLnodes/SoVRMLViewpoint.h
  \brief The SoVRMLViewpoint class is a perspective camera class.
*/

/*!
  \var SoSFVec3f SoVRMLViewpoint::position
  The viewpoint position. Default value is (0, 0, 0).
*/

/*!
  \var SoSFRotation SoVRMLViewpoint::orientation
  The camera orientation. By default the camera is aligned along the negative z-axis.
*/

/*!
  \var SoSFFloat SoVRMLViewpoint::fieldOfView
  Field of view. Default value is PI/4.
*/

/*!
  \var SoSFString SoVRMLViewpoint::description
  A textual viewpoint description. Is empty by default.
*/

/*!
  \var SoSFBool SoVRMLViewpoint::jump
  Jump TRUE/FALSE.
*/

/*!
  \var SoSFBool SoVRMLViewpoint::set_bind
  An eventIn that is used to bind the viewpoint.
*/

/*!
  \var SoSFTime SoVRMLViewpoint::bindTime
  An event out that is sent when the viewpoint is bound.
*/

/*!
  \var SoSFBool SoVRMLViewpoint::isBound
  An event out that is sent when the viewpoint is bound/unbound.
*/

#include <Inventor/VRMLnodes/SoVRMLViewpoint.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <math.h>

SO_NODE_SOURCE(SoVRMLViewpoint);

// Doc in parent
void
SoVRMLViewpoint::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLViewpoint, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLViewpoint::SoVRMLViewpoint(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLViewpoint);

  SO_VRMLNODE_ADD_EXPOSED_FIELD(position, (0.0f, 0.0f, 0.0f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(orientation, (SbRotation::identity()));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(fieldOfView, (float(M_PI)/4.0f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(jump, (TRUE));

  SO_VRMLNODE_ADD_FIELD(description, (""));

  SO_VRMLNODE_ADD_EVENT_IN(set_bind);
  SO_VRMLNODE_ADD_EVENT_OUT(bindTime);
  SO_VRMLNODE_ADD_EVENT_OUT(isBound);
}

/*!
  Destructor.
*/
SoVRMLViewpoint::~SoVRMLViewpoint()
{
}

// Doc in parent
void
SoVRMLViewpoint::GLRender(SoGLRenderAction * action)
{
}
