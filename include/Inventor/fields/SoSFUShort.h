/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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

#ifndef __SOSFUSHORT_H__
#define __SOSFUSHORT_H__

#include <Inventor/fields/SoSField.h>
#include <Inventor/fields/SoSubField.h>


class SoSFUShort : public SoSField {
  typedef SoSField inherited;

  SO_SFIELD_HEADER(SoSFUShort, unsigned short, const unsigned short);

public:
  static void initClass(void);

private:
  virtual void convertTo(SoField * dest) const;

  friend class SoMFUShort; // For readValue() & writeValue().
};

#endif // !__SOSFUSHORT_H__
