/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#ifdef HAVE_VRML97

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
#include "coindefs.h"

#include <Inventor/VRMLnodes/SoVRMLMacros.h>

#include "nodes/SoSubNodeP.h"

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
SoVRMLTexture::GLRender(SoGLRenderAction * COIN_UNUSED_ARG(action))
{
}

#endif // HAVE_VRML97
