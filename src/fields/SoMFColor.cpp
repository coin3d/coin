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
  \class SoMFColor SoMFColor.h Inventor/fields/SoMFColor.h
  \brief The SoMFColor class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoMFColor.h>
#if !defined(COIN_EXCLUDE_SOSFCOLOR)
#include <Inventor/fields/SoSFColor.h>
#endif // !COIN_EXCLUDE_SOSFCOLOR
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>
#include <assert.h>

// *************************************************************************

//$ BEGIN TEMPLATE MField( SoMFColor, SbColor, const SbColor & )

SoType SoMFColor::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
SoMFColor::getTypeId(void) const
{
  return SoMFColor::classTypeId;
}

/*!
  Returns a unique type identifier for the SoMFColor class.

  \sa getTypeId(), SoType
 */
SoType
SoMFColor::getClassTypeId(void)
{
  return SoMFColor::classTypeId;
}

/*!
  Constructs and returns a new instance of the SoMFColor class.
*/
void *
SoMFColor::createInstance(void)
{
  return new SoMFColor;
}
/*!
  Copy all data from \a field into this object. \a field \e must
  be of the same type as the field we are copying into.
*/
void
SoMFColor::copyFrom(const SoField & field)
{
#if 0 // COIN_DEBUG
  // Calling field.getTypeId() here fails when "this" is connected to "field"
  // and "field" is destructed. The error message is "pure virtual method
  // called" with egcs 1.0.2 under Linux.
  if (field.getTypeId() != this->getTypeId()) {
    SoDebugError::postWarning("SoMFColor::copyFrom",
                              "not of the same type: (this) '%s' (from) '%s'",
                              this->getTypeId().getName().getString(),
                              field.getTypeId().getName().getString());
    return;
  }
#endif // COIN_DEBUG

  this->operator=((const SoMFColor &)field);
}

/*!
  Tests \a field against this field for equality. Returns \a FALSE if they
  are not of the same type, or if they do not contain the same data.
*/
SbBool
SoMFColor::isSame(const SoField & field) const
{
  if (field.getTypeId() != this->getTypeId()) return FALSE;
  return this->operator==((const SoMFColor &) field);
}

/*!
  Copy all field values from \a field into this object.
*/
const SoMFColor &
SoMFColor::operator = (const SoMFColor & field)
{
  if (field.getNum() < this->getNum()) this->deleteAllValues();
  this->setValues(0, field.getNum(), field.getValues(0));
  return *this;
}

/*!
  Constructor.
*/
SoMFColor::SoMFColor(void)
{
  this->values = NULL;
}

/*!
  Destructor.
*/
SoMFColor::~SoMFColor(void)
{
  this->deleteAllValues();
}

/*!
  FIXME: write function documentation
*/
int
SoMFColor::fieldSizeof(void) const
{
  return sizeof(SbColor);
}

/*!
  FIXME: write function documentation
*/
void *
SoMFColor::valuesPtr(void)
{
  return (void *)this->values;
}

/*!
  FIXME: write function documentation
*/
void
SoMFColor::setValuesPtr(void * ptr)
{
  this->values = (SbColor *)ptr;
}

/*!
  FIXME: write function documentation
*/
int
SoMFColor::find(const SbColor & value, SbBool addIfNotFound)
{
  for (int i=0; i < this->num; i++) if (this->values[i] == value) return i;

  if (addIfNotFound) this->set1Value(this->num, value);
  return -1;
}

