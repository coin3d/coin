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
  SO_NODE_INTERNAL_INIT_CLASS(SoRotation);
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
