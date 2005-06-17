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

// FIXME: idea from thammer; make all the various operations connect
// through callback-plugins, for maximum flexibility. 20050606 mortene.

// FIXME: another idea; factor out all the internal Javascript
// handling, connecting up to fields, etc etc to a more generic
// internal interface -- as thammer's project would like to connect
// Javascript programs/routines to any scene graph field. 20050606 mortene.

// *************************************************************************

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

// FIXME: is this really necessary? Coin/src/Makefile.am stops
// traversal into src/vrml97/, so it seems
// superfluous. Investigate. 20050526 mortene.
#ifdef HAVE_VRML97

// *************************************************************************

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
  (<http://www.web3d.org/x3d/specifications/vrml/ISO-IEC-14772-IS-VRML97WithAmendment1/part1/concepts.html#4.5>).

  Browsers are not required to support any specific language. Detailed
  information on scripting languages is described in 4.12, Scripting
  (<http://www.web3d.org/x3d/specifications/vrml/ISO-IEC-14772-IS-VRML97WithAmendment1/part1/concepts.html#4.12>).

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

  If the Script node's \e mustEvaluate field is \c FALSE, the browser
  may delay sending input events to the script until its outputs are
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

  If the Script node's \e directOutput field is \c TRUE, the script
  may also send events directly to any node to which it has access,
  and may dynamically establish or break routes.

  If directOutput is \c FALSE (the default), the script may only
  affect the rest of the world via events sent through its
  eventOuts. The results are undefined if directOutput is \c FALSE and
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

// *************************************************************************

#include <Inventor/VRMLnodes/SoVRMLScript.h>

#include <assert.h>

#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/SbName.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/misc/SoProto.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/sensors/SoOneShotSensor.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/C/glue/spidermonkey.h>
#include <Inventor/C/tidbitsp.h>

// *************************************************************************

static SoVRMLScriptEvaluateCB * sovrmlscript_eval_cb = NULL;
static void * sovrmlscript_eval_closure = NULL;

// *************************************************************************

class SoVRMLScriptP {
public:
  SoVRMLScriptP(SoVRMLScript * m)
  {
    this->master = m;
    this->oneshotsensor = new SoOneShotSensor(SoVRMLScript::eval_cb, master);
    this->isreading = FALSE;
    this->isevaluating = FALSE;
  }

  ~SoVRMLScriptP()
  {
    delete this->oneshotsensor;
  }

  static void cleanup(void);

  static SbBool debug(void);
  static SbBool allowSpiderMonkey(void);
  static SbBool useSpiderMonkey(void);

  void evaluate(void);

  SbBool isreading, isevaluating;
  SoOneShotSensor * oneshotsensor;

  static int jsinit;
  static void initJS(void); // FIXME: should there be an engine for
                            // each Script node, or just one global,
                            // or...? Check the VRML spec. 20050602 mortene.
  static void shutdownJS(void);

  static struct JS {
    JSRuntime * rt;
    JSContext * cx;
    JSObject * global;
    JSClass globalclassdef;
  } SpiderMonkey;

  SbList<SbName> fieldnotifications, eventoutfields;
  void evaluateAllScripts(void);
  void executeScript(const char * script) const;
  void executeFunctions(void);

  static void field2jsval(const SoField * f, jsval * v);
  static void jsval2field(const jsval v, SoField * f);

private:
  SoVRMLScript * master;
};

#define PUBLIC(p) ((p)->master)
#define PRIVATE(p) ((p)->pimpl)

SoVRMLScriptP::JS SoVRMLScriptP::SpiderMonkey;
SbBool SoVRMLScriptP::jsinit = FALSE;

void
SoVRMLScriptP::cleanup(void)
{
  SoVRMLScriptP::shutdownJS();
  SoVRMLScriptP::jsinit = FALSE;
}

// *************************************************************************

SbBool
SoVRMLScriptP::debug(void)
{
  static int d = -1;
  if (d == -1) {
    const char * env = coin_getenv("COIN_DEBUG_VRMLSCRIPT");
    d = (env && (atoi(env) > 0)) ? 1 : 0;
  }
  return d ? TRUE : FALSE;
}

// The Javascript support is far from being compliant with the VRML
// specification, and has so far been developed just for internal SIM
// use, so one needs to explicitly activate it for now.
SbBool
SoVRMLScriptP::allowSpiderMonkey(void)
{
  static int d = -1;
  if (d == -1) {
    const char * env = coin_getenv("COIN_ALLOW_SPIDERMONKEY");
    d = (env && (atoi(env) > 0)) ? 1 : 0;
  }
  return d ? TRUE : FALSE;
}

SbBool
SoVRMLScriptP::useSpiderMonkey(void)
{
  if (!SoVRMLScriptP::allowSpiderMonkey()) { return FALSE; }
  if (!spidermonkey()->available) { return FALSE; }
  if (SoVRMLScriptP::SpiderMonkey.rt == NULL) { return FALSE; }
  return TRUE;
}

// *************************************************************************

SoType SoVRMLScript::classTypeId STATIC_SOTYPE_INIT;

// Doc in superclass
void
SoVRMLScript::initClass(void) // static
{
  SoVRMLScript::classTypeId =
    SoType::createType(SoNode::getClassTypeId(),
                       SbName("VRMLScript"),
                       SoVRMLScript::createInstance,
                       SoNode::nextActionMethodIndex++);
  SoNode::setCompatibilityTypes(SoVRMLScript::getClassTypeId(), SO_VRML97_NODE_TYPE);
}

// *************************************************************************

SoVRMLScript::SoVRMLScript(void)
  : fielddata(NULL)
{
  if (!SoVRMLScriptP::jsinit && SoVRMLScriptP::allowSpiderMonkey()) {
    SoVRMLScriptP::initJS();
    SoVRMLScriptP::jsinit = TRUE;
    coin_atexit((coin_atexit_f *)SoVRMLScriptP::cleanup, 0);
  }

  PRIVATE(this) = new SoVRMLScriptP(this);
  this->setNodeType(SoNode::VRML2);
  this->isBuiltIn = TRUE;

  this->isBuiltIn = TRUE;
  assert(SoVRMLScript::classTypeId != SoType::badType());

  this->url.setNum(0);
  this->url.setContainer(this);

  // FIXME: directOutput should default be FALSE, according to the
  // VRML97 spec doc. 20050526 mortene.
  this->directOutput.setValue(TRUE);
  this->directOutput.setContainer(this);

  // FIXME: shouldn't mustEvaluate be default FALSE? Seems like it
  // from the VRML97 spec doc. 20050526 mortene.
  this->mustEvaluate.setValue(TRUE);
  this->mustEvaluate.setContainer(this);

  this->initFieldData();
}

SoVRMLScript::~SoVRMLScript()
{
  delete PRIVATE(this);

  const int n = this->fielddata->getNumFields();
  for (int i = 0; i < n; i++) {
    SoField * f = this->fielddata->getField(this, i);
    if (f != &this->directOutput &&
        f != &this->url &&
        f != &this->mustEvaluate) delete f;
  }
  delete this->fielddata;
}

// *************************************************************************

// Doc in superclass
SoType
SoVRMLScript::getClassTypeId(void)
{
  return SoVRMLScript::classTypeId;
}

// Doc in superclass
SoType
SoVRMLScript::getTypeId(void) const
{
  return SoVRMLScript::classTypeId;
}

// *************************************************************************

/*!  
  Sets the callback that will be called when the script needs to be
  evaluated.  
*/
void 
SoVRMLScript::setScriptEvaluateCB(SoVRMLScriptEvaluateCB * cb,
                                  void * closure)
{
  sovrmlscript_eval_cb = cb;
  sovrmlscript_eval_closure = closure;
}

// *************************************************************************

// Doc in superclass
void
SoVRMLScript::doAction(SoAction * action)
{
}

// Doc in superclass
void
SoVRMLScript::callback(SoCallbackAction * action)
{
  SoVRMLScript::doAction((SoAction*) action);
}

// Doc in superclass
void
SoVRMLScript::GLRender(SoGLRenderAction * action)
{
  SoVRMLScript::doAction((SoAction*) action);
}

// Doc in superclass
void
SoVRMLScript::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoVRMLScript::doAction((SoAction*) action);
}

// Doc in superclass
void
SoVRMLScript::pick(SoPickAction * action)
{
  SoVRMLScript::doAction((SoAction*) action);
}

// Doc in superclass
void
SoVRMLScript::handleEvent(SoHandleEventAction * action)
{
  SoVRMLScript::doAction((SoAction*) action);
}

// *************************************************************************

// Doc in superclass
void
SoVRMLScript::write(SoWriteAction * action)
{
  int i;
  const SbName URL("url");
  const SbName DIRECTOUTPUT("directOutput");
  const SbName MUSTEVALUATE("mustEvaluate");
  const SoFieldData * fd = this->getFieldData();

  SoOutput * out = action->getOutput();
  if (out->getStage() == SoOutput::COUNT_REFS) {
    // We will always write NORMAL and EXPOSED fields, so do a
    // setDefault(FALSE) on them. We will not write other field types,
    // so do a setDefault(TRUE) on them.
    for (i = 0; i < fd->getNumFields(); i++) {
      const SoField * f = fd->getField(this, i);
      SbName fieldname = fd->getFieldName(i);
      if (fieldname != URL && fieldname != DIRECTOUTPUT &&
          fieldname != MUSTEVALUATE) {
        if ((f->getFieldType() == SoField::NORMAL_FIELD) ||
            (f->getFieldType() == SoField::EXPOSED_FIELD)) {
          ((SoField*)f)->setDefault(FALSE);
        }
        else ((SoField*)f)->setDefault(TRUE);
      }
    }
    inherited::write(action);
  }
  else if (out->getStage() == SoOutput::WRITE) {
    if (this->writeHeader(out, FALSE, FALSE))
      return;
    for (i = 0; i < fd->getNumFields(); i++) {
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

        if (writevalue) {
          f->write(out, fieldname);
        }
        else {
          out->write(fieldname.getString());
        }
        out->write("\n");
        ((SoField*)f)->setDefault(TRUE); // don't write again when we do a SoFieldData::write later. 
      }
    }
    fd->write(out, this);
    this->writeFooter(out);
  }
  else assert(0 && "unknown stage");
}

