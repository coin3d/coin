/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

// This is an internal class for holding user extension nodes from .iv
// files. The header file for this node is not exported at ``make
// install'', and the class is not documented for application
// programmers.

#include <Inventor/nodes/SoUnknownNode.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/nodes/SoGroup.h>



// The following code is used instead of SO_NODE_SOURCE() to let
// SoUnknownNodes have dynamic handling of SoFieldData objects.

PRIVATE_NODE_TYPESYSTEM_SOURCE(SoUnknownNode);

const SoFieldData * SoUnknownNode::getFieldData(void) const
{
  return this->classfielddata;
}

void *
SoUnknownNode::createInstance(void)
{
  return new SoUnknownNode;
}


// Node implementation starts "proper".


SoUnknownNode::SoUnknownNode(void)
{
  /* Catch attempts to use a node class which has not been initialized. */
  assert(SoUnknownNode::classTypeId != SoType::badType());
  /* Initialize a fielddata container for the instance. */
  this->classfielddata = new SoFieldData;

  this->isBuiltIn = FALSE;
  this->privatechildren = NULL;
  this->alternate = new SoChildList(this, 1);
}

SoUnknownNode::~SoUnknownNode()
{
  for (int i=0; i < this->classfielddata->getNumFields(); i++)
    delete this->classfielddata->getField(this, i);

  delete this->classfielddata;
  delete this->privatechildren;
  delete this->alternate;
}

void
SoUnknownNode::initClass(void)
{
  /* Make sure we only initialize once. */
  assert(SoUnknownNode::classTypeId == SoType::badType());
  /* Make sure superclass gets initialized before subclass. */
  assert(inherited::getClassTypeId() != SoType::badType());

  /* Set up entry in the type system. */
  SoUnknownNode::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "UnknownNode",
                       &SoUnknownNode::createInstance,
                       SoNode::nextActionMethodIndex++);
}

SbBool
SoUnknownNode::readInstance(SoInput * in, unsigned short flags)
{
  // Read fields.
  SbBool ok = inherited::readInstance(in, flags);
  if (!ok) return FALSE;

  // Set pointer to alternateRep node, if SoSFNode field with this
  // name is present.
  int i;
  for (i=0; i < this->classfielddata->getNumFields(); i++) {
    if (this->classfielddata->getFieldName(i) == "alternateRep") {
      SoSFNode * f = (SoSFNode *)this->classfielddata->getField(this, i);
      if (f->isOfType(SoSFNode::getClassTypeId())) {
#if COIN_DEBUG && 0 // debug
        SoDebugError::postInfo("SoUnknownNode::readInstance",
                               "found alternate representation");
#endif // debug
        this->alternate->truncate(0);
        this->alternate->append(f->getValue());
      }
      break;
    }
  }

  // Read children, if necessary.
  if (!in->isBinary() || (flags & SoBase::IS_GROUP)) {
    SoGroup * g = new SoGroup;
    g->ref();
    if (!g->readChildren(in)) {
      g->unref();
      return FALSE;
    }

#if COIN_DEBUG && 0 // debug
    SoDebugError::postInfo("SoUnknownNode::readInstance",
                           "read %d children successfully",
                           g->getNumChildren());
#endif // debug

    delete this->privatechildren;
    this->privatechildren = new SoChildList(this, * g->getChildren());
    g->unref();
  }

  // Check that all field descriptions did actually get a value
  // set. It is not a fatal error if this didn't happen, but: 1) on
  // subsequent write operations, information will be lost, and 2) the
  // field value will probably be completely random because we're
  // reading from uninitialized memory.
  for (i=0; i < this->classfielddata->getNumFields(); i++) {
    const SoField * f = this->classfielddata->getField(this, i);
    if (f->isDefault()) {
      SoReadError::post(in, "Field ``%s'' in extension node not given any value.",
                        this->classfielddata->getFieldName(i).getString());
    }
  }

  return TRUE;
}

// The name used for the nodes of this type in scene file.
void
SoUnknownNode::setNodeClassName(const SbName & name)
{
  this->classname = name;
}

// Overloaded from SoBase.
const char *
SoUnknownNode::getFileFormatName(void) const
{
  return this->classname.getString();
}

// Overloaded from SoNode. SoChildList contains either 0 or 1
// elements, depending on if an alternate representation was
// specified.
SoChildList *
SoUnknownNode::getChildren(void) const
{
  return this->alternate;
}

// Write action method is overloaded from SoNode to handle children.
void
SoUnknownNode::write(SoWriteAction * action)
{
  SoOutput * out = action->getOutput();
  if (out->getStage() == SoOutput::COUNT_REFS) {
    inherited::write(action);
    // Only increase number of writereferences to the top level node
    // in a tree which is used multiple times.
    if (!this->hasMultipleWriteRefs())
      if (this->privatechildren) this->privatechildren->traverse(action);
  }
  else if (out->getStage() == SoOutput::WRITE) {
    if (this->writeHeader(out, this->privatechildren ? TRUE : FALSE, FALSE))
      return;
    this->writeInstance(out);
    if (out->isBinary())
      if (this->privatechildren) out->write(this->privatechildren->getLength());
    if (this->privatechildren) this->privatechildren->traverse(action);
    this->writeFooter(out);
  }
  else assert(0 && "unknown stage");
}

// Action methods overloaded from SoNode to traverse alternateRep (and
// below, if alternateRep is a group node).

void
SoUnknownNode::search(SoSearchAction * action)
{
  inherited::search(action);
  if (action->isFound()) return;
  this->alternate->traverse(action);
}

void
SoUnknownNode::GLRender(SoGLRenderAction * action)
{
  this->alternate->traverse(action);
}

void
SoUnknownNode::getBoundingBox(SoGetBoundingBoxAction * action)
{
  this->alternate->traverse(action);
}

void
SoUnknownNode::pick(SoPickAction * action)
{
  this->alternate->traverse(action);
}
