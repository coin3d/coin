#ifndef COIN_SOGLSLSHADERPROGRAM_H
#define COIN_SOGLSLSHADERPROGRAM_H

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

#include <Inventor/lists/SbList.h>

#include "misc/SbHash.h"
#include "glue/glp.h"

class SoGLSLShaderObject;
class SoState;
class SbName;

// *************************************************************************

class SoGLSLShaderProgram
{
public:
  void addShaderObject(SoGLSLShaderObject *shaderObject);
  void removeShaderObjects(void);
  void enable(const cc_glglue * g);
  void disable(const cc_glglue * g);
  void postShouldLink(void);

  void updateCoinParameter(SoState * state, const SbName & name, const int value);
  void addProgramParameter(int mode, int value);
  void removeProgramParameters(void);

#if defined(SOURCE_HINT)
  SbString getSourceHint(void) const;
#endif

public:
  SoGLSLShaderProgram(void);
  ~SoGLSLShaderProgram();

  COIN_GLhandle getProgramHandle(const cc_glglue * g, const SbBool create = FALSE);
  SbBool neededLinking(void) const;

protected:
  SbList <int> programParameters;
  SbList <SoGLSLShaderObject *> shaderObjects;
  SbHash<uint32_t, COIN_GLhandle> programHandles;

  SbBool isExecutable;
  SbBool neededlinking;

  int indexOfShaderObject(SoGLSLShaderObject * shaderObject);
  void ensureLinking(const cc_glglue * g);
  void ensureProgramHandle(const cc_glglue * g);

private:
  void deletePrograms(void);
  void deleteProgram(const cc_glglue * g);

  static void context_destruction_cb(uint32_t cachecontext, void * userdata);
  static void really_delete_object(void * closure, uint32_t contextid);

};

#endif /* ! COIN_SOGLSLSHADERPROGRAM_H */
