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

#ifndef __SOMFROTATION_H__
#define __SOMFROTATION_H__

#include <Inventor/fields/SoMField.h>
#include <Inventor/SbRotation.h>

#if defined(COIN_EXCLUDE_SOMFROTATION)
#error "Configuration settings not respected, should not include this file!"
#endif // COIN_EXCLUDE_SOMFROTATION


class SoMFRotation : public SoMField {
  typedef SoMField inherited;

//$ BEGIN TEMPLATE MField(SoMFRotation, SbRotation, const SbRotation &)
private:
  static SoType classTypeId;

public:
  static void * createInstance(void);
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;

  static void initClass(void);
  static void cleanClass(void);

  virtual void copyFrom(const SoField & field);
  const SoMFRotation & operator = (const SoMFRotation & field);
  virtual SbBool isSame(const SoField & field) const;
public:
  SoMFRotation(void);
  virtual ~SoMFRotation(void);
private:
  virtual SbBool read1Value(SoInput * in, int idx);
  virtual void write1Value(SoOutput * out, int idx) const;
public:
  /*! Returns the element at the \a idx position. */
  const SbRotation & operator [] (const int idx) const
    { this->evaluate(); return this->values[idx]; }
  /*! Returns a pointer to an array of element starting at the \a start position. */
  const SbRotation * getValues(const int start) const
    { this->evaluate(); return (const SbRotation *)(this->values + start); }
  int find(const SbRotation & value, SbBool addIfNotFound = FALSE);
  void setValues(const int start, const int num, const SbRotation * values);
  void set1Value(const int idx, const SbRotation & value);
  void setValue(const SbRotation & value);
  /*! Make field contain a just the single value \a val. */
  const SbRotation & operator = (const SbRotation & val)
    { this->setValue(val); return val; }
  SbBool operator == (const SoMFRotation & field) const;
  /*! Returns \a TRUE if this field is \e not equal to \a field. */
  SbBool operator != (const SoMFRotation & field) const
    { return ! operator == (field); }
  /*! Returns a pointer to the array of values for editing. \e Must be matched
      with a call to finishEditing() upon completion. */
  SbRotation * startEditing(void)
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
  SbRotation * values;
//$ END TEMPLATE MField

public:
  void setValues(const int start, const int num, const float q[][4]);
  void set1Value(const int idx, const float q0, const float q1, const float q2, const float q3);
  void set1Value(const int idx, const float q[4]);
  void set1Value(const int idx, const SbVec3f & axis, const float angle);
  void setValue(const float q0, const float q1, const float q2, const float q3);
  void setValue(const float q[4]);
  void setValue(const SbVec3f & axis, const float angle);

private:
  virtual void convertTo(SoField * dest) const;
};

#endif // !__SOMFROTATION_H__
