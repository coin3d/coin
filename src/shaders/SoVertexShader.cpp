/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoVertexShader SoVertexShader.h Inventor/nodes/SoVertexShader.h
  \brief The SoVertexShader class is used for setting up vertex shader programs.
  \ingroup nodes

  See \link coin_shaders Shaders in Coin \endlink for more information
  on how to set up a scene graph with shaders.

  <b>FILE FORMAT/DEFAULTS:</b>
  \code
    VertexShader {
      isActive TRUE
      sourceType FILENAME
      sourceProgram ""
      parameter []
    }
  \endcode

  \sa SoShaderObject
  \sa SoShaderProgram
  \since Coin 2.5
*/

#include <Inventor/nodes/SoVertexShader.h>
#include <Inventor/misc/SoGLDriverDatabase.h>

#include "nodes/SoSubNodeP.h"
#include "glue/glp.h"

// *************************************************************************

SO_NODE_SOURCE(SoVertexShader);

// *************************************************************************

// doc from parent
void
SoVertexShader::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVertexShader,
                              SO_FROM_COIN_2_5|SO_FROM_INVENTOR_5_0);
}

/*!
  Constructor.
 */
SoVertexShader::SoVertexShader(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVertexShader);
}

/*!
  Destructor.
*/
SoVertexShader::~SoVertexShader()
{
}

/*!
  Returns a boolean indicating whether the requested source type is
  supported by the OpenGL driver or not.

  <i>Beware:</i> To get a correct answer, a valid OpenGL context must
  be available.
*/
SbBool
SoVertexShader::isSupported(SourceType sourceType)
{
  // The function signature is not very well designed, as we really
  // need a guaranteed GL context for this. (We've chosen to be
  // compatible with TGS Inventor, so don't change the signature.)

  void * ptr = coin_gl_current_context();
  assert(ptr && "No active OpenGL context found!");
  if (!ptr) return FALSE; // Always bail out. Even when compiled in 'release' mode.

  const cc_glglue * glue = cc_glglue_instance_from_context_ptr(ptr);

  if (sourceType == ARB_PROGRAM) {
    return SoGLDriverDatabase::isSupported(glue, SO_GL_ARB_VERTEX_PROGRAM);
  }
  else if (sourceType == GLSL_PROGRAM) {
    return SoGLDriverDatabase::isSupported(glue, SO_GL_ARB_SHADER_OBJECT);
  }
  // FIXME: Add support for detecting missing Cg support
  // (20050427 handegar)
  else if (sourceType == CG_PROGRAM) return TRUE;

  return FALSE;
}

#ifdef COIN_TEST_SUITE

BOOST_AUTO_TEST_CASE(initialized)
{
  SoVertexShader * node = new SoVertexShader;
  assert(node);
  node->ref();
  BOOST_CHECK_MESSAGE(node->getTypeId() != SoType::badType(),
                      "missing class initialization");
  node->unref();
}

#endif // COIN_TEST_SUITE
