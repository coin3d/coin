/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
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

class shapePrimitiveData {
public:
  shapePrimitiveData(void);

  ~shapePrimitiveData();

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


#endif // COIN_SOSHAPE_PRIMDATA_H
