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
  \class SoTranslation SoTranslation.h Inventor/nodes/SoTranslation.h
  \brief The SoTranslation class is a node type for specifying geometry translations.
  \ingroup nodes

  For simply translating some geometry in a scene graph, you can use
  this node type.
*/

#include <Inventor/nodes/SoTranslation.h>

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
  SO_NODE_INTERNAL_INIT_CLASS(SoTranslation);
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
