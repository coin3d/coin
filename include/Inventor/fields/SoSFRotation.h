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

#ifndef __SOSFROTATION_H__
#define __SOSFROTATION_H__

#include <Inventor/fields/SoSField.h>
#include <Inventor/SbRotation.h>

#if defined(COIN_EXCLUDE_SOSFROTATION)
#error "Configuration settings not respected, should not include this file!"
#endif // COIN_EXCLUDE_SOSFROTATION


class SoSFRotation : public SoSField {
  typedef SoSField inherited;

//$ BEGIN TEMPLATE SField( SoSFRotation, SbRotation, const SbRotation & )
private:
  static SoType classTypeId;

public:
  static void * createInstance(void);
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;

  static void initClass(void);
  static void cleanClass(void);

  virtual void copyFrom(const SoField & field);
  const SoSFRotation & operator = (const SoSFRotation & field);
  virtual SbBool isSame(const SoField & field) const;
public:
  SoSFRotation(void);
  virtual ~SoSFRotation(void);
private:
  virtual SbBool readValue(SoInput * in);
  virtual void writeValue(SoOutput * out) const;
public:
  /*! Returns this field's value. */
  const SbRotation & getValue(void) const
    { this->evaluate(); return this->value; }
  void setValue(const SbRotation & newvalue);
  /*! Copy value from \a newvalue into this field. */
  const SbRotation & operator = (const SbRotation & newvalue)
    { this->setValue(newvalue); return this->value; }

  int operator == (const SoSFRotation & field) const;
  /*! Returns \a TRUE if this field is not equal to \a field. */
  int operator != (const SoSFRotation & field) const
    { return ! operator == (field); }

protected:
  /*! The value contained in the field. */
  SbRotation value;
//$ END TEMPLATE SField

public:
  void getValue(SbVec3f & axis, float & angle) const;
  void setValue(const float q0, const float q1, const float q2, const float q3);
  void setValue(const float q[4]);
  void setValue(const SbVec3f & axis, const float angle);

private:
  virtual void convertTo(SoField * dest) const;

  friend class SoMFRotation; // For readValue() & writeValue().
};

#endif // !__SOSFROTATION_H__
