/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoVRMLTextureTransform SoVRMLTextureTransform.h Inventor/VRMLnodes/SoVRMLTextureTransform.h
  \brief The SoVRMLTextureTransform class defines a transformation applied to texture coordinates.
  \ingroup VRMLnodes

  \WEB3DCOPYRIGHT

  \verbatim
  TextureTransform {
    exposedField SFVec2f center      0 0     # (-inf, inf)
    exposedField SFFloat rotation    0       # (-inf, inf)
    exposedField SFVec2f scale       1 1     # (-inf, inf)
    exposedField SFVec2f translation 0 0     # (-inf, inf)
  }
  \endverbatim

  The TextureTransform node defines a 2D transformation that is
  applied to texture coordinates (see SoVRMLTextureCoordinate).  This
  node affects the way textures coordinates are applied to the
  geometric surface. The transformation consists of (in order):

  - a translation;
  - a rotation about the centre point;
  - a non-uniform scale about the centre point.

  These parameters support changes to the size, orientation, and
  position of textures on shapes. Note that these operations appear
  reversed when viewed on the surface of geometry. For example, a
  scale value of (2 2) will scale the texture coordinates and have the
  net effect of shrinking the texture size by a factor of 2 (texture
  coordinates are twice as large and thus cause the texture to
  repeat). A translation of (0.5 0.0) translates the texture
  coordinates +.5 units along the S-axis and has the net effect of
  translating the texture -0.5 along the S-axis on the geometry's
  surface.  A rotation of pi/2 of the texture coordinates results in a
  -pi/2 rotation of the texture on the geometry.

  The \e center field specifies a translation offset in texture
  coordinate space about which the rotation and scale fields are
  applied.  The \e scale field specifies a scaling factor in S and T
  of the texture coordinates about the center point. scale values
  shall be in the range (-pi, pi). The \e rotation field specifies a
  rotation in radians of the texture coordinates about the center
  point after the scale has been applied. A positive rotation value
  makes the texture coordinates rotate counterclockwise about the
  centre, thereby rotating the appearance of the texture itself
  clockwise. The \e translation field specifies a translation of the
  texture coordinates.  

  In matrix transformation notation, where Tc is the untransformed
  texture coordinate, Tc' is the transformed texture coordinate, C
  (center), T (translation), R (rotation), and S (scale) are the
  intermediate transformation matrices, 
  
  \verbatim
  Tc' = -C × S × R × C × T × Tc
  \endverbatim

  Note that this transformation order is the reverse of the Transform
  node transformation order since the texture coordinates, not the
  texture, are being transformed (i.e., the texture coordinate
  system).

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
  SO_VRMLNODE_INTERNAL_CONSTRUCTOR(SoVRMLTextureTransform);

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
