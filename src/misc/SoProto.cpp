/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoProto SoProto.h Inventor/misc/SoProto.h
  \brief The SoProto class handles PROTO definitions.

  \internal
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
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/SoDB.h>

static SoType soproto_type;

static SbList <SoProto*> * protolist;
static SoFetchExternProtoCB * soproto_fetchextern_cb = NULL;
static void * soproto_fetchextern_closure = NULL;

static SoProto *
soproto_fetchextern_default_cb(SoInput * in,
                               const SbString * urls,
                               const int numurls,
                               void * closure)
{
  if (numurls == 0) return NULL;
  SbString filename(urls[0]);
  SbString name("");

  int nameidx = filename.find("#");
  if (nameidx >= 1) {
    SbString tmp(filename);
    filename = tmp.getSubString(0, nameidx-1);
    name = tmp.getSubString(nameidx+1);
  }
 
  if (!in->pushFile(filename.getString())) {
    SoReadError::post(in, "Unable to find EXTERNPROTO file: ``%s''",
                      filename.getString());
    return NULL;
  }

  SoSeparator * root = SoDB::readAll(in);
  if (!root) {
    // Take care of popping the file off the stack. This is a bit
    // "hack-ish", but its done this way instead of loosening the
    // protection of SoInput::popFile().
    if (in->getCurFileName() == filename) {
      char dummy;
      while (!in->eof()) in->get(dummy);
    }
    
    SoReadError::post(in, "Unable to read EXTERNPROTO file: ``%s''",
                      filename.getString());
    return NULL;
  }
  else {
    root->ref();
    SoProto * foundproto = NULL;

    SoSearchAction sa;
    sa.setType(SoProto::getClassTypeId());
    sa.setSearchingAll(TRUE);
    sa.setInterest(SoSearchAction::ALL);
    sa.apply(root);
    
    SoPathList & pl = sa.getPaths();

    if (pl.getLength() == 1) {
      foundproto = (SoProto*) pl[0]->getTail();
      if (name.getLength() && name != foundproto->getProtoName().getString()) {
        foundproto = NULL;
      }
    }
    else if (name.getLength()) {
      int i;
      for (i = 0; i < pl.getLength(); i++) {
        SoProto * proto = (SoProto*) pl[i]->getTail();
        if (name == proto->getProtoName().getString()) break;
      }
      if (i < pl.getLength()) {
        foundproto = (SoProto*) pl[i]->getTail();
      }
    }
    sa.reset(); // clear paths in action.
    if (foundproto) foundproto->ref();
    root->unref();
    if (foundproto) foundproto->unrefNoDelete();
    return foundproto;
  }

  // just in case to fool stupid compilers
  return NULL;
}

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
  SoProto * extprotonode;
};

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
                                    SbName("SoProto"), NULL,
                                    SoNode::nextActionMethodIndex++);
  protolist = new SbList<SoProto*>;
  // this will set a default callback
  SoProto::setFetchExternProtoCallback(NULL, NULL);
}

#define PRIVATE(obj) ((obj)->pimpl)

/*!
  Constructor.
*/
SoProto::SoProto(const SbBool externproto)
{
  PRIVATE(this) = new SoProtoP;
  PRIVATE(this)->externurl = NULL;
  if (externproto) {
    PRIVATE(this)->externurl = new SoMFString;
  }
  PRIVATE(this)->fielddata = new SoFieldData;
  PRIVATE(this)->defroot = new SoGroup;
  PRIVATE(this)->defroot->ref();
  PRIVATE(this)->extprotonode = NULL;

  protolist->insert(this, 0);
}

/*!
  Destructor.
*/
SoProto::~SoProto()
{
  const int n = PRIVATE(this)->fielddata->getNumFields();
  for (int i = 0; i < n; i++) {
    delete PRIVATE(this)->fielddata->getField(this, i);
  }
  PRIVATE(this)->defroot->unref();
  delete PRIVATE(this)->externurl;

  if (PRIVATE(this)->extprotonode) {
    PRIVATE(this)->extprotonode->unref();
  }
  delete PRIVATE(this);
}

