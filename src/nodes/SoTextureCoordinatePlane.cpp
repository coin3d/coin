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
  \class SoTextureCoordinatePlane SoTextureCoordinatePlane.h Inventor/nodes/SoTextureCoordinatePlane.h
  \brief The SoTextureCoordinatePlane class generates texture coordinates by projecting onto a plane.
  \ingroup nodes

  The plane is specified using two direction vectors: directionS
  and directionT. The S and T texture coordinates are computed as the
  distance from the origin to the projected point, in the respective
  directions.
*/

#include <Inventor/nodes/SoTextureCoordinatePlane.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#include <Inventor/system/gl.h>

/*!
  \var SoSFVec3f SoTextureCoordinatePlane::directionS
  The S texture coordinate plane direction.
*/
/*!
  \var SoSFVec3f SoTextureCoordinatePlane::directionT
  The T texture coordinate plane direction.
*/

// *************************************************************************

SO_NODE_SOURCE(SoTextureCoordinatePlane);

/*!
  Constructor.
*/
SoTextureCoordinatePlane::SoTextureCoordinatePlane()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoTextureCoordinatePlane);

  SO_NODE_ADD_FIELD(directionS, (1.0f, 0.0f, 0.0f));
  SO_NODE_ADD_FIELD(directionT, (0.0f, 1.0f, 0.0f));
}

/*!
  Destructor.
*/
SoTextureCoordinatePlane::~SoTextureCoordinatePlane()
{
}

// doc from parent
void
SoTextureCoordinatePlane::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoTextureCoordinatePlane, SO_FROM_INVENTOR_2_0);
}

// generates texture coordinates for callback and raypick action
const SbVec4f &
SoTextureCoordinatePlane::generate(void * userdata,
                                   const SbVec3f &p,
                                   const SbVec3f & /* n */)
{
  SoTextureCoordinatePlane *thisp =
    (SoTextureCoordinatePlane*) userdata;

  thisp->gencache.ret.setValue(thisp->gencache.s.dot(p) * thisp->gencache.mul_s,
                               thisp->gencache.t.dot(p) * thisp->gencache.mul_t,
                               0.0f, 1.0f);
  return thisp->gencache.ret;
}

// doc from parent
void
SoTextureCoordinatePlane::doAction(SoAction * action)
{
  SoTextureCoordinateElement::setFunction(action->getState(), this,
                                          SoTextureCoordinatePlane::generate,
                                          this);
}

// doc from parent
void
SoTextureCoordinatePlane::GLRender(SoGLRenderAction * action)
{
  SoTextureCoordinatePlane::doAction((SoAction *)action);

  this->gencache.s = this->directionS.getValue();
  this->gencache.t = this->directionT.getValue();
  float lens = this->gencache.s.length();
  float lent = this->gencache.t.length();
  this->gencache.mul_s = 1.0f / lens;
  this->gencache.mul_t = 1.0f / lent;
  this->gencache.s /= lens;
  this->gencache.t /= lent;

  SoGLTextureCoordinateElement::setTexGen(action->getState(),
                                          this,
                                          SoTextureCoordinatePlane::handleTexgen,
                                          this);
}

// doc from parent
void
SoTextureCoordinatePlane::callback(SoCallbackAction * action)
{
  SoTextureCoordinatePlane::doAction((SoAction *)action);
}

// doc from parent
void
SoTextureCoordinatePlane::pick(SoPickAction * action)
{
  SoTextureCoordinatePlane::doAction((SoAction *)action);
}

// texgen callback. Turns on plane texgen in OpenGL
void
SoTextureCoordinatePlane::handleTexgen(void *data)
{
  SoTextureCoordinatePlane *thisp = (SoTextureCoordinatePlane*)data;
  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

  float plane[4];
  plane[3] = 0.0f;
  const SbVec3f &s = thisp->directionS.getValue();
  plane[0] = s[0];
  plane[1] = s[1];
  plane[2] = s[2];
  glTexGenfv(GL_S, GL_OBJECT_PLANE, plane);
  const SbVec3f &t = thisp->directionT.getValue();
  plane[0] = t[0];
  plane[1] = t[1];
  plane[2] = t[2];
  glTexGenfv(GL_T, GL_OBJECT_PLANE, plane);

  glEnable(GL_TEXTURE_GEN_S);
  glEnable(GL_TEXTURE_GEN_T);
}
