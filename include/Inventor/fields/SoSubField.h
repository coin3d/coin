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

#ifndef __SOSUBFIELD_H__
#define __SOSUBFIELD_H__

#include <Inventor/misc/SoBasic.h> // for SO__QUOTE() definition
#include <assert.h>


/**************************************************************************
 *
 * Header macros for single-value fields.
 *
 **************************************************************************/

#define SO_SFIELD_CONSTRUCTOR_HEADER(_class_) \
public: \
  _class_(void); \
  virtual ~_class_()


#define SO_SFIELD_REQUIRED_HEADER(_class_) \
private: \
  static SoType classTypeId; \
public: \
  static void * createInstance(void); \
  static SoType getClassTypeId(void); \
  virtual SoType getTypeId(void) const; \
 \
  virtual void copyFrom(const SoField & field); \
  const _class_ & operator=(const _class_ & field); \
  virtual SbBool isSame(const SoField & field) const


#define PRIVATE_SFIELD_IO_HEADER() \
private: \
  virtual SbBool readValue(SoInput * in); \
  virtual void writeValue(SoOutput * out) const


#define SO_SFIELD_VALUE_HEADER(_class_, _valtype_, _valref_) \
  PRIVATE_SFIELD_IO_HEADER(); \
protected: \
  _valtype_ value; \
 \
public: \
  _valref_ getValue(void) const { this->evaluate(); return this->value; } \
  void setValue(_valref_ newvalue); \
  _valref_ operator=(_valref_ newvalue) { this->setValue(newvalue); return this->value; } \
 \
  int operator==(const _class_ & field) const; \
  int operator!=(const _class_ & field) const { return ! operator==(field); }


// FIXME: is really the operator=() definition below necessary?
// 19991226 mortene.
#define SO_SFIELD_DERIVED_VALUE_HEADER(_class_, _valtype_, _valref_) \
  PRIVATE_SFIELD_IO_HEADER(); \
public: \
  _valref_ operator=(_valref_ newvalue) { this->setValue(newvalue); return this->value; }



#define SO_SFIELD_HEADER(_class_, _valtype_, _valref_) \
  SO_SFIELD_CONSTRUCTOR_HEADER(_class_); \
  SO_SFIELD_REQUIRED_HEADER(_class_); \
  SO_SFIELD_VALUE_HEADER(_class_, _valtype_, _valref_)


#define SO_SFIELD_DERIVED_HEADER(_class_, _valtype_, _valref_) \
  SO_SFIELD_CONSTRUCTOR_HEADER(_class_); \
  SO_SFIELD_REQUIRED_HEADER(_class_); \
  SO_SFIELD_DERIVED_VALUE_HEADER(_class_, _valtype_, _valref_)



/**************************************************************************
 *
 * Source macros for single-value fields.
 *
 **************************************************************************/

#define PRIVATE_FIELD_INIT_CLASS(_class_, _classname_, _parent_, _createfunc_) \
  do { \
    /* Make sure superclass get initialized before subclass. */ \
    assert(_parent_::getClassTypeId() != SoType::badType()); \
    /* Make sure we only initialize once. */ \
    assert(_class_::classTypeId == SoType::badType()); \
    _class_::classTypeId = \
      SoType::createType(_parent_::getClassTypeId(), _classname_, _createfunc_); \
  } while (0)



#define SO_SFIELD_INIT_CLASS(_class_, _parent_) \
  do { \
    const char * classname = SO__QUOTE(_class_); \
    PRIVATE_FIELD_INIT_CLASS(_class_, classname, _parent_, &_class_::createInstance); \
  } while (0)



#define SO_SFIELD_INTERNAL_INIT_CLASS(_class_) \
  do { \
    const char * classname = SO__QUOTE(_class_); \
    PRIVATE_FIELD_INIT_CLASS(_class_, &classname[2], inherited, &_class_::createInstance); \
  } while (0)



#define SO_SFIELD_CONSTRUCTOR_SOURCE(_class_) \
_class_::_class_(void) { assert(_class_::classTypeId != SoType::badType()); } \
_class_::~_class_() { }



#define SO_SFIELD_VALUE_SOURCE(_class_, _valtype_, _valref_) \
void \
_class_::setValue(_valref_ value) { \
  this->value = value; \
  this->valueChanged(); \
} \
 \
SbBool \
_class_::operator==(const _class_ & field) const \
{ \
  return (this->getValue() == field.getValue()); \
}


#define PRIVATE_TYPEID_SOURCE(_class_) \
SoType _class_::classTypeId = SoType::badType(); \
 \
SoType _class_::getTypeId(void) const { return _class_::classTypeId; } \
SoType _class_::getClassTypeId(void) { return _class_::classTypeId; } \
void * _class_::createInstance(void) { return new _class_; }



