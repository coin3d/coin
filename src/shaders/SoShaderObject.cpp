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

#include <Inventor/nodes/SoShaderObject.h>

#include <assert.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLShaderProgramElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoFragmentShader.h>
#include <Inventor/nodes/SoGLShaderProgram.h>
#include <Inventor/nodes/SoShaderParameter.h>
#include <Inventor/nodes/SoVertexShader.h>
#include <Inventor/sensors/SoNodeSensor.h>

#include "SoGLARBShader.h"
#include "SoGLCgShader.h"
#include "SoGLSLShader.h"
#include "SoGLShaderObject.h"

// *************************************************************************

class SoShaderObjectP 
{
public:
  SoShaderObjectP(SoShaderObject *ownerptr); 
  ~SoShaderObjectP();

  void GLRender(SoGLRenderAction *action); 

  SoShaderObject * owner;
  SoGLShaderObject * glShaderObject;
  SbBool glShaderShouldLoad;
  SbBool glShaderIsInGLProgram;
  SoShaderObject::SourceType cachedSourceType;
  SbString cachedSourceProgram;

  void updateParameters(int start, int num);
  void updateAllParameters(); 
  void updateStateMatrixParameters();
  SbBool containStateMatrixParameters() const;
  void removeGLShaderFromGLProgram(SoGLShaderProgram *glProgram);

private:
  SoNodeSensor *sensor;
  static void sensorCB(void *data, SoSensor *);

  void checkType(); // sets cachedSourceType
  void readSource(); // sets cachedSourceProgram depending on sourceType

  SbBool isSupported(SoShaderObject::SourceType sourceType);

#if defined(SOURCE_HINT)
  SbString getSourceHint() const;
#endif
};

#define SELF this->pimpl

// *************************************************************************

SO_NODE_ABSTRACT_SOURCE(SoShaderObject);

// *************************************************************************

void SoShaderObject::initClass()
{
  SO_NODE_INIT_ABSTRACT_CLASS(SoShaderObject, SoGroup, "Group");
}

SoShaderObject::SoShaderObject()
{
  SO_NODE_CONSTRUCTOR(SoShaderObject);

  SO_NODE_ADD_FIELD(isActive, (TRUE));

  SO_NODE_DEFINE_ENUM_VALUE(SourceType, ARB_PROGRAM);
  SO_NODE_DEFINE_ENUM_VALUE(SourceType, CG_PROGRAM);
  SO_NODE_DEFINE_ENUM_VALUE(SourceType, GLSL_PROGRAM);
  SO_NODE_DEFINE_ENUM_VALUE(SourceType, FILENAME);

  SO_NODE_ADD_FIELD(sourceType, (FILENAME));
  SO_NODE_SET_SF_ENUM_TYPE(sourceType, SourceType);

  SO_NODE_ADD_FIELD(sourceProgram, (""));
  SO_NODE_ADD_FIELD(parameter, (NULL)); parameter.deleteValues(0,1);

  SELF = new SoShaderObjectP(this);
}

SoShaderObject::~SoShaderObject()
{
  delete SELF;
}

void SoShaderObject::GLRender(SoGLRenderAction * action)
{
  SELF->GLRender(action);
}

// Really send the parameters from start to start + num to the shaders. 
void SoShaderObject::updateParameters(int start, int num)
{
  SELF->updateParameters(start, num);
}

// Really send all the parameters to the shaders. 
void SoShaderObject::updateAllParameters()
{
  SELF->updateAllParameters();
}

// Update state matrix paramaters
void SoShaderObject::updateStateMatrixParameters()
{
  SELF->updateStateMatrixParameters();
}

SbBool SoShaderObject::containStateMatrixParameters() const
{
  return SELF->containStateMatrixParameters();
}

SoGLShaderObject * SoShaderObject::getGLShaderObject() const
{
  return SELF->glShaderObject;
}


SoShaderObject::SourceType SoShaderObject::getSourceType() const
{
  return SELF->cachedSourceType;
}

SbString SoShaderObject::getSourceProgram() const
{
  return SELF->cachedSourceProgram;
}

void SoShaderObject::removeGLShaderFromGLProgram(SoGLShaderProgram *glProgram)
{
  SELF->removeGLShaderFromGLProgram(glProgram);
}

