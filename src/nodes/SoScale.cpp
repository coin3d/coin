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
  \class SoScale SoScale.h Inventor/nodes/SoScale.h
  \brief The SoScale class is a node type for scaling scene graph geometry.
  \ingroup nodes

  Use nodes of this type to apply scaling during e.g. rendering
  traversal. Scale values are specified in a triple-value vector, with
  one scale factor for each of the 3 axes.
*/

#include <Inventor/nodes/SoScale.h>

#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoGLNormalizeElement.h>

/*!
  \var SoSFVec3f SoScale::scaleFactor

  Specifies scale values along the 3 axes (i.e. scaling does not need
  to be uniform). The most common use of scaling operations is to do
  scaling along a single direction, in this case set the scale factor
  values of the other two axes to 1.0.

  The default value of this vector field is <1, 1, 1>.
*/

// *************************************************************************

SO_NODE_SOURCE(SoScale);

/*!
  Constructor.
*/
SoScale::SoScale(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoScale);

  SO_NODE_ADD_FIELD(scaleFactor, (1.0f, 1.0f, 1.0f));
}

/*!
  Destructor.
*/
SoScale::~SoScale()
{
}

// Doc in superclass.
void
SoScale::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoScale);
}

// Doc in superclass.
void
SoScale::doAction(SoAction * action)
{
  SoModelMatrixElement::scaleBy(action->getState(), this,
                                this->scaleFactor.getValue());
}

// Doc in superclass.
void
SoScale::GLRender(SoGLRenderAction * action)
{
  if (scaleFactor.getValue() != SbVec3f(1.0f, 1.0f, 1.0f)) {
    SoGLNormalizeElement::setMatrixState(action->getState(), FALSE);
  }

  SoScale::doAction((SoAction *)action);
}

// Doc in superclass.
void
SoScale::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoScale::doAction((SoAction *)action);
}

// Doc in superclass.
void
SoScale::callback(SoCallbackAction *action)
{
  SoScale::doAction((SoAction*)action);
}

// Doc in superclass.
void
SoScale::getMatrix(SoGetMatrixAction * action)
{
  SbVec3f scalevec = this->scaleFactor.getValue();
  SbMatrix m;

  m.setScale(scalevec);
  action->getMatrix().multLeft(m);

  m.setScale(SbVec3f(1.0f / scalevec[0], 1.0f / scalevec[1], 1.0f / scalevec[2]));
  action->getInverse().multRight(m);
}

// Doc in superclass.
void
SoScale::pick(SoPickAction *action)
{
  SoScale::doAction((SoAction*)action);
}

// Doc in superclass.
void
SoScale::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  SoScale::doAction((SoAction*)action);
}
