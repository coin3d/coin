#ifndef COIN_SOSHAPE_H
#define COIN_SOSHAPE_H

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

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/SbBox3f.h>

class SoPrimitiveVertex;
class SoDetail;
class SoPickedPoint;
class SoFaceDetail;
class SoState;
class SoTextureCoordinateElement;
class SbVec2f;
class SoMaterialBundle;

class COIN_DLL_API SoShape : public SoNode {
  typedef SoNode inherited;

  SO_NODE_ABSTRACT_HEADER(SoShape);

public:
  static void initClass(void);

  enum TriangleShape {
    TRIANGLE_STRIP, TRIANGLE_FAN, TRIANGLES, POLYGON,
    // The rest of the enums are not part of the original Inventor API.
    QUADS, QUAD_STRIP, POINTS, LINES, LINE_STRIP
  };

  virtual SbBool affectsState(void) const;
  virtual void notify(SoNotList * nl);

  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void rayPick(SoRayPickAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void computeBBox(SoAction * action, SbBox3f & box,
                           SbVec3f & center) =  0;
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

  static void getScreenSize(SoState * const state, const SbBox3f & boundingbox,
                            SbVec2s & rectsize);
  static float getDecimatedComplexity(SoState * state, float complexity);

protected:
  SoShape(void);
  virtual ~SoShape();

  float getComplexityValue(SoAction * action);
  virtual void generatePrimitives(SoAction * action) =  0;
  virtual SbBool shouldGLRender(SoGLRenderAction * action);
  void beginSolidShape(SoGLRenderAction * action);
  void endSolidShape(SoGLRenderAction * action);
  void GLRenderBoundingBox(SoGLRenderAction * action);
  SbBool shouldPrimitiveCount(SoGetPrimitiveCountAction * action);

  SbBool shouldRayPick(SoRayPickAction * const action);
  void computeObjectSpaceRay(SoRayPickAction * const action);
  void computeObjectSpaceRay(SoRayPickAction * const action,
                             const SbMatrix & matrix);
  virtual SoDetail * createTriangleDetail(SoRayPickAction * action,
                                          const SoPrimitiveVertex * v1,
                                          const SoPrimitiveVertex * v2,
                                          const SoPrimitiveVertex * v3,
                                          SoPickedPoint * pp);
  virtual SoDetail * createLineSegmentDetail(SoRayPickAction * action,
                                             const SoPrimitiveVertex * v1,
                                             const SoPrimitiveVertex * v2,
                                             SoPickedPoint * pp);
  virtual SoDetail * createPointDetail(SoRayPickAction * action,
                                       const SoPrimitiveVertex * v,
                                       SoPickedPoint * pp);

  void invokeTriangleCallbacks(SoAction * const action,
                               const SoPrimitiveVertex * const v1,
                               const SoPrimitiveVertex * const v2,
                               const SoPrimitiveVertex * const v3);
  void invokeLineSegmentCallbacks(SoAction * const action,
                                  const SoPrimitiveVertex * const v1,
                                  const SoPrimitiveVertex * const v2);
  void invokePointCallbacks(SoAction * const action,
                            const SoPrimitiveVertex * const v);
  void beginShape(SoAction * const action, const TriangleShape shapetype,
                  SoDetail * const detail = NULL);
  void shapeVertex(const SoPrimitiveVertex * const v);
  void endShape(void);

  void generateVertex(SoPrimitiveVertex * const pv,
                      const SbVec3f & point,
                      const SbBool useTexFunc,
                      const SoTextureCoordinateElement * const tce,
                      const float s,
                      const float t,
                      const SbVec3f & normal);
  void generateVertex(SoPrimitiveVertex * const pv,
                      const SbVec3f & point,
                      const SbBool useTexFunc,
                      const SoTextureCoordinateElement * const tce,
                      const float s,
                      const float t,
                      const float r,
                      const SbVec3f & normal);

private:
  class SoShapeP * pimpl;

  void getBBox(SoAction * action, SbBox3f & box, SbVec3f & center);
  void rayPickBoundingBox(SoRayPickAction * action);
  friend class soshape_primdata;           // internal class
  friend class so_generate_prim_private;   // a very private class
};

#endif // !COIN_SOSHAPE_H
