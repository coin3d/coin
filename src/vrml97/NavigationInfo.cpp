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
  \class SoVRMLNavigationInfo SoVRMLNavigationInfo.h Inventor/VRMLnodes/SoVRMLNavigationInfo.h
  \brief The SoVRMLNavigationInfo class is used to specify avatar and viewer settings.
*/

/*!
  \var SoMFString SoVRMLNavigationInfo::type
  Types of viewer. Possible values are "WALK", "ANY", "EXAMINE", "FLY" and "NONE".
  Is set to "WALK" and "ANY" by default.
*/

/*!
  \var SoSFFloat SoVRMLNavigationInfo::speed
  Navigation speed. Default value is 1.0. 
*/

/*!
  \var SoMFFloat SoVRMLNavigationInfo::avatarSize
  Size of avatar. Default value is  (0.25, 1.6, 0.75).
*/

/*!
  \var SoSFFloat SoVRMLNavigationInfo::visibilityLimit
  Visibility limit. Default value is 0.0.
*/

/*!
  \var SoSFBool SoVRMLNavigationInfo::headlight
  Specifies whether headlight should be enabled. Default value is TRUE.
*/


#include <Inventor/VRMLnodes/SoVRMLNavigationInfo.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/nodes/SoSubNodeP.h>

SO_NODE_SOURCE(SoVRMLNavigationInfo);

// Doc in parent
void
SoVRMLNavigationInfo::initClass(void) // static
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLNavigationInfo, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLNavigationInfo::SoVRMLNavigationInfo(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLNavigationInfo);

  SO_VRMLNODE_ADD_EXPOSED_FIELD(type, ("WALK"));
  this->type.setNum(2);
  this->type.set1Value(1, "ANY");
  this->type.setDefault(TRUE);

  SO_VRMLNODE_ADD_EXPOSED_FIELD(speed, (1.0f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(avatarSize, (0.25f));
  this->avatarSize.setNum(3);
  this->avatarSize.set1Value(1, 1.6f);
  this->avatarSize.set1Value(2, 0.75f);
  this->avatarSize.setDefault(TRUE);

  SO_VRMLNODE_ADD_EXPOSED_FIELD(visibilityLimit, (0.0f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(headlight, (TRUE));

  SO_VRMLNODE_ADD_EVENT_IN(set_bind);
  SO_VRMLNODE_ADD_EVENT_OUT(isBound);
}

/*!
  Destructor.
*/
SoVRMLNavigationInfo::~SoVRMLNavigationInfo() // virtual, protected
{
}

// Doc in parent
void
SoVRMLNavigationInfo::GLRender(SoGLRenderAction * action)
{
}
