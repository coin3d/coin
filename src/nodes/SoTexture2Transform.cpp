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
  \class SoTexture2Transform SoTexture2Transform.h Inventor/nodes/SoTexture2Transform.h
  \brief The SoTexture2Transform class is used to define 2D texture transformations.
  \ingroup nodes

  Textures applied to shapes in the scene can be transformed by
  "prefixing" in the state with instances of this node
  type. Translations, rotations and scaling in 2D can all be done.

  The default settings of this node's fields equals a "null
  transform", ie no transformation.

  \sa SoTexture3Transform
*/

#include <Inventor/nodes/SoTexture2Transform.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/elements/SoGLTextureMatrixElement.h>
#include <Inventor/actions/SoCallbackAction.h>


/*!
  \var SoSFVec2f SoTexture2Transform::translation

  Texture coordinate translation. Default value is [0, 0].
*/
/*!
  \var SoSFFloat SoTexture2Transform::rotation

  Texture coordinate rotation (around z-axis, s is x-axis and t is
  y-axis).  Defaults to an identity rotation (ie zero rotation).
*/
/*!
  \var SoSFVec2f SoTexture2Transform::scaleFactor

  Texture coordinate scale factors. Default value is [1, 1].
*/
/*!
  \var SoSFVec2f SoTexture2Transform::center

  Center for scale and rotation. Default value is [0, 0].
*/

// *************************************************************************

SO_NODE_SOURCE(SoTexture2Transform);

/*!
  Constructor.
*/
SoTexture2Transform::SoTexture2Transform(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoTexture2Transform);

  SO_NODE_ADD_FIELD(translation, (0.0f, 0.0f));
  SO_NODE_ADD_FIELD(rotation, (0.0f));
  SO_NODE_ADD_FIELD(scaleFactor, (1.0f, 1.0f));
  SO_NODE_ADD_FIELD(center, (0.0f, 0.0f));
}

/*!
  Destructor.
*/
SoTexture2Transform::~SoTexture2Transform()
{
}

// Documented in superclass.
void
SoTexture2Transform::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoTexture2Transform, SO_FROM_INVENTOR_2_1);

  SO_ENABLE(SoGLRenderAction, SoGLTextureMatrixElement);
  SO_ENABLE(SoCallbackAction, SoTextureMatrixElement);
  SO_ENABLE(SoPickAction, SoTextureMatrixElement);
}


// Documented in superclass.
void
SoTexture2Transform::GLRender(SoGLRenderAction * action)
{
  SoTexture2Transform::doAction(action);
}

// Documented in superclass.
void
SoTexture2Transform::doAction(SoAction *action)
{
  SbMatrix mat;
  this->makeMatrix(mat);
  SoTextureMatrixElement::mult(action->getState(), this,
                               mat);
}

// Documented in superclass.
void
SoTexture2Transform::callback(SoCallbackAction *action)
{
  SoTexture2Transform::doAction(action);
}

// Documented in superclass.
void
SoTexture2Transform::getMatrix(SoGetMatrixAction * action)
{
  SbMatrix mat;
  this->makeMatrix(mat);
  action->getTextureMatrix().multLeft(mat);
  action->getTextureInverse().multRight(mat.inverse());
}

// Documented in superclass.
void
SoTexture2Transform::pick(SoPickAction * action)
{
  SoTexture2Transform::doAction(action);
}

//
// generate a matrix based on the fields
//
void
SoTexture2Transform::makeMatrix(SbMatrix & mat)
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
