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
  \class SoProto SoProto.h Inventor/misc/SoProto.h
  \brief The SoProto class handles PROTO definitions.
  
  FIXME: This is work in progress. Document later.
*/


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
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/engines/SoNodeEngine.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/SoOutput.h>

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
  SoMFString * externurl;
};

#endif // DOXYGEN_SKIP_THIS

// doc in parent
SoType
SoProto::getTypeId(void) const
{
  return soproto_type;
}

// doc in parent
SoType
SoProto::getClassTypeId(void)
{
  return soproto_type;
}

// doc in parent
void
SoProto::initClass(void)
{
  soproto_type = SoType::createType(SoNode::getClassTypeId(),
                                    SbName("SoProto"));
  protolist = new SbList<SoProto*>;
}

#undef THIS
#define THIS this->pimpl

/*!
  Constructor.
*/
SoProto::SoProto(const SbBool externproto)
{
  THIS = new SoProtoP;
  THIS->externurl = NULL;
  if (externproto) {
    THIS->externurl = new SoMFString;
  }
  THIS->fielddata = new SoFieldData;
  THIS->defroot = new SoGroup;
  THIS->defroot->ref();

  protolist->insert(this, 0);
}

/*!
  Destructor.
*/
SoProto::~SoProto()
{
  const int n = THIS->fielddata->getNumFields();
  for (int i = 0; i < n; i++) {
    delete THIS->fielddata->getField(this, i);
  }
  THIS->defroot->unref();
  delete THIS->externurl;
  delete THIS;
}

/*!
  Returns the PROTO definition named \a name or NULL if not found.
*/
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

/*!
  Creates an instance of the PROTO.
*/
SoProtoInstance *
SoProto::createProtoInstance(void)
{
  SoProtoInstance * inst = new SoProtoInstance(this,
                                               THIS->fielddata);
  inst->ref();
  inst->setRootNode(this->createInstanceRoot(inst));
  return inst;
}

/*!
  Returns the PROTO name.
*/
SbName
SoProto::getProtoName(void) const
{
  return THIS->name;
}

// Documented in superclass. Overridden to read Proto definition.
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
  else if (!THIS->externurl) {
    ok = in->read(c) && c == '{';
    if (ok) ok = this->readDefinition(in);
  }
  else {
    ok = THIS->externurl->read(in, SbName("EXTERNPROTO URL"));
    if (ok) {
    }
    // FIXME: fetch URL data.
  }
  return ok;
}

// Doc in parent
void
SoProto::destroy(void)
{
  int idx = protolist->find(this);
  assert(idx >= 0);
  protolist->remove(idx);
  SoBase::destroy();
  // FIXME: remove from static list of PROTOs
}

// doc in parent
void
SoProto::write(SoWriteAction * action)
{
  SoOutput * out = action->getOutput();
  out->pushProto(this);

  if (out->getStage() == SoOutput::COUNT_REFS) {
    this->addWriteReference(out, FALSE);
    if (THIS->defroot && !THIS->externurl) {
      this->writeDefinition(action);
    }
  }
  else if (out->getStage() == SoOutput::WRITE) {
    out->indent();
    out->write(THIS->externurl ? "EXTERNPROTO " : "PROTO ");
    out->write(THIS->name.getString());
    out->write(" [\n");
    out->incrementIndent();

    this->writeInterface(out);

    out->decrementIndent();
    out->indent();
    out->write("]\n");
    if (THIS->externurl) {
      this->writeURLs(out);
    }
    else {
      out->indent();
      out->write("{\n");
      out->incrementIndent();
      
      if (THIS->defroot) {
        this->writeDefinition(action);
      }
      out->resolveROUTEs();
      out->decrementIndent();
      out->indent();
      out->write("}\n");
    }
  }
  else assert(0 && "unknown stage");

  out->popProto();
}

