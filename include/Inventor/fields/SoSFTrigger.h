#ifndef COIN_SOSFTRIGGER_H
#define COIN_SOSFTRIGGER_H

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

#include <Inventor/fields/SoSField.h>
#include <Inventor/fields/SoSubField.h>

class SoNotList;

class COIN_DLL_API SoSFTrigger : public SoSField {
  typedef SoSField inherited;

  SO_SFIELD_CONSTRUCTOR_HEADER(SoSFTrigger);
  SO_SFIELD_REQUIRED_HEADER(SoSFTrigger);

public:
  static void initClass(void);

  void setValue(void);
  void getValue(void) const;

  int operator==(const SoSFTrigger & trigger) const;
  int operator!=(const SoSFTrigger & trigger) const;

  virtual void startNotify(void);
  virtual void notify(SoNotList * l);
  virtual void touch(void);

private:
  virtual SbBool readValue(SoInput * in);
  virtual void writeValue(SoOutput * out) const;
};

#endif // !COIN_SOSFTRIGGER_H
