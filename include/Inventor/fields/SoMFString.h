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

#ifndef __SOMFSTRING_H__
#define __SOMFSTRING_H__

#include <Inventor/fields/SoMField.h>
#include <Inventor/SbString.h>

#if defined(COIN_EXCLUDE_SOMFSTRING)
#error "Configuration settings not respected, should not include this file!"
#endif // COIN_EXCLUDE_SOMFSTRING


class SoMFString : public SoMField {
  typedef SoMField inherited;

//$ BEGIN TEMPLATE MField(SoMFString, SbString, const SbString &)
private:
  static SoType classTypeId;

public:
  static void * createInstance(void);
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;

  static void initClass(void);

  virtual void copyFrom(const SoField & field);
  const SoMFString & operator = (const SoMFString & field);
  virtual SbBool isSame(const SoField & field) const;
public:
  SoMFString(void);
  virtual ~SoMFString(void);
private:
  virtual SbBool read1Value(SoInput * in, int idx);
  virtual void write1Value(SoOutput * out, int idx) const;
public:
  /*! Returns the element at the \a idx position. */
  const SbString & operator [] (const int idx) const
    { this->evaluate(); return this->values[idx]; }
  /*! Returns a pointer to an array of element starting at the \a start position. */
  const SbString * getValues(const int start) const
    { this->evaluate(); return (const SbString *)(this->values + start); }
  int find(const SbString & value, SbBool addIfNotFound = FALSE);
  void setValues(const int start, const int num, const SbString * values);
  void set1Value(const int idx, const SbString & value);
  void setValue(const SbString & value);
  /*! Make field contain a just the single value \a val. */
  const SbString & operator = (const SbString & val)
    { this->setValue(val); return val; }
  SbBool operator == (const SoMFString & field) const;
  /*! Returns \a TRUE if this field is \e not equal to \a field. */
  SbBool operator != (const SoMFString & field) const
    { return ! operator == (field); }
  /*! Returns a pointer to the array of values for editing. \e Must be matched
      with a call to finishEditing() upon completion. */
  SbString * startEditing(void)
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
  SbString * values;
//$ END TEMPLATE MField

public:
  void setValues(const int start, const int num, const char * const strings[]);
  void setValue(const char * const string);
  void deleteText(const int fromLine, const int fromChar,
		  const int toLine, const int toChar);

  // SbString values need to have their constructors called.
  virtual void allocValues(int num);

private:
  virtual void convertTo(SoField * dest) const;
};

#endif // !__SOMFSTRING_H__
