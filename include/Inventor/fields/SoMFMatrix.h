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

#ifndef __SOMFMATRIX_H__
#define __SOMFMATRIX_H__

#include <Inventor/fields/SoMField.h>
#include <Inventor/SbMatrix.h>

#if defined(COIN_EXCLUDE_SOMFMATRIX)
#error "Configuration settings not respected, should not include this file!"
#endif // COIN_EXCLUDE_SOMFMATRIX

class SoMFMatrix : public SoMField {
  typedef SoMField inherited;

//$ BEGIN TEMPLATE MField(SoMFMatrix, SbMatrix, const SbMatrix &)
private:
  static SoType classTypeId;

public:
  static void * createInstance(void);
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;

  static void initClass(void);

  virtual void copyFrom(const SoField & field);
  const SoMFMatrix & operator = (const SoMFMatrix & field);
  virtual SbBool isSame(const SoField & field) const;
public:
  SoMFMatrix(void);
  virtual ~SoMFMatrix(void);
private:
  virtual SbBool read1Value(SoInput * in, int idx);
  virtual void write1Value(SoOutput * out, int idx) const;
public:
  /*! Returns the element at the \a idx position. */
  const SbMatrix & operator [] (const int idx) const
    { this->evaluate(); return this->values[idx]; }
  /*! Returns a pointer to an array of element starting at the \a start position. */
  const SbMatrix * getValues(const int start) const
    { this->evaluate(); return (const SbMatrix *)(this->values + start); }
  int find(const SbMatrix & value, SbBool addIfNotFound = FALSE);
  void setValues(const int start, const int num, const SbMatrix * values);
  void set1Value(const int idx, const SbMatrix & value);
  void setValue(const SbMatrix & value);
  /*! Make field contain a just the single value \a val. */
  const SbMatrix & operator = (const SbMatrix & val)
    { this->setValue(val); return val; }
  SbBool operator == (const SoMFMatrix & field) const;
  /*! Returns \a TRUE if this field is \e not equal to \a field. */
  SbBool operator != (const SoMFMatrix & field) const
    { return ! operator == (field); }
  /*! Returns a pointer to the array of values for editing. \e Must be matched
      with a call to finishEditing() upon completion. */
  SbMatrix * startEditing(void)
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
  SbMatrix * values;
//$ END TEMPLATE MField

public:
  void setValue(const float a11, const float a12,
		const float a13, const float a14,

		const float a21, const float a22,
		const float a23, const float a24,

		const float a31, const float a32,
		const float a33, const float a34,

		const float a41, const float a42,
		const float a43, const float a44);

private:
  virtual void convertTo(SoField * dest) const;
};

#endif // !__SOMFMATRIX_H__
