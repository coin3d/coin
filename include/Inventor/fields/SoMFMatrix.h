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

#ifndef COIN_SOMFMATRIX_H
#define COIN_SOMFMATRIX_H

#include <Inventor/fields/SoMField.h>
#include <Inventor/fields/SoSubField.h>
#include <Inventor/SbMatrix.h>


class COIN_DLL_API SoMFMatrix : public SoMField {
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
