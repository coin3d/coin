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
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOSHAPEHINTSELEMENT_H
#define COIN_SOSHAPEHINTSELEMENT_H

#include <Inventor/elements/SoSubElement.h>


class COIN_DLL_API SoShapeHintsElement : public SoElement {
  typedef SoElement inherited;

  SO_ELEMENT_HEADER(SoShapeHintsElement);
public:
  static void initClass(void);
protected:
  virtual ~SoShapeHintsElement();

public:
  enum VertexOrdering {
    UNKNOWN_ORDERING,
    CLOCKWISE,
    COUNTERCLOCKWISE,
    ORDERING_AS_IS
  };

  enum ShapeType {
    UNKNOWN_SHAPE_TYPE,
    SOLID,
    SHAPE_TYPE_AS_IS
  };

  enum FaceType {
    UNKNOWN_FACE_TYPE,
    CONVEX,
    FACE_TYPE_AS_IS
  };

  virtual void init(SoState * state);
  virtual void push(SoState * state);

  virtual SbBool matches(const SoElement * element) const;
  virtual SoElement *copyMatchInfo(void) const;

  static void set(SoState * const state, SoNode * const node,
                  const VertexOrdering vertexOrdering,
                  const ShapeType shapeType, const FaceType faceType);
  static void set(SoState * const state,
                  const VertexOrdering vertexOrdering,
                  const ShapeType shapeType, const FaceType faceType);
  static void get(SoState * const state, VertexOrdering & vertexOrdering,
                  ShapeType & shapeType, FaceType & faceType);

  static VertexOrdering getVertexOrdering(SoState * const state);
  static ShapeType getShapeType(SoState * const state);
  static FaceType getFaceType(SoState * const state);

  static VertexOrdering getDefaultVertexOrdering();
  static ShapeType getDefaultShapeType();
  static FaceType getDefaultFaceType();

  virtual void print(FILE * file) const;

protected:
  virtual void setElt(VertexOrdering vertexOrdering,
                      ShapeType shapeType, FaceType faceType);

  VertexOrdering vertexOrdering;
  ShapeType shapeType;
  FaceType faceType;

};

#endif // !COIN_SOSHAPEHINTSELEMENT_H
