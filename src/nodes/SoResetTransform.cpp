/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoResetTransform SoResetTransform.h Inventor/nodes/SoResetTransform.h
  \brief The SoResetTransform class is a node type which makes it possible to "nullify" state during traversal.
  \ingroup nodes

  SoResetTransform is useful for setting up geometry in the scene
  graph which will not be influenced by the transformation nodes
  before it during traversal.
*/

#include <Inventor/nodes/SoResetTransform.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/elements/SoModelMatrixElement.h>


/*!
  \enum SoResetTransform::ResetType

  The different options for what to reset.
*/
/*!
  \var SoResetTransform::ResetType SoResetTransform::TRANSFORM

  Reset current model transformation.
*/
/*!
  \var SoResetTransform::ResetType SoResetTransform::BBOX

  Reset current bounding box settings.
*/


/*!
  \var SoSFBitMask SoResetTransform::whatToReset

  What this node instance should reset in the state when met during
  traversal. Default value is SoResetTransform::TRANSFORM.
*/


// *************************************************************************

SO_NODE_SOURCE(SoResetTransform);

/*!
  Constructor.
*/
SoResetTransform::SoResetTransform(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoResetTransform);

  SO_NODE_ADD_FIELD(whatToReset, (SoResetTransform::TRANSFORM));

  SO_NODE_DEFINE_ENUM_VALUE(ResetType, TRANSFORM);
  SO_NODE_DEFINE_ENUM_VALUE(ResetType, BBOX);
  SO_NODE_SET_SF_ENUM_TYPE(whatToReset, ResetType);
}

/*!
  Destructor.
*/
SoResetTransform::~SoResetTransform()
{
}

// Doc from superclass.
void
SoResetTransform::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoResetTransform, SO_FROM_INVENTOR_1);
}

// Doc from superclass.
void
SoResetTransform::GLRender(SoGLRenderAction * action)
{
  if (this->whatToReset.getValue() & SoResetTransform::TRANSFORM) {
    SoModelMatrixElement::set(action->getState(), this,
                              SbMatrix::identity());
  }
}

// Doc from superclass.
void
SoResetTransform::getBoundingBox(SoGetBoundingBoxAction * action)
{
  if (this->whatToReset.getValue() & SoResetTransform::BBOX) {
    action->getXfBoundingBox().makeEmpty();
    action->resetCenter();
  }

  if (this->whatToReset.getValue() & SoResetTransform::TRANSFORM)
    SoModelMatrixElement::set(action->getState(), this,
                              SbMatrix::identity());
}

// Doc from superclass.
void
SoResetTransform::doAction(SoAction *action)
{
  if (this->whatToReset.getValue() & SoResetTransform::TRANSFORM)
    SoModelMatrixElement::set(action->getState(), this,
                              SbMatrix::identity());
}

// Doc from superclass.
void
SoResetTransform::callback(SoCallbackAction *action)
{
  SoResetTransform::doAction((SoAction*)action);
}

// Doc from superclass.
void
SoResetTransform::getMatrix(SoGetMatrixAction *action)
{
  if (this->whatToReset.getValue() & SoResetTransform::TRANSFORM) {
    action->getMatrix().makeIdentity();
    action->getInverse().makeIdentity();
  }
}

// Doc from superclass.
void
SoResetTransform::pick(SoPickAction *action)
{
  SoResetTransform::doAction((SoAction*)action);
}

// Doc from superclass.
void
SoResetTransform::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  SoResetTransform::doAction((SoAction*)action);
}
