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

#ifndef __SOSFBITMASK_H__
#define __SOSFBITMASK_H__

#include <Inventor/fields/SoSFEnum.h>

#if defined(COIN_EXCLUDE_SOSFBITMASK)
#error "Configuration settings not respected, should not include this file!"
#endif // COIN_EXCLUDE_SOSFBITMASK

class SoSFBitMask : public SoSFEnum {
  typedef SoSFEnum inherited;

//$ BEGIN TEMPLATE SFieldDerived(SoSFBitMask, int)
private:
  static SoType classTypeId;

public:
  static void * createInstance(void);
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;

  static void initClass(void);

  virtual void copyFrom(const SoField & field);
  const SoSFBitMask & operator = (const SoSFBitMask & field);
  virtual SbBool isSame(const SoField & field) const;
private:
  virtual SbBool readValue(SoInput * in);
  virtual void writeValue(SoOutput * out) const;
public:
  int operator = (const int newvalue)
    { this->setValue(newvalue); return this->value; }
//$ END TEMPLATE SFieldDerived

protected:
  virtual SbBool findEnumValue(const SbName & name, int & val);

private:
  virtual void convertTo(SoField * dest) const;

  friend class SoMFBitMask; // For readValue() & writeValue().
};

#endif // !__SOSFBITMASK_H__
