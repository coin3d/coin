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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

/*!
  \class SoMFBitMask SoMFBitMask.h Inventor/fields/SoMFBitMask.h
  \brief The SoMFBitMask class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoMFBitMask.h>

#if !defined(COIN_EXCLUDE_SOSFBITMASK)
#include <Inventor/fields/SoSFBitMask.h>
#endif // !COIN_EXCLUDE_SOSFBITMASK
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>

// *************************************************************************

//$ BEGIN TEMPLATE MFieldRequired( SoMFBitMask )

SoType SoMFBitMask::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
SoMFBitMask::getTypeId(void) const
{
  return SoMFBitMask::classTypeId;
}

/*!
  Returns a unique type identifier for the SoMFBitMask class.

  \sa getTypeId(), SoType
 */
SoType
SoMFBitMask::getClassTypeId(void)
{
  return SoMFBitMask::classTypeId;
}

/*!
  Constructs and returns a new instance of the SoMFBitMask class.
*/
void *
SoMFBitMask::createInstance(void)
{
  return new SoMFBitMask;
}
/*!
  Copy all data from \a field into this object. \a field \e must
  be of the same type as the field we are copying into.
*/
void
SoMFBitMask::copyFrom(const SoField & field)
{
#if 0 // COIN_DEBUG
  // Calling field.getTypeId() here fails when "this" is connected to "field"
  // and "field" is destructed. The error message is "pure virtual method
  // called" with egcs 1.0.2 under Linux.
  if (field.getTypeId() != this->getTypeId()) {
    SoDebugError::postWarning("SoMFBitMask::copyFrom",
                              "not of the same type: (this) '%s' (from) '%s'",
                              this->getTypeId().getName().getString(),
                              field.getTypeId().getName().getString());
    return;
  }
#endif // COIN_DEBUG

  this->operator=((const SoMFBitMask &)field);
}

/*!
  Tests \a field against this field for equality. Returns \a FALSE if they
  are not of the same type, or if they do not contain the same data.
*/
SbBool
SoMFBitMask::isSame(const SoField & field) const
{
  if (field.getTypeId() != this->getTypeId()) return FALSE;
  return this->operator==((const SoMFBitMask &) field);
}

/*!
  Copy all field values from \a field into this object.
*/
const SoMFBitMask &
SoMFBitMask::operator = (const SoMFBitMask & field)
{
  if (field.getNum() < this->getNum()) this->deleteAllValues();
  this->setValues(0, field.getNum(), field.getValues(0));
  return *this;
}
//$ END TEMPLATE MFieldRequired

// *************************************************************************

/*!
  Does initialization common for all objects of the
  SoMFBitMask class. This includes setting up the
  type system, among other things.
*/
void
SoMFBitMask::initClass(void)
{
//$ BEGIN TEMPLATE FieldInitClass( MFBitMask )
  // Make sure we only initialize once.
  assert(SoMFBitMask::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoMFBitMask::classTypeId =
    SoType::createType(SoMFBitMask::getClassTypeId(),
                       "MFBitMask", &SoMFBitMask::createInstance);
//$ END TEMPLATE FieldInitClass
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoMFBitMask::cleanClass(void)
{
}

// *************************************************************************

/*!
  FIXME: write function documentation
*/
SbBool
SoMFBitMask::read1Value(SoInput * in, int idx)
{
  assert(0 && "FIXME: not implemented yet");
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
void
SoMFBitMask::write1Value(SoOutput * out, int idx) const
{
  SbBool paran=FALSE;
  int out_vals_written = 0;
  
  int restval = this->values[idx];
  int i=0;
  while (restval) {
    if (i>=numEnums) break;
    if (enumValues[i] & restval) {
      restval &= ~enumValues[i];
      if (!out_vals_written && restval) {
	out->write('(');
	paran=TRUE;
      }
      if (out_vals_written++) out->write(" | ");
      out->write((const char *)enumNames[i].getString());
    }

    i++;
  }
  if (paran) out->write(')');
  if (!out_vals_written) out->write("()");

  if(restval) {
    SoDebugError::post("SoMFBitMask::write1Value",
		       "unable to write some bits (0x%x)",
		       restval);
  }
}

/*!
  FIXME: write function documentation
*/
SbBool
SoMFBitMask::findEnumValue(const SbName & name, int & val)
{
  // Look through names table for one that matches
  for (int i = 0; i < numEnums; i++) {
    if (name == enumNames[i]) {
      val = enumValues[i];
      return TRUE;
    }
  }
  return FALSE;
}

void
SoMFBitMask::convertTo(SoField * dest) const
{
  if (0);
#if !defined(COIN_EXCLUDE_SOSFBITMASK)
  else if (dest->getTypeId()==SoSFBitMask::getClassTypeId()) {
    if (this->getNum()>0)
      ((SoSFBitMask *)dest)->setValue((*this)[0]);
  }
#endif // !COIN_EXCLUDE_SOSFBITMASK
#if COIN_DEBUG
  else {
    SoDebugError::post("SoMFBitMask::convertTo",
		       "Can't convert from %s to %s",
		       this->getTypeId().getName().getString(),
		       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
