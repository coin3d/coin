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
  \brief The SoClipPlane class is a node type for specifying clipping planes.
  \ingroup nodes

  A scene graph \e without any SoClipPlane nodes uses six clipping
  planes to define the viewing frustum: top, bottom, left, right, near
  and far. If you want extra clipping planes for "slicing" the visible
  geometry, you can do that by using nodes of this type.

  Note that OpenGL implementations have a fixed maximum number of
  clipping planes available. To find out what this number is, you can
  use the following code:

  \code
      #include <Inventor/elements/SoGLClipPlaneElement.h>
#include <Inventor/nodes/SoSubNodeP.h>
      // ...[snip]...
      int maxplanes = SoGLClipPlaneElement::getMaxGLPlanes();
  \endcode
*/

#include <Inventor/nodes/SoClipPlane.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/elements/SoGLClipPlaneElement.h>

/*!
  \var SoSFPlane SoClipPlane::plane
  Definition of clipping plane.
*/
/*!
  \var SoSFBool SoClipPlane::on
  Whether clipping plane should be on or off. Defaults to \c TRUE.
*/


// *************************************************************************

SO_NODE_SOURCE(SoClipPlane);

/*!
  Constructor.
*/
SoClipPlane::SoClipPlane(void)
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

// Doc from superclass.
void
SoClipPlane::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoClipPlane);

  SO_ENABLE(SoGLRenderAction, SoGLClipPlaneElement);
  SO_ENABLE(SoPickAction, SoClipPlaneElement);
  SO_ENABLE(SoCallbackAction, SoClipPlaneElement);
}

// Doc from superclass.
void
SoClipPlane::doAction(SoAction * action)
{
  SbBool ison = on.isIgnored() ? TRUE : on.getValue();
  if (ison && !plane.isIgnored()) {
    SoClipPlaneElement::add(action->getState(), this, plane.getValue());
  }
}

// Doc from superclass.
void
SoClipPlane::GLRender(SoGLRenderAction * action)
{
  SoClipPlane::doAction(action);
}

// Doc from superclass.
void
SoClipPlane::callback(SoCallbackAction * action)
{
  SoClipPlane::doAction(action);
}

// Doc from superclass.
void
SoClipPlane::pick(SoPickAction * action)
{
  SoClipPlane::doAction(action);
}