// *************************************************************************

// Doc in superclass
void
SoVRMLScript::copyContents(const SoFieldContainer * from,
                           SbBool copyConn)
{
  assert(from->isOfType(SoVRMLScript::getClassTypeId()));

  const SoVRMLScript * fromnode = (SoVRMLScript*) from;

  const SoFieldData * src = from->getFieldData();
  const int n = src->getNumFields();
  for (int i = 0; i < n; i++) {
    const SoField * f = src->getField(from, i);
    if (f != &fromnode->directOutput &&
        f != &fromnode->url &&
        f != &fromnode->mustEvaluate) {
      SoField * cp = (SoField*) f->getTypeId().createInstance();
      cp->setFieldType(f->getFieldType());
      cp->setContainer(this);
      this->fielddata->addField(this, src->getFieldName(i), cp);
    }
  }
  inherited::copyContents(from, copyConn);
}

// *************************************************************************

// Doc in superclass
void 
SoVRMLScript::notify(SoNotList * l)
{
  const SoField * f = l->getLastField();

  if (!PRIVATE(this)->isreading && !PRIVATE(this)->isevaluating) {
    if (f == &this->mustEvaluate) {
      int pri = this->mustEvaluate.getValue() ? 0 : 
        SoDelayQueueSensor::getDefaultPriority();
      PRIVATE(this)->oneshotsensor->setPriority(pri);
    }
    else {
      SbName name;
      SbBool ok = this->getFieldName(f, name);
      assert(ok);
      if (PRIVATE(this)->fieldnotifications.find(name) == -1) {
        PRIVATE(this)->fieldnotifications.append(name);
      }

      if (!PRIVATE(this)->oneshotsensor->isScheduled()) {
        PRIVATE(this)->oneshotsensor->schedule();
      }
    }
  }

  if (f == &this->url) { PRIVATE(this)->evaluateAllScripts(); }

  inherited::notify(l);
}

