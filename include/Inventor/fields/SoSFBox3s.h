#ifndef COIN_SOSFBOX3S_H
#define COIN_SOSFBOX3S_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/


#include <Inventor/fields/SoSField.h>
#include <Inventor/fields/SoSubField.h>
#include <Inventor/SbBox3s.h>

class SoSFBox3s : public SoSField {
  typedef SoSField inherited;

  SO_SFIELD_HEADER(SoSFBox3s, SbBox3s, SbBox3s);

public:
  static void initClass(void);

  void setValue(short xmin,
                short ymin,
                short zmin,
                short xmax,
                short ymax,
                short zmax);
  void setValue(const SbVec3s & minvec, const SbVec3s & maxvec);
  void getValue(SbBox3s & box);
};

#endif // !COIN_SOSFBOX3S_H


