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

#ifndef __SOSFVEC2F_H__
#define __SOSFVEC2F_H__

#include <Inventor/fields/SoSField.h>
#include <Inventor/SbVec2f.h>

#if defined(COIN_EXCLUDE_SOSFVEC2F)
#error "Configuration settings not respected, should not include this file!"
#endif // COIN_EXCLUDE_SOSFVEC2F

class SoSFVec2f : public SoSField {
  typedef SoSField inherited;

//$ BEGIN TEMPLATE SField( SoSFVec2f, SbVec2f, const SbVec2f & )
private:
  static SoType classTypeId;

public:
  static void * createInstance(void);
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;

  static void initClass(void);
  static void cleanClass(void);

  virtual void copyFrom(const SoField & field);
  const SoSFVec2f & operator = (const SoSFVec2f & field);
  virtual SbBool isSame(const SoField & field) const;
public:
  SoSFVec2f(void);
  virtual ~SoSFVec2f(void);
private:
  virtual SbBool readValue(SoInput * in);
  virtual void writeValue(SoOutput * out) const;
public:
  /*! Returns this field's value. */
  const SbVec2f & getValue(void) const
    { this->evaluate(); return this->value; }
  void setValue(const SbVec2f & newvalue);
  /*! Copy value from \a newvalue into this field. */
  const SbVec2f & operator = (const SbVec2f & newvalue)
    { this->setValue(newvalue); return this->value; }

  int operator == (const SoSFVec2f & field) const;
  /*! Returns \a TRUE if this field is not equal to \a field. */
  int operator != (const SoSFVec2f & field) const
    { return ! operator == (field); }

protected:
  /*! The value contained in the field. */
  SbVec2f value;
//$ END TEMPLATE SField

public:
  void setValue(const float x, const float y);
  void setValue(const float xy[2]);

private:
  virtual void convertTo(SoField * dest) const;
};

#endif // !__SOSFVEC2F_H__
