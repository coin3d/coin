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
  \ingroup VRMLnodes

  \WEB3DCOPYRIGHT

  \verbatim
  ProximitySensor {
    exposedField SFVec3f    center      0 0 0    # (-,)
    exposedField SFVec3f    size        0 0 0    # [0,)
    exposedField SFBool     enabled     TRUE
    eventOut     SFBool     isActive
    eventOut     SFVec3f    position_changed
    eventOut     SFRotation orientation_changed
    eventOut     SFTime     enterTime
    eventOut     SFTime     exitTime
  }
  \endverbatim
  
  The ProximitySensor node generates events when the viewer enters,
  exits, and moves within a region in space (defined by a box). 
  
  A proximity sensor is enabled or disabled by sending it an enabled
  event with a value of TRUE or FALSE. A disabled sensor does not send
  events.

  A ProximitySensor node generates isActive TRUE/FALSE events as the
  viewer enters and exits the rectangular box defined by its center
  and size fields. Browsers shall interpolate viewer positions and
  timestamp the isActive events with the exact time the viewer first
  intersected the proximity region.

  The \e center field defines the centre point of the proximity region
  in object space. The \e size field specifies a vector which defines
  the width (x), height (y), and depth (z) of the box bounding the
  region. The components of the size field shall be greater than or
  equal to zero. ProximitySensor nodes are affected by the
  hierarchical transformations of their parents.

  The \e enterTime event is generated whenever the isActive TRUE event
  is generated (user enters the box), and \e exitTime events are
  generated whenever an isActive FALSE event is generated (user exits
  the box).

  The \e position_changed and \e orientation_changed eventOuts send
  events whenever the user is contained within the proximity region
  and the position and orientation of the viewer changes with respect
  to the ProximitySensor node's coordinate system including enter and
  exit times.
  
  The viewer movement may be a result of a variety of circumstances
  resulting from browser navigation, ProximitySensor node's coordinate
  system changes, or bound Viewpoint node's position or orientation
  changes.

  Each ProximitySensor node behaves independently of all other
  ProximitySensor nodes. Every enabled ProximitySensor node that is
  affected by the viewer's movement receives and sends events,
  possibly resulting in multiple

  ProximitySensor nodes receiving and sending events
  simultaneously. Unlike TouchSensor nodes, there is no notion of a
  ProximitySensor node lower in the scene graph "grabbing" events.

  Instanced (DEF/USE) ProximitySensor nodes use the union of all the
  boxes to check for enter and exit. A multiply instanced
  ProximitySensor node will detect enter and exit for all instances of
  the box and send enter/exit events appropriately. However, the
  results are undefined if the any of the boxes of a multiply
  instanced ProximitySensor node overlap.

  A ProximitySensor node that surrounds the entire world has an
  enterTime equal to the time that the world was entered and can be
  used to start up animations or behaviours as soon as a world is
  loaded.

  A ProximitySensor node with a box containing zero volume (i.e., any
  size field element of 0.0) cannot generate events. This is
  equivalent to setting the enabled field to FALSE.

  A ProximitySensor read from a VRML file shall generate isActive
  TRUE, position_changed, orientation_changed and enterTime events if
  the sensor is enabled and the viewer is inside the proximity region.

  A ProximitySensor inserted into the transformation hierarchy shall
  generate isActive TRUE, position_changed, orientation_changed and
  enterTime events if the sensor is enabled and the viewer is inside
  the proximity region.
  
  A ProximitySensor removed from the transformation hierarchy shall
  generate isActive FALSE, position_changed, orientation_changed and
  exitTime events if the sensor is enabled and the viewer is inside
  the proximity region.

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
