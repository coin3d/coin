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

#ifndef COIN_SOMFMATRIX_H
#define COIN_SOMFMATRIX_H

#include <Inventor/fields/SoMField.h>
#include <Inventor/fields/SoSubField.h>
#include <Inventor/SbMatrix.h>


class COIN_DLL_EXPORT SoMFMatrix : public SoMField {
  typedef SoMField inherited;

  SO_MFIELD_HEADER(SoMFMatrix, SbMatrix, const SbMatrix &);

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

#endif // !COIN_SOMFMATRIX_H
