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
  \class SoTranslation SoTranslation.h Inventor/nodes/SoTranslation.h
  \brief The SoTranslation class is a node type for specifying geometry translations.
  \ingroup nodes

  For simply translating some geometry in a scene graph, you can use
  this node type.
*/

#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/elements/SoModelMatrixElement.h>

/*!
  \var SoSFVec3f SoTranslation::translation
  Set the X, Y and Z translation values. Defaults to <0, 0, 0>.
*/

// *************************************************************************

SO_NODE_SOURCE(SoTranslation);

/*!
  Constructor.
*/
SoTranslation::SoTranslation(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoTranslation);

  SO_NODE_ADD_FIELD(translation, (0.0f, 0.0f, 0.0f));
}

/*!
  Destructor.
*/
SoTranslation::~SoTranslation()
{
}

// Doc in superclass.
void
SoTranslation::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoTranslation, SO_FROM_INVENTOR_1|SoNode::VRML1);
}

// Doc in superclass.
void
SoTranslation::doAction(SoAction * action)
{
  if (this->translation.getValue() != SbVec3f(0.0f, 0.0f, 0.0f)) {
    SoModelMatrixElement::translateBy(action->getState(), this,
                                      this->translation.getValue());
  }
}

// Doc in superclass.
void
SoTranslation::GLRender(SoGLRenderAction * action)
{
  SoTranslation::doAction((SoAction *)action);
}

// Doc in superclass.
void
SoTranslation::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoTranslation::doAction(action);
}

// Doc in superclass.
void
SoTranslation::callback(SoCallbackAction * action)
{
  SoTranslation::doAction((SoAction *)action);
}

// Doc in superclass.
void
SoTranslation::getMatrix(SoGetMatrixAction * action)
{
  SbVec3f v = this->translation.getValue();
  SbMatrix m;
  m.setTranslate(v);
  action->getMatrix().multLeft(m);
  m.setTranslate(-v);
  action->getInverse().multRight(m);
}

// Doc in superclass.
void
SoTranslation::pick(SoPickAction * action)
{
  SoTranslation::doAction((SoAction *)action);
}

// Doc in superclass.
void
SoTranslation::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  SoTranslation::doAction((SoAction *)action);
}