// *************************************************************************

// Doc in superclass
void *
SoVRMLScript::createInstance(void)
{
  return (void *)new SoVRMLScript;
}

// Doc in superclass
const SoFieldData *
SoVRMLScript::getFieldData(void) const
{
  return this->fielddata;
}

// *************************************************************************

// Doc in superclass
SbBool
SoVRMLScript::readInstance(SoInput * in, unsigned short flags)
{
  // avoid triggering the eval cb while reading the file.
  PRIVATE(this)->isreading = TRUE;

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
            PRIVATE(this)->eventoutfields.append(fname);

            if (SoVRMLScriptP::useSpiderMonkey()) {
              jsval initval;
              SoVRMLScriptP::field2jsval(f, &initval);
              const JSBool ok =
                spidermonkey()->JS_SetProperty(SoVRMLScriptP::SpiderMonkey.cx,
                                               SoVRMLScriptP::SpiderMonkey.global,
                                               fname.getString(), &initval);
              if (!ok) {
                SoDebugError::post("SoVRMLScript::readInstance",
                                   "Could not set up eventOut field '%s' for "
                                   "Javascript engine.");
              }
            }
          }
          else if (name == EXPOSEDFIELD) {
            f->setFieldType(SoField::EXPOSED_FIELD);
          }
          f->setContainer(this);
          this->fielddata->addField(this, fname, f);
          if (name == FIELD || name == EXPOSEDFIELD) { // only read field values for fields
            err = ! f->read(in, fname);
            if (err) {
              SoReadError::post(in, "Unable to read default value for '%s'.", 
                                fname.getString());
            }
          }
          else {
            (void) in->checkISReference(this, fname, err);
            err = !err;
            if (err) {
              SoReadError::post(in, "Error while parsing IS reference for '%s'.", 
                                fname.getString());
            }
          }
        }
        else {
          err = TRUE;
          SoReadError::post(in, "Unknown field type.");
        }
      }
      else {
        SoReadError::post(in, "Unable to read field name.");
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
      else {
        SoReadError::post(in, "Error while reading field '%s'.", 
                          name.getString());
      }
    }
    else ok = FALSE;
  }
  PRIVATE(this)->isreading = FALSE;
  
  if (!err) {
    if (name != "") in->putBack(name.getString());
    // evaluate script
    PRIVATE(this)->oneshotsensor->schedule();
  }
  return !err;
}

