/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2007 by Systems in Motion.  All rights reserved.
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
 *  See http://www.coin3d.org/ for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include "SoShader.h"

#include <Inventor/nodes/SoShaderProgram.h>
#include <Inventor/nodes/SoShaderObject.h>
#include <Inventor/nodes/SoFragmentShader.h>
#include <Inventor/nodes/SoVertexShader.h>
#include <Inventor/nodes/SoGeometryShader.h>
#include <Inventor/nodes/SoShaderParameter.h>
#include <Inventor/elements/SoGLShaderProgramElement.h>
#include <Inventor/C/glue/cg.h>
#include <Inventor/C/tidbits.h>
#include <Inventor/misc/SbHash.h>
#include <Inventor/errors/SoDebugError.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// *************************************************************************

static const char * SO_SHADER_DIR = NULL;
static SbHash <char *, const char *> * shader_dict = NULL;

static void
soshader_cleanup_callback(const char * const & key,
                          char * const & obj,
                          void * closure)
{
  delete[] obj;
}

static void 
soshader_cleanup(void)
{
  shader_dict->apply(soshader_cleanup_callback, NULL);
  delete shader_dict;
}

void
SoShader::init(void)
{
  // Trigger loading and init of Cg library glue.
  //
  // FIXME: this function should rather be used from the relevant
  // class(es), so it is loaded only on demand. 20050125 mortene.
  (void)cc_cgglue_available();

  // --- initialization of elements (must be done first) ---------------
  if (SoGLShaderProgramElement::getClassTypeId() == SoType::badType())
    SoGLShaderProgramElement::initClass();

  // --- initialization of shader nodes --------------------------------
  if (SoShaderProgram::getClassTypeId() == SoType::badType())
    SoShaderProgram::initClass();  
  if (SoShaderObject::getClassTypeId() == SoType::badType())
    SoShaderObject::initClass();
  if (SoFragmentShader::getClassTypeId() == SoType::badType())
    SoFragmentShader::initClass();
  if (SoVertexShader::getClassTypeId() == SoType::badType())
    SoVertexShader::initClass();
  if (SoGeometryShader::getClassTypeId() == SoType::badType())
    SoGeometryShader::initClass();

  // --- initialization of parameter nodes -----------------------------
  if (SoShaderParameter::getClassTypeId() == SoType::badType())
    SoShaderParameter::initClass();
  if (SoUniformShaderParameter::getClassTypeId() == SoType::badType())
    SoUniformShaderParameter::initClass();

  // float vector parameter nodes
  if (SoShaderParameter1f::getClassTypeId() == SoType::badType())
    SoShaderParameter1f::initClass();
  if (SoShaderParameter2f::getClassTypeId() == SoType::badType())
    SoShaderParameter2f::initClass();
  if (SoShaderParameter3f::getClassTypeId() == SoType::badType())
    SoShaderParameter3f::initClass();
  if (SoShaderParameter4f::getClassTypeId() == SoType::badType())
    SoShaderParameter4f::initClass();

  // float vector array parameter nodes
  if (SoShaderParameterArray1f::getClassTypeId() == SoType::badType())
    SoShaderParameterArray1f::initClass();
  if (SoShaderParameterArray2f::getClassTypeId() == SoType::badType())
    SoShaderParameterArray2f::initClass();
  if (SoShaderParameterArray3f::getClassTypeId() == SoType::badType())
    SoShaderParameterArray3f::initClass();
  if (SoShaderParameterArray4f::getClassTypeId() == SoType::badType())
    SoShaderParameterArray4f::initClass();

  // matrix parameter nodes
  if (SoShaderStateMatrixParameter::getClassTypeId() == SoType::badType())
    SoShaderStateMatrixParameter::initClass();
  if (SoShaderParameterMatrix::getClassTypeId() == SoType::badType())
    SoShaderParameterMatrix::initClass();
  if (SoShaderParameterMatrixArray::getClassTypeId() == SoType::badType())
    SoShaderParameterMatrixArray::initClass();

  // int32 support
  if (SoShaderParameter1i::getClassTypeId() == SoType::badType())
    SoShaderParameter1i::initClass();

  // FIXME: Do we need int32 support (like in TGS)? 20040924 martin 
#if 0 
  if (SoShaderParameter2i::getClassTypeId() == SoType::badType())
    SoShaderParameter2i::initClass();
  if (SoShaderParameter3i::getClassTypeId() == SoType::badType())
    SoShaderParameter3i::initClass();
  if (SoShaderParameter4i::getClassTypeId() == SoType::badType())
    SoShaderParameter4i::initClass();
  if (SoShaderParameterArray1i::getClassTypeId() == SoType::badType())
    SoShaderParameterArray1i::initClass();
  if (SoShaderParameterArray2i::getClassTypeId() == SoType::badType())
    SoShaderParameterArray2i::initClass();
  if (SoShaderParameterArray3i::getClassTypeId() == SoType::badType())
    SoShaderParameterArray3i::initClass();
  if (SoShaderParameterArray4i::getClassTypeId() == SoType::badType())
    SoShaderParameterArray4i::initClass();
#endif

  SO_SHADER_DIR = coin_getenv("SO_SHADER_DIR");
  shader_dict = new SbHash <char *, const char *>;

  coin_atexit((coin_atexit_f*) soshader_cleanup, CC_ATEXIT_NORMAL);
}

const char * 
SoShader::getNamedScript(const SbName & name, const Type type)
{
  // FIXME: temporary until we can make compiled-in versions of the shaders
  if (SO_SHADER_DIR == NULL) {
    SoDebugError::postWarning("SoShader::getNamedScript",
                              "SO_SHADER_DIR not set");
    return NULL;
  }
  
  SbString filename(SO_SHADER_DIR);
  filename += "/";
  filename += name.getString();

  switch (type) {
  case ARB_SHADER:
    filename += ".arb";
    break;
  case CG_SHADER:
    filename += ".cg";
    break;
  case GLSL_SHADER:
    filename += ".glsl";
    break;
  default:
    assert(0 && "unknown shader type");
    break;
  }

  SbName shadername(filename.getString());
  char * shader = NULL;
  
  if (!shader_dict->get(shadername.getString(), shader)) {
    FILE * fp = fopen(filename.getString(), "rb");
    if (fp) {
      (void) fseek(fp, 0, SEEK_END);
      size_t size = (size_t) ftell(fp);
      (void) fseek(fp, 0, SEEK_SET);

      shader = new char[size+1];
      shader[size] = 0;
      shader_dict->put(shadername, shader);
      
      if (!fread(shader, size, 1, fp) == 1) {
        SoDebugError::postWarning("SoShader::getNamedScript",
                                  "Unable to read shader: %s",
                                  filename.getString());
      }
      fclose(fp);
    }
    else {
      shader_dict->put(shadername, NULL);
      SoDebugError::postWarning("SoShader::getNamedScript",
                                "Unable to find shader: %s",
                                filename.getString());
    }
  }
  return shader;
}
