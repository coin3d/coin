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
  \class SoTextureCoordinateEnvironment SoTextureCoordinateEnvironment.h Inventor/nodes/SoTextureCoordinateEnvironment.h
  \brief The SoTextureCoordinateEnvironment class generates texture coordinates by projecting onto a surrounding texture.
  \ingroup nodes

  The texture specifying the enviroment will be mapped around the 
  scenegraph below this node using a sphere. The texture will be mapped
  onto the scenegraph taking camera position into account. This will
  lead to an object reflecting its enviroment.

  Here is a scenegraph example showing how enviroment mapping can be
  applied to an object:

  <code>
  #Inventor V2.1 ascii

  Separator {

    Texture2 {
      filename "ocean.jpg" # the enviroment, in this case ocean
    }
    TextureCoordinateEnvironment {}

    Cube {} # the enviromentally mapped object
  }
  </code>

*/

// FIXME: Can this somehow relate to 3D textures? (kintel 20020203)

#include <Inventor/nodes/SoTextureCoordinateEnvironment.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/C/tidbits.h>
#include <stdlib.h>

#ifdef HAVE_CONFIG_H
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
  SO_NODE_INTERNAL_INIT_CLASS(SoTextureCoordinateEnvironment, SO_FROM_INVENTOR_1);

  SoTextureCoordinateEnvironmentP::dummy_texcoords = new SbVec4f(0.0f, 0.0f, 0.0f, 1.0f);
  coin_atexit((coin_atexit_f *)SoTextureCoordinateEnvironmentP::cleanup_func, 0);
}

// generates texture coordinates for GLRender, callback and pick actions
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

  (*SoTextureCoordinateEnvironmentP::dummy_texcoords)[0] = r[0] / m + 0.5f;
  (*SoTextureCoordinateEnvironmentP::dummy_texcoords)[1] = r[1] / m + 0.5f;
  return *SoTextureCoordinateEnvironmentP::dummy_texcoords;
}

// doc from parent
void
SoTextureCoordinateEnvironment::doAction(SoAction * action)
{
  SoTextureCoordinateElement::setFunction(action->getState(), this,
                                          generate,
                                          action->getState());
}

// doc from parent
void
SoTextureCoordinateEnvironment::GLRender(SoGLRenderAction * action)
{
  SoTextureCoordinateEnvironment::doAction((SoAction *)action);
  SoGLTextureCoordinateElement::setTexGen(action->getState(),
                                          this, handleTexgen);
}

// doc from parent
void
SoTextureCoordinateEnvironment::callback(SoCallbackAction * action)
{
  SoTextureCoordinateEnvironment::doAction((SoAction *)action);
}

// doc from parent
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
