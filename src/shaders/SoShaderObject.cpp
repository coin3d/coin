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
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoGLShaderProgramElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoFragmentShader.h>
#include <Inventor/nodes/SoShaderParameter.h>
#include <Inventor/nodes/SoVertexShader.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/SoInput.h>
#include <Inventor/lists/SbStringList.h>
#include <Inventor/misc/SbHash.h>

#include "SoGLARBShaderObject.h"
#include "SoGLCgShaderObject.h"
#include "SoGLSLShaderObject.h"
#include "SoGLShaderProgram.h"

// *************************************************************************

class SoShaderObjectP
{
public:
  SoShaderObjectP(SoShaderObject *ownerptr);
  ~SoShaderObjectP();

  // FIXME: add a cache context destruction callback, pederb 2005-11-30

  void GLRender(SoGLRenderAction *action);

  SoGLShaderObject * getGLShaderObject(const uint32_t cachecontext) {
    SoGLShaderObject * obj = NULL;
    if (this->glshaderobjects.get(cachecontext, obj)) return obj;
    return NULL;
  }
  void setGLShaderObject(SoGLShaderObject * obj, const uint32_t cachecontext) {
    SoGLShaderObject * oldshader;
    if (this->glshaderobjects.get(cachecontext, oldshader)) {
      deleteGLShader(oldshader);
    }
    (void) this->glshaderobjects.put(cachecontext, obj);
  }
  void deleteGLShaderObjects(void) {
    SbList <uint32_t> keylist;
    this->glshaderobjects.makeKeyList(keylist);
    for (int i = 0; i < keylist.getLength(); i++) {
      SoGLShaderObject * glshader = NULL;
      (void) this->glshaderobjects.get(keylist[i], glshader);
      deleteGLShader(glshader);
    }
    this->glshaderobjects.clear();
  }
  static void deleteGLShader(SoGLShaderObject * obj) {
    // FIXME: schedule delete, pederb 2005-11-30
  }

  void invalidateParameters(void) {
    SbList <uint32_t> keylist;
    this->glshaderobjects.makeKeyList(keylist);
    for (int i = 0; i < keylist.getLength(); i++) {
      SoGLShaderObject * glshader = NULL;
      (void) this->glshaderobjects.get(keylist[i], glshader);
      glshader->setParametersDirty(TRUE);
    }
  }

  SoShaderObject * owner;
  SoShaderObject::SourceType cachedSourceType;
  SbString cachedSourceProgram;
  SbBool didSetSearchDirectories;
  SbBool shouldload;
  SoNodeSensor *sensor;

  void updateParameters(const uint32_t cachecontext, int start, int num);
  void updateAllParameters(const uint32_t cachecontext);
  void updateStateMatrixParameters(const uint32_t cachecontext);
  SbBool containStateMatrixParameters(void) const;
  void setSearchDirectories(const SbStringList & list);

private:
  static void sensorCB(void *data, SoSensor *);

  SbStringList searchdirectories;
  SbHash <SoGLShaderObject *, uint32_t> glshaderobjects;

  void checkType(void); // sets cachedSourceType
  void readSource(void); // sets cachedSourceProgram depending on sourceType

  SbBool isSupported(SoShaderObject::SourceType sourceType, const cc_glglue * glue);

#if defined(SOURCE_HINT)
  SbString getSourceHint(void) const;
#endif
};

#define SELF this->pimpl

// *************************************************************************

SO_NODE_ABSTRACT_SOURCE(SoShaderObject);

// *************************************************************************

void SoShaderObject::initClass(void)
{
  SO_NODE_INTERNAL_INIT_ABSTRACT_CLASS(SoShaderObject,
                                       SO_FROM_COIN_2_4|SO_FROM_INVENTOR_5_0);
}

