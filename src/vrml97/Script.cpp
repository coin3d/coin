/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoVRMLScript SoVRMLScript.h Inventor/VRMLnodes/SoVRMLScript.h
  \brief The SoVRMLScript class is used to control the scene using scripts.
*/

#include <Inventor/VRMLnodes/SoVRMLScript.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/SbName.h>
#include <Inventor/SoInput.h>
#include <Inventor/misc/SoProto.h>
#include <Inventor/errors/SoReadError.h>
#include <assert.h>

SoType SoVRMLScript::classTypeId;

// Doc in parent
void
SoVRMLScript::initClass(void) // static
{
  SoVRMLScript::classTypeId =
    SoType::createType(SoNode::getClassTypeId(),
                       SbName("VRMLScript"),
                       SoVRMLScript::createInstance);
}

SoVRMLScript::SoVRMLScript(void)
  : fielddata(NULL)
{
  this->isBuiltIn = TRUE;
  assert(SoVRMLScript::classTypeId != SoType::badType());

  this->url.setNum(0);
  this->url.setContainer(this);

  this->directOutput.setValue(TRUE);
  this->directOutput.setContainer(this);

  this->mustEvaluate.setValue(TRUE);
  this->mustEvaluate.setContainer(this);

  this->initFieldData();
}

SoVRMLScript::~SoVRMLScript() // virtual, protected
{
  const int n = this->fielddata->getNumFields();
  for (int i = 0; i < n; i++) {
    SoField * f = this->fielddata->getField(this, i);
    if (f != &this->directOutput &&
        f != &this->url &&
        f != &this->mustEvaluate) delete f;
  }
  delete this->fielddata;
}


// Doc in parent
SoType
SoVRMLScript::getClassTypeId(void)
{
  return SoVRMLScript::classTypeId;
}

// Doc in parent
SoType
SoVRMLScript::getTypeId(void) const
{
  return SoVRMLScript::classTypeId;
}

// Doc in parent
void
SoVRMLScript::doAction(SoAction * action)
{
}

// Doc in parent
void
SoVRMLScript::callback(SoCallbackAction * action)
{
}

// Doc in parent
void
SoVRMLScript::GLRender(SoGLRenderAction * action)
{
}

// Doc in parent
void
SoVRMLScript::getBoundingBox(SoGetBoundingBoxAction * action)
{
}

// Doc in parent
void
SoVRMLScript::pick(SoPickAction * action)
{
}

// Doc in parent
void
SoVRMLScript::handleEvent(SoHandleEventAction * action)
{
}

// Doc in parent
void
SoVRMLScript::write(SoWriteAction * action)
{
}

// Doc in parent
void
SoVRMLScript::copyContents(const SoFieldContainer * from,
                           SbBool copyConn)
{
  assert(from->isOfType(SoVRMLScript::getClassTypeId()));
  this->initFieldData();

  const SoVRMLScript * fromnode = (SoVRMLScript*) from;

  const SoFieldData * src = from->getFieldData();
  const int n = src->getNumFields();
  for (int i = 0; i < n; i++) {
    const SoField * f = src->getField(from, i);
    if (f != &fromnode->directOutput &&
        f != &fromnode->url &&
        f != &fromnode->mustEvaluate) {
      SoField * cp = (SoField*) f->getTypeId().createInstance();
      cp->setContainer(this);
      this->fielddata->addField(this, src->getFieldName(i), cp);
    }
  }
  inherited::copyContents(from, copyConn);
}

// Doc in parent
void *
SoVRMLScript::createInstance(void)
{
  return (void*) new SoVRMLScript;
}

// Doc in parent
const SoFieldData *
SoVRMLScript::getFieldData(void) const
{
  return this->fielddata;
}

// Doc in parent
SbBool
SoVRMLScript::readInstance(SoInput * in, unsigned short flags)
{
  // FIXME: error messages
  SbName name("");
  SbBool ok;

  ok = in->read(name, TRUE);

  const SbName URL("url");
  const SbName DIRECTOUTPUT("directOutput");
  const SbName MUSTEVALUATE("mustEvaluate");
  const SbName EVENTIN("eventIn");
  const SbName EVENTOUT("eventOut");
  const SbName FIELD("field");
  const SbName EXPOSEDFIELD("exposedField");

  SbBool err = FALSE;

  while (!err && ok) {
    if (name == EVENTIN ||
        name == EVENTOUT ||
        name == FIELD ||
        name == EXPOSEDFIELD) {
      SbName ftype, fname;
      err = ! (in->read(ftype, TRUE) && in->read(fname, TRUE));
      if (!err) {
        SoType type = SoType::fromName(ftype);
        if (type.isDerivedFrom(SoField::getClassTypeId()) && type.canCreateInstance()) {
          SoField * f = (SoField*) type.createInstance();

          if (name == EVENTIN) {
            f->setFieldType(SoField::EVENTIN_FIELD);
          }
          else if (name == EVENTOUT) {
            f->setFieldType(SoField::EVENTOUT_FIELD);
          }
          else if (name == EXPOSEDFIELD) {
            f->setFieldType(SoField::EXPOSED_FIELD);
          }
          f->setContainer(this);
          this->fielddata->addField(this, fname, f);
          if (name == FIELD || name == EXPOSEDFIELD) { // only read field values for fields
            err = ! f->read(in, fname);
          }
          else {
            (void) in->checkISReference(this, fname, err);
            err = !err;
          }
        }
        else {
          err = TRUE;
        }
      }
      if (!err) {
        name = "";
        ok = in->read(name, TRUE);
      }
    }
    else ok = FALSE;
  }
  if (!err) {
    if (name != "") in->putBack(name.getString());
    return inherited::readInstance(in, flags);
  }
  return FALSE;
}

//
// Private method that initializes the field data and adds the default
// fields.
//
void
SoVRMLScript::initFieldData(void)
{
  if (this->fielddata) delete this->fielddata;
  this->fielddata = new SoFieldData;
  this->fielddata->addField(this, "url", &this->url);
  this->fielddata->addField(this, "directOutput", &this->directOutput);
  this->fielddata->addField(this, "mustEvaluate", &this->mustEvaluate);
}
