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

#include <Inventor/misc/SoProtoInstance.h>
#include <Inventor/misc/SoProto.h>
#include <Inventor/SoInput.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/SbDict.h>
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
}

void 
SoProtoInstance::cleanupClass(void)
{
  
}

#undef THIS
#define THIS this->pimpl

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

const SoFieldData *
SoProtoInstance::getFieldData(void) const
{
  return THIS->fielddata;
}

SoProto *
SoProtoInstance::getPROTODefinition(void) const
{
  return THIS->protodef;
}

SbName
SoProtoInstance::getProtoName(void) const
{
  if (THIS->protodef) return THIS->protodef->getProtoName();
  return SbName("");
}

SbBool
SoProtoInstance::readInstance(SoInput * in, unsigned short flags)
{
  return inherited::readInstance(in, flags);
  //  return FALSE;
}

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

SoNode *
SoProtoInstance::getRootNode(void)
{
  return THIS->root;
}

void
SoProtoInstance::writeInstance(SoOutput * out)
{
#if COIN_DEBUG && 1 // debug
  SoDebugError::postInfo("SoProtoInstance::write",
                         "Not implemented");
#endif // debug  
}

SoProtoInstance * 
SoProtoInstance::findProtoInstance(const SoNode * rootnode)
{
  void * tmp;
  if (protoinstance_dict->find((unsigned long) rootnode, tmp)) {
    return (SoProtoInstance*) tmp;
  }
  return NULL;
}

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
  }
  SoFieldContainer::copyDone();
}

void
SoProtoInstance::sensorCB(void * data, SoSensor *)
{
  SoProtoInstance * thisp = (SoProtoInstance*) data;
  thisp->pimpl->rootsensor->detach();
  thisp->pimpl->root = NULL;
  thisp->unref();
}