//
// Writes the PROTO interface
//
SbBool
SoProto::writeInterface(SoOutput * out)
{
  const SoFieldData * fd = THIS->fielddata;

  for (int i = 0; i < fd->getNumFields(); i++) {
    out->indent();
    const SoField * f = fd->getField(this, i);
    SoType t = f->getTypeId();
    switch (f->getFieldType()) {
    case SoField::NORMAL_FIELD:
      out->write("field ");
      out->write(t.getName().getString());
      if (THIS->externurl) {
        out->write(' ');
        out->write(fd->getFieldName(i).getString());
        out->write("\n");
      }
      else {
        f->write(out, fd->getFieldName(i));
      }
      break;
    case SoField::EXPOSED_FIELD:
      out->write("exposedField ");
      out->write(t.getName().getString());
      if (THIS->externurl) {
        out->write(' ');
        out->write(fd->getFieldName(i).getString());        
        out->write("\n");
      }
      else {
        f->write(out, fd->getFieldName(i));
      }
      break;
    case SoField::EVENTIN_FIELD:
      out->write("eventIn ");
      out->write(t.getName().getString());
      out->write(fd->getFieldName(i).getString());
      break;
    case SoField::EVENTOUT_FIELD:
      out->write("eventOut ");
      out->write(t.getName().getString());
      out->write(fd->getFieldName(i).getString());
      break;
    default:
      assert(0 && "invalid field type");
      break;
    }
  }
  return TRUE;
}

//
// Writes the PROTO definition
//
SbBool
SoProto::writeDefinition(SoWriteAction * action)
{
  SoOutput * out = action->getOutput();
  SoGroup * def = THIS->defroot;

  if (out->getStage() == SoOutput::COUNT_REFS) {
    for (int i = 0; i < def->getNumChildren(); i++) {
      def->getChild(i)->write(action);
    }
  }
  else if (out->getStage() == SoOutput::WRITE) {
    for (int i = 0; i < def->getNumChildren(); i++) {
      def->getChild(i)->write(action);
    }
  }
  else assert(0 && "unknown stage");
  return TRUE;
}

SbBool 
SoProto::writeURLs(SoOutput * out)
{
  // We use this code to write the URLs to get nicer indentation. Just
  // calling THIS->externurl->write(out, SbName("")) would also have
  // produced a valid VRML file.

  const int n = THIS->externurl->getNum();
  if (n == 1) {
    out->indent();
    out->write('\"');
    out->write((*THIS->externurl)[0].getString());
    out->write("\"\n");
  }
  else {
    out->indent();
    out->write("[\n");
    out->incrementIndent();
    for (int i = 0; i < n; i++) {
      out->indent();
      out->write('\"');
      out->write((*THIS->externurl)[i].getString());
      out->write(i < n-1 ? "\",\n" : "\"\n");
    }
    out->decrementIndent();
    out->indent();
    out->write("]\n");
  }
  return TRUE;
}

/*!
  Adds an IS reference for this PROTO definition.
*/
void
SoProto::addISReference(SoNode * container,
                        const SbName & fieldname,
                        const SbName & interfacename)
{
  assert(container->isOfType(SoNode::getClassTypeId()));
  THIS->isnodelist.append(container);
  THIS->isfieldlist.append(fieldname);
  THIS->isnamelist.append(interfacename);
}

/*!
  If \a container is a PROTO definition node with an IS interface
  field named \a fieldname, return the interface name, otherwise
  return an empty SbName.
*/
SbName
SoProto::findISReference(const SoFieldContainer * container,
                         const SbName & fieldname)
{
  const int n = THIS->isnodelist.getLength();
  for (int i = 0; i < n; i++) {
    if (THIS->isnodelist[i] == container &&
        THIS->isfieldlist[i] == fieldname) return THIS->isnamelist[i];
  }
  return SbName("");
}


/*!
  Adds a reference for this PROTO definition.
*/
void
SoProto::addReference(const SbName & name, SoBase * base)
{
  THIS->refdict.enter((unsigned long)name.getString(), (void *) base);
}

/*!
  Removes a reference for this PROTO definition.
*/
void
SoProto::removeReference(const SbName & name)
{
  THIS->refdict.remove((unsigned long)name.getString());
}