// *************************************************************************

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

// *************************************************************************

void
SoVRMLScriptP::evaluateAllScripts(void)
{
  if (PUBLIC(this)->url.getNum() == 0) { return; }

  // FIXME: lift this limitation, of course. 20050602 mortene.
  if (PUBLIC(this)->url.getNum() == 1) {
    SbString s(PUBLIC(this)->url[0].getString());
    const char * prefix = "javascript:";
    const size_t prefixlen = strlen(prefix);

    // FIXME: lift this limitation, of course. 20050602 mortene.
    if ((s.getLength() > (int)prefixlen) && (s.getSubString(0, prefixlen - 1) == prefix)) {
      s = s.getSubString(prefixlen);
      const char * script = s.getString();
      this->executeScript(script);
    }
    else {
      SoDebugError::postInfo("SoVRMLScriptP::evaluateAllScripts",
                             "only \"javascript:\" URLs supported yet");
    }
  }
  else {
    SoDebugError::postInfo("SoVRMLScriptP::evaluateAllScripts",
                           "only supports a single script in the \"url\" field yet");
  }
}

// *************************************************************************

void 
SoVRMLScriptP::evaluate(void)
{
  if (SoVRMLScriptP::debug()) {
    SoDebugError::postInfo("SoVRMLScript::eval_cb", "invoked");
    for (int i = 0; i < this->fieldnotifications.getLength(); i++) {
      SoDebugError::postInfo("SoVRMLScriptP::evaluate",
                             "notification on field '%s'",
                             this->fieldnotifications[i].getString());
    }
  }

  if (sovrmlscript_eval_cb) {
    sovrmlscript_eval_cb(sovrmlscript_eval_closure, PUBLIC(this));
  }
  else if (SoVRMLScriptP::useSpiderMonkey()) {
    this->executeFunctions();
  }
  else {
    // FIXME: improve on this to be of more informational value to
    // both the app programmer and end-user. 20050526 mortene.
    static SbBool first = TRUE;
    if (first) {
      first = FALSE;
      SoDebugError::postWarning("SoVRMLScript::eval_cb",
                                "No script language evaluation engine available.");
    }
  }
}

