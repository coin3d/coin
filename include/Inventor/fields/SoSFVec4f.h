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

#ifndef __SOSFVEC4F_H__
#define __SOSFVEC4F_H__

#include <Inventor/fields/SoSField.h>
#include <Inventor/fields/SoSubField.h>
#include <Inventor/SbVec4f.h>

class SoSFVec4f : public SoSField {
  typedef SoSField inherited;

  SO_SFIELD_HEADER(SoSFVec4f, SbVec4f, const SbVec4f &);

public:
  static void initClass(void);

  void setValue(const float x, const float y, const float z, const float w);
  void setValue(const float xyzw[4]);

private:
  virtual void convertTo(SoField * dest) const;

  friend class SoMFVec4f; // For readValue() & writeValue().
};

#endif // !__SOSFVEC4F_H__
