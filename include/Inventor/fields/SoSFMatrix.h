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

#ifndef __SOSFMATRIX_H__
#define __SOSFMATRIX_H__

#include <Inventor/fields/SoSField.h>
#include <Inventor/SbMatrix.h>

#if defined(COIN_EXCLUDE_SOSFMATRIX)
#error "Configuration settings not respected, should not include this file!"
#endif // COIN_EXCLUDE_SOSFMATRIX

class SoSFMatrix : public SoSField {
  typedef SoSField inherited;

//$ BEGIN TEMPLATE SField(SoSFMatrix, SbMatrix, const SbMatrix &)
private:
  static SoType classTypeId;

public:
  static void * createInstance(void);
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;

  static void initClass(void);

  virtual void copyFrom(const SoField & field);
  const SoSFMatrix & operator = (const SoSFMatrix & field);
  virtual SbBool isSame(const SoField & field) const;
public:
  SoSFMatrix(void);
  virtual ~SoSFMatrix(void);
private:
  virtual SbBool readValue(SoInput * in);
  virtual void writeValue(SoOutput * out) const;
public:
  /*! Returns this field's value. */
  const SbMatrix & getValue(void) const
    { this->evaluate(); return this->value; }
  void setValue(const SbMatrix & newvalue);
  /*! Copy value from \a newvalue into this field. */
  const SbMatrix & operator = (const SbMatrix & newvalue)
    { this->setValue(newvalue); return this->value; }

  int operator == (const SoSFMatrix & field) const;
  /*! Returns \a TRUE if this field is not equal to \a field. */
  int operator != (const SoSFMatrix & field) const
    { return ! operator == (field); }

protected:
  /*! The value contained in the field. */
  SbMatrix value;
//$ END TEMPLATE SField

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

  friend class SoMFMatrix; // For readValue() & writeValue().
};

#endif // !__SOSFMATRIX_H__