// callback for oneshotsensor
void 
SoVRMLScript::eval_cb(void * data, SoSensor *)
{
  SoVRMLScript * thisp = (SoVRMLScript *)data;

  // FIXME: this wrapping is too simple, we can loose important new
  // input events. What we need to do is ignoring any writes to the
  // output field(s) that are updated. 20050602 mortene.
  PRIVATE(thisp)->isevaluating = TRUE;
  PRIVATE(thisp)->evaluate();
  PRIVATE(thisp)->isevaluating = FALSE;
}

// *************************************************************************

void
SoVRMLScriptP::field2jsval(const SoField * f, jsval * v)
{
  if (f->isOfType(SoSFBool::getClassTypeId())) {
    const SbBool val = ((SoSFBool *)f)->getValue();
    *v = BOOLEAN_TO_JSVAL(val);
  }
  else {
    // FIXME: fix, obviously. 20050602 mortene.
    SoDebugError::postInfo("SoVRMLScriptP::field2jsval",
                           "only know how to handle SoSFBool yet...");
  }
}

void
SoVRMLScriptP::jsval2field(const jsval v, SoField * f)
{
  if (f->isOfType(SoSFBool::getClassTypeId())) {
    const SbBool b = JSVAL_TO_BOOLEAN(v);
    ((SoSFBool *)f)->setValue(b);
  }
  else {
    // FIXME: fix, obviously. 20050602 mortene.
    SoDebugError::postInfo("SoVRMLScriptP::jsval2field",
                           "only know how to handle SoSFBool yet...");
  }
}

// *************************************************************************

void
SoVRMLScriptP::executeFunctions(void)
{
  int i;

  // Execute all functions for input fields that have been changed.

  for (i = 0; i < this->fieldnotifications.getLength(); i++) {
    const SbName & n = this->fieldnotifications[i];
    const SoField * f = PUBLIC(this)->getField(n);
    assert(f);

    jsval argv[1];
    SoVRMLScriptP::field2jsval(f, &argv[0]);

    jsval rval;
    JSBool ok =
      spidermonkey()->JS_CallFunctionName(SoVRMLScriptP::SpiderMonkey.cx,
                                          SoVRMLScriptP::SpiderMonkey.global,
                                          n.getString(), 1, argv, &rval);
    if (!ok) {
      SoDebugError::postWarning("SoVRMLScriptP::executeFunctions",
                                "JS_CallFunctionName(..., \"%s\", ...) "
                                "failed!", n.getString());
    }
  }
  this->fieldnotifications.truncate(0);
  

  // Then, pick up all new eventOut values.

  for (i = 0; i < this->eventoutfields.getLength(); i++) {
    jsval val;
    const SbName & name = this->eventoutfields[i];
    if (debug()) {
      SoDebugError::postInfo("SoVRMLScriptP::executeFunctions",
                             "Picking up new (?) value of eventOut '%s'...",
                             name.getString());
    }
    const JSBool ok =
      spidermonkey()->JS_GetProperty(SoVRMLScriptP::SpiderMonkey.cx,
                                     SoVRMLScriptP::SpiderMonkey.global,
                                     name.getString(), &val);
    if (!ok) {
      SoDebugError::post("SoVRMLScriptP::executeFunctions",
                         "Could not find eventOut '%s' as SpiderMonkey "
                         "object property!", name.getString());
      continue;
    }
    SoField * f = PUBLIC(this)->getEventOut(name);
    assert(f);

    // FIXME: should probably rather compare new value with old before
    // actually pushing it. Perhaps there's something about this in
    // the spec. 20050606 mortene.
    SoVRMLScriptP::jsval2field(val, f);
  }
}

// *************************************************************************

void
SoVRMLScriptP::executeScript(const char * script) const
{
  if (SoVRMLScriptP::debug()) {
    SoDebugError::postInfo("SoVRMLScript::executeScript", "script=='%s'", script);
  }

  jsval rval;
  JSBool ok =
    spidermonkey()->JS_EvaluateScript(SoVRMLScriptP::SpiderMonkey.cx,
                                      SoVRMLScriptP::SpiderMonkey.global,
                                      script, strlen(script),
                                      "inline-script", 1, &rval);
  if (SoVRMLScriptP::debug()) {
    SoDebugError::postInfo("SoVRMLScript::executeScript", "JS_EvaluateScript() => %d", ok);
  }

  if (ok) {
    JSString * str =
      spidermonkey()->JS_ValueToString(SoVRMLScriptP::SpiderMonkey.cx, rval);

    if (SoVRMLScriptP::debug()) {
      SoDebugError::postInfo("SoVRMLScript::executeScript",
                             "script result: '%s'",
                             spidermonkey()->JS_GetStringBytes(str));
    }
  }
  else {
    // FIXME: improve on this. 20050526 mortene.
    SoDebugError::postWarning("SoVRMLScript::executeScript",
                              "Script evaluation failed!");
  }
}

