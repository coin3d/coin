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
  \class SoTextureCoordinateEnvironment SoTextureCoordinateEnvironment.h Inventor/nodes/SoTextureCoordinateEnvironment.h
  \brief The SoTextureCoordinateEnvironment class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoTextureCoordinateEnvironment.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <../tidbits.h> // coin_atexit()
#include <stdlib.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#include <Inventor/system/gl.h>
#include <float.h>

// *************************************************************************

class SoTextureCoordinateEnvironmentP {
public:
  static SbVec4f * dummy_texcoords;
  static void cleanup_func(void);
};

SbVec4f * SoTextureCoordinateEnvironmentP::dummy_texcoords = NULL;

void
SoTextureCoordinateEnvironmentP::cleanup_func(void)
{
  delete SoTextureCoordinateEnvironmentP::dummy_texcoords;
}

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

// doc in super
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

  if (SoTextureCoordinateEnvironmentP::dummy_texcoords == NULL) {
    SoTextureCoordinateEnvironmentP::dummy_texcoords =
      new SbVec4f(0.0f, 0.0f, 0.0f, 1.0f);
    coin_atexit((coin_atexit_f *)SoTextureCoordinateEnvironmentP::cleanup_func);
  }
  (*SoTextureCoordinateEnvironmentP::dummy_texcoords)[0] = r[0] / m + 0.5f;
  (*SoTextureCoordinateEnvironmentP::dummy_texcoords)[1] = r[1] / m + 0.5f;
  return *SoTextureCoordinateEnvironmentP::dummy_texcoords;
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
