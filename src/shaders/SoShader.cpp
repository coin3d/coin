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

#include <Inventor/nodes/SoShader.h>

#include <Inventor/nodes/SoShaderProgram.h>
#include <Inventor/nodes/SoShaderObject.h>
#include <Inventor/nodes/SoFragmentShader.h>
#include <Inventor/nodes/SoVertexShader.h>
#include <Inventor/nodes/SoShaderParameter.h>
#include <Inventor/elements/SoGLShaderProgramElement.h>
#include <Inventor/C/glue/cg.h>

// *************************************************************************

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

  // --- initialization of parameter nodes -----------------------------
  if (SoShaderParameter::getClassTypeId() == SoType::badType())
    SoShaderParameter::initClass();
  if (SoUniformShaderParameter::getClassTypeId() == SoType::badType())
    SoUniformShaderParameter::initClass();
  if (SoShaderParameter1f::getClassTypeId() == SoType::badType())
    SoShaderParameter1f::initClass();
  if (SoShaderParameter2f::getClassTypeId() == SoType::badType())
    SoShaderParameter2f::initClass();
  if (SoShaderParameter3f::getClassTypeId() == SoType::badType())
    SoShaderParameter3f::initClass();
  if (SoShaderParameter4f::getClassTypeId() == SoType::badType())
    SoShaderParameter4f::initClass();

  if (SoShaderStateMatrixParameter::getClassTypeId() == SoType::badType())
    SoShaderStateMatrixParameter::initClass();

  // FIXME: array parameters are not implemented yet 20040924 martin
#if 0 // --- array parameter support ------------------------------------
  if (SoShaderParameterArray1f::getClassTypeId() == SoType::badType())
    SoShaderParameterArray1f::initClass();
  if (SoShaderParameterArray2f::getClassTypeId() == SoType::badType())
    SoShaderParameterArray2f::initClass();
  if (SoShaderParameterArray3f::getClassTypeId() == SoType::badType())
    SoShaderParameterArray3f::initClass();
  if (SoShaderParameterArray4f::getClassTypeId() == SoType::badType())
    SoShaderParameterArray4f::initClass();
  if (SoShaderParameterMatrix::getClassTypeId() == SoType::badType())
    SoShaderParameterMatrix::initClass();
  if (SoShaderParameterMatrixArray::getClassTypeId() == SoType::badType())
    SoShaderParameterMatrixArray::initClass();
#endif

  // FIXME: Do we need int32 support (like in TGS)? 20040924 martin 
#if 0 // int32 support
  if (SoShaderParameter1i::getClassTypeId() == SoType::badType())
    SoShaderParameter1i::initClass();
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
}
