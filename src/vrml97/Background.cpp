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
  \class SoVRMLBackground SoVRMLBackground Inventor/VRMLnodes/SoVRMLBackground.h
  \brief The SoVRMLBackground class is used for specifying a viewer panorama.
  
*/

/*!  
  SoMFFloat SoVRMLBackground::groundAngle

  The ground angles where different colors should be used.
*/

/*!  
  SoMFColor SoVRMLBackground::groundColor
  
  The color for each groundAngle.
*/

/*!  
  SoMFFloat SoVRMLBackground::skyAngle

  The sky angles where different colors should be used.
*/

/*!  
  SoMFColor SoVRMLBackground::skyColor

  The color for each skyAngle.
*/

/*!  
  SoMFString SoVRMLBackground::backUrl

  URL for the background image.
*/

/*!  
  SoMFString SoVRMLBackground::bottomUrl

  URL for the bottom image.
*/

/*!  
  SoMFString SoVRMLBackground::frontUrl

  URL for the front image.
*/

/*!  
  SoMFString SoVRMLBackground::leftUrl

  URL for the left image.
*/

/*!  
  SoMFString SoVRMLBackground::rightUrl

  URL for the right image.
*/

/*!  
  SoMFString SoVRMLBackground::topUrl

  URL for the top image.
*/

/*!  
  SoSFBool SoVRMLBackground::set_bind
  An eventIn which is triggered when the node is bound.
*/

/*!  
  SoSFBool SoVRMLBackground::isBound
  An eventOut that is sent after the node has been bound/unbound.
*/


#include <Inventor/VRMLnodes/SoVRMLBackground.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>

SO_NODE_SOURCE(SoVRMLBackground);

// Doc in parent
void
SoVRMLBackground::initClass(void) // static
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLBackground, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLBackground::SoVRMLBackground(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLBackground);

  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(groundColor);
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(skyColor);
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(groundAngle);
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(skyAngle);
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(backUrl);
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(bottomUrl);
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(frontUrl);
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(leftUrl);
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(rightUrl);
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(topUrl);

  SO_VRMLNODE_ADD_EVENT_IN(set_bind);
  SO_VRMLNODE_ADD_EVENT_OUT(isBound);
}

/*!
  Destructor.
*/
SoVRMLBackground::~SoVRMLBackground()
{
}

// Doc in parent
void
SoVRMLBackground::GLRender(SoGLRenderAction * action)
{
}

