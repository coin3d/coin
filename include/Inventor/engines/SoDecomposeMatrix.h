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

#ifndef COIN_SODECOMPOSEMATRIX_H
#define COIN_SODECOMPOSEMATRIX_H

// Warning for application programmers: do not include this header
// file in your application code if you want to stay sourcecode
// compatible with SGI or TGS Inventor.  In those Inventor
// implementations, all SoCompose* and SoDecompose* engine classes are
// defined in the header file SoCompose.h.

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/fields/SoMFMatrix.h>
#include <Inventor/fields/SoMFVec3f.h>


class COIN_DLL_API SoDecomposeMatrix : public SoEngine {
  typedef SoEngine inherited;
  SO_COMPOSE__HEADER(SoDecomposeMatrix);

public:
  SoMFMatrix matrix;
  SoMFVec3f center;

  SoEngineOutput translation; // SoMFVec3f
  SoEngineOutput rotation; // SoMFRotation
  SoEngineOutput scaleFactor; // SoMFVec3f
  SoEngineOutput scaleOrientation; // SoMFRotation
};

#endif // !COIN_SODECOMPOSEMATRIX_H
