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
  \class SoTransformSeparator SoTransformSeparator.h Inventor/nodes/SoTransformSeparator.h
  \brief The SoTransformSeparator class is a group node preserving the current transformations.
  \ingroup nodes

  This node works like the SoSeparator group node, except that it only
  stores and restores the current model matrix transformation. Other
  appearance settings, like materials, textures, cameras, lights, etc,
  will affect the remaining parts of the scenegraph after traversal,
  just like as for the SoGroup node.

  \sa SoSeparator, SoGroup
*/


#include <Inventor/nodes/SoTransformSeparator.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/elements/SoBBoxModelMatrixElement.h>
#include <Inventor/misc/SoChildList.h>

SO_NODE_SOURCE(SoTransformSeparator);

/*!
  Default constructor.
*/
SoTransformSeparator::SoTransformSeparator(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoTransformSeparator);
}

/*!
  Constructor.

  The argument should be the approximate number of children which is
  expected to be inserted below this node. The number need not be
  exact, as it is only used as a hint for better memory resource
  allocation.
*/
SoTransformSeparator::SoTransformSeparator(int nChildren)
  : inherited(nChildren)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoTransformSeparator);
}

/*!
  Destructor.
*/
SoTransformSeparator::~SoTransformSeparator()
{
}

// Documented in superclass.
void
SoTransformSeparator::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoTransformSeparator, SO_FROM_INVENTOR_1);
}

// Documented in superclass.
void
SoTransformSeparator::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SbMatrix matrix, localMatrix;
  SoBBoxModelMatrixElement::pushMatrix(action->getState(),
                                       matrix,
                                       localMatrix);
  inherited::getBoundingBox(action);
  SoBBoxModelMatrixElement::popMatrix(action->getState(),
                                      matrix,
                                      localMatrix);
}

// Documented in superclass.
void
SoTransformSeparator::doAction(SoAction *action)
{
  SbMatrix matrix = SoModelMatrixElement::pushMatrix(action->getState());
  inherited::doAction(action);
  SoModelMatrixElement::popMatrix(action->getState(), matrix);
}

// Documented in superclass.
void
SoTransformSeparator::callback(SoCallbackAction * action)
{
  SbMatrix matrix = SoModelMatrixElement::pushMatrix(action->getState());
  inherited::callback(action);
  SoModelMatrixElement::popMatrix(action->getState(), matrix);
}

// Documented in superclass.
void
SoTransformSeparator::GLRender(SoGLRenderAction * action)
{
  SbMatrix matrix = SoModelMatrixElement::pushMatrix(action->getState());
  inherited::GLRender(action);
  SoModelMatrixElement::popMatrix(action->getState(), matrix);
}

// Documented in superclass.
void
SoTransformSeparator::pick(SoPickAction * action)
{
  SbMatrix matrix = SoModelMatrixElement::pushMatrix(action->getState());
  inherited::pick(action);
  SoModelMatrixElement::popMatrix(action->getState(), matrix);
}

// Documented in superclass.
void
SoTransformSeparator::getMatrix(SoGetMatrixAction * action)
{
  // Will only need to traverse if IN_PATH. Other path codes will have
  // no effect on the result.
  int numindices;
  const int * indices;
  if (action->getPathCode(numindices, indices) == SoAction::IN_PATH) {
    this->children->traverseInPath(action, numindices, indices);
  }
}

// Documented in superclass.
void
SoTransformSeparator::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  // FIXME: this looks mysterious -- doesn't we implicitly assume
  // SoGroup::getPrimitiveCount() here? If so, that shouldn't be
  // necessary. Ask pederb. 20020107 mortene.
  SoTransformSeparator::doAction((SoAction *)action);
}
