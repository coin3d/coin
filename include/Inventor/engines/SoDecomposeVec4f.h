/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#ifndef COIN_SODECOMPOSEVEC4F_H
#define COIN_SODECOMPOSEVEC4F_H

// Warning for application programmers: do not include this header
// file in your application code if you want to stay sourcecode
// compatible with SGI or TGS Inventor.  In those Inventor
// implementations, all SoCompose* and SoDecompose* engine classes are
// defined in the header file SoCompose.h.

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/fields/SoMFFloat.h>
#include <Inventor/fields/SoMFVec4f.h>


class COIN_DLL_API SoDecomposeVec4f : public SoEngine {
  typedef SoEngine inherited;
  SO_COMPOSE__HEADER(SoDecomposeVec4f);

public:
  SoMFVec4f vector;

  SoEngineOutput x; // SoMFFloat
  SoEngineOutput y; // SoMFFloat
  SoEngineOutput z; // SoMFFloat
  SoEngineOutput w; // SoMFFloat
};

#endif // !COIN_SODECOMPOSEVEC4F_H
