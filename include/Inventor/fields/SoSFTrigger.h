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

#ifndef __SOSFTRIGGER_H__
#define __SOSFTRIGGER_H__

#include <Inventor/fields/SoSField.h>

class SoNotList;


class SoSFTrigger : public SoSField {
  typedef SoSField inherited;

//$ BEGIN TEMPLATE Field(SoSFTrigger)
private:
  static SoType classTypeId;

public:
  static void * createInstance(void);
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;

  static void initClass(void);

  virtual void copyFrom(const SoField & field);
  const SoSFTrigger & operator = (const SoSFTrigger & field);
  virtual SbBool isSame(const SoField & field) const;
//$ END TEMPLATE Field
//$ BEGIN TEMPLATE FieldConstructor(SoSFTrigger)
public:
  SoSFTrigger(void);
  virtual ~SoSFTrigger(void);
//$ END TEMPLATE FieldConstructor
//$ BEGIN TEMPLATE SFieldRW()
private:
  virtual SbBool readValue(SoInput * in);
  virtual void writeValue(SoOutput * out) const;
//$ END TEMPLATE SFieldRW


public:
  void setValue(void);
  void getValue(void) const;

  int operator == (const SoSFTrigger & trigger) const;
  int operator != (const SoSFTrigger & trigger) const;

  virtual void startNotify(void);
  virtual void notify(SoNotList * list);
  virtual void touch(void);
};

#endif // !__SOSFTRIGGER_H__
