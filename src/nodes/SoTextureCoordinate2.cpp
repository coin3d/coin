/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

/*!
  \class SoTextureCoordinate2 SoTextureCoordinate2.h Inventor/nodes/SoTextureCoordinate2.h
  \brief The SoTextureCoordinate2 class ...
  \ingroup nodes

  FIXME: write class doc
*/


#include <Inventor/nodes/SoTextureCoordinate2.h>
#include <Inventor/nodes/SoSubNodeP.h>


#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#include <Inventor/actions/SoCallbackAction.h>

/*!
  \var SoMFVec2f SoTextureCoordinate2::point
  FIXME: write documentation for field
*/

// *************************************************************************

SO_NODE_SOURCE(SoTextureCoordinate2);

/*!
  Constructor.
*/
SoTextureCoordinate2::SoTextureCoordinate2()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoTextureCoordinate2);
  SO_NODE_ADD_FIELD(point, (NULL));
}

/*!
  Destructor.
*/
SoTextureCoordinate2::~SoTextureCoordinate2()
{
}

/*!
  Does initialization common for all objects of the
  SoTextureCoordinate2 class. This includes setting up the
  type system, among other things.
*/
void
SoTextureCoordinate2::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoTextureCoordinate2);

  SO_ENABLE(SoGLRenderAction, SoGLTextureCoordinateElement);

  SO_ENABLE(SoCallbackAction, SoTextureCoordinateElement);
}

/*!
  FIXME: write function documentation
*/
void
SoTextureCoordinate2::doAction(SoAction * action)
{
  SoTextureCoordinateElement::set2(action->getState(), this,
                                   point.getNum(),
                                   point.getValues(0));
}

/*!
  FIXME: write function documentation
*/
void
SoTextureCoordinate2::GLRender(SoGLRenderAction * action)
{
  SoGLTextureCoordinateElement::setTexGen(action->getState(),
                                          this, NULL);
  SoTextureCoordinate2::doAction((SoAction *)action);
}

/*!
  FIXME: write function documentation
*/
void
SoTextureCoordinate2::callback(SoCallbackAction * action)
{
  SoTextureCoordinate2::doAction((SoAction *)action);
}

/*!
  FIXME: write function documentation
*/
void
SoTextureCoordinate2::pick(SoPickAction * action)
{
  SoTextureCoordinate2::doAction((SoAction *)action);
}
