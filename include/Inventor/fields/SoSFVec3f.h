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

#ifndef __SOSFVEC3F_H__
#define __SOSFVEC3F_H__

#include <Inventor/fields/SoSField.h>
#include <Inventor/SbVec3f.h>

#if defined(COIN_EXCLUDE_SOSFVEC3F)
#error "Configuration settings not respected, should not include this file!"
#endif // COIN_EXCLUDE_SOSFVEC3F


class SoSFVec3f : public SoSField {
  typedef SoSField inherited;

//$ BEGIN TEMPLATE SField(SoSFVec3f, SbVec3f, const SbVec3f &)
private:
  static SoType classTypeId;

public:
  static void * createInstance(void);
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;

  static void initClass(void);
  static void cleanClass(void);

  virtual void copyFrom(const SoField & field);
  const SoSFVec3f & operator = (const SoSFVec3f & field);
  virtual SbBool isSame(const SoField & field) const;
public:
  SoSFVec3f(void);
  virtual ~SoSFVec3f(void);
private:
  virtual SbBool readValue(SoInput * in);
  virtual void writeValue(SoOutput * out) const;
public:
  /*! Returns this field's value. */
  const SbVec3f & getValue(void) const
    { this->evaluate(); return this->value; }
  void setValue(const SbVec3f & newvalue);
  /*! Copy value from \a newvalue into this field. */
  const SbVec3f & operator = (const SbVec3f & newvalue)
    { this->setValue(newvalue); return this->value; }

  int operator == (const SoSFVec3f & field) const;
  /*! Returns \a TRUE if this field is not equal to \a field. */
  int operator != (const SoSFVec3f & field) const
    { return ! operator == (field); }

protected:
  /*! The value contained in the field. */
  SbVec3f value;
//$ END TEMPLATE SField

public:
  void setValue(const float x, const float y, const float z);
  void setValue(const float xyz[3]);

private:
  virtual void convertTo(SoField * dest) const;

  friend class SoMFVec3f; // For readValue() & writeValue().
};

//$ IMPORT INLINE ../../../src/fields/SoSFVec3f.cpp

#endif // !__SOSFVEC3F_H__
