#ifndef COIN_SOGLSLSHADERPARAMETER_H
#define COIN_SOGLSLSHADERPARAMETER_H

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

#include <Inventor/SbString.h>
#include <Inventor/C/glue/glp.h>

#include "SoGLShaderParameter.h"

// *************************************************************************

class SoGLSLShaderParameter : public SoGLShaderParameter
{
public:
  SoGLSLShaderParameter(const cc_glglue * g,
                        COIN_GLhandle program, 
                        const char* name, 
                        SoGLShader::ValueType type);
  virtual ~SoGLSLShaderParameter();

  virtual inline SbBool isTexture(void) { return FALSE; }
  virtual inline SbBool isReferenced(void) { return (this->location >= 0); }

  virtual void set1f(const cc_glglue * g, const float value, const char * name, const int id);
  virtual void set2f(const cc_glglue * g, const float * value, const char * name, const int id);
  virtual void set3f(const cc_glglue * g, const float * value, const char * name, const int id);
  virtual void set4f(const cc_glglue * g, const float * value, const char * name, const int id);

  virtual SoGLShader::ShaderType shaderType(void) const;

private:
  GLint location;
  SbString name;

  static SoGLShader::ValueType getParameterTypeFor(GLenum type);
};

#endif /* ! COIN_SOGLSLSHADERPARAMETER_H */