#define PRIVATE_EQUALITY_SOURCE(_class_) \
void \
_class_::copyFrom(const SoField & field) \
{ \
  this->operator=((const _class_ &)field); \
} \
 \
SbBool \
_class_::isSame(const SoField & field) const \
{ \
  if (field.getTypeId() != this->getTypeId()) return FALSE; \
  return this->operator==((const _class_ &) field); \
}



#define SO_SFIELD_REQUIRED_SOURCE(_class_) \
PRIVATE_TYPEID_SOURCE(_class_); \
PRIVATE_EQUALITY_SOURCE(_class_); \
 \
const _class_ & \
_class_::operator=(const _class_ & field) \
{ \
  this->setValue(field.getValue()); \
  return *this; \
}



#define SO_SFIELD_SOURCE(_class_, _valtype_, _valref_) \
  SO_SFIELD_CONSTRUCTOR_SOURCE(_class_); \
  SO_SFIELD_VALUE_SOURCE(_class_, _valtype_, _valref_); \
  SO_SFIELD_REQUIRED_SOURCE(_class_)



#define SO_SFIELD_DERIVED_SOURCE(_class_, _valtype_, _valref_) \
  SO_SFIELD_CONSTRUCTOR_SOURCE(_class_); \
  SO_SFIELD_REQUIRED_SOURCE(_class_)


/**************************************************************************
 *
 * Header macros for multiple-value fields.
 *
 **************************************************************************/

#define PRIVATE_MFIELD_IO_HEADER() \
private: \
  virtual SbBool read1Value(SoInput * in, int idx); \
  virtual void write1Value(SoOutput * out, int idx) const



#define SO_MFIELD_VALUE_HEADER(_class_, _valtype_, _valref_) \
  PRIVATE_MFIELD_IO_HEADER(); \
protected: \
  virtual void deleteAllValues(void); \
  virtual void copyValue(int to, int from); \
  virtual int fieldSizeof(void) const; \
  virtual void * valuesPtr(void); \
  virtual void setValuesPtr(void * ptr); \
  virtual void allocValues(int num); \
 \
  _valtype_ * values; \
public: \
  _valref_ operator[](const int idx) const \
    { this->evaluate(); return this->values[idx]; } \
  const _valtype_ * getValues(const int start) const \
    { this->evaluate(); return (const _valtype_ *)(this->values + start); } \
  int find(_valref_ value, SbBool addifnotfound = FALSE); \
  void setValues(const int start, const int num, const _valtype_ * newvals); \
  void set1Value(const int idx, _valref_ value); \
  void setValue(_valref_ value); \
  _valref_ operator=(_valref_ val) { this->setValue(val); return val; } \
  SbBool operator==(const _class_ & field) const; \
  SbBool operator!=(const _class_ & field) const { return !operator==(field); } \
  _valtype_ * startEditing(void) { this->evaluate(); return this->values; } \
  void finishEditing(void) { this->valueChanged(); }



#define SO_MFIELD_DERIVED_VALUE_HEADER(_class_, _valtype_, _valref_) \
  PRIVATE_MFIELD_IO_HEADER(); \
  _valref_ operator=(_valref_ val) { this->setValue(val); return val; }



#define SO_MFIELD_HEADER(_class_, _valtype_, _valref_) \
  SO_SFIELD_CONSTRUCTOR_HEADER(_class_); \
  SO_SFIELD_REQUIRED_HEADER(_class_); \
  SO_MFIELD_VALUE_HEADER(_class_, _valtype_, _valref_)



#define SO_MFIELD_DERIVED_HEADER(_class_, _valtype_, _valref_) \
  SO_SFIELD_CONSTRUCTOR_HEADER(_class_); \
  SO_SFIELD_REQUIRED_HEADER(_class_); \
  SO_MFIELD_DERIVED_VALUE_HEADER(_class_, _valtype_, _valref_)



/**************************************************************************
 *
 * Source macros for multiple-value fields.
 *
 **************************************************************************/


#define SO_MFIELD_INIT_CLASS(_class_, _parent_) \
  SO_SFIELD_INIT_CLASS(_class_, _parent_)



#define SO_MFIELD_INTERNAL_INIT_CLASS(_class_) \
  SO_SFIELD_INTERNAL_INIT_CLASS(_class_)



#define SO_MFIELD_CONSTRUCTOR_SOURCE(_class_) \
_class_::_class_(void) \
{ \
  assert(_class_::classTypeId != SoType::badType()); \
  this->values = NULL; \
} \
 \
_class_::~_class_(void) \
{ \
  this->deleteAllValues(); \
}



#define SO_MFIELD_DERIVED_CONSTRUCTOR_SOURCE(_class_) \
_class_::_class_(void) { } \
_class_::~_class_(void) { }



