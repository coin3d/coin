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
  \class SoVRMLFog SoVRMLFog.h Inventor/VRMLnodes/SoVRMLFog.h
  \brief The SoVRMLFog class is used to specify a global scene fog.
*/

/*!
  \var SoSFString SoVRMLFog::fogType
  Type of fog. Can be either "LINEAR" or "EXPONENTIAL". Default value is "LINEAR".
*/

/*!
  \var SoSFFloat SoVRMLFog::visibilityRange
  Range of visibility. The distance when only the fog color can be seen. Default value is (0.0).
*/

/*!
  \var SoSFColor SoVRMLFog::color
  The fog color. Default value is (1, 1, 1).
*/

/*!
  \var SoSFBool SoVRMLFog::set_bind
  An eventIn that is used to bind this node (make the node active).
*/

/*!
  \var SoSFBool SoVRMLFog::isBound
  An eventOut that is sent when the node is bound/unbound.
*/


#include <Inventor/VRMLnodes/SoVRMLFog.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/nodes/SoSubNodeP.h>

SO_NODE_SOURCE(SoVRMLFog);

// Doc in parent
void
SoVRMLFog::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLFog, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLFog::SoVRMLFog(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLFog);

  SO_VRMLNODE_ADD_EXPOSED_FIELD(fogType, ("LINEAR"));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(visibilityRange, (0.0f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(color, (1.0f, 1.0f, 1.0f));

  SO_VRMLNODE_ADD_EVENT_IN(set_bind);
  SO_VRMLNODE_ADD_EVENT_OUT(isBound);
}

/*!
  Destructor.
*/
SoVRMLFog::~SoVRMLFog()
{
}

// Doc in parent
void
SoVRMLFog::GLRender(SoGLRenderAction * action)
{
  // FIXME: implement.
}


