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
  \class SoVRMLProximitySensor SoVRMLProximitySensor.h Inventor/VRMLnodes/SoVRMLProximitySensor.h
  \brief The SoVRMLProximitySensor class is used to generate events when the viewer enters or exits a region.
*/

/*!
  \var SoSFVec3f SoVRMLProximitySensor::center
  The center of the region. Default value is (0,0,0).
*/

/*!
  \var SoSFVec3f SoVRMLProximitySensor::size
  The region size. Default value is (0, 0, 0).
*/

/*!
  \var SoSFBool SoVRMLProximitySensor::enabled
  Enable/disable sensor. Default value is TRUE.
*/


/*!
  \var SoSFBool SoVRMLProximitySensor::isActive
  An eventOut that is sent every time the sensor changes state.
*/

/*!
  \var SoSFVec3f SoVRMLProximitySensor::position_changed

  An eventOut that is sent when the viewer is inside the region and
  the viewer position or orientation is changed.

*/

/*!
  \var SoSFRotation SoVRMLProximitySensor::orientation_changed

  An eventOut that is sent when the viewer is inside the region and
  the viewer position or orientation is changed.

*/

/*!
  \var SoSFTime SoVRMLProximitySensor::enterTime
  An eventOut that is sent when the viewer enters the region.
*/

/*!
  \var SoSFTime SoVRMLProximitySensor::exitTime
  An eventOut that is sent when the viewer exits the region.
*/

#include <Inventor/VRMLnodes/SoVRMLProximitySensor.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/nodes/SoSubNodeP.h>

SO_NODE_SOURCE(SoVRMLProximitySensor);

// Doc in parent
void
SoVRMLProximitySensor::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLProximitySensor, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLProximitySensor::SoVRMLProximitySensor(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLProximitySensor);

  SO_VRMLNODE_ADD_EXPOSED_FIELD(center, (0.0f, 0.0f, 0.0f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(size, (0.0f, 0.0f, 0.0f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(enabled, (TRUE));

  SO_VRMLNODE_ADD_EVENT_OUT(isActive);
  SO_VRMLNODE_ADD_EVENT_OUT(position_changed);
  SO_VRMLNODE_ADD_EVENT_OUT(orientation_changed);
  SO_VRMLNODE_ADD_EVENT_OUT(enterTime);
  SO_VRMLNODE_ADD_EVENT_OUT(exitTime);
}

/*!
  Destructor.
*/
SoVRMLProximitySensor::~SoVRMLProximitySensor()
{
}

// Doc in parent
SbBool
SoVRMLProximitySensor::affectsState(void) const
{
  return FALSE;
}

// Doc in parent
void
SoVRMLProximitySensor::doAction(SoAction * action)
{
}

// Doc in parent
void
SoVRMLProximitySensor::GLRender(SoGLRenderAction * action)
{
  SoVRMLProximitySensor::doAction((SoAction*) action);
}

// Doc in parent
void
SoVRMLProximitySensor::callback(SoCallbackAction * action)
{
  SoVRMLProximitySensor::doAction((SoAction*) action);
}

// Doc in parent
void
SoVRMLProximitySensor::rayPick(SoRayPickAction * action)
{
  SoVRMLProximitySensor::doAction((SoAction*) action);
}

// Doc in parent
void
SoVRMLProximitySensor::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoVRMLProximitySensor::doAction((SoAction*) action);
}

// Doc in parent
void
SoVRMLProximitySensor::notify(SoNotList * list)
{
  inherited::notify(list);
}

/*!
  Send events.
*/
void
SoVRMLProximitySensor::updateState(SbVec3f & viewerpos,
                                   SbVec3f & viewerorientation,
                                   SbBool inside)
{
}
