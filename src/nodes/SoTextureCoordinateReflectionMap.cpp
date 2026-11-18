/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

/*!
  \class SoTextureCoordinateReflectionMap SoTextureCoordinateReflectionMap.h Inventor/nodes/SoTextureCoordinateReflectionMap.h
  \brief The SoTextureCoordinateReflectionMap class generates 3D reflection texture coordinates.

  \ingroup coin_nodes

  This node is usually used along with a SoTextureCubeMap node. The
  coordinates are the eye-space reflection direction, as in
  GL_REFLECTION_MAP, with a homogeneous coordinate of 1. The incident
  direction runs from the eye to the surface point; normals use the inverse
  transpose of the model-view transformation. For zero normals or a point
  at the eye, CPU generation retains the historical normal-based calculation.

  <b>FILE FORMAT/DEFAULTS:</b>
  \code
    TextureCoordinateReflectionMap {
    }
  \endcode
*/

// *************************************************************************

#include <Inventor/nodes/SoTextureCoordinateReflectionMap.h>
#include "coindefs.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/C/glue/gl.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoGLMultiTextureCoordinateElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoViewingMatrixElement.h>
#include <Inventor/elements/SoTextureUnitElement.h>
#include <Inventor/system/gl.h>

#include "tidbitsp.h"
#include "nodes/SoSubNodeP.h"
#include "nodes/SoTextureCoordinateReflectionP.h"

// *************************************************************************

class SoTextureCoordinateReflectionMapP {
public:
  static SbVec4f * dummy_texcoords;
  static void cleanup_func(void);
};

SbVec4f * SoTextureCoordinateReflectionMapP::dummy_texcoords = NULL;

void
SoTextureCoordinateReflectionMapP::cleanup_func(void)
{
  delete SoTextureCoordinateReflectionMapP::dummy_texcoords;
}

// *************************************************************************

SO_NODE_SOURCE(SoTextureCoordinateReflectionMap);

/*!
  Constructor.
*/
SoTextureCoordinateReflectionMap::SoTextureCoordinateReflectionMap()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoTextureCoordinateReflectionMap);
}

/*!
  Destructor.
*/
SoTextureCoordinateReflectionMap::~SoTextureCoordinateReflectionMap()
{
}

// doc in super
/*!
  \copybrief SoBase::initClass(void)
*/
void
SoTextureCoordinateReflectionMap::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoTextureCoordinateReflectionMap, SO_FROM_INVENTOR_1);

  SoTextureCoordinateReflectionMapP::dummy_texcoords = new SbVec4f(0.0f, 0.0f, 0.0f, 1.0f);
  coin_atexit((coin_atexit_f *)SoTextureCoordinateReflectionMapP::cleanup_func, CC_ATEXIT_NORMAL);
}

// generates texture coordinates for GLRender, callback and pick actions
const SbVec4f &
SoTextureCoordinateReflectionMap::generate(void * userdata,
                                         const SbVec3f & p,
                                         const SbVec3f & n)
{
  double reflection[3];
  coin_texture_reflection(static_cast<SoState *>(userdata), p, n, reflection);

  // The element API returns a reference. Keep result storage local to each
  // traversal thread without changing the public node's layout.
  static thread_local SbVec4f texcoords;
  texcoords.setValue(float(reflection[0]), float(reflection[1]),
                     float(reflection[2]), 1.0f);
  return texcoords;
}

// doc from parent
void
SoTextureCoordinateReflectionMap::doAction(SoAction * action)
{
  SoState * state = action->getState();
  int unit = SoTextureUnitElement::get(state);

  SoMultiTextureCoordinateElement::setFunction(action->getState(), this, unit,
                                               generate,
                                               action->getState());
}

// doc from parent
void
SoTextureCoordinateReflectionMap::GLRender(SoGLRenderAction * action)
{
  SoState * state = action->getState();
  int unit = SoTextureUnitElement::get(state);

  const cc_glglue * glue = cc_glglue_instance(SoGLCacheContextElement::get(state));
  int maxunits = cc_glglue_max_texture_units(glue);
  if (unit < 0 || unit >= maxunits) return;

  SoMultiTextureCoordinateElement::setFunction(action->getState(), this,
                                               unit,
                                               generate,
                                               action->getState());
  SoGLMultiTextureCoordinateElement::setTexGen(action->getState(),
                                               this, unit, handleTexgen, 
                                               action,
                                                 generate,
                                               action->getState());
  
}

// doc from parent
void
SoTextureCoordinateReflectionMap::callback(SoCallbackAction * action)
{
  SoTextureCoordinateReflectionMap::doAction((SoAction *)action);
}

// doc from parent
void
SoTextureCoordinateReflectionMap::pick(SoPickAction * action)
{
  SoTextureCoordinateReflectionMap::doAction((SoAction *)action);
}

void
SoTextureCoordinateReflectionMap::handleTexgen(void * COIN_UNUSED_ARG(data))
{
  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);  
  glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
}
