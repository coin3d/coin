/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

#ifndef COIN_SOSFTRIGGER_H
#define COIN_SOSFTRIGGER_H

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
