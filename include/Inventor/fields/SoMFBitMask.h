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

#ifndef __SOMFBITMASK_H__
#define __SOMFBITMASK_H__

#include <Inventor/fields/SoMFEnum.h>

#if defined(COIN_EXCLUDE_SOMFBITMASK)
#error "Configuration settings not respected, should not include this file!"
#endif // COIN_EXCLUDE_SOMFBITMASK

class SoMFBitMask : public SoMFEnum {
  typedef SoMFEnum inherited;

//$ BEGIN TEMPLATE MFieldDerived(SoMFBitMask, int)
private:
  static SoType classTypeId;

public:
  static void * createInstance(void);
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;

  static void initClass(void);

  virtual void copyFrom(const SoField & field);
  const SoMFBitMask & operator = (const SoMFBitMask & field);
  virtual SbBool isSame(const SoField & field) const;
private:
  virtual SbBool read1Value(SoInput * in, int idx);
  virtual void write1Value(SoOutput * out, int idx) const;
public:
  /*! Make field contain a just the single value \a val. */
  int operator = (int val)
    { this->setValue(val); return val; }
//$ END TEMPLATE MFieldDerived

protected:
  virtual SbBool findEnumValue(const SbName & name, int & value);

private:
  virtual void convertTo(SoField * dest) const;
};

#endif // !__SOMFBITMASK_H__