/* ***************************************************************************
 * *** private implementation of SoShaderObjectP ***
 * ***************************************************************************/

SoShaderObjectP::SoShaderObjectP(SoShaderObject * ownerptr)
{
  this->owner = ownerptr;
  this->sensor = new SoNodeSensor(SoShaderObjectP::sensorCB, this);
  this->sensor->attach(ownerptr);

  this->glShaderObject = NULL;
  this->glShaderShouldLoad = TRUE;
  this->glShaderIsInGLProgram = FALSE;
  this->cachedSourceType = SoShaderObject::FILENAME;
}

SoShaderObjectP::~SoShaderObjectP()
{
  delete this->sensor;

  if (this->glShaderObject) {
    delete this->glShaderObject; this->glShaderObject = NULL;
  }
}

void
SoShaderObjectP::GLRender(SoGLRenderAction * action)
{
  SoState * state = action->getState();
  SoGLShaderProgram * shaderProgram = SoGLShaderProgramElement::get(state);

  assert(shaderProgram);

  SbBool flag = this->owner->isActive.getValue();

  if (!flag) {
    if (this->glShaderObject) this->glShaderObject->setIsActive(FALSE);
    return;
  }

  if (this->owner->sourceProgram.isDefault() ||
      this->owner->sourceProgram.getValue().getLength() == 0) { return; }

  if (this->glShaderShouldLoad) {
    this->glShaderShouldLoad = FALSE;
    checkType(); // set this->cachedSourceType
    readSource(); // set this->cachedSourceProgram

    // if file could not be read
    if (this->cachedSourceType == SoShaderObject::FILENAME) return;

    if (!this->isSupported(this->cachedSourceType)) {
      SbString s;
      switch (this->cachedSourceType) {
      case SoShaderObject::ARB_PROGRAM: s = "ARB_PROGRAM "; break;
      case SoShaderObject::CG_PROGRAM: s = "CG_PROGRAM "; break;
      case SoShaderObject::GLSL_PROGRAM: s = "GLSL_PROGRAM "; break;
      default: assert(FALSE && "unknown shader");
      }
      SoDebugError::postWarning("SoShaderObjectP::GLRender",
                                "%s is not supported", s.getString());
      return;
    }

    // Is current shaderObject of proper type? -> if not: remove it
    if (this->glShaderObject) {
      SbBool flag = FALSE;

      switch (this->glShaderObject->shaderType()) {
      case SoGLShader::ARB_SHADER: 
        if (this->cachedSourceType != SoShaderObject::ARB_PROGRAM) flag=TRUE;
        break;
      case SoGLShader::CG_SHADER: 
        if (this->cachedSourceType != SoShaderObject::CG_PROGRAM) flag=TRUE;
        break;
      case SoGLShader::GLSL_SHADER: 
        if (this->cachedSourceType != SoShaderObject::GLSL_PROGRAM)flag=TRUE;
        break;
      default:
        assert(FALSE && "This shouldn't happen!");
      }
      if (flag) {
        if (this->glShaderIsInGLProgram) {
          shaderProgram->removeShaderObject(this->glShaderObject);
          this->glShaderIsInGLProgram = FALSE;
        }
        if (this->glShaderObject) {
          delete this->glShaderObject;
          this->glShaderObject = NULL;
        }
      }
    }
    // Is current shaderObject already created? -> if not: create it
    if (!this->glShaderObject) {
      switch (this->cachedSourceType) {
      case SoShaderObject::ARB_PROGRAM:
        this->glShaderObject = (SoGLShaderObject*) new SoGLARBShaderObject;
        break;
#if defined(SO_CG_SHADER_SUPPORT)
      case SoShaderObject::CG_PROGRAM:
        this->glShaderObject = (SoGLShaderObject*) new SoGLCgShaderObject;
        break;
#endif
#if defined(SO_GLSL_SHADER_SUPPORT)
      case SoShaderObject::GLSL_PROGRAM:
        this->glShaderObject = (SoGLShaderObject*) new SoGLSLShaderObject;
        break;
#endif
      default:
        assert(FALSE && "This shouldn't happen!");
      }
      this->glShaderObject->setIsVertexShader(this->owner->isVertexShader());
    }
    else
      shaderProgram->postShouldLink();

#if defined(SOURCE_HINT)
    this->glShaderObject->sourceHint = getSourceHint();
#endif
    this->glShaderObject->load(this->cachedSourceProgram.getString());
  }
  if (this->glShaderObject) {
    if (!this->glShaderIsInGLProgram) {
      this->glShaderIsInGLProgram = TRUE;
      shaderProgram->addShaderObject(this->glShaderObject);
    }
    this->glShaderObject->setIsActive(flag);
  }
}


