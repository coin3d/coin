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
  SO_VRMLNODE_INTERNAL_CONSTRUCTOR(SoVRMLTexture);

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


