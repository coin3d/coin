/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOSFMATRIX_H
#define COIN_SOSFMATRIX_H

#include <Inventor/fields/SoSField.h>
#include <Inventor/fields/SoSubField.h>
#include <Inventor/SbMatrix.h>


class COIN_DLL_EXPORT SoSFMatrix : public SoSField {
  typedef SoSField inherited;

  SO_SFIELD_HEADER(SoSFMatrix, SbMatrix, const SbMatrix &);

public:
  static void initClass(void);

  void setValue(const float a11, const float a12,
                const float a13, const float a14,
                const float a21, const float a22,
                const float a23, const float a24,
                const float a31, const float a32,
                const float a33, const float a34,
                const float a41, const float a42,
                const float a43, const float a44);
};

#endif // !COIN_SOSFMATRIX_H
