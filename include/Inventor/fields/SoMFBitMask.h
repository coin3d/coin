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


class SoMFBitMask : public SoMFEnum {
  typedef SoMFEnum inherited;

  SO_MFIELD_DERIVED_HEADER(SoMFBitMask, not_used, int)

public:
  static void initClass(void);

protected:
  virtual SbBool findEnumValue(const SbName & name, int & value);

private:
  virtual void convertTo(SoField * dest) const;
};

#endif // !__SOMFBITMASK_H__