/*!
  FIXME: write function documentation
*/
void
SoMFColor::setValues(const int start, const int num,
                       const SbColor * newValues)
{
  if (start+num > this->maxNum) this->allocValues(start+num);
  else if (start+num > this->num) this->num = start+num;

  for (int i=0; i < num; i++)
    this->values[i+start] = (SbColor) newValues[i];
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
void
SoMFColor::set1Value(const int idx, const SbColor & value)
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
SoMFColor::setValue(const SbColor & value)
{
  this->allocValues(1);
  this->values[0] = value;
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
SbBool
SoMFColor::operator == (const SoMFColor & field) const
{
  if (this == &field) return TRUE;
  if (this->getNum() != field.getNum()) return FALSE;

  const SbColor * const lhs = this->getValues(0);
  const SbColor * const rhs = field.getValues(0);
  for (int i = 0; i < num; i++) if (lhs[i] != rhs[i]) return FALSE;
  return TRUE;
}

/*!
  FIXME: write function documentation
*/
void
SoMFColor::deleteAllValues(void)
{
  this->allocValues(0);
}

/*!
  FIXME: write function documentation
*/
void
SoMFColor::copyValue(int to, int from)
{
  assert(this->values && SbMax(to, from) < num);
  this->values[to] = this->values[from];
  this->valueChanged();
}
//$ END TEMPLATE MField

// *************************************************************************

/*!
  Does initialization common for all objects of the
  SoMFColor class. This includes setting up the
  type system, among other things.
*/
void
SoMFColor::initClass(void)
{
//$ BEGIN TEMPLATE FieldInitClass( MFColor )
  // Make sure we only initialize once.
  assert(SoMFColor::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoMFColor::classTypeId =
    SoType::createType(SoMFColor::getClassTypeId(),
                       "MFColor", &SoMFColor::createInstance);
//$ END TEMPLATE FieldInitClass
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoMFColor::cleanClass(void)
{
}


// *************************************************************************

/*!
  FIXME: write function documentation
*/
SbBool
SoMFColor::read1Value(SoInput * in, int index)
{
  float rgb[3];
  
  SbBool result = in->read(rgb[0]) && in->read(rgb[1]) && in->read(rgb[2]);
  if(result) this->values[index].setValue(rgb);
  return result;
}

/*!
  FIXME: write function documentation
*/
void
SoMFColor::write1Value(SoOutput * out, int idx) const
{
  out->write(this->values[idx][0]);
  if(!out->isBinary()) out->write(' ');
  out->write(this->values[idx][1]);
  if(!out->isBinary()) out->write(' ');
  out->write(this->values[idx][2]);
}

/*!
  FIXME: write function documentation
*/
void
SoMFColor::setValues(const int start, const int num, const float rgb[][3])
{
  if(start+num > this->maxNum) this->allocValues(start+num);
  else if(start+num > this->num) this->num = start+num;

  for(int i=0; i < num; i++) this->values[i+start].setValue(rgb[i]);
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
void
SoMFColor::setHSVValues(const int start, const int num, const float hsv[][3])
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoMFColor::setValue(const SbVec3f & vec)
{
  this->setValue(SbColor(vec));
}

/*!
  FIXME: write function documentation
*/
void
SoMFColor::setValue(const float r, const float g, const float b)
{
  this->setValue(SbColor(r, g, b));
}

/*!
  FIXME: write function documentation
*/
void
SoMFColor::setValue(const float rgb[3])
{
  this->setValue(SbColor(rgb));
}

/*!
  FIXME: write function documentation
*/
void
SoMFColor::setHSVValue(const float h, const float s, const float v)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoMFColor::setHSVValue(const float hsv[3])
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoMFColor::set1Value(const int idx, const SbVec3f & vec)
{
  this->set1Value(idx, SbColor(vec));
}

/*!
  FIXME: write function documentation
*/
void
SoMFColor::set1Value(const int idx, const float r, const float g, const float b)
{
  this->set1Value(idx,SbColor(r,g,b));
}

/*!
  FIXME: write function documentation
*/
void
SoMFColor::set1Value(const int idx, const float rgb[3])
{
  this->set1Value(idx,SbColor(rgb));
}

/*!
  FIXME: write function documentation
*/
void
SoMFColor::set1HSVValue(const int idx, const float h, const float s, const float v)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoMFColor::set1HSVValue(const int idx, const float hsv[3])
{
  assert(0 && "FIXME: not implemented yet");
}

void
SoMFColor::convertTo(SoField * dest) const
{
  if (0);
#if !defined(COIN_EXCLUDE_SOSFCOLOR)
  else if (dest->getTypeId()==SoSFColor::getClassTypeId()) {
    if (this->getNum()>0)
      ((SoSFColor *)dest)->setValue((*this)[0]);
  }
#endif // !COIN_EXCLUDE_SOSFCOLOR
#if COIN_DEBUG
  else {
    SoDebugError::post("SoMFColor::convertTo",
		       "Can't convert from %s to %s",
		       this->getTypeId().getName().getString(),
		       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}
