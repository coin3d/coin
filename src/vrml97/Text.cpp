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
  \class SoVRMLText SoVRMLText.h Inventor/VRMLnodes/SoVRMLText.h
  \brief The SoVRMLText class is used to represent text in a scene.
  \ingroup VRMLnodes

  FIXME: implement and document
*/

/*!
  \var SoMFString SoVRMLText::string
  The strings. Empty by default.
*/

/*!
  \var SoMFFloat SoVRMLText::length
  Length of each string in the local coordinate system.
*/

/*!
  \var SoSFNode SoVRMLText::fontStyle
  Can contain an SoVRMLFontStyle node.
*/

/*!
  \var SoSFFloat SoVRMLText::maxExtent
  Maximum object space extent of longest string.
*/

#include <Inventor/VRMLnodes/SoVRMLText.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <stddef.h>

SO_NODE_SOURCE(SoVRMLText);

// Doc in parent
void
SoVRMLText::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLText, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLText::SoVRMLText(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLText);

  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(string);

  SO_VRMLNODE_ADD_EXPOSED_FIELD(fontStyle, (NULL));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(maxExtent, (0.0f));
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(length);
}

/*!
  Destructor.
*/
SoVRMLText::~SoVRMLText()
{
}

// Doc in parent
void
SoVRMLText::GLRender(SoGLRenderAction * action)
{
}

// Doc in parent
void
SoVRMLText::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
}

// Doc in parent
void
SoVRMLText::notify(SoNotList * list)
{
  inherited::notify(list);
}

// Doc in parent
SoChildList *
SoVRMLText::getChildren(void) const
{
  return NULL;
}


// Doc in parent
void
SoVRMLText::computeBBox(SoAction * action,
                        SbBox3f & box,
                        SbVec3f & center)
{
}

// Doc in parent
void
SoVRMLText::generatePrimitives(SoAction * action)
{
}