// *************************************************************************

static void
SpiderMonkey_ErrorHandler(JSContext * cx, const char * message, JSErrorReport * report)
{
  SoDebugError::postWarning("SpiderMonkey_ErrorHandler", "%s", message);
}

void
SoVRMLScriptP::initJS(void)
{
  if (!spidermonkey()->available) { return; }

  SoVRMLScriptP::SpiderMonkey.rt = NULL;
  SoVRMLScriptP::SpiderMonkey.cx = NULL;

  JSRuntime * rt = SoVRMLScriptP::SpiderMonkey.rt =
    spidermonkey()->JS_NewRuntime(/* maxbytes allocated before GC runs: */ 4 * 1024 * 1024);
  if (!rt) {
    SoDebugError::postWarning("SoVRMLScriptP::initJS",
                              "SpiderMonkey Javascript engine available, "
                              "but failed to instantiate a JSRuntime!");
    return;
  }

  JSContext * cx = SoVRMLScriptP::SpiderMonkey.cx =
    spidermonkey()->JS_NewContext(rt, /* stack chunk size: */ 8192);
  if (!cx) {
    SoDebugError::postWarning("SoVRMLScriptP::initJS",
                              "SpiderMonkey Javascript engine available, "
                              "but failed to set up a JSContext!");
    SoVRMLScriptP::shutdownJS();
    return;
  }

  (void)spidermonkey()->JS_SetErrorReporter(cx, SpiderMonkey_ErrorHandler);
  
  JSClass jclass = {
    "Coin SoVRMLScript global object class", 0,
    spidermonkey()->JS_PropertyStub,
    spidermonkey()->JS_PropertyStub,
    spidermonkey()->JS_PropertyStub,
    spidermonkey()->JS_PropertyStub,
    spidermonkey()->JS_EnumerateStub,
    spidermonkey()->JS_ResolveStub,
    spidermonkey()->JS_ConvertStub,
    spidermonkey()->JS_FinalizeStub,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0
  };

  SoVRMLScriptP::SpiderMonkey.globalclassdef = jclass;

  // FIXME: the eventOut fields are stored as variables (or
  // "properties" in JS-lingo) in this object, so this can not be
  // global for all VRMLScript nodes. 20050602 mortene.
  JSObject * global = SoVRMLScriptP::SpiderMonkey.global =
    spidermonkey()->JS_NewObject(cx, &SoVRMLScriptP::SpiderMonkey.globalclassdef, NULL, NULL);
  if (!global) {
    SoDebugError::postWarning("SoVRMLScriptP::initJS",
                              "SpiderMonkey Javascript engine available, "
                              "but failed to set up a global JSObject!");
    SoVRMLScriptP::shutdownJS();
    return;
  }

  JSBool ok = spidermonkey()->JS_InitStandardClasses(cx, global);
  if (!ok) {
    SoDebugError::postWarning("SoVRMLScriptP::initJS",
                              "SpiderMonkey Javascript engine available, "
                              "but failed to init standard classes for "
                              "global JSObject!");
    SoVRMLScriptP::shutdownJS();
    return;
  }
}

void
SoVRMLScriptP::shutdownJS(void)
{
  if (!spidermonkey()->available) { return; }

  if (SoVRMLScriptP::SpiderMonkey.cx) {
    spidermonkey()->JS_DestroyContext(SoVRMLScriptP::SpiderMonkey.cx);
  }

  if (SoVRMLScriptP::SpiderMonkey.rt) {
    spidermonkey()->JS_DestroyRuntime(SoVRMLScriptP::SpiderMonkey.rt);
  }

  spidermonkey()->JS_ShutDown();
}

// *************************************************************************

#undef PRIVATE
#undef PUBLIC

#endif // HAVE_VRML97
