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

#ifndef __SOSFUINT32_H__
#define __SOSFUINT32_H__

#include <Inventor/fields/SoSField.h>

#if defined(COIN_EXCLUDE_SOSFUINT32)
#error "Configuration settings not respected, should not include this file!"
#endif // COIN_EXCLUDE_SOSFUINT32


class SoSFUInt32 : public SoSField {
  typedef SoSField inherited;

//$ BEGIN TEMPLATE SField(SoSFUInt32, uint32_t, const uint32_t)
private:
  static SoType classTypeId;

public:
  static void * createInstance(void);
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;

  static void initClass(void);
  static void cleanClass(void);

  virtual void copyFrom(const SoField & field);
  const SoSFUInt32 & operator = (const SoSFUInt32 & field);
  virtual SbBool isSame(const SoField & field) const;
public:
  SoSFUInt32(void);
  virtual ~SoSFUInt32(void);
private:
  virtual SbBool readValue(SoInput * in);
  virtual void writeValue(SoOutput * out) const;
public:
  /*! Returns this field's value. */
  const uint32_t getValue(void) const
    { this->evaluate(); return this->value; }
  void setValue(const uint32_t newvalue);
  /*! Copy value from \a newvalue into this field. */
  const uint32_t operator = (const uint32_t newvalue)
    { this->setValue(newvalue); return this->value; }

  int operator == (const SoSFUInt32 & field) const;
  /*! Returns \a TRUE if this field is not equal to \a field. */
  int operator != (const SoSFUInt32 & field) const
    { return ! operator == (field); }

protected:
  /*! The value contained in the field. */
  uint32_t value;
//$ END TEMPLATE SField

private:
  virtual void convertTo(SoField * dest) const;

  friend class SoMFUInt32; // For readValue() & writeValue().
};

#endif // !__SOSFUINT32_H__
