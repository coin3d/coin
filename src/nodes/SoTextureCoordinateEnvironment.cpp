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
  \class SoTextureCoordinateEnvironment SoTextureCoordinateEnvironment.h Inventor/nodes/SoTextureCoordinateEnvironment.h
  \brief The SoTextureCoordinateEnvironment class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoTextureCoordinateEnvironment.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/elements/SoTextureCoordinateElement.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32
#include <GL/gl.h>
#include <math.h>
#include <float.h>

// *************************************************************************

SO_NODE_SOURCE(SoTextureCoordinateEnvironment);

/*!
  Constructor.
*/
SoTextureCoordinateEnvironment::SoTextureCoordinateEnvironment()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoTextureCoordinateEnvironment);
}

/*!
  Destructor.
*/
SoTextureCoordinateEnvironment::~SoTextureCoordinateEnvironment()
{
}

/*!
  Does initialization common for all objects of the
  SoTextureCoordinateEnvironment class. This includes setting up the
  type system, among other things.
*/
void
SoTextureCoordinateEnvironment::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoTextureCoordinateEnvironment);
}

/*!
  FIXME: write function documentation
*/
const SbVec4f &
SoTextureCoordinateEnvironment::generate(void *userdata,
                                         const SbVec3f & /* p */,
                                         const SbVec3f &n)
{
  //
  // from formula in the Red Book
  //

  SoState *state = (SoState*)userdata;
  SbVec3f wn; // normal in world (eye) coordinates
  SoModelMatrixElement::get(state).multDirMatrix(n, wn);
  SbVec3f u = n;

  u.normalize();
  wn.normalize();

  // reflection vector
  SbVec3f r = u - SbVec3f(2.0f*wn[0]*wn[0]*u[0],
                          2.0f*wn[1]*wn[1]*u[1],
                          2.0f*wn[2]*wn[2]*u[2]);
  r.normalize();

  float tmp = 1.0f + r[2];
  float m = 2.0f * (float)sqrt(r[0]*r[0] + r[1]*r[1] + tmp*tmp);

  // in case an empty normal was supplied
  if (fabs(m) <= FLT_EPSILON) m = 1.0f;

  static SbVec4f texcoords(0,0,0,1); // needed to return a reference
  texcoords[0] = r[0] / m + 0.5f;
  texcoords[1] = r[1] / m + 0.5f;
  return texcoords;
}

/*!
  FIXME: write function documentation
*/
void
SoTextureCoordinateEnvironment::doAction(SoAction * action)
{
  SoTextureCoordinateElement::setFunction(action->getState(), this,
                                          generate,
                                          action->getState());
}

/*!
  FIXME: write function documentation
*/
void
SoTextureCoordinateEnvironment::GLRender(SoGLRenderAction * action)
{
  SoTextureCoordinateEnvironment::doAction((SoAction *)action);
  SoGLTextureCoordinateElement::setTexGen(action->getState(),
                                          this, handleTexgen);
}

/*!
  FIXME: write function documentation
*/
void
SoTextureCoordinateEnvironment::callback(SoCallbackAction * action)
{
  SoTextureCoordinateEnvironment::doAction((SoAction *)action);
}

/*!
  FIXME: write function documentation
*/
void
SoTextureCoordinateEnvironment::pick(SoPickAction * action)
{
  SoTextureCoordinateEnvironment::doAction((SoAction *)action);
}

void
SoTextureCoordinateEnvironment::handleTexgen(void * /* data */)
{
#if 0 // from red book
  glTexGenfv(GL_S, GL_SPHERE_MAP, 0);
  glTexGenfv(GL_T, GL_SPHERE_MAP, 0);
  glEnable(GL_TEXTURE_GEN_S);
  glEnable(GL_TEXTURE_GEN_T);
#else // from siggraph 96
  glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
  glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
  glEnable(GL_TEXTURE_GEN_S);
  glEnable(GL_TEXTURE_GEN_T);
#endif
}
