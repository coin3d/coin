#ifndef COIN_SOGLCGSHADEROBJECT_H
#define COIN_SOGLCGSHADEROBJECT_H

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

#ifndef COIN_INTERNAL
#error this is a private header file
#endif

// *************************************************************************

#include "shaders/SoGLShaderObject.h"

#include <Inventor/C/glue/gl.h>

#include "glue/cg.h"

// *************************************************************************

class SoGLCgShaderObject : public SoGLShaderObject
{
public:
  SoGLCgShaderObject(const uint32_t cachecontext);
  virtual ~SoGLCgShaderObject();

  virtual SbBool isLoaded(void) const;
  virtual void load(const char * sourceString);
  virtual void unload(void);
  virtual SoShader::Type shaderType(void) const;
  virtual SoGLShaderParameter* getNewParameter(void) const;
  
  void enable(void);
  void disable(void);

private:
  CGprofile getProfile(void) const;

  CGprogram cgProgram;
  CGprofile cgProfile;

  static CGcontext cgContext;
  static int instanceCount;
  static void ensureCgContext(void);
  static void destroyCgContext(void);
  static void cgErrorCallback(void);
  static void printError(CGerror error, CGcontext context);

  friend class SoGLCgShaderParameter;
  friend class SoGLCgShaderProgram;
};

#endif /* ! COIN_SOGLCGSHADEROBJECT_H */
