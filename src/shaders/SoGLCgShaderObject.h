#ifndef COIN_SOGLCGSHADEROBJECT_H
#define COIN_SOGLCGSHADEROBJECT_H

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

#include "SoGLShaderObject.h"

#include <Inventor/C/glue/gl.h>
#include <Inventor/C/glue/cg.h>

// *************************************************************************

class SoGLCgShaderObject : public SoGLShaderObject
{
public:
  SoGLCgShaderObject(const cc_glglue * g);
  virtual ~SoGLCgShaderObject();

  virtual SbBool isLoaded(void) const;
  virtual void load(const char * sourceString);
  virtual void unload(void);
  virtual SoShaders::ShaderType shaderType(void) const;
  virtual SoGLShaderParameter * getParameter(int index, const char* name, SoShaders::ValueType type);
 
private:
  CGprofile getProfile(void) const;

  virtual void enable(void);
  virtual void disable(void);

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