SoShaderObject::SoShaderObject(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoShaderObject);

  SO_NODE_ADD_FIELD(isActive, (TRUE));

  SO_NODE_DEFINE_ENUM_VALUE(SourceType, ARB_PROGRAM);
  SO_NODE_DEFINE_ENUM_VALUE(SourceType, CG_PROGRAM);
  SO_NODE_DEFINE_ENUM_VALUE(SourceType, GLSL_PROGRAM);
  SO_NODE_DEFINE_ENUM_VALUE(SourceType, FILENAME);

  SO_NODE_ADD_FIELD(sourceType, (FILENAME));
  SO_NODE_SET_SF_ENUM_TYPE(sourceType, SourceType);

  SO_NODE_ADD_FIELD(sourceProgram, (""));
  SO_NODE_ADD_FIELD(parameter, (NULL)); 
  this->parameter.setNum(0);
  this->parameter.setDefault(TRUE);

  SELF = new SoShaderObjectP(this);
}

SoShaderObject::~SoShaderObject()
{
  delete SELF;
}

void
SoShaderObject::GLRender(SoGLRenderAction * action)
{
  SELF->GLRender(action);
}

void
SoShaderObject::search(SoSearchAction * action)
{
  // Include this node in the search.
  SoNode::search(action);
  if (action->isFound()) return;

  // If we're not the one being sought after, try shader parameter.
  int numindices;
  const int * indices;
  if (action->getPathCode(numindices, indices) == SoAction::IN_PATH) {
    // FIXME: not implemented -- 20050129 martin
  }
  else { // traverse all shader parameter
    int num = this->parameter.getNum();
    for (int i=0; i<num; i++) {
      SoNode * node = this->parameter[i];
      action->pushCurPath(i, node);
      node->search(action);
      action->popCurPath();
      if (action->isFound()) return;
    }
  }
}

SbBool 
SoShaderObject::readInstance(SoInput * in, unsigned short flags)
{
  SELF->sensor->detach();
  SELF->deleteGLShaderObjects();

  SbBool ret = inherited::readInstance(in, flags);
  if (ret) {
    SELF->setSearchDirectories(SoInput::getDirectories());
  }
  SELF->sensor->attach(this);

  return ret;
}

SoShaderObject::SourceType SoShaderObject::getSourceType(void) const
{
  return SELF->cachedSourceType;
}

SbString SoShaderObject::getSourceProgram(void) const
{
  return SELF->cachedSourceProgram;
}

void 
SoShaderObject::updateParameters(const uint32_t cachecontext)
{
  SELF->updateAllParameters(cachecontext);
  SELF->updateStateMatrixParameters(cachecontext);
}

/* ***************************************************************************
 * *** private implementation of SoShaderObjectP ***
 * ***************************************************************************/

SoShaderObjectP::SoShaderObjectP(SoShaderObject * ownerptr)
{
  this->owner = ownerptr;
  this->sensor = new SoNodeSensor(SoShaderObjectP::sensorCB, this);
  this->sensor->setPriority(0);
  this->sensor->attach(ownerptr);

  this->cachedSourceType = SoShaderObject::FILENAME;
  this->didSetSearchDirectories = FALSE;
  this->shouldload = TRUE;
}

SoShaderObjectP::~SoShaderObjectP()
{
  this->deleteGLShaderObjects();

  SbStringList empty;
  this->setSearchDirectories(empty);
  delete this->sensor;
}

