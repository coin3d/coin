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

#ifndef __SOSFBOOL_H__
#define __SOSFBOOL_H__

#include <Inventor/fields/SoSField.h>

#if defined(COIN_EXCLUDE_SOSFBOOL)
#error "Configuration settings not respected, should not include this file!"
#endif // COIN_EXCLUDE_SOSFBOOL

class SoSFBool : public SoSField {
  typedef SoSField inherited;
//$ BEGIN TEMPLATE SField(SoSFBool, SbBool, const SbBool)
private:
  static SoType classTypeId;

public:
  static void * createInstance(void);
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;

  static void initClass(void);

  virtual void copyFrom(const SoField & field);
  const SoSFBool & operator = (const SoSFBool & field);
  virtual SbBool isSame(const SoField & field) const;
public:
  SoSFBool(void);
  virtual ~SoSFBool(void);
private:
  virtual SbBool readValue(SoInput * in);
  virtual void writeValue(SoOutput * out) const;
public:
  /*! Returns this field's value. */
  const SbBool getValue(void) const
    { this->evaluate(); return this->value; }
  void setValue(const SbBool newvalue);
  /*! Copy value from \a newvalue into this field. */
  const SbBool operator = (const SbBool newvalue)
    { this->setValue(newvalue); return this->value; }

  int operator == (const SoSFBool & field) const;
  /*! Returns \a TRUE if this field is not equal to \a field. */
  int operator != (const SoSFBool & field) const
    { return ! operator == (field); }

protected:
  /*! The value contained in the field. */
  SbBool value;
//$ END TEMPLATE SField

private:
  virtual void convertTo(SoField * dest) const;

  friend class SoMFBool; // For readValue() & writeValue().
};

#endif // !__SOSFBOOL_H__
