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
#if !defined(COIN_EXCLUDE_SOMFNODE)
#include <Inventor/fields/SoMFNode.h>
#endif // !COIN_EXCLUDE_SOMFNODE
#include <Inventor/nodes/SoNode.h>
#include <Inventor/SbName.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoReadError.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

// *************************************************************************

//$ BEGIN TEMPLATE SFieldRequired(SoSFNode)

SoType SoSFNode::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
SoSFNode::getTypeId(void) const
{
  return SoSFNode::classTypeId;
}

/*!
  Returns a unique type identifier for the SoSFNode class.

  \sa getTypeId(), SoType
 */
SoType
SoSFNode::getClassTypeId(void)
{
  return SoSFNode::classTypeId;
}

/*!
  Constructs and returns a new instance of the SoSFNode class.
*/
void *
SoSFNode::createInstance(void)
{
  return new SoSFNode;
}
/*!
  Copy all data from \a field into this object. \a field \e must
  be of the same type as the field we are copying into.
*/
void
SoSFNode::copyFrom(const SoField & field)
{
#if 0 // COIN_DEBUG
  // Calling field.getTypeId() here fails when "this" is connected to "field"
  // and "field" is destructed. The error message is "pure virtual method
  // called" with egcs 1.0.2 under Linux. 19990713 mortene.
  if (field.getTypeId() != this->getTypeId()) {
    SoDebugError::postWarning("SoSFNode::copyFrom",
                              "not of the same type: (this) '%s' (from) '%s'",
                              this->getTypeId().getName().getString(),
                              field.getTypeId().getName().getString());
    return;
  }
#endif // COIN_DEBUG

  this->operator=((const SoSFNode &)field);
}

/*!
  Tests \a field against this field for equality. Returns \a FALSE if they
  are not of the same type, or if they do not contain the same data.
*/
SbBool
SoSFNode::isSame(const SoField & field) const
{
  if (field.getTypeId() != this->getTypeId()) return FALSE;
  return this->operator==((const SoSFNode &) field);
}

/*!
  Copy field value from \a field into this object.
*/
const SoSFNode &
SoSFNode::operator = (const SoSFNode & field)
{
  this->setValue(field.getValue());
  return *this;
}
//$ END TEMPLATE SFieldRequired

// *************************************************************************

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
    return (getValue() == field.getValue());
}

/*!
  Does initialization common for all objects of the
  SoSFNode class. This includes setting up the
  type system, among other things.
*/
void
SoSFNode::initClass(void)
{
//$ BEGIN TEMPLATE FieldInitClass(SFNode)
  // Make sure we only initialize once.
  assert(SoSFNode::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoSFNode::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "SFNode", &SoSFNode::createInstance);
//$ END TEMPLATE FieldInitClass
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
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
SbBool
SoSFNode::referencesCopy(void) const
{
  assert(0 && "FIXME: not implemented yet");
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
      assert(0 && "FIXME: not implemented yet");
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
  if (0);
#if !defined(COIN_EXCLUDE_SOMFNODE)
  else if (dest->getTypeId()==SoMFNode::getClassTypeId()) {
    ((SoMFNode *)dest)->setValue(this->getValue());
  }
#endif // !COIN_EXCLUDE_SOMFNODE
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFNode::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
