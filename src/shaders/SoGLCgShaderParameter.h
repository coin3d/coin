#ifndef COIN_SOGLCGSHADER_H
#define COIN_SOGLCGSHADER_H

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

#ifndef COIN_INTERNAL
#error this is a private header file
#endif

// *************************************************************************

#include "SoGLShaderParameter.h"

#include <Inventor/C/glue/cg.h>

class SoGLCgShaderObject;

// *************************************************************************

class SoGLCgShaderParameter : public SoGLShaderParameter
{
public:
  SoGLCgShaderParameter(SoGLCgShaderObject * shader, 
                        const char * name,
                        SoGLShader::ValueType type);
  virtual ~SoGLCgShaderParameter();

  virtual inline SbBool isReferenced();

  virtual void set1f(const cc_glglue * g, const float value, const char * name, const int id);
  virtual void set2f(const cc_glglue * g, const float * value, const char * name, const int id);
  virtual void set3f(const cc_glglue * g, const float * value, const char * name, const int id);
  virtual void set4f(const cc_glglue * g, const float * value, const char * name, const int id);

  virtual SoGLShader::ShaderType shaderType() const;

public:
  void setState(CGGLenum matrix, CGGLenum transform, const char* name);

private:
  CGprogram* cgProgram;
  CGparameter cgParameter;

  SbBool isCorrectType(SoGLShader::ValueType theType);
  static SoGLShader::ValueType getParameterTypeFor(CGtype type);
  SbBool ensureParameter(const char* name, SoGLShader::ValueType theType);
};

#endif /* ! COIN_SOGLCGSHADER_H */
