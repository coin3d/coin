/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoTextureCoordinateDefault SoTextureCoordinateDefault.h Inventor/nodes/SoTextureCoordinateDefault.h
  \brief The SoTextureCoordinateDefault class removes texture coordinates from the state.
  \ingroup nodes

  Shapes below this node in the scenegraph will have to use its default 
  texture coordinates as SoTextureCoordinateDefault cleans out all previously
  defined texture coordinates and texture coordinate functions.
*/

#include <Inventor/nodes/SoTextureCoordinateDefault.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>

// *************************************************************************

SO_NODE_SOURCE(SoTextureCoordinateDefault);

/*!
  Constructor.
*/
SoTextureCoordinateDefault::SoTextureCoordinateDefault()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoTextureCoordinateDefault);
}

/*!
  Destructor.
*/
SoTextureCoordinateDefault::~SoTextureCoordinateDefault()
{
}

// doc in super
void
SoTextureCoordinateDefault::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoTextureCoordinateDefault, SO_FROM_INVENTOR_1);
}

// doc from parent
void
SoTextureCoordinateDefault::doAction(SoAction * action)
{
  SoTextureCoordinateElement::setDefault(action->getState(), this);
}

// doc from parent
void
SoTextureCoordinateDefault::GLRender(SoGLRenderAction * action)
{
  SoGLTextureCoordinateElement::setTexGen(action->getState(),
                                          this, NULL);
  SoTextureCoordinateDefault::doAction((SoAction *)action);
}

// doc from parent
void
SoTextureCoordinateDefault::callback(SoCallbackAction * action)
{
  SoTextureCoordinateDefault::doAction((SoAction *)action);
}

// doc from parent
void
SoTextureCoordinateDefault::pick(SoPickAction * action)
{
  SoTextureCoordinateDefault::doAction((SoAction *)action);
}
