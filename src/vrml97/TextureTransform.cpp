/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoVRMLTextureTransform SoVRMLTextureTransform.h Inventor/VRMLnodes/SoVRMLTextureTransform.h
  \brief The SoVRMLTextureTransform class defines a transformation applied to texture coordinates.
*/

/*!
  \var SoSFVec2f SoVRMLTextureTransform::translation
  Translation value. Default values is (0, 0, 0).
*/

/*!
  \var SoSFFloat SoVRMLTextureTransform::rotation
  Rotation, in radians, around the centre points. Default value is 0.
*/

/*!
  \var SoSFVec2f SoVRMLTextureTransform::scale
  Scale vector about the centre point. Default value is (1, 1).
*/

/*!
  \var SoSFVec2f SoVRMLTextureTransform::center
  Texture centre. Default value is (0.0, 0.0).
*/

#include <Inventor/VRMLnodes/SoVRMLTextureTransform.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/elements/SoTextureMatrixElement.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/SbRotation.h>
#include <math.h>

SO_NODE_SOURCE(SoVRMLTextureTransform);

// Doc in parent
void
SoVRMLTextureTransform::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLTextureTransform, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLTextureTransform::SoVRMLTextureTransform(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLTextureTransform);

  SO_VRMLNODE_ADD_EXPOSED_FIELD(translation, (0.0f, 0.0f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(rotation, (0.0f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(scale, (1.0f, 1.0f));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(center, (0.0f, 0.0f));
}

/*!
  Destructor.
*/
SoVRMLTextureTransform::~SoVRMLTextureTransform()
{
}

// Doc in parent
void
SoVRMLTextureTransform::doAction(SoAction * action)
{
  SbMatrix mat;
  this->makeMatrix(mat);
  SoTextureMatrixElement::mult(action->getState(), this,
                               mat);
}

// Doc in parent
void
SoVRMLTextureTransform::callback(SoCallbackAction * action)
{
  SoVRMLTextureTransform::doAction((SoAction*)action);
}

// Doc in parent
void
SoVRMLTextureTransform::GLRender(SoGLRenderAction * action)
{
  SoVRMLTextureTransform::doAction((SoAction*)action);
}

// Doc in parent
void
SoVRMLTextureTransform::getMatrix(SoGetMatrixAction * action)
{
  SbMatrix mat;
  this->makeMatrix(mat);
  action->getTextureMatrix().multLeft(mat);
  action->getTextureInverse().multRight(mat.inverse());
}

// Doc in parent
void
SoVRMLTextureTransform::pick(SoPickAction * action)
{
  SoVRMLTextureTransform::doAction((SoAction*)action);
}

//
// generate a matrix based on the fields
//
void
SoVRMLTextureTransform::makeMatrix(SbMatrix & mat) const
{
  SbMatrix tmp;
  SbVec2f c = this->center.isIgnored() ?
    SbVec2f(0.0f, 0.0f) :
    center.getValue();

  mat.makeIdentity();
  mat[3][0] = -c[0];
  mat[3][1] = -c[1];

  SbVec2f scale = this->scale.getValue();
  if (!this->scale.isIgnored() &&
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
  if (!this->translation.isIgnored()) c+= this->translation.getValue();
  if (c != SbVec2f(0.0f, 0.0f)) {
    tmp.makeIdentity();
    tmp[3][0] = c[0];
    tmp[3][1] = c[1];
    mat.multRight(tmp);
  }
}
