/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/


/*!
  \class SoProtoInstance SoProtoInstance.h Inventor/misc/SoProtoInstance.h
  \brief The SoProtoInstance class handles PROTO instances.
  
  FIXME: This is work in progress. Document later.
*/

#include <Inventor/misc/SoProtoInstance.h>
#include <Inventor/misc/SoProto.h>
#include <Inventor/SoInput.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/SbDict.h>
#include <Inventor/SoOutput.h>
#include <Inventor/actions/SoWriteAction.h>
#include <stdlib.h>
#include <assert.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#ifndef DOXYGEN_SKIP_THIS

class SoProtoInstanceP {
public:
  SoProtoInstanceP() :
    fielddata(NULL),
    protodef(NULL),
    root(NULL),
    rootsensor(NULL)
  { }

  SoFieldData * fielddata;
  SoProto * protodef;
  SoNode * root;
  SoNodeSensor * rootsensor;
};

#endif // DOXYGEN_SKIP_THIS


// The following code is used instead of SO_NODE_SOURCE() to let
// SoUnknownNodes have dynamic handling of SoFieldData objects.

PRIVATE_NODE_TYPESYSTEM_SOURCE(SoProtoInstance);

static SbDict * protoinstance_dict;

// doc in parent
void
SoProtoInstance::initClass(void)
{
 /* Make sure we only initialize once. */
  assert(SoProtoInstance::classTypeId == SoType::badType());
  /* Make sure superclass gets initialized before subclass. */
  assert(inherited::getClassTypeId() != SoType::badType());

  /* Set up entry in the type system. */
  SoProtoInstance::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "ProtoInstance",
                       NULL,
                       SoNode::nextActionMethodIndex++);

  protoinstance_dict = new SbDict;
  atexit(SoProtoInstance::cleanupClass);
}

void
SoProtoInstance::cleanupClass(void)
{
  delete protoinstance_dict;
}

#undef THIS
#define THIS this->pimpl

/*!
  Constructor.
*/
SoProtoInstance::SoProtoInstance(SoProto * proto,
                                 const SoFieldData * deffielddata)
{
  THIS = new SoProtoInstanceP;
  THIS->fielddata = new SoFieldData;
  THIS->protodef = proto;
  THIS->rootsensor = new SoNodeSensor;
  THIS->rootsensor->setDeleteCallback(sensorCB, this);
  if (proto) proto->ref();
  this->copyFieldData(deffielddata);
}

/*!
  Destructor.
*/
SoProtoInstance::~SoProtoInstance()
{
  this->setRootNode(NULL);
  delete THIS->rootsensor;
  const int n = THIS->fielddata->getNumFields();
  for (int i = 0; i < n; i++) {
    delete THIS->fielddata->getField(this, i);
  }
  delete THIS->fielddata;
  if (THIS->protodef) THIS->protodef->unref();
}

// doc in parent
const SoFieldData *
SoProtoInstance::getFieldData(void) const
{
  return THIS->fielddata;
}

/*!
  Returns the PROTO definition for this instance.
*/
SoProto *
SoProtoInstance::getProtoDefinition(void) const
{
  return THIS->protodef;
}

/*!
  Returns the PROTO defintion name.
*/
SbName
SoProtoInstance::getProtoName(void) const
{
  if (THIS->protodef) return THIS->protodef->getProtoName();
  return SbName("");
}

// Doc in parent
SbBool
SoProtoInstance::readInstance(SoInput * in, unsigned short flags)
{
  return inherited::readInstance(in, flags);
  //  return FALSE;
}

/*!
  Sets the root node for this instance.
*/
void
SoProtoInstance::setRootNode(SoNode * root)
{
  if (THIS->root) {
    protoinstance_dict->remove((unsigned long) root);
    THIS->rootsensor->detach();
  }
  THIS->root = root;
  if (root) {
    THIS->rootsensor->attach(root);
    protoinstance_dict->enter((unsigned long) root, (void*) this);
  }
}

/*!
  Returns the instance root node.
*/
SoNode *
SoProtoInstance::getRootNode(void)
{
  return THIS->root;
}

// Doc in parent
void
SoProtoInstance::write(SoWriteAction * action)
{
#if 0 // just testing, disabled pederb, 2002-06-18
  SoOutput * out = action->getOutput();
  if (out->getStage() == SoOutput::COUNT_REFS) {
    this->addWriteReference(out, FALSE);
  }
  else if (out->getStage() == SoOutput::WRITE) {
  }
  else assert(0 && "unknown stage");
#else
  inherited::write(action);
#endif
}

// Doc in parent
const char * 
SoProtoInstance::getFileFormatName(void) const
{
  return THIS->protodef->getProtoName().getString();
}

/*!
  Given root node \a rootnode, return the PROTO instance, or NULL if
  \a rootnode is not a PROTO instance root node.
*/
SoProtoInstance *
SoProtoInstance::findProtoInstance(const SoNode * rootnode)
{
  void * tmp;
  if (protoinstance_dict->find((unsigned long) rootnode, tmp)) {
    return (SoProtoInstance*) tmp;
  }
  return NULL;
}

// Doc in parent
void
SoProtoInstance::copyFieldData(const SoFieldData * src)
{
  const int n = src->getNumFields();
  SoFieldContainer::initCopyDict();
  for (int i = 0; i < n; i++) {
    SoField * f = src->getField(THIS->protodef, i);
    SoField * cp = (SoField*) f->getTypeId().createInstance();
    cp->setContainer(this);
    THIS->fielddata->addField(this, src->getFieldName(i), cp);
    if (f->getFieldType() == SoField::NORMAL_FIELD ||
        f->getFieldType() == SoField::EXPOSED_FIELD) {
      cp->copyFrom(*f);
      cp->fixCopy(TRUE);
    }
    cp->setDefault(f->isDefault());
  }
  SoFieldContainer::copyDone();
}

//
// Used to detect when the PROTO instance root node is destructed
//
void
SoProtoInstance::sensorCB(void * data, SoSensor *)
{
  SoProtoInstance * thisp = (SoProtoInstance*) data;
  thisp->pimpl->rootsensor->detach();
  thisp->pimpl->root = NULL;
  thisp->unref();
}