void
SoShaderObjectP::GLRender(SoGLRenderAction * action)
{
  SbBool isactive = this->owner->isActive.getValue();
  if (!isactive) return;
  
  SoState * state = action->getState();

  SoGLShaderProgram * shaderProgram = SoGLShaderProgramElement::get(state);
  assert(shaderProgram);

  const uint32_t cachecontext = SoGLCacheContextElement::get(state);
  const cc_glglue * glue = cc_glglue_instance(cachecontext);

  SoGLShaderObject * shaderobject = this->getGLShaderObject(cachecontext);

  if (this->owner->sourceProgram.isDefault() ||
      this->owner->sourceProgram.getValue().getLength() == 0) { return; }

  if (shaderobject == NULL) {
    if (this->shouldload) {
      this->checkType(); // set this->cachedSourceType
      this->readSource(); // set this->cachedSourceProgram
      this->shouldload = FALSE;
    }
    // if file could not be read
    if (this->cachedSourceType == SoShaderObject::FILENAME) return;

    if (!this->isSupported(this->cachedSourceType, glue)) {
      SbString s;
      switch (this->cachedSourceType) {
      case SoShaderObject::ARB_PROGRAM: s = "ARB_PROGRAM"; break;
      case SoShaderObject::CG_PROGRAM: s = "CG_PROGRAM"; break;
      case SoShaderObject::GLSL_PROGRAM: s = "GLSL_PROGRAM"; break;
      default: assert(FALSE && "unknown shader");
      }
      SoDebugError::postWarning("SoShaderObjectP::GLRender",
                                "%s is not supported", s.getString());
      return;
    }

    switch (this->cachedSourceType) {
    case SoShaderObject::ARB_PROGRAM:
      shaderobject = (SoGLShaderObject *)new SoGLARBShaderObject(cachecontext);
      break;
    case SoShaderObject::CG_PROGRAM:
      shaderobject = (SoGLShaderObject*) new SoGLCgShaderObject(cachecontext);
      break;
    case SoShaderObject::GLSL_PROGRAM:
      shaderobject = (SoGLShaderObject*) new SoGLSLShaderObject(cachecontext);
      break;
    default:
      assert(FALSE && "This shouldn't happen!");
    }
    shaderobject->setIsVertexShader(this->owner->isVertexShader());
#if defined(SOURCE_HINT)
    shaderobject->sourceHint = getSourceHint();
#endif
    shaderobject->load(this->cachedSourceProgram.getString());
    this->setGLShaderObject(shaderobject, cachecontext);
  }
  if (shaderobject) {
    shaderProgram->addShaderObject(shaderobject);
    shaderobject->setIsActive(isactive);
  }
}

  void updateParameters(const uint32_t cachecontext);


// sets this->cachedSourceType to [ARB|CG|GLSL]_PROGRAM
// this->cachedSourceType will be set to FILENAME, if sourceType is unknown
void
SoShaderObjectP::checkType(void)
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

      SbStringList subdirs;
      subdirs.append(new SbString("shader"));
      subdirs.append(new SbString("shaders"));
      SbString fileName = SoInput::searchForFile(this->owner->sourceProgram.getValue(),
                                                 this->searchdirectories,
                                                 subdirs);
      // delete allocated subdirs before continuing
      delete subdirs[0];
      delete subdirs[1];
      
      FILE * f = fopen(fileName.getString(), "rb");
      SbBool readok = FALSE;
      if (f) {
        if (fseek(f, 0L, SEEK_END) == 0) {
          const long length = ftell(f);
          if ((length > 0) && (fseek(f, 0L, SEEK_SET) == 0)) {
            char * srcstr = new char[length+1];
            size_t readlen = fread(srcstr, 1, length, f);
            if (readlen == (size_t) length) {
              srcstr[length] = '\0';
              this->cachedSourceProgram = srcstr;
              readok = TRUE;
            }
            delete[] srcstr;
          }
        }
      }
      if (!readok) {
        this->cachedSourceType = SoShaderObject::FILENAME;
        SoDebugError::postWarning("SoShaderObjectP::readSource",
                                  "Could not read shader file '%s'",
                                  fileName.getString());
      }
    }
  }
}

SbBool
SoShaderObjectP::isSupported(SoShaderObject::SourceType sourceType, const cc_glglue * glue)
{
  if (this->owner->isVertexShader()) {
    // don't call this function. It's not context safe. pederb, 20051103
    // return SoVertexShader::isSupported(sourceType);    

    if (sourceType == SoShaderObject::ARB_PROGRAM) {
      return cc_glglue_has_arb_vertex_program(glue);
    }
    else if (sourceType == SoShaderObject::GLSL_PROGRAM) {
      // FIXME: Maybe we should check for OpenGL 2.0 aswell? (20050428
      // handegar)
      return cc_glglue_has_arb_shader_objects(glue);
    }
    // FIXME: Add support for detecting missing Cg support
    // (20050427 handegar)
    else if (sourceType == SoShaderObject::CG_PROGRAM) return TRUE;
    return FALSE;
  }
  else {
    // don't call this function. It's not context safe. pederb, 20051103
    // return SoFragmentShader::isSupported(sourceType);

    if (sourceType == SoShaderObject::ARB_PROGRAM) {
      return cc_glglue_has_arb_fragment_program(glue);
    }
    else if (sourceType == SoShaderObject::GLSL_PROGRAM) {    
      // FIXME: Maybe we should check for OpenGL 2.0 aswell? (20050428
      // handegar)
      return cc_glglue_has_arb_shader_objects(glue);
    } 
    // FIXME: Add support for detecting missing Cg support (20050427
    // handegar)
    else if (sourceType == SoShaderObject::CG_PROGRAM) return TRUE;
    return FALSE;
  }
}

