#ifndef COIN_SOMFSTRING_H
#define COIN_SOMFSTRING_H

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
#include <Inventor/SbString.h>

class COIN_DLL_API SoMFString : public SoMField {
  typedef SoMField inherited;

  SO_MFIELD_HEADER(SoMFString, SbString, const SbString &);

public:
  static void initClass(void);

  void setValues(const int start, const int num, const char * strings[]);
  void setValue(const char * string);
  void deleteText(const int fromline, const int fromchar,
                  const int toline, const int tochar);
};

#endif // !COIN_SOMFSTRING_H
