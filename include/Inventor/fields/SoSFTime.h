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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

#ifndef __SOSFTIME_H__
#define __SOSFTIME_H__

#include <Inventor/fields/SoSField.h>
#include <Inventor/SbTime.h>

#if defined(COIN_EXCLUDE_SOSFTIME)
#error "Configuration settings not respected, should not include this file!"
#endif // COIN_EXCLUDE_SOSFTIME


class SoSFTime : public SoSField {
  typedef SoSField inherited;

//$ BEGIN TEMPLATE SField( SoSFTime, SbTime, const SbTime & )
private:
  static SoType classTypeId;

public:
  static void * createInstance(void);
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;

  static void initClass(void);
  static void cleanClass(void);

  virtual void copyFrom(const SoField & field);
  const SoSFTime & operator = (const SoSFTime & field);
  virtual SbBool isSame(const SoField & field) const;
public:
  SoSFTime(void);
  virtual ~SoSFTime(void);
private:
  virtual SbBool readValue(SoInput * in);
  virtual void writeValue(SoOutput * out) const;
public:
  /*! Returns this field's value. */
  const SbTime & getValue(void) const
    { this->evaluate(); return this->value; }
  void setValue(const SbTime & newvalue);
  /*! Copy value from \a newvalue into this field. */
  const SbTime & operator = (const SbTime & newvalue)
    { this->setValue(newvalue); return this->value; }

  int operator == (const SoSFTime & field) const;
  /*! Returns \a TRUE if this field is not equal to \a field. */
  int operator != (const SoSFTime & field) const
    { return ! operator == (field); }

protected:
  /*! The value contained in the field. */
  SbTime value;
//$ END TEMPLATE SField

private:
  virtual void convertTo(SoField * dest) const;

  friend class SoMFTime; // For readValue() & writeValue().
};

#endif // !__SOSFTIME_H__
