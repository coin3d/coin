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

#ifndef COIN_SOSHAPEHINTS_H
#define COIN_SOSHAPEHINTS_H

#include <Inventor/elements/SoShapeHintsElement.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoSubNode.h>


class SoShapeHints : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoShapeHints);

public:
  static void initClass(void);
  SoShapeHints(void);

  enum VertexOrdering {
    UNKNOWN_ORDERING = SoShapeHintsElement::UNKNOWN_ORDERING,
    CLOCKWISE = SoShapeHintsElement::CLOCKWISE,
    COUNTERCLOCKWISE = SoShapeHintsElement::COUNTERCLOCKWISE
  };

  enum ShapeType {
    UNKNOWN_SHAPE_TYPE = SoShapeHintsElement::UNKNOWN_SHAPE_TYPE,
    SOLID = SoShapeHintsElement::SOLID
  };

  enum FaceType {
    UNKNOWN_FACE_TYPE = SoShapeHintsElement::UNKNOWN_FACE_TYPE,
    CONVEX = SoShapeHintsElement::CONVEX
  };

  SoSFEnum vertexOrdering;
  SoSFEnum shapeType;
  SoSFEnum faceType;
  SoSFFloat creaseAngle;


  virtual void doAction(SoAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void pick(SoPickAction * action);

protected:
  virtual ~SoShapeHints();
};

#endif // !COIN_SOSHAPEHINTS_H
