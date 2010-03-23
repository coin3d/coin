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
  \class SoResetTransform SoResetTransform.h Inventor/nodes/SoResetTransform.h
  \brief The SoResetTransform class is a node type which makes it possible to "nullify" state during traversal.
  \ingroup nodes

  SoResetTransform is useful for setting up geometry in the scene
  graph which will not be influenced by the transformation nodes
  before it during traversal.

  <b>FILE FORMAT/DEFAULTS:</b>
  \code
    ResetTransform {
        whatToReset TRANSFORM
    }
  \endcode
*/

// *************************************************************************

#include <Inventor/nodes/SoResetTransform.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/elements/SoGLModelMatrixElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoCacheElement.h>

#include "nodes/SoSubNodeP.h"

// *************************************************************************

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
  if (!this->whatToReset.isIgnored() &&
      (this->whatToReset.getValue() & SoResetTransform::TRANSFORM)) {
    SoState * state = action->getState();
    SoGLModelMatrixElement::makeIdentity(state, this);
  }
}

// Doc from superclass.
void
SoResetTransform::getBoundingBox(SoGetBoundingBoxAction * action)
{
  if (!this->whatToReset.isIgnored() &&
      (this->whatToReset.getValue() & SoResetTransform::TRANSFORM)) {
    SoState * state = action->getState();
    // do this instead of calling SoResetTransform::doAction(action), so
    // that SoLocalBBoxMatrixElement and SoBBoxModelMatrixElement can do
    // the right thing.
    SoModelMatrixElement::mult(state, 
                               this, 
                               SoModelMatrixElement::get(state).inverse());
  }
  if (!this->whatToReset.isIgnored() &&
      (this->whatToReset.getValue() & SoResetTransform::BBOX)) {
    action->getXfBoundingBox().makeEmpty();
    action->resetCenter();
  }
}

// Doc from superclass.
void
SoResetTransform::doAction(SoAction *action)
{
  if (!this->whatToReset.isIgnored() &&
      (this->whatToReset.getValue() & SoResetTransform::TRANSFORM)) {
    SoModelMatrixElement::makeIdentity(action->getState(), this);
  }
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
  if (!this->whatToReset.isIgnored() &&
      (this->whatToReset.getValue() & SoResetTransform::TRANSFORM)) {
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

// Doc from superclass. Overrides the traversal method in this class for
// the SoGetPrimitiveCountAction because the number of primitives can
// be different depending on scene location (and thereby distance to
// camera) if there are e.g. SoLOD nodes in the scene.
void
SoResetTransform::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  SoResetTransform::doAction((SoAction*)action);
}
