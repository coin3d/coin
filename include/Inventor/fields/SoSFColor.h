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

#ifndef __SOSFCOLOR_H__
#define __SOSFCOLOR_H__

#include <Inventor/fields/SoSField.h>
#include <Inventor/SbColor.h>

#if defined(COIN_EXCLUDE_SOSFCOLOR)
#error "Configuration settings not respected, should not include this file!"
#endif // COIN_EXCLUDE_SOSFCOLOR

class SoSFColor : public SoSField {
  typedef SoSField inherited;

//$ BEGIN TEMPLATE SField(SoSFColor, SbColor, const SbColor &)
private:
  static SoType classTypeId;

public:
  static void * createInstance(void);
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;

  static void initClass(void);

  virtual void copyFrom(const SoField & field);
  const SoSFColor & operator = (const SoSFColor & field);
  virtual SbBool isSame(const SoField & field) const;
public:
  SoSFColor(void);
  virtual ~SoSFColor(void);
private:
  virtual SbBool readValue(SoInput * in);
  virtual void writeValue(SoOutput * out) const;
public:
  /*! Returns this field's value. */
  const SbColor & getValue(void) const
    { this->evaluate(); return this->value; }
  void setValue(const SbColor & newvalue);
  /*! Copy value from \a newvalue into this field. */
  const SbColor & operator = (const SbColor & newvalue)
    { this->setValue(newvalue); return this->value; }

  int operator == (const SoSFColor & field) const;
  /*! Returns \a TRUE if this field is not equal to \a field. */
  int operator != (const SoSFColor & field) const
    { return ! operator == (field); }

protected:
  /*! The value contained in the field. */
  SbColor value;
//$ END TEMPLATE SField

public:
  void setValue(const SbVec3f & vec);
  void setValue(const float red, const float green, const float blue);
  void setValue(const float rgb[3]);
  void setHSVValue(const float h, const float s, const float v);
  void setHSVValue(const float hsv[3]);

private:
  virtual void convertTo(SoField * dest) const;

  friend class SoMFColor; // For readValue() & writeValue().
};

//$ IMPORT INLINE ../../../src/fields/SoSFColor.cpp

#endif // !__SOSFCOLOR_H__
