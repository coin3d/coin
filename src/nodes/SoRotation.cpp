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
  \class SoRotation SoRotation.h Inventor/nodes/SoRotation.h
  \brief The SoRotation class specifies a rotation transformation.
  \ingroup nodes

  Use nodes of this class type to re-orient geometry data within the
  scene graph.

  See SoTransformation class documentation for a short usage example.

  <b>FILE FORMAT/DEFAULTS:</b>
  \code
    Rotation {
        rotation 0 0 1  0
    }
  \endcode

  \sa SbRotation, SoRotationXYZ
*/

// *************************************************************************

#include <Inventor/nodes/SoRotation.h>

#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/elements/SoModelMatrixElement.h>

#include "nodes/SoSubNodeP.h"

// *************************************************************************

/*!
  \var SoSFRotation SoRotation::rotation

  Rotation specification. Defaults to no rotation at all. See the
  SbRotation documentation for instructions on how to set the value of
  this field.


  Note that there is one \e very common mistake that is easy to make
  when setting the value of a an SoSFRotation field, and that is to
  inadvertently use the wrong SbRotation constructor. This example
  should clarify the problem:

  \code
  mytransformnode->rotation.setValue(0, 0, 1, 1.5707963f);
  \endcode

  The programmer clearly tries to set a PI/2 rotation around the Z
  axis, but this will fail, as the SbRotation constructor invoked
  above is the one that takes as arguments the 4 floats of a \e
  quaternion. What the programmer almost certainly wanted to do was to
  use the SbRotation constructor that takes a rotation vector and a
  rotation angle, which is invoked like this:

  \code
  mytransformnode->rotation.setValue(SbVec3f(0, 0, 1), 1.5707963f);
  \endcode


  Another common problem is to set the rotation value to exactly 0.0,
  while wanting to store just a rotation \e angle in the field:
  rotations are internally handled as quaternions, and when converting
  from an angle and a rotation value to a quaternion representation,
  the information about the angle "gets lost" if there is no actual
  rotation.
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
  SO_NODE_INTERNAL_INIT_CLASS(SoRotation, SO_FROM_INVENTOR_1|SoNode::VRML1);
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

// Doc from superclass. Overrides the traversal method in this class for
// the SoGetPrimitiveCountAction because the number of primitives can
// be different depending on scene location (and thereby distance to
// camera) if there are e.g. SoLOD nodes in the scene.
void
SoRotation::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  SoRotation::doAction((SoAction *)action);
}
