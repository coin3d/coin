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

#ifndef COIN_SOSHAPE_PRIMDATA_H
#define COIN_SOSHAPE_PRIMDATA_H

#include <Inventor/nodes/SoShape.h>

// Private class used only by SoShape to aid in generating primitives,
// handling details ++

class SoDetail;
class SoAction;
class SoPrimitiveVertex;
class SoPointDetail;
class SoFaceDetail;
class SoLineDetail;
class SbTesselator;

class soshape_primdata {
public:
  soshape_primdata(void);

  ~soshape_primdata();

  void beginShape(SoShape * shape, SoAction * action,
                  SoShape::TriangleShape shapetype,
                  SoDetail * detail);
  void endShape(void);

  void shapeVertex(const SoPrimitiveVertex * const v);

private:
  void copyVertex(const int src, const int dest);
  void setVertex(const int idx, const SoPrimitiveVertex * const v);
  void handleFaceDetail(const int numv);
  void handleLineDetail(void);
  SoDetail * createPickDetail(void);
  static void tess_callback(void * v0, void * v1, void * v2, void * data);

private:
  friend class SoShape;

  SoShape::TriangleShape shapetype;
  SoAction * action;
  SoShape * shape;
  SoPrimitiveVertex * vertsArray;
  SoPointDetail * pointDetails;
  SoFaceDetail * faceDetail;
  SoLineDetail * lineDetail;
  int arraySize;
  int counter;
  SbTesselator * tess;
  int faceCounter;
};


#endif // !COIN_SOSHAPE_PRIMDATA_H
