/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
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

#include <Inventor/misc/SoProtoInstance.h>
#include <Inventor/misc/SoProto.h>
#include <Inventor/SoInput.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <assert.h>

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
    delete THIS->fielddata->getField(this, 0);
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
  if (THIS->root) THIS->rootsensor->detach();
  THIS->root = root;
  if (root) THIS->rootsensor->attach(root);
}

SoNode * 
SoProtoInstance::getRootNode(void)
{
  return THIS->root;
}

void
SoProtoInstance::writeInstance(SoOutput * out)
{
}

void 
SoProtoInstance::copyFieldData(const SoFieldData * src)
{
  const int n = src->getNumFields();
  SoFieldContainer::initCopyDict();
  for (int i = 0; i < n; i++) {
    SoField * f = src->getField(NULL, i);
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
