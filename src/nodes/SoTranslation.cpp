/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoTranslation SoTranslation.h Inventor/nodes/SoTranslation.h
  \brief The SoTranslation class is a node type for specifying geometry translations.
  \ingroup nodes

  For simply translating some geometry in a scene graph, you can use
  this node type.

  <b>FILE FORMAT/DEFAULTS:</b>
  \code
    Translation {
        translation 0 0 0
    }
  \endcode
*/

// *************************************************************************

#include <Inventor/nodes/SoTranslation.h>

#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/elements/SoModelMatrixElement.h>

#include "nodes/SoSubNodeP.h"

// *************************************************************************

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

// Doc in superclass. Overrides the traversal method in this class for
// the SoGetPrimitiveCountAction because the number of primitives can
// be different depending on scene location (and thereby distance to
// camera) if there are e.g. SoLOD nodes in the scene.
void
SoTranslation::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  SoTranslation::doAction((SoAction *)action);
}
