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

#ifndef COIN_SOSFTRIGGER_H
#define COIN_SOSFTRIGGER_H

#include <Inventor/fields/SoSField.h>
#include <Inventor/fields/SoSubField.h>

class SoNotList;


class SoSFTrigger : public SoSField {
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
