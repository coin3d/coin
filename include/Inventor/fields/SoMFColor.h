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

#ifndef __SOMFCOLOR_H__
#define __SOMFCOLOR_H__

#include <Inventor/fields/SoMField.h>
#include <Inventor/fields/SoSubField.h>
#include <Inventor/SbColor.h>


class SoMFColor : public SoMField {
    typedef SoMField inherited;

  SO_MFIELD_HEADER(SoMFColor, SbColor, const SbColor &);

public:
  static void initClass(void);

  void setValues(const int start, const int num, const float rgb[][3]);
  void setHSVValues(const int start, const int num, const float hsv[][3]);

  void setValue(const SbVec3f & vec);
  void setValue(const float r, const float g, const float b);
  void setValue(const float rgb[3]);

  void setHSVValue(const float h, const float s, const float v);
  void setHSVValue(const float hsv[3]);

  void set1Value(const int idx, const SbVec3f & vec);
  void set1Value(const int idx, const float r, const float g, const float b);
  void set1Value(const int idx, const float rgb[3]);

  void set1HSVValue(const int idx, const float h, const float s, const float v);
  void set1HSVValue(const int idx, const float hsv[3]);

private:
  virtual void convertTo(SoField * dest) const;
};

//$ INSERT INLINE ../../../src/fields/SoMFColor.cpp

#endif // !__SOMFCOLOR_H__
