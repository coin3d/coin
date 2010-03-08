#ifndef COIN_SOSHAPE_TRIANGLESORT_H
#define COIN_SOSHAPE_TRIANGLESORT_H

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

#ifndef COIN_INTERNAL
#error this is a private header file
#endif /* !COIN_INTERNAL */

#include <Inventor/lists/SbList.h>
#include <Inventor/SoPrimitiveVertex.h>

class SoState;
class SoPrimitiveVertex;
class SoMaterialBundle;

class soshape_trianglesort {
public:
  soshape_trianglesort();
  ~soshape_trianglesort();

  void beginShape(SoState * state);
  void triangle(SoState * state,
                const SoPrimitiveVertex * v1,
                const SoPrimitiveVertex * v2,
                const SoPrimitiveVertex * v3);
  void endShape(SoState * state, SoMaterialBundle & mb);

  typedef struct {
    int idx : 31;
    unsigned int backface : 1;
    float dist;
  } sorted_triangle;

private:

  SbList <SoPrimitiveVertex> * pvlist;
  SbList <sorted_triangle> * trianglelist;
};

#endif // !COIN_SOSHAPE_TRIANGLESORT_H
