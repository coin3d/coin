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
  \class SoTextureCoordinatePlane SoTextureCoordinatePlane.h Inventor/nodes/SoTextureCoordinatePlane.h
  \brief The SoTextureCoordinatePlane class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoTextureCoordinatePlane.h>
#include <Inventor/elements/SoTextureCoordinateElement.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#ifdef _WIN32
#include <windows.h>
#endif // _WIN32
#include <GL/gl.h>
#include <coindefs.h> // COIN_STUB()

/*!
  \var SoSFVec3f SoTextureCoordinatePlane::directionS
  FIXME: write documentation for field
*/
/*!
  \var SoSFVec3f SoTextureCoordinatePlane::directionT
  FIXME: write documentation for field
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

/*!
  Does initialization common for all objects of the
  SoTextureCoordinatePlane class. This includes setting up the
  type system, among other things.
*/
void
SoTextureCoordinatePlane::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoTextureCoordinatePlane);
}

/*!
  FIXME: write function documentation
*/
const SbVec4f &
SoTextureCoordinatePlane::generate(void * /* userdata */,
                                   const SbVec3f & /* p */,
                                   const SbVec3f & /* n */)
{
  COIN_STUB();
  static SbVec4f s(0,0,0,1);
  return s;
}

/*!
  FIXME: write function documentation
*/
void
SoTextureCoordinatePlane::doAction(SoAction * action)
{
  SoTextureCoordinateElement::setFunction(action->getState(), this,
                                          generate,
                                          this);
}

/*!
  FIXME: write function documentation
*/
void
SoTextureCoordinatePlane::GLRender(SoGLRenderAction * action)
{
  SoTextureCoordinatePlane::doAction((SoAction *)action);
  SoGLTextureCoordinateElement::setTexGen(action->getState(),
                                          this, handleTexgen, this);
}

/*!
  FIXME: write function documentation
*/
void
SoTextureCoordinatePlane::callback(SoCallbackAction * action)
{
  SoTextureCoordinatePlane::doAction((SoAction *)action);
}

/*!
  FIXME: write function documentation
*/
void
SoTextureCoordinatePlane::pick(SoPickAction * action)
{
  SoTextureCoordinatePlane::doAction((SoAction *)action);
}

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
