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

/*!
  \class SoSFNode SoSFNode.h Inventor/fields/SoSFNode.h
  \brief The SoSFNode class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoMFNode.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/SbName.h>
#include <Inventor/misc/SoBasic.h> // COIN_STUB()
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoReadError.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG



// FIXME: can we use SO_SFIELD_SOURCE() instead? Depends on whether or
// not we should do reference counting, I think. 19991227 mortene.
SO_SFIELD_REQUIRED_SOURCE(SoSFNode);


/*!
  FIXME: write function documentation
*/
void
SoSFNode::setValue(SoNode * newval)
{
  // FIXME: should we really ref/unref here? 19990630 mortene.

  if (this->value) this->value->unref();
  this->value = newval;
  if (this->value) this->value->ref();
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
SbBool
SoSFNode::operator == (const SoSFNode & field) const
{
  return (this->getValue() == field.getValue());
}

/*!
  Does initialization common for all objects of the
  SoSFNode class. This includes setting up the
  type system, among other things.
*/
void
SoSFNode::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFNode);
}

/*!
  Constructor.
*/
SoSFNode::SoSFNode(void)
{
  this->value = NULL;
}

/*!
  Destructor.
*/
SoSFNode::~SoSFNode(void)
{
  this->setValue(NULL);
}

/*!
  FIXME: write function documentation
*/
SbBool
SoSFNode::readValue(SoInput * in)
{
  SoBase * baseptr = NULL;
  SbBool result = SoBase::read(in, baseptr, SoNode::getClassTypeId());

  if (result) {
    if (in->eof()) {
      SoReadError::post(in, "Premature end of file");
      result = FALSE;
    }
    else {
      this->setValue((SoNode *)baseptr);
    }
  }
  return result;
}

/*!
  FIXME: write function documentation
*/
void
SoSFNode::fixCopy(SbBool /* copyConnections */)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
SbBool
SoSFNode::referencesCopy(void) const
{
  COIN_STUB();
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
void
SoSFNode::writeValue(SoOutput * out) const
{
#if 0 // OBSOLETED: ugly and error-prone. 19991113 mortene.

  // Note: make sure this code is in sync with the code in
  // SoNode::write(). Any changes here might need to be propagated to
  // that method.

  // FIXME: could this be done in a better way? As it stands now, we
  // need a "friend SoSFNode" definition in SoBase (writeHeader() and
  // writeFooter() are protected members). 19991112 mortene.

  SoNode * node = this->getValue();

  if (out->getStage() == SoOutput::COUNT_REFS) {
    node->addWriteReference(out, FALSE);
  }
  else if (out->getStage() == SoOutput::WRITE) {
    if (node) {
      if (node->writeHeader(out, FALSE, FALSE)) return;
      node->writeInstance(out);
      node->writeFooter(out);
    }
    else {
      COIN_STUB();
    }
  }
  else assert(0 && "unknown stage");

#else // new code

  SoNode * node = this->getValue();
  if (node) {
    SoWriteAction wa(out);
    wa.continueToApply(node);
  }
  else {
    // Yep, this'll work for both ASCII and binary formats.
    out->write("NULL");
  }

#endif // new code
}

void
SoSFNode::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoMFNode::getClassTypeId()) {
    ((SoMFNode *)dest)->setValue(this->getValue());
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFNode::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
