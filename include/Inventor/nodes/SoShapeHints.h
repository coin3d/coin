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

#ifndef __SOSHAPEHINTS_H__
#define __SOSHAPEHINTS_H__

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/nodes/SoNode.h>


class SoShapeHints : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoShapeHints);

public:
  static void initClass(void);
  SoShapeHints(void);

  // These must match 100% with the enum in SoShapeHintsElement.
  enum VertexOrdering {
    UNKNOWN_ORDERING,
    CLOCKWISE,
    COUNTERCLOCKWISE
  };

  // These must match 100% with the enum in SoShapeHintsElement.
  enum ShapeType {
    UNKNOWN_SHAPE_TYPE,
    SOLID
  };

  // These must match 100% with the enum in SoShapeHintsElement.
  enum FaceType {
    UNKNOWN_FACE_TYPE,
    CONVEX
  };

  SoSFEnum vertexOrdering;
  SoSFEnum shapeType;
  SoSFEnum faceType;
  SoSFFloat creaseAngle;


  virtual void doAction(SoAction *action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction *action);
  virtual void pick(SoPickAction * action);

protected:
  virtual ~SoShapeHints();
};

#endif // !__SOSHAPEHINTS_H__
