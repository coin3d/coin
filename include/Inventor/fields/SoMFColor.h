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

#ifndef __SOMFCOLOR_H__
#define __SOMFCOLOR_H__

#include <Inventor/fields/SoMField.h>
#include <Inventor/SbColor.h>

#if defined(COIN_EXCLUDE_SOMFCOLOR)
#error "Configuration settings not respected, should not include this file!"
#endif // COIN_EXCLUDE_SOMFCOLOR


class SoMFColor : public SoMField {
    typedef SoMField inherited;

//$ BEGIN TEMPLATE MField(SoMFColor, SbColor, const SbColor &)
private:
  static SoType classTypeId;

public:
  static void * createInstance(void);
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;

  static void initClass(void);

  virtual void copyFrom(const SoField & field);
  const SoMFColor & operator = (const SoMFColor & field);
  virtual SbBool isSame(const SoField & field) const;
public:
  SoMFColor(void);
  virtual ~SoMFColor(void);
private:
  virtual SbBool read1Value(SoInput * in, int idx);
  virtual void write1Value(SoOutput * out, int idx) const;
public:
  /*! Returns the element at the \a idx position. */
  const SbColor & operator [] (const int idx) const
    { this->evaluate(); return this->values[idx]; }
  /*! Returns a pointer to an array of element starting at the \a start position. */
  const SbColor * getValues(const int start) const
    { this->evaluate(); return (const SbColor *)(this->values + start); }
  int find(const SbColor & value, SbBool addIfNotFound = FALSE);
  void setValues(const int start, const int num, const SbColor * values);
  void set1Value(const int idx, const SbColor & value);
  void setValue(const SbColor & value);
  /*! Make field contain a just the single value \a val. */
  const SbColor & operator = (const SbColor & val)
    { this->setValue(val); return val; }
  SbBool operator == (const SoMFColor & field) const;
  /*! Returns \a TRUE if this field is \e not equal to \a field. */
  SbBool operator != (const SoMFColor & field) const
    { return ! operator == (field); }
  /*! Returns a pointer to the array of values for editing. \e Must be matched
      with a call to finishEditing() upon completion. */
  SbColor * startEditing(void)
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
  SbColor * values;
//$ END TEMPLATE MField

public:
  void setValues(const int start, const int num, const float rgb[][3]);
  void setHSVValues(const int start, const int num, const float hsv[][3]);

  void setValue(const SbVec3f & vec);
  void setValue(const float r, const float g, const float b);
  void setValue(const float rgb[3]);

  void setHSVValue(const float h, const float s, const float v);
  void setHSVValue(const float hsv[3]);

  void set1Value(const int idx, const SbVec3f & vec);
  void set1Value(const int idx, const float r, const float g, const float b);
  void set1Value(const int idx, const float rgb[3]);

  void set1HSVValue(const int idx, const float h, const float s, const float v);
  void set1HSVValue(const int idx, const float hsv[3]);

private:
  virtual void convertTo(SoField * dest) const;
};

//$ INSERT INLINE ../../../src/fields/SoMFColor.cpp

#endif // !__SOMFCOLOR_H__
