#ifndef COIN_SOGLSLSHADEROBJECT_H
#define COIN_SOGLSLSHADEROBJECT_H

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

#include <Inventor/C/glue/glp.h>

#include "SoGLShaderObject.h"

// *************************************************************************

class SoGLSLShaderObject : public SoGLShaderObject
{
public:
  SoGLSLShaderObject(const cc_glglue * g);
  virtual ~SoGLSLShaderObject();

  virtual SoGLShader::ShaderType shaderType(void) const;

  virtual SoGLShaderParameter * getParameter(int index, const char * name,
                                             SoGLShader::ValueType type);

  virtual SbBool isLoaded(void) const;
  virtual void load(const char * sourceString);
  virtual void unload(void);

  void attach(COIN_GLhandle programHandle);
  void detach(void);

  // objType 0: program 1: vertexShader 2: fragmentShader
  static SbBool didOpenGLErrorOccur(int objType);
  static void printInfoLog(const cc_glglue * g, COIN_GLhandle handle, int objType);

private:
  COIN_GLhandle programHandle;
  COIN_GLhandle shaderHandle;
};

#endif /* ! COIN_SOGLSLSHADEROBJECT_H */
