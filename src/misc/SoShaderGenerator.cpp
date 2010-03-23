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

/*!
  \class SoShaderGenerator SoShaderGenerator.h Inventor/misc/SoShaderGenerator.h
  \brief The SoShaderGenerator class is used for simplifying the process of generating shader scripts.

  Currently only GLSL scripts are supported.
*/

#include "misc/SoShaderGenerator.h"

#include <Inventor/SbName.h>
#include <Inventor/errors/SoDebugError.h>

#include "shaders/SoShader.h"

/*!
  Constructor.
*/
SoShaderGenerator::SoShaderGenerator(void)
  : dirty(FALSE)
{
}

/*!
  Destructor.
*/
SoShaderGenerator::~SoShaderGenerator()
{
}

void 
SoShaderGenerator::reset(const SbBool freeoldstrings)
{
  this->version.makeEmpty(freeoldstrings);
  this->defines.makeEmpty(freeoldstrings);
  this->declarations.makeEmpty(freeoldstrings);
  this->functions.makeEmpty(freeoldstrings);
  this->main.makeEmpty(freeoldstrings);
  this->combined.makeEmpty(freeoldstrings);
  this->dirty = FALSE;
}


void 
SoShaderGenerator::setVersion(const SbString & str)
{
  this->version = str;
  this->version += "\n";
}

/*!
  Adds a define to the shader program.
*/
void 
SoShaderGenerator::addDefine(const SbString & str, const SbBool checkexists)
{
  if (!checkexists || (this->defines.find(str) < 0)) {
    this->dirty = TRUE;
    this->defines += str;
    this->defines += "\n";
  }
}

/*!
  Adds a declaration (varying or uniform) to the script.
*/
void 
SoShaderGenerator::addDeclaration(const SbString & str, const SbBool checkexists)
{
  if (!checkexists || (this->declarations.find(str) < 0)) {
    this->dirty = TRUE;
    this->declarations += str;
    this->declarations += "\n";
  }
}

/*!
  Adds a function to the script.
*/
void 
SoShaderGenerator::addFunction(const SbString & str, const SbBool checkexists)
{
  if (!checkexists || (this->functions.find(str) < 0)) {
    this->dirty = TRUE;
    this->functions += str;
    this->functions += "\n";
  }
}

/*!
  Adds a named function to the script.
*/
void 
SoShaderGenerator::addNamedFunction(const SbName & name, const SbBool checkexists)
{
  const char * func = SoShader::getNamedScript(name, SoShader::GLSL_SHADER);
  
  if (func) {
    this->addFunction(SbString(func), checkexists);
  }
  else {
    SoDebugError::postWarning("SoShaderGenerator::addNamedFunction",
                              "Unknown named script: %s",
                              name.getString());
  }
}

/*!
  Add a statment to the main function.
*/
void 
SoShaderGenerator::addMainStatement(const SbString & str)
{
  this->dirty = TRUE;
  this->main += str;
  this->main += "\n";
}

/*!
  Returns the complete shader program.
*/
const SbString &
SoShaderGenerator::getShaderProgram(void)
{
  if (this->dirty) {
    this->combined.makeEmpty(FALSE);
    this->combined += this->version;
    this->combined += this->defines;
  
    this->combined += this->declarations;
    this->combined += this->functions;
    this->combined += SbString("void main(void) {\n");
    this->combined += this->main;
    this->combined += "}\n";
  }
  return this->combined;
}
