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

#ifndef COIN_SOMFROTATION_H
#define COIN_SOMFROTATION_H

#include <Inventor/fields/SoMField.h>
#include <Inventor/fields/SoSubField.h>
#include <Inventor/SbRotation.h>


class COIN_DLL_API SoMFRotation : public SoMField {
  typedef SoMField inherited;

  SO_MFIELD_HEADER(SoMFRotation, SbRotation, const SbRotation &);

public:
  static void initClass(void);

  void setValues(const int start, const int num, const float q[][4]);
  void set1Value(const int idx, const float q0, const float q1, const float q2, const float q3);
  void set1Value(const int idx, const float q[4]);
  void set1Value(const int idx, const SbVec3f & axis, const float angle);
  void setValue(const float q0, const float q1, const float q2, const float q3);
  void setValue(const float q[4]);
  void setValue(const SbVec3f & axis, const float angle);
};

#endif // !COIN_SOMFROTATION_H
