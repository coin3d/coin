/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2007 by Systems in Motion.  All rights reserved.
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
 *  See http://www.coin3d.org/ for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoGeometryProgram SoGeometryProgram.h Inventor/nodes/SoGeometryProgram.h
  \brief The SoGeometryProgram class is used for loading geometry shader programs.
  \ingroup nodes
*/

#include <Inventor/nodes/SoGeometryShader.h>

#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/C/glue/glp.h>

// *************************************************************************

SO_NODE_SOURCE(SoGeometryShader);

// *************************************************************************

void
SoGeometryShader::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoGeometryShader,
                              SO_FROM_COIN_2_5);
}

SoGeometryShader::SoGeometryShader(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoGeometryShader);
}

SoGeometryShader::~SoGeometryShader()
{  
}

// *************************************************************************

/*!
  Returns a boolean indicating whether the requested source type is
  supported by the OpenGL driver or not. 

  <i>Beware:</i> To get a correct answer, a valid OpenGL context must
  be available.
*/
SbBool
SoGeometryShader::isSupported(SourceType sourceType)
{
  // The function signature is not very well designed, as we really
  // need a guaranteed GL context for this. (We've chosen to be
  // compatible with TGS Inventor, so don't change the signature.)
  
  void * ptr = coin_gl_current_context();
  assert(ptr && "No active OpenGL context found!");
  if (!ptr) return FALSE; // Always bail out. Even when compiled in 'release' mode.
  
  const cc_glglue * glue = cc_glglue_instance_from_context_ptr(ptr);
  
  if (sourceType == ARB_PROGRAM) {
    return FALSE;
  }
  else if (sourceType == GLSL_PROGRAM) {    
    // FIXME: Maybe we should check for OpenGL 2.0 aswell? (20050428
    // handegar)
    return cc_glglue_has_arb_shader_objects(glue);
  } 
  // AFAIK Cg has no support for geometry shaders (yet).
  // pederb, 20070410
  else if (sourceType == CG_PROGRAM) return FALSE;

  return FALSE;
}

// *************************************************************************
