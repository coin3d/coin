/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoTextureCoordinateDefault SoTextureCoordinateDefault.h Inventor/nodes/SoTextureCoordinateDefault.h
  \brief The SoTextureCoordinateDefault class ...
  \ingroup nodes

  FIXME: write class doc
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

/*!
  Does initialization common for all objects of the
  SoTextureCoordinateDefault class. This includes setting up the
  type system, among other things.
*/
void
SoTextureCoordinateDefault::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoTextureCoordinateDefault);
}

/*!
  FIXME: write function documentation
*/
void
SoTextureCoordinateDefault::doAction(SoAction * action)
{
  SoTextureCoordinateElement::setDefault(action->getState(), this);
}

/*!
  FIXME: write function documentation
*/
void
SoTextureCoordinateDefault::GLRender(SoGLRenderAction * action)
{
  SoGLTextureCoordinateElement::setTexGen(action->getState(),
                                          this, NULL);
  SoTextureCoordinateDefault::doAction((SoAction *)action);
}

/*!
  FIXME: write function documentation
*/
void
SoTextureCoordinateDefault::callback(SoCallbackAction * action)
{
  SoTextureCoordinateDefault::doAction((SoAction *)action);
}

/*!
  FIXME: write function documentation
*/
void
SoTextureCoordinateDefault::pick(SoPickAction * action)
{
  SoTextureCoordinateDefault::doAction((SoAction *)action);
}
