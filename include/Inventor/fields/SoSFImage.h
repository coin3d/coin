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

#ifndef __SOSFIMAGE_H__
#define __SOSFIMAGE_H__

#include <Inventor/fields/SoSField.h>
#include <Inventor/SbVec2s.h>


class SoSFImage : public SoSField {
  typedef SoSField inherited;

//$ BEGIN TEMPLATE Field(SoSFImage)
private:
  static SoType classTypeId;

public:
  static void * createInstance(void);
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;

  static void initClass(void);

  virtual void copyFrom(const SoField & field);
  const SoSFImage & operator = (const SoSFImage & field);
  virtual SbBool isSame(const SoField & field) const;
//$ END TEMPLATE Field
//$ BEGIN TEMPLATE FieldConstructor(SoSFImage)
public:
  SoSFImage(void);
  virtual ~SoSFImage(void);
//$ END TEMPLATE FieldConstructor
//$ BEGIN TEMPLATE SFieldRW()
private:
  virtual SbBool readValue(SoInput * in);
  virtual void writeValue(SoOutput * out) const;
//$ END TEMPLATE SFieldRW

public:
  const unsigned char * getValue(SbVec2s & size, int & nc) const;
  void setValue(const SbVec2s & size, const int nc,
                const unsigned char * const bytes);

  int operator == (const SoSFImage & field) const;
  /*! Returns \a TRUE if this field is not equal to \a field. */
  int operator != (const SoSFImage & field) const
    { return ! operator == (field); }

  unsigned char * startEditing(SbVec2s & size, int & nc);
  void finishEditing(void);

private:
  SbVec2s imgdim;
  unsigned char * pixblock;
  int bytedepth;
};

#endif // !__SOSFIMAGE_H__
