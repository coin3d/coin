#ifndef COIN_SOGLSLSHADEROBJECT_H
#define COIN_SOGLSLSHADEROBJECT_H

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

#include "glue/glp.h"

class SbName;
class SoState;

// *************************************************************************

class SoGLSLShaderObject : public SoGLShaderObject
{
  friend class SoGLSLShaderParameter;
public:
  SoGLSLShaderObject(const uint32_t cachecontext);
  virtual ~SoGLSLShaderObject();

  virtual SoShader::Type shaderType(void) const;
  virtual SoGLShaderParameter* getNewParameter(void) const;

  virtual SbBool isLoaded(void) const;
  virtual void load(const char * sourceString);
  virtual void unload(void);

  void attach(COIN_GLhandle programHandle);
  void detach(void);
  SbBool isAttached(void) const;

  // source should be the name of the calling function
  static SbBool didOpenGLErrorOccur(const SbString & source);
  static void printInfoLog(const cc_glglue * g, COIN_GLhandle handle, int objType);

  virtual void updateCoinParameter(SoState * state, const SbName & name, SoShaderParameter * param, const int value);

private:
  COIN_GLhandle programHandle;
  COIN_GLhandle shaderHandle;
  SbBool isattached;
  int32_t programid;
};

#endif /* ! COIN_SOGLSLSHADEROBJECT_H */
