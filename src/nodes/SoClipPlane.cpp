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
  \class SoClipPlane SoClipPlane.h Inventor/nodes/SoClipPlane.h
  \brief The SoClipPlane class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoClipPlane.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/elements/SoGLClipPlaneElement.h>
#include <Inventor/elements/SoClipPlaneElement.h>

/*!
  \var SoSFPlane SoClipPlane::plane
  FIXME: write documentation for field
*/
/*!
  \var SoSFBool SoClipPlane::on
  FIXME: write documentation for field
*/


// *************************************************************************

SO_NODE_SOURCE(SoClipPlane);

/*!
  Constructor.
*/
SoClipPlane::SoClipPlane()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoClipPlane);

  SO_NODE_ADD_FIELD(plane, (SbPlane(SbVec3f(1.0f, 0.0f, 0.0f), 0.0f)));
  SO_NODE_ADD_FIELD(on, (TRUE));
}

/*!
  Destructor.
*/
SoClipPlane::~SoClipPlane()
{
}

/*!
  Does initialization common for all objects of the
  SoClipPlane class. This includes setting up the
  type system, among other things.
*/
void
SoClipPlane::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoClipPlane);

  SO_ENABLE(SoGLRenderAction, SoGLClipPlaneElement);
  SO_ENABLE(SoPickAction, SoClipPlaneElement);
  SO_ENABLE(SoCallbackAction, SoClipPlaneElement);
}

/*!
  FIXME: write function documentation
*/
void
SoClipPlane::doAction(SoAction *action)
{
  SbBool ison = on.isIgnored() ? TRUE : on.getValue();
  if (ison && !plane.isIgnored()) {
    SoClipPlaneElement::add(action->getState(), this, plane.getValue());
  }
}

/*!
  FIXME: write function documentation
*/
void
SoClipPlane::GLRender(SoGLRenderAction * action)
{
  SoClipPlane::doAction(action);
}

/*!
  FIXME: write doc
*/
void
SoClipPlane::callback(SoCallbackAction *action)
{
  SoClipPlane::doAction((SoAction*)action);
}

/*!
  FIXME: write doc
*/
void
SoClipPlane::pick(SoPickAction *action)
{
  SoClipPlane::doAction(action);
}
