/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoClipPlane SoClipPlane.h Inventor/nodes/SoClipPlane.h
  \brief The SoClipPlane class is a node type for specifying clipping planes.
  \ingroup nodes

  A scene graph \e without any SoClipPlane nodes uses six clipping
  planes to define the viewing frustum: top, bottom, left, right, near
  and far. If you want extra clipping planes for "slicing" the visible
  geometry, you can do that by using nodes of this type.  Geometry on
  the back side of the clipping plane is clipped away.

  Note that OpenGL implementations have a fixed maximum number of
  clipping planes available. To find out what this number is, you can
  use the following code:

  \code
      #include <Inventor/elements/SoGLClipPlaneElement.h>
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
#include <Inventor/elements/SoCullElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>

/*!
  \var SoSFPlane SoClipPlane::plane
  Definition of clipping plane.
  Geometry on the back side of the clipping plane is clipped away.
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
  SO_NODE_INTERNAL_INIT_CLASS(SoClipPlane, SO_FROM_INVENTOR_1);

  SO_ENABLE(SoGLRenderAction, SoGLClipPlaneElement);
  SO_ENABLE(SoPickAction, SoClipPlaneElement);
  SO_ENABLE(SoCallbackAction, SoClipPlaneElement);
}

// Doc from superclass.
void
SoClipPlane::doAction(SoAction * action)
{
  if (this->on.isIgnored() || this->on.getValue()) {
    SoClipPlaneElement::add(action->getState(), this, plane.getValue());
  }
}

// Doc from superclass.
void
SoClipPlane::GLRender(SoGLRenderAction * action)
{
  SoClipPlane::doAction(action);
  if (this->on.isIgnored() || this->on.getValue()) {
    SbPlane p(SbVec3f(1.0f, 0.0f, 0.0f), 0.0f);
    if (!this->plane.isIgnored()) p = this->plane.getValue();
    p.transform(SoModelMatrixElement::get(action->getState()));
    SoCullElement::addPlane(action->getState(), p);
  }
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
