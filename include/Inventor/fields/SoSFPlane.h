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

#ifndef __SOSFPLANE_H__
#define __SOSFPLANE_H__

#include <Inventor/fields/SoSField.h>
#include <Inventor/SbPlane.h>

#if defined(COIN_EXCLUDE_SOSFPLANE)
#error "Configuration settings not respected, should not include this file!"
#endif // COIN_EXCLUDE_SOSFPLANE

class SoSFPlane : public SoSField {
  typedef SoSField inherited;

//$ BEGIN TEMPLATE SField(SoSFPlane, SbPlane, const SbPlane &)
private:
  static SoType classTypeId;

public:
  static void * createInstance(void);
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;

  static void initClass(void);

  virtual void copyFrom(const SoField & field);
  const SoSFPlane & operator = (const SoSFPlane & field);
  virtual SbBool isSame(const SoField & field) const;
public:
  SoSFPlane(void);
  virtual ~SoSFPlane(void);
private:
  virtual SbBool readValue(SoInput * in);
  virtual void writeValue(SoOutput * out) const;
public:
  /*! Returns this field's value. */
  const SbPlane & getValue(void) const
    { this->evaluate(); return this->value; }
  void setValue(const SbPlane & newvalue);
  /*! Copy value from \a newvalue into this field. */
  const SbPlane & operator = (const SbPlane & newvalue)
    { this->setValue(newvalue); return this->value; }

  int operator == (const SoSFPlane & field) const;
  /*! Returns \a TRUE if this field is not equal to \a field. */
  int operator != (const SoSFPlane & field) const
    { return ! operator == (field); }

protected:
  /*! The value contained in the field. */
  SbPlane value;
//$ END TEMPLATE SField

private:
  virtual void convertTo(SoField * dest) const;

  friend class SoMFPlane; // For readValue() & writeValue().
};

#endif // !__SOSFPLANE_H__
