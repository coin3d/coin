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

#ifndef __SOSUBFIELD_H__
#define __SOSUBFIELD_H__

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


#endif // !__SOSUBFIELD_H__
