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
  \class SoMFMatrix SoMFMatrix.h Inventor/fields/SoMFMatrix.h
  \brief The SoMFMatrix class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoMFMatrix.h>
#include <Inventor/fields/SoSFMatrix.h>

#if !defined(COIN_EXCLUDE_SOSFSTRING)
#include <Inventor/fields/SoSFString.h>
#endif // !COIN_EXCLUDE_SOSFSTRING
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG
#include <Inventor/SbName.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <assert.h>

#ifdef _WIN32
#include <strstrea.h>
#else // ! _WIN32
#include <strstream.h>
#endif // ! _WIN32

// *************************************************************************

//$ BEGIN TEMPLATE MField( SoMFMatrix, SbMatrix, const SbMatrix & )

SoType SoMFMatrix::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
SoMFMatrix::getTypeId(void) const
{
  return SoMFMatrix::classTypeId;
}

/*!
  Returns a unique type identifier for the SoMFMatrix class.

  \sa getTypeId(), SoType
 */
SoType
SoMFMatrix::getClassTypeId(void)
{
  return SoMFMatrix::classTypeId;
}

/*!
  Constructs and returns a new instance of the SoMFMatrix class.
*/
void *
SoMFMatrix::createInstance(void)
{
  return new SoMFMatrix;
}
/*!
  Copy all data from \a field into this object. \a field \e must
  be of the same type as the field we are copying into.
*/
void
SoMFMatrix::copyFrom(const SoField & field)
{
#if 0 // COIN_DEBUG
  // Calling field.getTypeId() here fails when "this" is connected to "field"
  // and "field" is destructed. The error message is "pure virtual method
  // called" with egcs 1.0.2 under Linux. 19990713 mortene.
  if (field.getTypeId() != this->getTypeId()) {
    SoDebugError::postWarning("SoMFMatrix::copyFrom",
                              "not of the same type: (this) '%s' (from) '%s'",
                              this->getTypeId().getName().getString(),
                              field.getTypeId().getName().getString());
    return;
  }
#endif // COIN_DEBUG

  this->operator=((const SoMFMatrix &)field);
}

/*!
  Tests \a field against this field for equality. Returns \a FALSE if they
  are not of the same type, or if they do not contain the same data.
*/
SbBool
SoMFMatrix::isSame(const SoField & field) const
{
  if (field.getTypeId() != this->getTypeId()) return FALSE;
  return this->operator==((const SoMFMatrix &) field);
}

/*!
  Copy all field values from \a field into this object.
*/
const SoMFMatrix &
SoMFMatrix::operator = (const SoMFMatrix & field)
{
  if (field.getNum() < this->getNum()) this->deleteAllValues();
  this->setValues(0, field.getNum(), field.getValues(0));
  return *this;
}

/*!
  Constructor.
*/
SoMFMatrix::SoMFMatrix(void)
{
  // Make sure we have initialized class.
  assert(SoMFMatrix::classTypeId != SoType::badType());
  this->values = NULL;
}

/*!
  Destructor.
*/
SoMFMatrix::~SoMFMatrix(void)
{
  this->deleteAllValues();
}

/*!
  FIXME: write function documentation
*/
int
SoMFMatrix::fieldSizeof(void) const
{
  return sizeof(SbMatrix);
}

/*!
  FIXME: write function documentation
*/
void *
SoMFMatrix::valuesPtr(void)
{
  return (void *)this->values;
}

/*!
  FIXME: write function documentation
*/
void
SoMFMatrix::setValuesPtr(void * ptr)
{
  this->values = (SbMatrix *)ptr;
}

/*!
  FIXME: write function documentation
*/
int
SoMFMatrix::find(const SbMatrix & value, SbBool addIfNotFound)
{
  for (int i=0; i < this->num; i++) if (this->values[i] == value) return i;

  if (addIfNotFound) this->set1Value(this->num, value);
  return -1;
}

