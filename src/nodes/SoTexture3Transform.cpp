/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoTexture3Transform SoTexture3Transform.h Inventor/nodes/SoTexture3Transform.h
  \brief The SoTexture3Transform class is used to define 3D texture transformations.
  \ingroup nodes

  Textures applied to shapes in the scene can be transformed by
  "prefixing" in the state with instances of this node
  type. Translations, rotations and scaling in 3D can all be done.

  The default settings of this node's fields equals a "null
  transform", ie no transformation.

  \sa SoTexture2Transform
*/

#include <Inventor/nodes/SoTexture3Transform.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/elements/SoGLTextureMatrixElement.h>
#include <Inventor/actions/SoCallbackAction.h>


/*!
  \var SoSFVec3f SoTexture3Transform::translation

  Texture coordinate translation. Default value is [0, 0, 0].
*/
/*!
  \var SoSFRotation SoTexture3Transform::rotation

  Texture coordinate rotation (s is x-axis, t is y-axis and r is
  z-axis).  Defaults to an identity rotation (ie zero rotation).
*/
/*!
  \var SoSFVec3f SoTexture3Transform::scaleFactor

  Texture coordinate scale factors. Default value is [1, 1, 1].
*/
/*!
  \var SoSFRotation SoTexture3Transform::scaleOrientation

  The orientation the texture is set to before scaling.  Defaults to
  an identity rotation (ie zero rotation).
*/
/*!
  \var SoSFVec3f SoTexture3Transform::center

  Center for scale and rotation. Default value is [0, 0, 0].
*/

// *************************************************************************

SO_NODE_SOURCE(SoTexture3Transform);

/*!
  Constructor.
*/
SoTexture3Transform::SoTexture3Transform(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoTexture3Transform);

  SO_NODE_ADD_FIELD(translation, (0.0f, 0.0f, 0.0f));
  SO_NODE_ADD_FIELD(rotation, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_NODE_ADD_FIELD(scaleFactor, (1.0f, 1.0f, 1.0f));
  SO_NODE_ADD_FIELD(scaleOrientation, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_NODE_ADD_FIELD(center, (0.0f, 0.0f, 0.0f));
}

/*!
  Destructor.
*/
SoTexture3Transform::~SoTexture3Transform()
{
}

// Documented in superclass.
void
SoTexture3Transform::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoTexture3Transform, SO_FROM_INVENTOR_1);

  SO_ENABLE(SoGLRenderAction, SoGLTextureMatrixElement);
  SO_ENABLE(SoCallbackAction, SoTextureMatrixElement);
  SO_ENABLE(SoPickAction, SoTextureMatrixElement);
}


// Documented in superclass.
void
SoTexture3Transform::GLRender(SoGLRenderAction * action)
{
  SoTexture3Transform::doAction(action);
}

// Documented in superclass.
void
SoTexture3Transform::doAction(SoAction *action)
{
  SbMatrix mat;
  mat.setTransform(this->translation.getValue(),
                   this->rotation.getValue(),
                   this->scaleFactor.getValue(),
                   this->scaleOrientation.getValue(),
                   this->center.getValue());
  SoTextureMatrixElement::mult(action->getState(), this, mat);
}

// Documented in superclass.
void
SoTexture3Transform::callback(SoCallbackAction *action)
{
  SoTexture3Transform::doAction(action);
}

// Documented in superclass.
void
SoTexture3Transform::getMatrix(SoGetMatrixAction * action)
{
  SbMatrix mat;
  mat.setTransform(this->translation.getValue(),
                   this->rotation.getValue(),
                   this->scaleFactor.getValue(),
                   this->scaleOrientation.getValue(),
                   this->center.getValue());
  action->getTextureMatrix().multLeft(mat);
  action->getTextureInverse().multRight(mat.inverse());
}

// Documented in superclass.
void
SoTexture3Transform::pick(SoPickAction * action)
{
  SoTexture3Transform::doAction(action);
}
