#ifndef COIN_SOSHAPEHINTSELEMENT_H
#define COIN_SOSHAPEHINTSELEMENT_H

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
  virtual void pop(SoState * state, const SoElement * prevtopelement);

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
  void updateLazyElement(SoState * state);
  virtual void setElt(VertexOrdering vertexOrdering,
                      ShapeType shapeType, FaceType faceType);

  VertexOrdering vertexOrdering;
  ShapeType shapeType;
  FaceType faceType;

};

#endif // !COIN_SOSHAPEHINTSELEMENT_H
