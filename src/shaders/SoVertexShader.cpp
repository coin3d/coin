/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

/*!
  \class SoVertexProgram SoVertexProgram.h Inventor/nodes/SoVertexProgram.h
  \brief The SoVertexProgram class is used for loading vertex shader programs.
  \ingroup nodes
*/

#include <Inventor/nodes/SoVertexShader.h>

#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/C/glue/glp.h>

// *************************************************************************

SO_NODE_SOURCE(SoVertexShader);

// *************************************************************************

void
SoVertexShader::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVertexShader,
                              SO_FROM_COIN_2_4|SO_FROM_INVENTOR_5_0);
}

SoVertexShader::SoVertexShader(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVertexShader);
}

SoVertexShader::~SoVertexShader()
{
}

SbBool
SoVertexShader::isVertexShader(void) const
{
  return TRUE;
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
  // FIXME: return correct values, based on checking the actual
  // capabilities of the GL driver. (Yes, this is part of the TGS
  // Inventor API -- the function signature is not very well designed,
  // as we really need a guaranteed GL context for this.)  20050120 mortene.

  void * ptr = coin_gl_current_context();
  assert(ptr && "No active OpenGL context found! Do not call");

  if (!ptr) return FALSE; // Always bail out. Even when compiled in 'release' mode.
  
  const cc_glglue * glue = cc_glglue_instance_from_context_ptr(ptr);
  
  if (sourceType == ARB_PROGRAM) {
    return TRUE && cc_glglue_has_arb_vertex_program(glue);
  }

  // FIXME: Add support for detecting missing GLSL and Cg support
  // aswell. (20050427 handegar)
  else if (sourceType == GLSL_PROGRAM) return TRUE;
  else if (sourceType == CG_PROGRAM) return TRUE;

  return FALSE;
}
