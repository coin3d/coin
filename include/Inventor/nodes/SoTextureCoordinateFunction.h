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

#ifndef COIN_SOTEXTURECOORDINATEFUNCTION_H
#define COIN_SOTEXTURECOORDINATEFUNCTION_H

#include <Inventor/nodes/SoSubNode.h>


class COIN_DLL_EXPORT SoTextureCoordinateFunction : public SoNode {
  typedef SoNode inherited;

  SO_NODE_ABSTRACT_HEADER(SoTextureCoordinateFunction);

public:
  static void initClass(void);

  // The constructor for this class is public in Inventor. Should
  // _probably_ have been protected, but we mimic Inventor for the
  // sake of compatibility.
  SoTextureCoordinateFunction(void);

protected:
  virtual ~SoTextureCoordinateFunction();
};

#endif // !COIN_SOTEXTURECOORDINATEFUNCTION_H
