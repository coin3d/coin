/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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

#ifndef __SOSHAPE_H__
#define __SOSHAPE_H__

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/SbBox3f.h>

#if defined(COIN_EXCLUDE_SOSHAPE)
#error Configuration settings disrespected -- do not include this file!
#endif // COIN_EXCLUDE_SOSHAPE

class SoPrimitiveVertex;
class SoDetail;
class SoPickedPoint;
class SoFaceDetail;
class SoState;
class SoTextureCoordinateElement;
class SbVec2f;
class SoMaterialBundle;


// *************************************************************************

class SoShape : public SoNode {
  typedef SoNode inherited;

  SO_NODE_ABSTRACT_HEADER(SoShape);

public:
  static void initClass(void);

  enum TriangleShape {
    TRIANGLE_STRIP,
    TRIANGLE_FAN,
    TRIANGLES,
    POLYGON,
    QUADS,
    // the following are not part of OIV-API
    QUAD_STRIP,
    POINTS,
    LINES,
    LINE_STRIP
  };

  virtual SbBool affectsState(void) const;

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  virtual void GLRender(SoGLRenderAction * action);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
  virtual void rayPick(SoRayPickAction * action);
#endif // !COIN_EXCLUDE_SORAYPICKACTION
#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
  virtual void callback(SoCallbackAction * action);
#endif // !COIN_EXCLUDE_SOCALLBACKACTION
#if !defined(COIN_EXCLUDE_SOACTION)
  virtual void computeBBox(SoAction * action, SbBox3f & box,
			   SbVec3f & center) =  0;
#endif // !COIN_EXCLUDE_SOACTION
#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION
  
  static void getScreenSize(SoState * const state, 
			    const SbBox3f & boundingBox,
			    SbVec2s & rectSize);
  static float getDecimatedComplexity(SoState * state, float complexity);

protected:
  friend class shapePrimitiveData; // internal class
  SoShape(void);
  virtual ~SoShape();

#if !defined(COIN_EXCLUDE_SOACTION)
  float getComplexityValue(SoAction *action);
  virtual void generatePrimitives(SoAction * action) =  0;
#endif // !COIN_EXCLUDE_SOACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  virtual SbBool shouldGLRender(SoGLRenderAction * action);
  void beginSolidShape(SoGLRenderAction * action);
  void endSolidShape(SoGLRenderAction * action);
  void GLRenderBoundingBox(SoGLRenderAction * action);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
  SbBool shouldPrimitiveCount(SoGetPrimitiveCountAction * action);
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  virtual SbBool willSetShadeModel() const;
  virtual SbBool willSetShapeHints() const;
  virtual SbBool willUpdateNormalizeElement(SoState *state) const;
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
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
#endif // !COIN_EXCLUDE_SORAYPICKACTION

#if !defined(COIN_EXCLUDE_SOACTION)
  void invokeTriangleCallbacks(SoAction * const action,
			       const SoPrimitiveVertex * const v1,
			       const SoPrimitiveVertex * const v2,
			       const SoPrimitiveVertex * const v3);
  void invokeLineSegmentCallbacks(SoAction * const action,
				  const SoPrimitiveVertex * const v1,
				  const SoPrimitiveVertex * const v2);
  void invokePointCallbacks(SoAction * const action,
			    const SoPrimitiveVertex * const v);
  void beginShape(SoAction * const action, const TriangleShape shapeType,
		  SoDetail * const detail = NULL);
  void shapeVertex(const SoPrimitiveVertex * const v);
  void endShape();
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  void generateVertex(SoPrimitiveVertex * const pv,
		      const SbVec3f & point,
		      const SbBool useTexFunc,
		      const SoTextureCoordinateElement * const tce,
		      const float s,
		      const float t,
		      const SbVec3f & normal);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
};

#endif // !__SOSHAPE_H__
