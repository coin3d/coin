/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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
#include <Inventor/errors/SoReadError.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/nodes/SoGroup.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


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
}

SoUnknownNode::~SoUnknownNode()
{
  for (int i=0; i < this->classfielddata->getNumFields(); i++)
    delete this->classfielddata->getField(this, i);
  
  delete this->classfielddata;
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
                       "UnknownNode", // FIXME: correct? 20000103 mortene.
                       &SoUnknownNode::createInstance,
                       SoNode::nextActionMethodIndex++);
}

SbBool
SoUnknownNode::readInstance(SoInput * in, unsigned short flags)
{
  // Read fields.
  SbBool ok = inherited::readInstance(in, flags);
  if (!ok) return FALSE;

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

    // FIXME: "steal" children. 20000103 mortene.
    g->unref();
  }

  // Check that all field descriptions did actually get a value
  // set. It is not a fatal error if this didn't happen, but: 1) on
  // subsequent write operations, information will be lost, and 2) the
  // field value will probably be completely random because we're
  // reading from uninitialized memory.
  for (int i=0; i < this->classfielddata->getNumFields(); i++) {
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

// Overridden from SoBase.
const char *
SoUnknownNode::getFileFormatName(void) const
{
  return this->classname.getString();
}

// FIXME: handle "SoSFNode alternateRep" field descriptions. 20000103 mortene.
