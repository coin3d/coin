#ifndef COIN_SBTESSELATOR_H
#define COIN_SBTESSELATOR_H

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

#include <Inventor/SbBasic.h>
#include <Inventor/tools/SbPimplPtr.h>
#include <Inventor/SbVec3f.h>
#include <stddef.h>

typedef void SbTesselatorCB(void * v0, void * v1, void * v2, void * data);

class COIN_DLL_API SbTesselator {
public:
  SbTesselator(SbTesselatorCB * func = NULL, void * data = NULL);
  ~SbTesselator(void);

  void beginPolygon(SbBool keepVertices = FALSE,
                    const SbVec3f & normal = SbVec3f(0.0f, 0.0f, 0.0f));
  void addVertex(const SbVec3f &v, void * data);
  void endPolygon(void);
  void setCallback(SbTesselatorCB * func, void * data);

private:
  class PImpl;
  SbPimplPtr<PImpl> pimpl;

  SbTesselator(const SbTesselator & rhs); // N/A
  SbTesselator & operator = (const SbTesselator & rhs); // N/A

}; // SbTessellator

#endif // !COIN_SBTESSELATOR_H