/*!
  Finds a reference for this PROTO definition.
*/
SoBase *
SoProto::findReference(const SbName & name) const
{
  void * base;

  if (THIS->refdict.find((unsigned long)name.getString(), base))
    return (SoBase *) base;
  return NULL;
}

/*!
  Adds a ROUTE for this PROTO definition.
*/
void
SoProto::addRoute(const SbName & fromnode, const SbName & fromfield,
                  const SbName & tonode, const SbName & tofield)
{
  THIS->routelist.append(fromnode);
  THIS->routelist.append(fromfield);
  THIS->routelist.append(tonode);
  THIS->routelist.append(tofield);
}

//
// Reads the interface
//
SbBool
SoProto::readInterface(SoInput * in)
{
  return THIS->fielddata->readFieldDescriptions(in, this, 4, THIS->externurl == NULL);
#if 0 // OBSOLETED
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
      if (!THIS->externurl) {
        ok = f->read(in, fname);
      }
    }
    else if (itype == EXPOSEDFIELD) {
      f->setFieldType(SoField::EXPOSED_FIELD);
      if (!THIS->externurl) {
        ok = f->read(in, fname);
      }
    }
    if (ok) ok = in->read(itype, FALSE);
  }
  return ok;
#endif // OBSOLETED
}

//
// Reads the definition
//
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

//
// Create a root node for a PROTO instance
//
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

//
// Connects all IS references for the a new instance
//
void
SoProto::connectISRefs(SoProtoInstance * inst, SoNode * src, SoNode * dst) const
{
  if (THIS->externurl) {
    SoDebugError::postWarning("SoProto::connectISRefs",
                              "EXTERNPROTO URL fetching is not yet supported.");
    return;
  }

  const int n = THIS->isfieldlist.getLength();

  SoSearchAction sa;
  for (int i = 0; i < n; i++) {
    SoNode * node = THIS->isnodelist[i];

    SbName fieldname = THIS->isfieldlist[i];
    SoField * dstfield = node->getField(fieldname);
    SoEngineOutput * eventout = NULL;

    if (!dstfield) {
      if (node->isOfType(SoNodeEngine::getClassTypeId())) {
        eventout = ((SoNodeEngine*)node)->getOutput(fieldname);
      }
      if (!eventout) {
#if COIN_DEBUG
        SoDebugError::postWarning("SoProto::connectISRefs",
                                  "Destionation field '%s' is not found in node type '%s'. "
                                  "Unable to resolve IS reference.",
                                  fieldname.getString(), node->getTypeId().getName().getString());
#endif // COIN_DEBUG
        continue; // skip to next field
      }
    }

    SbName iname = THIS->isnamelist[i];
    sa.setNode(node);
    sa.setInterest(SoSearchAction::FIRST);
    sa.setSearchingAll(TRUE);
    sa.apply(src);
    SoFullPath * path = (SoFullPath*) sa.getPath();
    if (!path) {
      SoDebugError::postWarning("SoProto::connectISRefs",
                                "Unable to resolve '%s' from '%s' in '%s' PROTO",
                                fieldname.getString(), iname.getString(), THIS->name.getString());

      continue;
    }
    node = dst;

    // browse path to find the correct (copied) node.
    for (int k = 1; k < path->getLength(); k++) {
      int idx = path->getIndex(k);
      node = (*(node->getChildren()))[idx];
    }
    if (dstfield) dstfield = node->getField(fieldname);
    else {
      assert(node->isOfType(SoNodeEngine::getClassTypeId()));
      eventout = ((SoNodeEngine*)node)->getOutput(fieldname);
    }
    assert(dstfield || eventout);
    SoField * srcfield = inst->getField(iname);
    if (srcfield) {
      // if destination field is an eventOut field, or an EngineOutput,
      // reverse the connection, since we then just need to route the
      // events to the srcfield.
      if (eventout) {
        srcfield->connectFrom(eventout);
      }
      else if (dstfield->getFieldType() == SoField::EVENTOUT_FIELD) {
        srcfield->connectFrom(dstfield);
      }
      else {
        dstfield->connectFrom(srcfield);
      }
    }
    else {
      assert(dstfield);
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
