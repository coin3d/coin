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

#include <Inventor/nodes/SoVertexShader.h>

// *************************************************************************

SO_NODE_SOURCE(SoVertexShader);

// *************************************************************************

void SoVertexShader::initClass()
{
  SO_NODE_INIT_CLASS(SoVertexShader, SoShaderObject, "SoShaderObject");
}

SoVertexShader::SoVertexShader()
{
  SO_NODE_CONSTRUCTOR(SoVertexShader);
}

SoVertexShader::~SoVertexShader()
{
  
}

SbBool SoVertexShader::isVertexShader() const
{
  return TRUE;
}

SbBool SoVertexShader::isSupported(SourceType
#if !defined(SO_ARB_SHADER_SUPPORT) || !defined(SO_CG_SHADER_SUPPORT)  || !defined(SO_GLSL_SHADER_SUPPORT)
 sourceType
#endif
)
{
  // FIXME: Martin
#if !defined(SO_ARB_SHADER_SUPPORT)
  if (sourceType == ARB_PROGRAM) return FALSE;
#endif
#if !defined(SO_CG_SHADER_SUPPORT)
  if (sourceType == CG_PROGRAM) return FALSE;
#endif
#if !defined(SO_GLSL_SHADER_SUPPORT)
  if (sourceType == GLSL_PROGRAM) return FALSE;
#endif
  return TRUE;
}

