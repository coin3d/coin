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
  \class SoRotation SoRotation.h Inventor/nodes/SoRotation.h
  \brief The SoRotation class specifies a rotation transformation.
  \ingroup nodes

  Use nodes of this class type to re-orient geometry data within the
  scene graph.

  \sa SbRotation, SoRotationXYZ
*/

#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoSubNodeP.h>


#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/elements/SoModelMatrixElement.h>

/*!
  \var SoSFRotation SoRotation::rotation

  Rotation specification. Defaults to no rotation at all. See the
  SbRotation documentation for instructions on how to set the value of
  this field.
*/

// *************************************************************************

SO_NODE_SOURCE(SoRotation);

/*!
  Constructor.
*/
SoRotation::SoRotation()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoRotation);

  SO_NODE_ADD_FIELD(rotation, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
}

/*!
  Destructor.
*/
SoRotation::~SoRotation()
{
}

// Doc from superclass.
void
SoRotation::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoRotation, SO_FROM_INVENTOR_2_1);
}

// Doc from superclass.
void
SoRotation::doAction(SoAction * action)
{
  if (!this->rotation.isIgnored()) {
    SoModelMatrixElement::rotateBy(action->getState(), this,
                                   this->rotation.getValue());
  }
}

// Doc from superclass.
void
SoRotation::callback(SoCallbackAction * action)
{
  SoRotation::doAction((SoAction *)action);
}

// Doc from superclass.
void
SoRotation::GLRender(SoGLRenderAction * action)
{
  SoRotation::doAction((SoAction *)action);
}

// Doc from superclass.
void
SoRotation::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoRotation::doAction((SoAction *)action);
}

// Doc from superclass.
void
SoRotation::getMatrix(SoGetMatrixAction * action)
{
  SbMatrix m;

  SbRotation r = this->rotation.getValue();
  r.getValue(m);
  action->getMatrix().multLeft(m);

  SbRotation ri = r.inverse();
  ri.getValue(m);
  action->getInverse().multRight(m);
}

// Doc from superclass.
void
SoRotation::pick(SoPickAction * action)
{
  SoRotation::doAction((SoAction *)action);
}

// Doc from superclass.
void
SoRotation::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  SoRotation::doAction((SoAction *)action);
}
