#ifndef COIN_SOGLSHADEROBJECT_H
#define COIN_SOGLSHADEROBJECT_H

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

#include <Inventor/C/glue/gl.h>
#include <Inventor/SbString.h>
#include <Inventor/nodes/SoShader.h>

class SoGLShaderParameter;

// *************************************************************************

class SoGLShaderObject
{
public:
  SoGLShaderObject(const uint32_t cachecontext);
  virtual ~SoGLShaderObject() { } 

  const cc_glglue * GLContext(void) const;
  uint32_t getCacheContext(void) const;

  virtual SbBool isLoaded(void) const = 0;
  virtual void load(const char * sourceString) = 0;
  virtual void unload(void) = 0;
  virtual SoShader::Type shaderType(void) const = 0;
  virtual SoGLShaderParameter* getNewParameter(void) const = 0;

public:

  enum ShaderType {
    VERTEX,
    FRAGMENT,
    GEOMETRY
  };

  void setShaderType(const ShaderType type);
  ShaderType getShaderType(void) const;

  void setIsActive(SbBool flag);
  SbBool isActive(void) const;
  
  void setParametersDirty(SbBool flag);
  SbBool getParametersDirty(void) const;

#if defined(SOURCE_HINT)
  SbString sourceHint; // either the file name or the first line of source code
#endif

protected:
  const cc_glglue * glctx;
  uint32_t cachecontext;

private:
  ShaderType shadertype;
  SbBool isActiveFlag ;
  SbBool paramsdirty;
};

#endif /* ! COIN_SOGLSHADEROBJECT_H */

