#ifndef COIN_SOMFUINT32_H
#define COIN_SOMFUINT32_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/fields/SoMField.h>
#include <Inventor/fields/SoSubField.h>

class COIN_DLL_API SoMFUInt32 : public SoMField {
  typedef SoMField inherited;

  SO_MFIELD_HEADER(SoMFUInt32, uint32_t, uint32_t);

  SO_MFIELD_SETVALUESPOINTER_HEADER(uint32_t);

public:
  static void initClass(void);

private:
  virtual int getNumValuesPerLine(void) const;
};

#endif // !COIN_SOMFUINT32_H