void
SoShaderObjectP::updateParameters(const uint32_t cachecontext, int start, int num)
{

  if (!this->owner->isActive.getValue()) return;
  if (start < 0 || num < 0) return;

  SoGLShaderObject * shaderobject = this->getGLShaderObject(cachecontext);
  if ((shaderobject == NULL) || !shaderobject->getParametersDirty()) return;

  int cnt = this->owner->parameter.getNum();
  int end = start+num;
  
  end = (end > cnt) ? cnt : end;
  for (int i=start; i<end; i++) {
    SoUniformShaderParameter * param =
      (SoUniformShaderParameter*)this->owner->parameter[i];
    param->updateParameter(shaderobject);
  }
}

void
SoShaderObjectP::updateAllParameters(const uint32_t cachecontext)
{
  if (!this->owner->isActive.getValue()) return;

  SoGLShaderObject * shaderobject = this->getGLShaderObject(cachecontext);
  if ((shaderobject == NULL) || !shaderobject->getParametersDirty()) return;

  int i, cnt = this->owner->parameter.getNum();

  for (i=0; i<cnt; i++) {
    SoUniformShaderParameter *param =
      (SoUniformShaderParameter*)this->owner->parameter[i];
    param->updateParameter(shaderobject);
  }
  shaderobject->setParametersDirty(FALSE);
}

// Update state matrix paramaters
void
SoShaderObjectP::updateStateMatrixParameters(const uint32_t cachecontext)
{
#define STATE_PARAM SoShaderStateMatrixParameter
  if (!this->owner->isActive.getValue()) return;
  
  SoGLShaderObject * shaderobject = this->getGLShaderObject(cachecontext);
  if (shaderobject == NULL) return;
  
  int i, cnt = this->owner->parameter.getNum();
  for (i= 0; i <cnt; i++) {
    STATE_PARAM * param = (STATE_PARAM*)this->owner->parameter[i];
    if (param->isOfType(STATE_PARAM::getClassTypeId()))
      param->updateParameter(shaderobject);
  }
#undef STATE_PARAM
}

SbBool
SoShaderObjectP::containStateMatrixParameters(void) const
{
#define STATE_PARAM SoShaderStateMatrixParameter
  int i, cnt = this->owner->parameter.getNum();
  for (i = 0; i < cnt; i++) {
    if (this->owner->parameter[i]->isOfType(STATE_PARAM::getClassTypeId()))
      return TRUE;
  }
#undef STATE_PARAM
  return FALSE;
}

#if defined(SOURCE_HINT)
SbString
SoShaderObjectP::getSourceHint(void) const
{
  SoShaderObject::SourceType srcType =
    (SoShaderObject::SourceType)this->owner->sourceType.getValue();

  if (srcType == SoShaderObject::FILENAME)
    return this->owner->sourceProgram.getValue();
  else
    return ""; // FIXME: should return first line of shader source code
}
#endif

void
SoShaderObjectP::sensorCB(void *data, SoSensor *sensor)
{
  SoShaderObjectP * thisp = (SoShaderObjectP*) data;
  SoField * field = ((SoNodeSensor *)sensor)->getTriggerField();

  if (field == &thisp->owner->sourceProgram || 
      field == &thisp->owner->sourceType) {
    thisp->deleteGLShaderObjects();
    thisp->shouldload = TRUE;
  }
  else if (field == &thisp->owner->parameter) {
    thisp->invalidateParameters();
  }
  if (!thisp->didSetSearchDirectories) {
    thisp->setSearchDirectories(SoInput::getDirectories());
  }
}

void 
SoShaderObjectP::setSearchDirectories(const SbStringList & list)
{
  int i;
  for (i = 0; i< this->searchdirectories.getLength(); i++) {
    delete this->searchdirectories[i];
  }
  
  for (i = 0; i < list.getLength(); i++) {
    this->searchdirectories.append(new SbString(*(list[i])));
  }
  this->didSetSearchDirectories = TRUE;
}