void 
SoProto::setFetchExternProtoCallback(SoFetchExternProtoCB * cb,
                                     void * closure)
{
  if (cb == NULL) {
    soproto_fetchextern_cb = soproto_fetchextern_default_cb;
    soproto_fetchextern_closure = NULL;    
  }
  else {
    soproto_fetchextern_cb = cb;
    soproto_fetchextern_closure = closure;
  }
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
  if (PRIVATE(this)->extprotonode) {
    return PRIVATE(this)->extprotonode->createProtoInstance();
  }
  SoProtoInstance * inst = new SoProtoInstance(this,
                                               PRIVATE(this)->fielddata);
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
  return PRIVATE(this)->name;
}

// Documented in superclass. Overridden to read Proto definition.
SbBool
SoProto::readInstance(SoInput * in, unsigned short flags)
{
  SbName protoname;

  char c;
  SbBool ok = in->read(protoname, TRUE);
  if (ok) {
    PRIVATE(this)->name = protoname;
    ok = this->readInterface(in);
  }
  if (!ok) {
    SoReadError::post(in, "Error parsing PROTO interface.");
  }
  else if (!PRIVATE(this)->externurl) {
    ok = in->read(c) && c == '{';
    if (ok) ok = this->readDefinition(in);
  }
  else {
    ok = PRIVATE(this)->externurl->read(in, SbName("EXTERNPROTO URL"));
    if (ok) {
      SoProto * proto = soproto_fetchextern_cb(in,
                                               PRIVATE(this)->externurl->getValues(0),
                                               PRIVATE(this)->externurl->getNum(),
                                               soproto_fetchextern_closure);
      if (proto == NULL) {
        SoReadError::post(in, "Error reading EXTERNPROTO definition.");        
        ok = FALSE;
      }
      else {
        ok = this->setupExtern(in, proto);
      }
    }
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
    if (PRIVATE(this)->defroot && !PRIVATE(this)->externurl) {
      this->writeDefinition(action);
    }
  }
  else if (out->getStage() == SoOutput::WRITE) {
    out->indent();
    out->write(PRIVATE(this)->externurl ? "EXTERNPROTO " : "PROTO ");
    out->write(PRIVATE(this)->name.getString());
    out->write(" [\n");
    out->incrementIndent();

    this->writeInterface(out);

    out->decrementIndent();
    out->indent();
    out->write("]\n");
    if (PRIVATE(this)->externurl) {
      this->writeURLs(out);
    }
    else {
      out->indent();
      out->write("{\n");
      out->incrementIndent();
      
      if (PRIVATE(this)->defroot) {
        this->writeDefinition(action);
      }
      out->resolveRoutes();
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
  const SoFieldData * fd = PRIVATE(this)->fielddata;

  for (int i = 0; i < fd->getNumFields(); i++) {
    out->indent();
    const SoField * f = fd->getField(this, i);
    SoType t = f->getTypeId();
    switch (f->getFieldType()) {
    case SoField::NORMAL_FIELD:
      out->write("field ");
      out->write(t.getName().getString());
      if (PRIVATE(this)->externurl) {
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
      if (PRIVATE(this)->externurl) {
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
  SoGroup * def = PRIVATE(this)->defroot;

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
  // calling PRIVATE(this)->externurl->write(out, SbName("")) would also have
  // produced a valid VRML file.

  const int n = PRIVATE(this)->externurl->getNum();
  if (n == 1) {
    out->indent();
    out->write('\"');
    out->write((*PRIVATE(this)->externurl)[0].getString());
    out->write("\"\n");
  }
  else {
    out->indent();
    out->write("[\n");
    out->incrementIndent();
    for (int i = 0; i < n; i++) {
      out->indent();
      out->write('\"');
      out->write((*PRIVATE(this)->externurl)[i].getString());
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
  PRIVATE(this)->isnodelist.append(container);
  PRIVATE(this)->isfieldlist.append(fieldname);
  PRIVATE(this)->isnamelist.append(interfacename);
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
  const int n = PRIVATE(this)->isnodelist.getLength();
  for (int i = 0; i < n; i++) {
    if (PRIVATE(this)->isnodelist[i] == container &&
        PRIVATE(this)->isfieldlist[i] == fieldname) return PRIVATE(this)->isnamelist[i];
  }
  return SbName("");
}


/*!
  Adds a reference for this PROTO definition.
*/
void
SoProto::addReference(const SbName & name, SoBase * base)
{
  PRIVATE(this)->refdict.enter((unsigned long)name.getString(), (void *) base);
}

/*!
  Removes a reference for this PROTO definition.
*/
void
SoProto::removeReference(const SbName & name)
{
  PRIVATE(this)->refdict.remove((unsigned long)name.getString());
}

/*!
  Finds a reference for this PROTO definition.
*/
SoBase *
SoProto::findReference(const SbName & name) const
{
  void * base;

  if (PRIVATE(this)->refdict.find((unsigned long)name.getString(), base))
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
  PRIVATE(this)->routelist.append(fromnode);
  PRIVATE(this)->routelist.append(fromfield);
  PRIVATE(this)->routelist.append(tonode);
  PRIVATE(this)->routelist.append(tofield);
}

//
// Reads the interface
//
SbBool
SoProto::readInterface(SoInput * in)
{
  return PRIVATE(this)->fielddata->readFieldDescriptions(in, this, 4, PRIVATE(this)->externurl == NULL);
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
    PRIVATE(this)->fielddata->addField(NULL, fname, f);

    if (itype == EVENTIN) {
      f->setFieldType(SoField::EVENTIN_FIELD);
    }
    else if (itype == EVENTOUT) {
      f->setFieldType(SoField::EVENTOUT_FIELD);
    }
    else if (itype == FIELD) {
      f->setFieldType(SoField::NORMAL_FIELD);
      if (!PRIVATE(this)->externurl) {
        ok = f->read(in, fname);
      }
    }
    else if (itype == EXPOSEDFIELD) {
      f->setFieldType(SoField::EXPOSED_FIELD);
      if (!PRIVATE(this)->externurl) {
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
        PRIVATE(this)->defroot->addChild((SoNode*) child);
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
  if (PRIVATE(this)->extprotonode) {
    return PRIVATE(this)->extprotonode->createInstanceRoot(inst);
  }
  
  SoNode * root;
  if (PRIVATE(this)->defroot->getNumChildren() == 1)
    root = PRIVATE(this)->defroot->getChild(0);
  else root = PRIVATE(this)->defroot;
  
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
  if (PRIVATE(this)->externurl) {
    SoDebugError::postWarning("SoProto::connectISRefs",
                              "EXTERNPROTO URL fetching is not yet supported.");
    return;
  }

  const int n = PRIVATE(this)->isfieldlist.getLength();

  SoSearchAction sa;
  for (int i = 0; i < n; i++) {
    SoNode * node = PRIVATE(this)->isnodelist[i];
    
    SbName fieldname = PRIVATE(this)->isfieldlist[i];
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
    
    SbBool isprotoinstance = FALSE;
    if (node->isOfType(SoProtoInstance::getClassTypeId())) {
      node = ((SoProtoInstance*) node)->getRootNode();
      isprotoinstance = TRUE;
    }
    SbName iname = PRIVATE(this)->isnamelist[i];
    sa.setNode(node);
    sa.setInterest(SoSearchAction::FIRST);
    sa.setSearchingAll(TRUE);
    sa.apply(src);
    SoFullPath * path = (SoFullPath*) sa.getPath();
    if (!path) {
      SoDebugError::postWarning("SoProto::connectISRefs",
                                "Unable to resolve '%s' from '%s' in '%s' PROTO",
                                fieldname.getString(), iname.getString(), PRIVATE(this)->name.getString());

      continue;
    }
    node = dst;

    // browse path to find the correct (copied) node.
    for (int k = 1; k < path->getLength(); k++) {
      int idx = path->getIndex(k);
      node = (*(node->getChildren()))[idx];
    }
    if (dstfield) {
      if (isprotoinstance) {
        node = SoProtoInstance::findProtoInstance(node);
        assert(node);
      }
      dstfield = node->getField(fieldname);
    }
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

SbBool
SoProto::setupExtern(SoInput * in, SoProto * externproto)
{
  assert(externproto);  
  PRIVATE(this)->extprotonode = externproto;
  PRIVATE(this)->extprotonode->ref();
  return TRUE;
}

#undef PRIVATE