/*!
  FIXME: write function documentation
*/
void
SoMFMatrix::setValues(const int start, const int num,
                       const SbMatrix * newValues)
{
  if (start+num > this->maxNum) this->allocValues(start+num);
  else if (start+num > this->num) this->num = start+num;

  for (int i=0; i < num; i++)
    this->values[i+start] = (SbMatrix) newValues[i];
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
void
SoMFMatrix::set1Value(const int idx, const SbMatrix & value)
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
SoMFMatrix::setValue(const SbMatrix & value)
{
  this->allocValues(1);
  this->values[0] = value;
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
SbBool
SoMFMatrix::operator == (const SoMFMatrix & field) const
{
  if (this == &field) return TRUE;
  if (this->getNum() != field.getNum()) return FALSE;

  const SbMatrix * const lhs = this->getValues(0);
  const SbMatrix * const rhs = field.getValues(0);
  for (int i = 0; i < num; i++) if (lhs[i] != rhs[i]) return FALSE;
  return TRUE;
}

/*!
  FIXME: write function documentation
*/
void
SoMFMatrix::deleteAllValues(void)
{
  this->allocValues(0);
}

/*!
  FIXME: write function documentation
*/
void
SoMFMatrix::copyValue(int to, int from)
{
  assert(this->values && SbMax(to, from) < num);
  this->values[to] = this->values[from];
  this->valueChanged();
}
//$ END TEMPLATE MField

// *************************************************************************

/*!
  Does initialization common for all objects of the
  SoMFMatrix class. This includes setting up the
  type system, among other things.
*/
void
SoMFMatrix::initClass(void)
{
//$ BEGIN TEMPLATE FieldInitClass( MFMatrix )
  // Make sure we only initialize once.
  assert(SoMFMatrix::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoMFMatrix::classTypeId =
    SoType::createType(SoMFMatrix::getClassTypeId(),
                       "MFMatrix", &SoMFMatrix::createInstance);
//$ END TEMPLATE FieldInitClass
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoMFMatrix::cleanClass(void)
{
}

// *************************************************************************

SbBool
SoMFMatrix::read1Value(SoInput * in, int idx)
{
  SoSFMatrix sfmatrix;
  SbBool result;
  if (result = sfmatrix.readValue(in)) this->set1Value(idx, sfmatrix.getValue());
  return result;
}

void
SoMFMatrix::write1Value(SoOutput * out, int idx) const
{
  SoSFMatrix sfmatrix;
  sfmatrix.setValue((*this)[idx]);
  sfmatrix.writeValue(out);
}

/*!
  FIXME: write function documentation
*/
void
SoMFMatrix::setValue(const float a11, const float a12,
		     const float a13, const float a14,
		     const float a21, const float a22,
		     const float a23, const float a24,
		     const float a31, const float a32,
		     const float a33, const float a34,
		     const float a41, const float a42,
		     const float a43, const float a44)
{
  this->setValue(SbMatrix(a11,a12,a13,a14,
			  a21,a22,a23,a24,
			  a31,a32,a33,a34,
			  a41,a42,a43,a44));
}

void
SoMFMatrix::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFMatrix::getClassTypeId()) {
    if (this->getNum()>0)
      ((SoSFMatrix *)dest)->setValue((*this)[0]);
  }
#if !defined(COIN_EXCLUDE_SOSFSTRING)
  else if (dest->getTypeId()==SoSFString::getClassTypeId()) {
    ostrstream ostr;
    int num=this->getNum();
    if (num>1) ostr << "[ ";
    for (int i=0;i<num;i++) {
      ostr << this->values[i][0][0] << " " <<
	this->values[i][0][1] << " " <<
	this->values[i][0][2] << " " <<
	this->values[i][0][3] << "\n\t";
      if (num>1) ostr << "  ";
      ostr << this->values[i][1][0] << " " <<
	this->values[i][1][1] << " " <<
	this->values[i][1][2] << " " <<
	this->values[i][1][3] << "\n\t";
      if (num>1) ostr << "  ";
      ostr << this->values[i][2][0] << " " <<
	this->values[i][2][1] << " " <<
	this->values[i][2][2] << " " <<
	this->values[i][2][3] << "\n\t";
      if (num>1) ostr << "  ";
      ostr << this->values[i][3][0] << " " <<
	this->values[i][3][1] << " " <<
	this->values[i][3][2] << " " <<
	this->values[i][3][3];
      if (i<num-1)
	ostr << ", ";
    }
    if (num>1) ostr << " ]";
    ostr << ends;
    ((SoSFString *)dest)->setValue(ostr.str());
  }
#endif // !COIN_EXCLUDE_SOSFSTRING
#if COIN_DEBUG
  else {
    SoDebugError::post("SoMFMatrix::convertTo",
		       "Can't convert from %s to %s",
		       this->getTypeId().getName().getString(),
		       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
