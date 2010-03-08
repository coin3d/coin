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
  \class SoBumpMapTransform SoBumpMapTransform.h Inventor/nodes/SoBumpMapTransform.h
  \brief The SoBumpMapTransform class is used to define 2D bump map transformations.
  \ingroup nodes

  Bump maps applied to shapes in the scene can be transformed by
  "prefixing" in the state with instances of this node
  type. Translations, rotations and scaling in 2D can all be done.

  The default settings of this node's fields equals a "null
  transform", ie no transformation.

  <b>FILE FORMAT/DEFAULTS:</b>
  \code
    BumpMapTransform {
        translation 0 0
        rotation 0
        scaleFactor 1 1
        center 0 0
    }
  \endcode

  \since Coin 2.2
*/

#include <Inventor/nodes/SoBumpMapTransform.h>
#include "coindefs.h"

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/elements/SoBumpMapMatrixElement.h>
#include <Inventor/actions/SoCallbackAction.h>

#include "nodes/SoSubNodeP.h"

/*!
  \var SoSFVec2f SoBumpMapTransform::translation

  Texture coordinate translation. Default value is [0, 0].
*/
/*!
  \var SoSFFloat SoBumpMapTransform::rotation

  Texture coordinate rotation (around z-axis, s is x-axis and t is
  y-axis).  Defaults to an identity rotation (ie zero rotation).
*/
/*!
  \var SoSFVec2f SoBumpMapTransform::scaleFactor

  Texture coordinate scale factors. Default value is [1, 1].
*/
/*!
  \var SoSFVec2f SoBumpMapTransform::center

  Center for scale and rotation. Default value is [0, 0].
*/

// *************************************************************************

SO_NODE_SOURCE(SoBumpMapTransform);

/*!
  Constructor.
*/
SoBumpMapTransform::SoBumpMapTransform(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoBumpMapTransform);

  SO_NODE_ADD_FIELD(translation, (0.0f, 0.0f));
  SO_NODE_ADD_FIELD(rotation, (0.0f));
  SO_NODE_ADD_FIELD(scaleFactor, (1.0f, 1.0f));
  SO_NODE_ADD_FIELD(center, (0.0f, 0.0f));
}

/*!
  Destructor.
*/
SoBumpMapTransform::~SoBumpMapTransform()
{
}

// Documented in superclass.
void
SoBumpMapTransform::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoBumpMapTransform, SO_FROM_COIN_2_2);

  SO_ENABLE(SoGLRenderAction, SoBumpMapMatrixElement);
  SO_ENABLE(SoCallbackAction, SoBumpMapMatrixElement);
  SO_ENABLE(SoPickAction, SoBumpMapMatrixElement);
}


// Documented in superclass.
void
SoBumpMapTransform::GLRender(SoGLRenderAction * action)
{
  SoBumpMapTransform::doAction(action);
}

// Documented in superclass.
void
SoBumpMapTransform::doAction(SoAction *action)
{
  SbMatrix mat;
  this->makeMatrix(mat);
  SoBumpMapMatrixElement::mult(action->getState(), this, mat);
}

// Documented in superclass.
void
SoBumpMapTransform::callback(SoCallbackAction *action)
{
  SoBumpMapTransform::doAction(action);
}

// Documented in superclass.
void
SoBumpMapTransform::getMatrix(SoGetMatrixAction * COIN_UNUSED_ARG(action))
{
  // do nothing
}

// Documented in superclass.
void
SoBumpMapTransform::pick(SoPickAction * action)
{
  SoBumpMapTransform::doAction(action);
}

//
// generate a matrix based on the fields
//
void
SoBumpMapTransform::makeMatrix(SbMatrix & mat)
{
  SbMatrix tmp;
  SbVec2f c = this->center.isIgnored() ?
    SbVec2f(0.0f, 0.0f) :
    center.getValue();

  mat.makeIdentity();
  mat[3][0] = -c[0];
  mat[3][1] = -c[1];

  SbVec2f scale = this->scaleFactor.getValue();
  if (!this->scaleFactor.isIgnored() &&
      scale != SbVec2f(1.0f, 1.0f)) {
    tmp.makeIdentity();
    tmp[0][0] = scale[0];
    tmp[1][1] = scale[1];
    mat.multRight(tmp);
  }
  if (!this->rotation.isIgnored() && (this->rotation.getValue() != 0.0f)) {
    float cosa = (float)cos(this->rotation.getValue());
    float sina = (float)sin(this->rotation.getValue());
    tmp.makeIdentity();
    tmp[0][0] = cosa;
    tmp[1][0] = -sina;
    tmp[0][1] = sina;
    tmp[1][1] = cosa;
    mat.multRight(tmp);
  }
  if (!translation.isIgnored()) c+= this->translation.getValue();
  if (c != SbVec2f(0.0f, 0.0f)) {
    tmp.makeIdentity();
    tmp[3][0] = c[0];
    tmp[3][1] = c[1];
    mat.multRight(tmp);
  }
}
