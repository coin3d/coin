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
  \class SoSFMatrix SoSFMatrix.h Inventor/fields/SoSFMatrix.h
  \brief The SoSFMatrix class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoSFMatrix.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SoInput.h>
#include <Inventor/SbName.h>

#if !defined(COIN_EXCLUDE_SOSFROTATION)
#include <Inventor/fields/SoSFRotation.h>
#endif // !COIN_EXCLUDE_SOSFROTATION
#if !defined(COIN_EXCLUDE_SOSFSTRING)
#include <Inventor/fields/SoSFString.h>
#endif // !COIN_EXCLUDE_SOSFSTRING
#if !defined(COIN_EXCLUDE_SOMFMATRIX)
#include <Inventor/fields/SoMFMatrix.h>
#endif // !COIN_EXCLUDE_SOMFMATRIX

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG
#include <strstream.h>

// *************************************************************************

//$ BEGIN TEMPLATE SField( SoSFMatrix, const SbMatrix & )

SoType SoSFMatrix::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
SoSFMatrix::getTypeId(void) const
{
  return SoSFMatrix::classTypeId;
}

/*!
  Returns a unique type identifier for the SoSFMatrix class.

  \sa getTypeId(), SoType
 */
SoType
SoSFMatrix::getClassTypeId(void)
{
  return SoSFMatrix::classTypeId;
}

/*!
  Constructs and returns a new instance of the SoSFMatrix class.
*/
void *
SoSFMatrix::createInstance(void)
{
  return new SoSFMatrix;
}
/*!
  Copy all data from \a field into this object. \a field \e must
  be of the same type as the field we are copying into.
*/
void
SoSFMatrix::copyFrom(const SoField & field)
{
#if 0 // COIN_DEBUG
  // Calling field.getTypeId() here fails when "this" is connected to "field"
  // and "field" is destructed. The error message is "pure virtual method
  // called" with egcs 1.0.2 under Linux. 19990713 mortene.
  if (field.getTypeId() != this->getTypeId()) {
    SoDebugError::postWarning("SoSFMatrix::copyFrom",
                              "not of the same type: (this) '%s' (from) '%s'",
                              this->getTypeId().getName().getString(),
                              field.getTypeId().getName().getString());
    return;
  }
#endif // COIN_DEBUG

  this->operator=((const SoSFMatrix &)field);
}

/*!
  Tests \a field against this field for equality. Returns \a FALSE if they
  are not of the same type, or if they do not contain the same data.
*/
SbBool
SoSFMatrix::isSame(const SoField & field) const
{
  if (field.getTypeId() != this->getTypeId()) return FALSE;
  return this->operator==((const SoSFMatrix &) field);
}

/*!
  Copy field value from \a field into this object.
*/
const SoSFMatrix &
SoSFMatrix::operator = (const SoSFMatrix & field)
{
  this->setValue(field.getValue());
  return *this;
}

/*!
  Constructor.
*/
SoSFMatrix::SoSFMatrix(void)
{
  // Make sure we have initialized class.
  assert(SoSFMatrix::classTypeId != SoType::badType());
}

/*!
  Destructor.
*/
SoSFMatrix::~SoSFMatrix()
{
}

/*!
  Set this field's value.

  \sa getValue()
*/
void
SoSFMatrix::setValue(const SbMatrix & value)
{
  this->value = value;
  // FIXME: how about doing a new vs old comparison here? 19990620 mortene.
  this->valueChanged();
}

/*!
  Returns \a TRUE if this field is equal to \a field.
*/
SbBool
SoSFMatrix::operator == (const SoSFMatrix & field) const
{
  return (this->getValue() == field.getValue());
}
//$ END TEMPLATE SField


