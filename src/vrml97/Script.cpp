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
  \ingroup VRMLnodes

  \WEB3DCOPYRIGHT

  \verbatim
  Script {
    exposedField MFString url           []
    field        SFBool   directOutput  FALSE
    field        SFBool   mustEvaluate  FALSE
    # And any number of:
    eventIn      eventType eventName
    field        fieldType fieldName initialValue
    eventOut     eventType eventName
  }
  \endverbatim

  The Script node is used to program behaviour in a scene. Script nodes
  typically

  - signify a change or user action;
  - receive events from other nodes;
  - contain a program module that performs some computation;
  - effect change somewhere else in the scene by sending events.

  Each Script node has associated programming language code,
  referenced by the url field, that is executed to carry out the
  Script node's function. That code is referred to as the "script" in
  the rest of this description. Details on the url field can be found
  in 4.5, VRML and the World Wide Web
  (<http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.5>).

  Browsers are not required to support any specific language. Detailed
  information on scripting languages is described in 4.12, Scripting
  (<http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.12>).

  Browsers supporting a scripting language for which a language
  binding is specified shall adhere to that language binding.
  Sometime before a script receives the first event it shall be
  initialized (any language-dependent or user-defined initialize() is
  performed).

  The script is able to receive and process events that are sent to
  it. Each event that can be received shall be declared in the Script
  node using the same syntax as is used in a prototype definition:

  \verbatim
  eventIn type name
  \endverbatim

  The type can be any of the standard VRML fields
  (as defined in 5, Field and event reference). Name shall be an
  identifier that is unique for this Script node.

  The Script node is able to generate events in response to the
  incoming events. Each event that may be generated shall be declared
  in the Script node using the following syntax:

  \verbatim
  eventOut type name
  \endverbatim

  With the exception of the url field, exposedFields are not allowed
  in Script nodes.

  If the Script node's \e mustEvaluate field is FALSE, the browser may
  delay sending input events to the script until its outputs are
  needed by the browser. If the \e mustEvaluate field is TRUE, the
  browser shall send input events to the script as soon as possible,
  regardless of whether the outputs are needed. The \e mustEvaluate
  field shall be set to TRUE only if the Script node has effects that
  are not known to the browser (such as sending information across the
  network). Otherwise, poor performance may result.

  Once the script has access to a VRML node (via an SoSFNode or
  SoMFNode value either in one of the Script node's fields or passed
  in as an eventIn), the script is able to read the contents of that
  node's exposed fields.

  If the Script node's \e directOutput field is TRUE, the script may
  also send events directly to any node to which it has access, and
  may dynamically establish or break routes.

  If directOutput is FALSE (the default), the script may only
  affect the rest of the world via events sent through its
  eventOuts. The results are undefined if directOutput is FALSE and
  the script sends events directly to a node to which it has access.

  A script is able to communicate directly with the VRML browser to
  get information such as the current time and the current world
  URL. This is strictly defined by the API for the specific scripting
  language being used.  The location of the Script node in the scene
  graph has no affect on its operation. For example, if a parent of a
  Script node is a Switch node with whichChoice set to "-1" (i.e.,
  ignore its children), the Script node continues to operate as
  specified (i.e., it receives and sends events).

*/

#include <Inventor/VRMLnodes/SoVRMLScript.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/SbName.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/misc/SoProto.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/actions/SoWriteAction.h>
#include <assert.h>

SoType SoVRMLScript::classTypeId;

// Doc in parent
void
SoVRMLScript::initClass(void) // static
{
  SoVRMLScript::classTypeId =
    SoType::createType(SoNode::getClassTypeId(),
                       SbName("VRMLScript"),
                       SoVRMLScript::createInstance,
                       SoNode::nextActionMethodIndex++);
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
  SoOutput * out = action->getOutput();
  if (out->getStage() == SoOutput::COUNT_REFS) {
    inherited::write(action);
  }
  else if (out->getStage() == SoOutput::WRITE) {
    if (this->writeHeader(out, FALSE, FALSE))
      return;
    const SbName URL("url");
    const SbName DIRECTOUTPUT("directOutput");
    const SbName MUSTEVALUATE("mustEvaluate");
    const SoFieldData * fd = this->getFieldData();

    SbString value;
    for (int i = 0; i < fd->getNumFields(); i++) {
      const SoField * f = fd->getField(this, i);
      SbName fieldname = fd->getFieldName(i);
      SbBool writevalue = FALSE;
      if (fieldname != URL && fieldname != DIRECTOUTPUT &&
          fieldname != MUSTEVALUATE) {
        out->indent();
        switch (f->getFieldType()) {
        case SoField::NORMAL_FIELD:        
          out->write("field ");
          writevalue = TRUE;
          break;
        case SoField::EVENTIN_FIELD:
          out->write("eventIn ");
          break;
        case SoField::EVENTOUT_FIELD:
          out->write("eventOut ");
          break;
        case SoField::EXPOSED_FIELD:
          out->write("exposedField ");
          writevalue = TRUE;
          break;
        default:
          break;
        }
        out->write(f->getTypeId().getName().getString());
        out->write(' ');
        out->write(fieldname.getString());
        if (writevalue) {
          ((SoField*)f)->get(value);
          out->write(' ');
          out->write(value.getString());
        }
        out->write("\n");
      }
    }
    fd->write(out, this);
    this->writeFooter(out);
  }
  else assert(0 && "unknown stage");
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

  SoField * builtinfield;

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
    else if ((builtinfield = this->getField(name)) != NULL) {
      err = !builtinfield->read(in, name);
      if (!err) {
        name = "";
        ok = in->read(name, TRUE);
      }
    }
    else ok = FALSE;
  }
  if (!err) {
    if (name != "") in->putBack(name.getString());
    //    return inherited::readInstance(in, flags);
    return TRUE;
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