// sets this->cachedSourceType to [ARB|CG|GLSL]_PROGRAM
// this->cachedSourceType will be set to FILENAME, if sourceType is unknown
void
SoShaderObjectP::checkType()
{
  this->cachedSourceType = 
    (SoShaderObject::SourceType)this->owner->sourceType.getValue();

  if (this->cachedSourceType != SoShaderObject::FILENAME) return;

  // determine sourceType from file extension
  SbString fileName = this->owner->sourceProgram.getValue();
  int len = fileName.getLength();
  if (len > 5) {
    SbString subStr = fileName.getSubString(len-5);
    if (subStr == ".glsl") {
      this->cachedSourceType = SoShaderObject::GLSL_PROGRAM;
      return;
    }
  }
  if (len > 3) {
    SbString subStr = fileName.getSubString(len-3);
    if (subStr == ".cg") {
      this->cachedSourceType = SoShaderObject::CG_PROGRAM;
      return;
    }
    if (subStr == ".fp") {
      this->cachedSourceType = this->owner->isVertexShader() 
        ? SoShaderObject::FILENAME : SoShaderObject::ARB_PROGRAM;
      return;
    }
    if (subStr==".vp") {
      this->cachedSourceType = this->owner->isVertexShader() 
        ? SoShaderObject::ARB_PROGRAM : SoShaderObject::FILENAME;
      return;
    }
  }
  SoDebugError::postWarning("SoShaderObjectP::checkType",
                            "Could not determine shader type of file '%s'!\n"
                            "Following file extensions are supported:\n"
                            "*.fp -> ARB_PROGRAM (fragment)\n"
                            "*.vp -> ARB_PROGRAM (vertex)\n"
                            "*.cg -> CG_PROGRAM (fragment|vertex)\n"
                            "*.glsl -> GLSL_PROGRAM (fragment|vertex)\n",
                            fileName.getString());
  // error: could not determine SourceType
  this->cachedSourceType = SoShaderObject::FILENAME;
}

// read the file if neccessary and assign content to this->cachedSourceProgram
void
SoShaderObjectP::readSource(void)
{
  SoShaderObject::SourceType srcType = 
    (SoShaderObject::SourceType)this->owner->sourceType.getValue();

  this->cachedSourceProgram.makeEmpty();

  if (this->owner->sourceProgram.isDefault())
    return;
  else if (srcType != SoShaderObject::FILENAME)
    this->cachedSourceProgram = this->owner->sourceProgram.getValue();
  else {
    if (this->cachedSourceType != SoShaderObject::FILENAME) {
      SbString fileName = this->owner->sourceProgram.getValue();

      // FIXME: should really look for the file in all paths that
      // SoInput::getDirectories() had when .iv-file was
      // read. 20050120 mortene. 
      FILE * f = fopen(fileName.getString(), "rb");
      if (f) {
        int ok = fseek(f, 0L, SEEK_END);
        assert(ok); // FIXME: be more robust. 20050120 mortene. 
        const long length = ftell(f);
        assert(length != -1); // FIXME: be more robust. 20050120 mortene. 
        assert(length != 0); // FIXME: be more robust. 20050120 mortene. 

        ok = fseek(f, 0L, SEEK_SET);
        assert(ok); // FIXME: be more robust. 20050120 mortene. 

        char * srcstr = new char[length+1];
        size_t readlen = fread(srcstr, 1, length, f);
        assert(readlen == (size_t)length); // FIXME: be more robust. 20050120 mortene. 

        srcstr[length] = '\0';
        this->cachedSourceProgram = srcstr;
        delete[] srcstr;
      }
      else {
        this->cachedSourceType = SoShaderObject::FILENAME;

        SoDebugError::postWarning("SoShaderObjectP::readSource",
                                  "Could not read shader file '%s'",
                                  fileName.getString());
      }
    }
  }
}

