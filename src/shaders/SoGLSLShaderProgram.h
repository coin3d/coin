#ifndef COIN_SOGLSLSHADERPROGRAM_H
#define COIN_SOGLSLSHADERPROGRAM_H

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

#include <Inventor/lists/SbList.h>
#include <Inventor/C/glue/glp.h>

class SoGLSLShaderObject;

// *************************************************************************

class SoGLSLShaderProgram
{
public:
  void addShaderObject(SoGLSLShaderObject *shaderObject);
  void removeShaderObject(SoGLSLShaderObject *shaderObject);
  void enable(const cc_glglue * g);
  void disable(const cc_glglue * g);
  void postShouldLink(void);

#if defined(SOURCE_HINT)
  SbString getSourceHint(void) const;
#endif

public:
  SoGLSLShaderProgram(void);
  ~SoGLSLShaderProgram();

protected:
  SbList<SoGLSLShaderObject *> shaderObjects;
  COIN_GLhandle programHandle;
  SbBool shouldLink;
  SbBool isExecutable;

  int indexOfShaderObject(SoGLSLShaderObject *shaderObject);
  void ensureLinking(const cc_glglue * g);
  void ensureProgramHandle(const cc_glglue * g);
};

#endif /* ! COIN_SOGLSLSHADERPROGRAM_H */
