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
  \class SoVRMLTexture SoVRMLTexture.h Inventor/VRMLnodes/SoVRMLTexture.h
  \brief The SoVRMLTexture class is a superclass for VRML texture nodes.
*/

/*!
  \var SoSFBool SoVRMLTexture::repeatS
  TRUE if texure should be repeated in the S direction. Default is TRUE.
*/

/*!
  \var SoSFBool SoVRMLTexture::repeatT
  TRUE if texure should be repeated in the T direction. Default is TRUE.
*/

#include <Inventor/VRMLnodes/SoVRMLTexture.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/nodes/SoSubNodeP.h>

SO_NODE_ABSTRACT_SOURCE(SoVRMLTexture);

// Doc in parent
void
SoVRMLTexture::initClass(void)
{
  SO_NODE_INTERNAL_INIT_ABSTRACT_CLASS(SoVRMLTexture, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLTexture::SoVRMLTexture(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLTexture);

  SO_VRMLNODE_ADD_FIELD(repeatS, (TRUE));
  SO_VRMLNODE_ADD_FIELD(repeatT, (TRUE));
}

/*!
  Destructor.
*/
SoVRMLTexture::~SoVRMLTexture()
{
}

// Doc in parent
void
SoVRMLTexture::GLRender(SoGLRenderAction * action)
{
}