SbBool SoShaderObjectP::isSupported(SoShaderObject::SourceType sourceType)
{
  if (this->owner->isVertexShader())
    return SoVertexShader::isSupported(sourceType);
  else
    return SoFragmentShader::isSupported(sourceType);
}

void SoShaderObjectP::updateParameters(int start, int num)
{
  // FIXME: the group's children are ignored at the moment (20041011 martin)
  if (!this->owner->isActive.getValue() || this->glShaderObject==NULL) return;
  if (start < 0 || num < 0) return;

  int cnt = this->owner->parameter.getNum();
  int end = start+num;

  end = (end > cnt) ? cnt : end;
  for (int i=start; i<end; i++) {
    SoUniformShaderParameter * param = 
      (SoUniformShaderParameter*)this->owner->parameter[i];
    param->updateParameter(this->glShaderObject);
  }
}

void SoShaderObjectP::updateAllParameters()
{
  if (this->glShaderObject==NULL || !this->glShaderObject->isActive()) return;

  int i, cnt = this->owner->parameter.getNum();

  for (i=0; i<cnt; i++) {
    SoUniformShaderParameter *param = 
      (SoUniformShaderParameter*)this->owner->parameter[i];
    param->updateParameter(this->glShaderObject);
  }
  cnt = this->owner->getNumChildren();
  for (i=0; i<cnt; i++) {
    SoUniformShaderParameter *param = 
      (SoUniformShaderParameter*)this->owner->getChild(i);
    param->updateParameter(this->glShaderObject);
  }
}

// Update state matrix paramaters
void SoShaderObjectP::updateStateMatrixParameters()
{
#if defined(SO_CG_SHADER_SUPPORT)
#define STATE_PARAM SoShaderStateMatrixParameter 
  if (this->glShaderObject==NULL || !this->glShaderObject->isActive()) return;

  int i, cnt = this->owner->parameter.getNum();
  for (i=0; i<cnt; i++) {
    STATE_PARAM *param = (STATE_PARAM*)this->owner->parameter[i];
    if (param->isOfType(STATE_PARAM::getClassTypeId()))
      param->updateParameter(this->glShaderObject);
  }
  cnt = this->owner->getNumChildren();
  for (i=0; i<cnt; i++) {
    STATE_PARAM *param = (STATE_PARAM*)this->owner->getChild(i);
    if (param->isOfType(STATE_PARAM::getClassTypeId()))
      param->updateParameter(this->glShaderObject);
  }
#undef STATE_PARAM
#endif
}

SbBool SoShaderObjectP::containStateMatrixParameters() const
{
#if defined(SO_CG_SHADER_SUPPORT)
#define STATE_PARAM SoShaderStateMatrixParameter 
  int i, cnt = this->owner->parameter.getNum();
  for (i=0; i<cnt; i++) {
    if (this->owner->parameter[i]->isOfType(STATE_PARAM::getClassTypeId()))
      return TRUE;
  }
  cnt = this->owner->getNumChildren();
  for (i=0; i<cnt; i++) {
    if (this->owner->getChild(i)->isOfType(STATE_PARAM::getClassTypeId()))
      return TRUE;
  }
#undef STATE_PARAM
#endif
  return FALSE;
}

void SoShaderObjectP::removeGLShaderFromGLProgram(SoGLShaderProgram *glProgram)
{ 
  if (glProgram == NULL) return;

  if (this->glShaderObject != NULL) {
    if (this->glShaderIsInGLProgram) {
      glProgram->removeShaderObject(this->glShaderObject);
      this->glShaderIsInGLProgram = FALSE;
    }
  }
}

#if defined(SOURCE_HINT)
SbString SoShaderObjectP::getSourceHint() const
{
  SoShaderObject::SourceType srcType =
    (SoShaderObject::SourceType)this->owner->sourceType.getValue();

  if (srcType == SoShaderObject::FILENAME)
    return this->owner->sourceProgram.getValue();
  else
    return ""; // FIXME: should return first line of shader source code
}
#endif

void SoShaderObjectP::sensorCB(void *data, SoSensor *)
{
  ((SoShaderObjectP *)data)->glShaderShouldLoad = TRUE;
}
