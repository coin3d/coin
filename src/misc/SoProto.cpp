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

#include <Inventor/misc/SoProto.h>
#include <Inventor/misc/SoProtoInstance.h>
#include <Inventor/SbName.h>
#include <Inventor/SoInput.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/lists/SoNodeList.h>
#include <Inventor/SbDict.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/engines/SoNodeEngine.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

static SoType soproto_type;

static SbList <SoProto*> * protolist;

#ifndef DOXYGEN_SKIP_THIS

class SoProtoP {
public:
  SoProtoP() : fielddata(NULL), defroot(NULL) { }

  SoFieldData * fielddata;
  SoGroup * defroot;
  SbName name;
  SbList <SoNode*> isnodelist; // FIXME: consider using SoNodeList 
  SbList <SbName> isfieldlist;
  SbList <SbName> isnamelist;
  SbDict refdict;
  SbList <SbName> routelist;
};

#endif // DOXYGEN_SKIP_THIS

// type system

SoType
SoProto::getTypeId(void) const
{
  return soproto_type;
}

SoType
SoProto::getClassTypeId(void)
{
  return soproto_type;
}

void
SoProto::initClass(void)
{
  soproto_type = SoType::createType(SoBase::getClassTypeId(),
                                    SbName("SoProto"));
  protolist = new SbList<SoProto*>;
}

#undef THIS
#define THIS this->pimpl

SoProto::SoProto(void)
{
  THIS = new SoProtoP;
  THIS->fielddata = new SoFieldData;
  THIS->defroot = new SoGroup;
  THIS->defroot->ref();

  protolist->insert(this, 0);
}

SoProto::~SoProto()
{
  const int n = THIS->fielddata->getNumFields();
  for (int i = 0; i < n; i++) {
    delete THIS->fielddata->getField(NULL, i);
  }
  THIS->defroot->unref();
  delete THIS;
}

SoProto *
SoProto::findProto(const SbName & name)
{
  if (protolist) {
    const int n = protolist->getLength();
    SoProto * const * ptr = protolist->getArrayPtr();
    for (int i = 0; i < n; i++) {
      if (ptr[i]->getProtoName() == name) return ptr[i];
    }
  }
  return NULL;
}

SoProtoInstance *
SoProto::findProtoInstance(SoNode * protoinstanceroot)
{
  return NULL;
}

SoProtoInstance *
SoProto::createProtoInstance(void)
{
  SoProtoInstance * inst = new SoProtoInstance(this,
                                               THIS->fielddata);
  inst->ref();
  inst->setRootNode(this->createInstanceRoot(inst));
  return inst;
}

SbName
SoProto::getProtoName(void) const
{
  return THIS->name;
}

/*!
  Overloaded to read Proto definition.
*/
SbBool
SoProto::readInstance(SoInput * in, unsigned short flags)
{
  SbName protoname;

  char c;
  SbBool ok = in->read(protoname, TRUE);
  if (ok) {
    THIS->name = protoname;
    ok = this->readInterface(in);
  }
  if (!ok) {
    SoReadError::post(in, "Error parsing PROTO interface.");
  }
  else {
    ok = in->read(c) && c == '{';
    if (ok) ok = this->readDefinition(in);
  }
  return ok;
}

void
SoProto::destroy(void)
{
  int idx = protolist->find(this);
  assert(idx >= 0);
  protolist->remove(idx);
  SoBase::destroy();
  // FIXME: remove from static list of PROTOs
}

void
SoProto::writeInstance(SoOutput * out)
{
}

void 
SoProto::addISReference(SoNode * container, 
                        const SbName & fieldname, 
                        const SbName & interfacename)
{
  assert(container->isOfType(SoNode::getClassTypeId()));

  fprintf(stderr,"Add IS Ref: %s %s %s\n",
          container->getTypeId().getName().getString(),
          fieldname.getString(), interfacename.getString());

  THIS->isnodelist.append(container);
  THIS->isfieldlist.append(fieldname);
  THIS->isnamelist.append(interfacename);
}

void
SoProto::addReference(const SbName & name, SoBase * base)
{
  THIS->refdict.enter((unsigned long)name.getString(), (void *) base);
}

void
SoProto::removeReference(const SbName & name)
{
  THIS->refdict.remove((unsigned long)name.getString());
}

SoBase *
SoProto::findReference(const SbName & name) const
{
  void * base;

  if (THIS->refdict.find((unsigned long)name.getString(), base))
    return (SoBase *) base;
  return NULL;
}

void
SoProto::addRoute(const SbName & fromnode, const SbName & fromfield,
                  const SbName & tonode, const SbName & tofield)
{
  THIS->routelist.append(fromnode);
  THIS->routelist.append(fromfield);
  THIS->routelist.append(tonode);
  THIS->routelist.append(tofield);
}

