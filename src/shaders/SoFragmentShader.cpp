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

#include <Inventor/nodes/SoFragmentShader.h>

// *************************************************************************

SO_NODE_SOURCE(SoFragmentShader);

// *************************************************************************

void
SoFragmentShader::initClass(void)
{
  SO_NODE_INIT_CLASS(SoFragmentShader, SoShaderObject, "SoShaderObject");
}

SoFragmentShader::SoFragmentShader(void)
{
  SO_NODE_CONSTRUCTOR(SoFragmentShader);
}

SoFragmentShader::~SoFragmentShader()
{  
}

SbBool
SoFragmentShader::isVertexShader(void) const
{
  return FALSE;
}

SbBool
SoFragmentShader::isSupported(SourceType sourceType)
{
  // FIXME: return correct values, based on checking the actual
  // capabilities of the GL driver. (But first investigate whether
  // this is part of the TGS Inventor API -- the function signature is
  // not very well designed, as we really need a guaranteed GL context
  // for this.)  20050120 mortene.
  if (sourceType == ARB_PROGRAM) return TRUE;
  if (sourceType == GLSL_PROGRAM) return TRUE;

  // FIXME: 20041001 martin
#if !defined(SO_CG_SHADER_SUPPORT)
  if (sourceType == CG_PROGRAM) return FALSE;
#endif

  return TRUE;
}

// *************************************************************************
