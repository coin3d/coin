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

#ifndef __SOMFBOOL_H__
#define __SOMFBOOL_H__

#include <Inventor/fields/SoMField.h>

#if defined(COIN_EXCLUDE_SOMFBOOL)
#error "Configuration settings not respected, should not include this file!"
#endif // COIN_EXCLUDE_SOMFBOOL


class SoMFBool : public SoMField {
  typedef SoMField inherited;

//$ BEGIN TEMPLATE MField( SoMFBool, SbBool, SbBool )
private:
  static SoType classTypeId;

public:
  static void * createInstance(void);
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;

  static void initClass(void);
  static void cleanClass(void);

  virtual void copyFrom(const SoField & field);
  const SoMFBool & operator = (const SoMFBool & field);
  virtual SbBool isSame(const SoField & field) const;
public:
  SoMFBool(void);
  virtual ~SoMFBool(void);
private:
  virtual SbBool read1Value(SoInput * in, int idx);
  virtual void write1Value(SoOutput * out, int idx) const;
public:
  /*! Returns the element at the \a idx position. */
  SbBool operator [] (const int idx) const
    { this->evaluate(); return this->values[idx]; }
  /*! Returns a pointer to an array of element starting at the \a start position. */
  const SbBool * getValues(const int start) const
    { this->evaluate(); return (const SbBool *)(this->values + start); }
  int find(SbBool value, SbBool addIfNotFound = FALSE);
  void setValues(const int start, const int num, const SbBool * values);
  void set1Value(const int idx, SbBool value);
  void setValue(SbBool value);
  /*! Make field contain a just the single value \a val. */
  SbBool operator = (SbBool val)
    { this->setValue(val); return val; }
  SbBool operator == (const SoMFBool & field) const;
  /*! Returns \a TRUE if this field is \e not equal to \a field. */
  SbBool operator != (const SoMFBool & field) const
    { return ! operator == (field); }
  /*! Returns a pointer to the array of values for editing. \e Must be matched
      with a call to finishEditing() upon completion. */
  SbBool * startEditing(void)
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
  SbBool * values;
//$ END TEMPLATE MField

private:
  virtual int getNumValuesPerLine(void) const;
  virtual void convertTo(SoField * dest) const;
};

#endif // !__SOMFBOOL_H__