/*!
  Does initialization common for all objects of the
  SoSFMatrix class. This includes setting up the
  type system, among other things.
*/
void
SoSFMatrix::initClass(void)
{
//$ BEGIN TEMPLATE FieldInitClass( SFMatrix )
  // Make sure we only initialize once.
  assert(SoSFMatrix::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoSFMatrix::classTypeId =
    SoType::createType(SoSFMatrix::getClassTypeId(),
                       "SFMatrix", &SoSFMatrix::createInstance);
//$ END TEMPLATE FieldInitClass
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoSFMatrix::cleanClass(void)
{
}

SbBool
SoSFMatrix::readValue(SoInput * in)
{
  SbMat mat;

  SbBool result =
    in->read(mat[0][0]) && in->read(mat[0][1]) &&
    in->read(mat[0][2]) && in->read(mat[0][3]) &&
    in->read(mat[1][0]) && in->read(mat[1][1]) &&
    in->read(mat[1][2]) && in->read(mat[1][3]) &&
    in->read(mat[2][0]) && in->read(mat[2][1]) &&
    in->read(mat[2][2]) && in->read(mat[2][3]) &&
    in->read(mat[3][0]) && in->read(mat[3][1]) &&
    in->read(mat[3][2]) && in->read(mat[3][3]);
  
  if (result) this->value.setValue(mat);
  return result;
}

void
SoSFMatrix::writeValue(SoOutput * out) const
{
  if (out->isBinary()) {
    for(int i = 0; i < 4; i++) {
      for(int j = 0; j < 4; j++) {
	out->write(this->getValue()[i][j]);
      }
    }
    return;
  }


  for(int k=0; k < 4; k++) {
    out->write(this->getValue()[0][k]);
    if(k != 3) out->write(' ');
  }

  out->write('\n'); // FIXME: ok on mac/win32?
  out->incrementIndent();

  for(int i=1; i < 4; i++) {
    out->indent();
    for(int j=0; j < 4; j++) {
      out->write(this->getValue()[i][j]);
      if(j != 3) out->write(' ');
    }
    if(i != 3) out->write('\n'); // FIXME: ok on mac/win32?
  }

  out->decrementIndent();
}

/*!
  FIXME: write function documentation
*/
void
SoSFMatrix::setValue(const float a11, const float a12,
		     const float a13, const float a14,
		     const float a21, const float a22,
		     const float a23, const float a24,
		     const float a31, const float a32,
		     const float a33, const float a34,
		     const float a41, const float a42,
		     const float a43, const float a44)
{
  this->setValue(SbMatrix(a11,a12,a13,a14,a21,a22,a23,a24,
			  a31,a32,a33,a34,a41,a42,a43,a44));
}

void
SoSFMatrix::convertTo(SoField * dest) const
{
  if (0);
#if !defined(COIN_EXCLUDE_SOSFROTATION)
  else if (dest->getTypeId()==SoSFRotation::getClassTypeId()) {
    ((SoSFRotation *)dest)->setValue(this->getValue());
  }
#endif // !COIN_EXCLUDE_SOSFROTATION
#if !defined(COIN_EXCLUDE_SOSFSTRING)
  else if (dest->getTypeId()==SoSFString::getClassTypeId()) {
    ostrstream ostr;
    const SbMatrix mat=this->getValue();
    ostr << mat[0][0] << " " <<
      mat[0][1] << " " <<
      mat[0][2] << " " <<
      mat[0][3] << "\n\t" <<
      mat[1][0] << " " <<
      mat[1][1] << " " <<
      mat[1][2] << " " <<
      mat[1][3] << "\n\t" <<
      mat[2][0] << " " <<
      mat[2][1] << " " <<
      mat[2][2] << " " <<
      mat[2][3] << "\n\t" <<
      mat[3][0] << " " <<
      mat[3][1] << " " <<
      mat[3][2] << " " <<
      mat[3][3] << ends;

    ((SoSFString *)dest)->setValue(ostr.str());
  }
#endif // !COIN_EXCLUDE_SOSFSTRING
#if !defined(COIN_EXCLUDE_SOMFMATRIX)
  else if (dest->getTypeId()==SoMFMatrix::getClassTypeId()) {
    ((SoMFMatrix *)dest)->setValue(this->getValue());
  }
#endif // !COIN_EXCLUDE_SOMFMATRIX
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFMatrix::convertTo",
		       "Can't convert from %s to %s",
		       this->getTypeId().getName().getString(),
		       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