SbBool
SoProto::readInterface(SoInput * in)
{
  return THIS->fielddata->readFieldDescriptions(in, NULL, 4);

  const SbName EVENTIN("eventIn");
  const SbName EVENTOUT("eventOut");
  const SbName FIELD("field");
  const SbName EXPOSEDFIELD("exposedField");
  const SbName EOI("]");

  SbName itype;
  SbName ftype;
  SbName fname;

  SbBool ok = in->read(itype, FALSE);
  while (ok && itype != "") {
    ok = in->read(ftype, TRUE) && in->read(fname, TRUE);
    if (!ok) break;
    SoType type = SoType::fromName(ftype);
    ok = type.isDerivedFrom(SoField::getClassTypeId()) &&
      type.canCreateInstance();
    if (!ok) break;

    SoField * f = (SoField*) type.createInstance();

    f->setContainer(NULL);
    THIS->fielddata->addField(NULL, fname, f);

    if (itype == EVENTIN) {
      f->setFieldType(SoField::EVENTIN_FIELD);
    }
    else if (itype == EVENTOUT) {
      f->setFieldType(SoField::EVENTOUT_FIELD);
    }
    else if (itype == FIELD) {
      f->setFieldType(SoField::NORMAL_FIELD);
      ok = f->read(in, fname);
    }
    else if (itype == EXPOSEDFIELD) {
      f->setFieldType(SoField::EXPOSED_FIELD);
      ok = f->read(in, fname);
    }
    if (ok) ok = in->read(itype, FALSE);
  }
  return ok;
}

SbBool
SoProto::readDefinition(SoInput * in)
{
  SbBool ok = TRUE;
  SoBase * child;
  in->pushProto(this);

  while (ok) {
    ok = SoBase::read(in, child, SoNode::getClassTypeId());
    if (ok) {
      if (child == NULL) {
        if (in->eof()) {
          ok = FALSE;
          SoReadError::post(in, "Premature end of file");
        }
        break; // finished reading, break out
      }
      else {
        THIS->defroot->addChild((SoNode*) child);
      }
    }
  }
  in->popProto();
  char c;
  return ok && in->read(c) && c == '}';
}

SoNode *
SoProto::createInstanceRoot(SoProtoInstance * inst) const
{
  SoNode * root;
  if (THIS->defroot->getNumChildren() == 1)
    root = THIS->defroot->getChild(0);
  else root = THIS->defroot;

  SoNode * cpy;
  cpy = root->copy(FALSE);
  cpy->ref();
  this->connectISRefs(inst, root, cpy);
  cpy->unrefNoDelete();
  return cpy;
}

void
SoProto::connectISRefs(SoProtoInstance * inst, SoNode * src, SoNode * dst) const
{
  const int n = THIS->isfieldlist.getLength();

  SoSearchAction sa;
  for (int i = 0; i < n; i++) {
    SoNode * node = THIS->isnodelist[i];
    
    SbName fieldname = THIS->isfieldlist[i];
    SoField * dstfield = node->getField(fieldname);
    if (!dstfield) {
      if (node->isOfType(SoNodeEngine::getClassTypeId()) &&
          ((SoNodeEngine*)node)->getOutput(fieldname)) {
#if COIN_DEBUG && 1 // debug
        SoDebugError::postInfo("SoProto::connectISRefs",
                               "FIXME: implement eventOut (SoEngineOutput) alias support");
#endif // debug
        continue;
      }
#if COIN_DEBUG
      SoDebugError::postWarning("SoProto::connectISRefs",
                                "Destionation field '%s' is not found in node type '%s'. "
                                "Unable to resolve IS reference.",
                                fieldname.getString(), node->getTypeId().getName().getString());
#endif // COIN_DEBUG
      continue; // try next field
    }

    SbName iname = THIS->isnamelist[i];
#if 0
    fprintf(stderr,"Resolve IS Ref: %s %s %s\n",
            node->getTypeId().getName().getString(),
            fieldname.getString(), iname.getString());
#endif
    sa.setNode(node);
    sa.setInterest(SoSearchAction::FIRST);
    sa.setSearchingAll(TRUE);
    sa.apply(src);
    SoFullPath * path = (SoFullPath*) sa.getPath();
    assert(path);
    node = dst;
    
    // browse path to find the correct (copied) node.
    for (int k = 1; k < path->getLength(); k++) {
      int idx = path->getIndex(k);
      node = (*(node->getChildren()))[idx];
    }
    dstfield = node->getField(fieldname);
    assert(dstfield);
    SoField * srcfield = inst->getField(iname);
    if (srcfield) {
      dstfield->connectFrom(srcfield);
    }
    else {
      SoEngineOutput * output = NULL;
      if (inst->isOfType(SoNodeEngine::getClassTypeId())) {
        output = ((SoNodeEngine*) inst)->getOutput(iname);
      }
      if (output) {
        dstfield->connectFrom(output);
      }
#if COIN_DEBUG
      else {
        SoDebugError::postWarning("SoProto::connectISRefs",
                                  "Source field or engine output '%s' is not found in node type '%s'. "
                                  "Unable to resolve IS reference.",
                                  iname.getString(), node->getTypeId().getName().getString());
      }
#endif // COIN_DEBUG
    }
    sa.reset();
  }
}