#define SO_MFIELD_REQUIRED_SOURCE(_class_) \
PRIVATE_TYPEID_SOURCE(_class_); \
PRIVATE_EQUALITY_SOURCE(_class_); \
const _class_ & \
_class_::operator = (const _class_ & field) \
{ \
  if (field.getNum() < this->getNum()) this->deleteAllValues(); \
  this->setValues(0, field.getNum(), field.getValues(0)); \
  return *this; \
}



#define SO_MFIELD_VALUE_SOURCE(_class_, _valtype_, _valref_) \
int \
_class_::fieldSizeof(void) const \
{ \
  return sizeof(_valtype_); \
} \
 \
void * \
_class_::valuesPtr(void) \
{ \
  return (void *)this->values; \
} \
 \
void \
_class_::setValuesPtr(void * ptr) \
{ \
  this->values = (_valtype_ *)ptr; \
} \
 \
int \
_class_::find(_valref_ value, SbBool addifnotfound) \
{ \
  for (int i=0; i < this->num; i++) if (this->values[i] == value) return i; \
 \
  if (addifnotfound) this->set1Value(this->num, value); \
  return -1; \
} \
 \
void \
_class_::setValues(const int start, const int num, const _valtype_ * newvals) \
{ \
  if (start+num > this->maxNum) this->allocValues(start+num); \
  else if (start+num > this->num) this->num = start+num; \
 \
  for (int i=0; i < num; i++) \
    this->values[i+start] = (_valtype_) newvals[i]; \
  this->valueChanged(); \
} \
 \
void \
_class_::set1Value(const int idx, _valref_ value) \
{ \
  if (idx+1 > this->maxNum) this->allocValues(idx+1); \
  else if (idx+1 > this->num) this->num = idx+1; \
  this->values[idx] = value; \
  this->valueChanged(); \
} \
 \
void \
_class_::setValue(_valref_ value) \
{ \
  this->allocValues(1); \
  this->values[0] = value; \
  this->valueChanged(); \
} \
 \
SbBool \
_class_::operator == (const _class_ & field) const \
{ \
  if (this == &field) return TRUE; \
  if (this->getNum() != field.getNum()) return FALSE; \
 \
  const _valtype_ * const lhs = this->getValues(0); \
  const _valtype_ * const rhs = field.getValues(0); \
  for (int i = 0; i < num; i++) if (lhs[i] != rhs[i]) return FALSE; \
  return TRUE; \
} \
 \
void \
_class_::deleteAllValues(void) \
{ \
  this->allocValues(0); \
} \
 \
void \
_class_::copyValue(int to, int from) \
{ \
  assert(this->values && SbMax(to, from) < num); \
  this->values[to] = this->values[from]; \
  this->valueChanged(); \
}


#define SO_MFIELD_ALLOC_SOURCE(_class_, _valtype_) \
void \
_class_::allocValues(int number) \
{ \
  assert(number >= 0); \
 \
  if (number == 0) { \
    delete[] this->values; \
    this->values = NULL; \
  } \
  else { \
    if (this->values) { \
      _valtype_ * newblock = new _valtype_[number]; \
      for (int i=0; i < SbMin(this->num, number); i++) \
        newblock[i] = this->values[i]; \
 \
      delete[] this->values; \
      this->values = newblock; \
    } \
    else { \
      this->values = new _valtype_[number]; \
    } \
  } \
 \
  SbBool valchanged = number < this->num ? TRUE : FALSE; \
  this->num = number; \
  this->maxNum = number; \
  if (valchanged) this->valueChanged(); \
}



#define SO_MFIELD_MALLOC_SOURCE(_class_, _valtype_) \
void \
_class_::allocValues(int number) \
{ \
  SoMField::allocValues(number); \
}



#define SO_MFIELD_SOURCE_MALLOC(_class_, _valtype_, _valref_) \
  SO_MFIELD_REQUIRED_SOURCE(_class_); \
  SO_MFIELD_CONSTRUCTOR_SOURCE(_class_); \
  SO_MFIELD_MALLOC_SOURCE(_class_, _valtype_); \
  SO_MFIELD_VALUE_SOURCE(_class_, _valtype_, _valref_)



#define SO_MFIELD_SOURCE(_class_, _valtype_, _valref_) \
  SO_MFIELD_REQUIRED_SOURCE(_class_); \
  SO_MFIELD_CONSTRUCTOR_SOURCE(_class_); \
  SO_MFIELD_ALLOC_SOURCE(_class_, _valtype_); \
  SO_MFIELD_VALUE_SOURCE(_class_, _valtype_, _valref_)



#define SO_MFIELD_DERIVED_SOURCE(_class_, _valtype_, _valref_) \
  SO_MFIELD_REQUIRED_SOURCE(_class_); \
  SO_MFIELD_DERIVED_CONSTRUCTOR_SOURCE(_class_)



#endif // !__SOSUBFIELD_H__
