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

#ifndef __SOMFENUM_H__
#define __SOMFENUM_H__

#include <Inventor/fields/SoMField.h>

#if defined(COIN_EXCLUDE_SOMFENUM)
#error "Configuration settings not respected, should not include this file!"
#endif // COIN_EXCLUDE_SOMFENUM


class SoMFEnum : public SoMField {
  typedef SoMField inherited;

//$ BEGIN TEMPLATE MField(SoMFEnum, int, int)
private:
  static SoType classTypeId;

public:
  static void * createInstance(void);
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;

  static void initClass(void);

  virtual void copyFrom(const SoField & field);
  const SoMFEnum & operator = (const SoMFEnum & field);
  virtual SbBool isSame(const SoField & field) const;
public:
  SoMFEnum(void);
  virtual ~SoMFEnum(void);
private:
  virtual SbBool read1Value(SoInput * in, int idx);
  virtual void write1Value(SoOutput * out, int idx) const;
public:
  /*! Returns the element at the \a idx position. */
  int operator [] (const int idx) const
    { this->evaluate(); return this->values[idx]; }
  /*! Returns a pointer to an array of element starting at the \a start position. */
  const int * getValues(const int start) const
    { this->evaluate(); return (const int *)(this->values + start); }
  int find(int value, SbBool addIfNotFound = FALSE);
  void setValues(const int start, const int num, const int * values);
  void set1Value(const int idx, int value);
  void setValue(int value);
  /*! Make field contain a just the single value \a val. */
  int operator = (int val)
    { this->setValue(val); return val; }
  SbBool operator == (const SoMFEnum & field) const;
  /*! Returns \a TRUE if this field is \e not equal to \a field. */
  SbBool operator != (const SoMFEnum & field) const
    { return ! operator == (field); }
  /*! Returns a pointer to the array of values for editing. \e Must be matched
      with a call to finishEditing() upon completion. */
  int * startEditing(void)
    { this->evaluate(); return this->values; }
  /*! Call this method to notify Coin that you're through editing the data. */
  void finishEditing(void)
    { this->valueChanged(); }

protected:
  virtual void deleteAllValues(void);
  virtual void copyValue(int to, int from);
  virtual int fieldSizeof(void) const;
  virtual void * valuesPtr(void);
  virtual void setValuesPtr(void * ptr);

  /*! Pointer to array of values. */
  int * values;
//$ END TEMPLATE MField

public:
  void setValue(const SbName name);
  void set1Value(const int idx, const SbName name);
  void setEnums(const int num, const int * const values,
                const SbName * const names);

protected:
  virtual SbBool findEnumValue(const SbName & name, int & value);
  virtual SbBool findEnumName(int val, const SbName * & name) const;

  SbBool legalValuesSet;
  int numEnums;
  int * enumValues;
  SbName * enumNames;

private:
  virtual void convertTo(SoField * dest) const;
};

#endif // !__SOMFENUM_H__
