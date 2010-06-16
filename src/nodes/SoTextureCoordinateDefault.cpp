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

/*!
  \class SoTextureCoordinateDefault SoTextureCoordinateDefault.h Inventor/nodes/SoTextureCoordinateDefault.h
  \brief The SoTextureCoordinateDefault class removes texture coordinates from the state.
  \ingroup nodes

  Shapes below this node in the scenegraph will have to use its default 
  texture coordinates as SoTextureCoordinateDefault cleans out all previously
  defined texture coordinates and texture coordinate functions.

  <b>FILE FORMAT/DEFAULTS:</b>
  \code
    TextureCoordinateDefault {
    }
  \endcode
*/

// *************************************************************************

#include <Inventor/nodes/SoTextureCoordinateDefault.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLMultiTextureCoordinateElement.h>
#include <Inventor/elements/SoTextureUnitElement.h>

#include "nodes/SoSubNodeP.h"

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
  int unit = SoTextureUnitElement::get(action->getState());
  SoMultiTextureCoordinateElement::setDefault(action->getState(), this, unit);
}

// doc from parent
void
SoTextureCoordinateDefault::GLRender(SoGLRenderAction * action)
{
  /*int unit = */SoTextureUnitElement::get(action->getState());
  SoGLMultiTextureCoordinateElement::setTexGen(action->getState(),
                                               this, 0, NULL);
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
