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
  \class SoMFNode SoMFNode.h Inventor/fields/SoMFNode.h
  \brief The SoMFNode class ...
  \ingroup fields

  FIXME: write class doc
*/

// FIXME:
/*¡
  SoMFNode::setValue() and SoMFNode::set1Value() should ref() and unref()
  nodes. I think. 19991113 mortene.
 */

#include <Inventor/errors/SoReadError.h>
#include <Inventor/fields/SoMFNode.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>
#include <Inventor/misc/SoBasic.h> // COIN_STUB()
#include <assert.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

// *************************************************************************

//$ BEGIN TEMPLATE MField(SoMFNode, SoNode *, SoNode *)

SoType SoMFNode::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
SoMFNode::getTypeId(void) const
{
  return SoMFNode::classTypeId;
}

/*!
  Returns a unique type identifier for the SoMFNode class.

  \sa getTypeId(), SoType
 */
SoType
SoMFNode::getClassTypeId(void)
{
  return SoMFNode::classTypeId;
}

/*!
  Constructs and returns a new instance of the SoMFNode class.
*/
void *
SoMFNode::createInstance(void)
{
  return new SoMFNode;
}
/*!
  Copy all data from \a field into this object. \a field \e must
  be of the same type as the field we are copying into.
*/
void
SoMFNode::copyFrom(const SoField & field)
{
#if 0 // COIN_DEBUG
  // Calling field.getTypeId() here fails when "this" is connected to "field"
  // and "field" is destructed. The error message is "pure virtual method
  // called" with egcs 1.0.2 under Linux. 19990713 mortene.
  if (field.getTypeId() != this->getTypeId()) {
    SoDebugError::postWarning("SoMFNode::copyFrom",
                              "not of the same type: (this) '%s' (from) '%s'",
                              this->getTypeId().getName().getString(),
                              field.getTypeId().getName().getString());
    return;
  }
#endif // COIN_DEBUG

  this->operator=((const SoMFNode &)field);
}

/*!
  Tests \a field against this field for equality. Returns \a FALSE if they
  are not of the same type, or if they do not contain the same data.
*/
SbBool
SoMFNode::isSame(const SoField & field) const
{
  if (field.getTypeId() != this->getTypeId()) return FALSE;
  return this->operator==((const SoMFNode &) field);
}

/*!
  Copy all field values from \a field into this object.
*/
const SoMFNode &
SoMFNode::operator = (const SoMFNode & field)
{
  if (field.getNum() < this->getNum()) this->deleteAllValues();
  this->setValues(0, field.getNum(), field.getValues(0));
  return *this;
}

/*!
  Constructor.
*/
SoMFNode::SoMFNode(void)
{
  // Make sure we have initialized class.
  assert(SoMFNode::classTypeId != SoType::badType());
  this->values = NULL;
}

/*!
  Destructor.
*/
SoMFNode::~SoMFNode(void)
{
  this->deleteAllValues();
}

/*!
  FIXME: write function documentation
*/
int
SoMFNode::fieldSizeof(void) const
{
  return sizeof(SoNode *);
}

/*!
  FIXME: write function documentation
*/
void *
SoMFNode::valuesPtr(void)
{
  return (void *)this->values;
}

/*!
  FIXME: write function documentation
*/
void
SoMFNode::setValuesPtr(void * ptr)
{
  this->values = (SoNode * *)ptr;
}

/*!
  FIXME: write function documentation
*/
int
SoMFNode::find(SoNode * value, SbBool addIfNotFound)
{
  for (int i=0; i < this->num; i++) if (this->values[i] == value) return i;

  if (addIfNotFound) this->set1Value(this->num, value);
  return -1;
}

/*!
  FIXME: write function documentation
*/
void
SoMFNode::setValues(const int start, const int num,
                       const SoNode * * newValues)
{
  if (start+num > this->maxNum) this->allocValues(start+num);
  else if (start+num > this->num) this->num = start+num;

  for (int i=0; i < num; i++)
    this->values[i+start] = (SoNode *) newValues[i];
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
void
SoMFNode::set1Value(const int idx, SoNode * value)
{
  if (idx+1 > this->maxNum) this->allocValues(idx+1);
  else if (idx+1 > this->num) this->num = idx+1;
  this->values[idx] = value;
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
void
SoMFNode::setValue(SoNode * value)
{
  this->allocValues(1);
  this->values[0] = value;
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
SbBool
SoMFNode::operator == (const SoMFNode & field) const
{
  if (this == &field) return TRUE;
  if (this->getNum() != field.getNum()) return FALSE;

  const SoNode * * const lhs = this->getValues(0);
  const SoNode * * const rhs = field.getValues(0);
  for (int i = 0; i < num; i++) if (lhs[i] != rhs[i]) return FALSE;
  return TRUE;
}

/*!
  FIXME: write function documentation
*/
void
SoMFNode::deleteAllValues(void)
{
  this->allocValues(0);
}

/*!
  FIXME: write function documentation
*/
void
SoMFNode::copyValue(int to, int from)
{
  assert(this->values && SbMax(to, from) < num);
  this->values[to] = this->values[from];
  this->valueChanged();
}
//$ END TEMPLATE MField

// *************************************************************************

/*!
  Does initialization common for all objects of the
  SoMFNode class. This includes setting up the
  type system, among other things.
*/
void
SoMFNode::initClass(void)
{
//$ BEGIN TEMPLATE FieldInitClass(MFNode)
  // Make sure we only initialize once.
  assert(SoMFNode::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoMFNode::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "MFNode", &SoMFNode::createInstance);
//$ END TEMPLATE FieldInitClass
}

// *************************************************************************


/*!
  FIXME: write function documentation
*/
SbBool
SoMFNode::read1Value(SoInput * in, int index)
{
  SoSFNode sfnode;
  SbBool result = sfnode.readValue(in);
  if (result) this->set1Value(index, sfnode.getValue());
  return result;
}

/*!
  FIXME: write function documentation
*/
void
SoMFNode::write1Value(SoOutput * out, int idx) const
{
  SoSFNode sfnode;
  sfnode.setValue((*this)[idx]);
  sfnode.writeValue(out);
}

/*!
  FIXME: write function documentation
*/
void
SoMFNode::fixCopy(SbBool /* copyConnections */)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
SbBool
SoMFNode::referencesCopy(void) const
{
  COIN_STUB();
  return FALSE;
}

void
SoMFNode::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFNode::getClassTypeId()) {
    if (this->getNum()>0)
      ((SoSFNode *)dest)->setValue((*this)[0]);
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoMFNode::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
