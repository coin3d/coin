#ifndef COIN_SOGLSHADERPROGRAM_H
#define COIN_SOGLSHADERPROGRAM_H

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

#include <Inventor/SbString.h>
#include <Inventor/C/glue/gl.h>
#include <Inventor/nodes/SoShaderProgram.h>
#include <Inventor/lists/SbList.h>

class SoGLShaderObject;
class SoState;
class SbName;

// *************************************************************************

class SoGLShaderProgram
{
public:
  SoGLShaderProgram(void);
  ~SoGLShaderProgram();
  void addShaderObject(SoGLShaderObject * shaderObject);
  void removeShaderObjects(void);
  void enable(SoState * state);
  void disable(SoState * state);
  SbBool isEnabled(void) const;

  void setEnableCallback(SoShaderProgramEnableCB * cb,
                         void * closure);


#if defined(SOURCE_HINT) // FIXME: what's this? 20050120 mortene.
  SbString getSourceHint(void);
#endif

  void updateCoinParameter(SoState * state, const SbName & name, const int value);
  void addProgramParameter(int name, int value);

  void getShaderObjectIds(SbList <uint32_t> & ids) const;
  uint32_t getGLSLShaderProgramHandle(SoState * state) const;
  SbBool glslShaderProgramLinked(void) const;
private:

  class SoGLARBShaderProgram * arbShaderProgram;
  class SoGLCgShaderProgram  * cgShaderProgram;
  class SoGLSLShaderProgram  * glslShaderProgram;

  SbBool isenabled;
  SoShaderProgramEnableCB * enablecb;
  void * enablecbclosure;
  SbList <uint32_t> objectids;
};

#endif /* ! COIN_SOGLSHADERPROGRAM_H */
