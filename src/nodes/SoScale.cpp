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
  \class SoScale SoScale.h Inventor/nodes/SoScale.h
  \brief The SoScale class is a node type for scaling scene graph geometry.
  \ingroup nodes

  Use nodes of this type to apply scaling operations during scenegraph
  traversals for e.g. rendering. Scale values are specified in a
  triple-value vector, with one scale factor for each of the 3
  principal axes.
*/

#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoGLNormalizeElement.h>

/*!
  \var SoSFVec3f SoScale::scaleFactor

  Specifies scaling along the 3 axes.

  To get a uniform scale applied to the affected shapes, set the
  scaleFactor field to a vector with the same value for all
  components.

  A common error when doing non-uniform scaling in a single direction
  is to set the value for the other two components of the scaleFactor
  vector to 0. This is obviously wrong, they should be set to 1 to \e
  not scale the shape(s) in the other two directions.

  Be careful with setting scaleFactor component values to 0 or to
  negative values.  Most shapes should handle those cases somehow, but
  the results are undefined unless otherwise specified.

  The default value of this vector field is [1.0, 1.0, 1.0].
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
  SO_NODE_INTERNAL_INIT_CLASS(SoScale, SO_FROM_INVENTOR_1|SoNode::VRML1);
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
