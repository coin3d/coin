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

#ifndef __SOSHAPEHINTS_H__
#define __SOSHAPEHINTS_H__

#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/nodes/SoNode.h>

#if defined(COIN_EXCLUDE_SOSHAPEHINTS)
#error Configuration settings disrespected -- do not include this file!
#endif // COIN_EXCLUDE_SOSHAPEHINTS

// *************************************************************************

class SoShapeHints : public SoNode {
  typedef SoNode inherited;

//$ BEGIN TEMPLATE NodeHeader(SoShapeHints)
private:
  static SoType classTypeId;

public:
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
  static void * createInstance(void);
public:
  static void initClass(void);
  static void cleanClass(void);

  SoShapeHints(void);
protected:
  virtual ~SoShapeHints();
//$ END TEMPLATE NodeHeader

public:
  // These must match 100% with the enum in SoShapeHintsElement.
  enum VertexOrdering {
    UNKNOWN_ORDERING,
    CLOCKWISE,
    COUNTERCLOCKWISE,
  };

  // These must match 100% with the enum in SoShapeHintsElement.
  enum ShapeType {
    UNKNOWN_SHAPE_TYPE,
    SOLID,
  };
  
  // These must match 100% with the enum in SoShapeHintsElement.
  enum FaceType {
    UNKNOWN_FACE_TYPE,
    CONVEX,
  };

  SoSFEnum vertexOrdering;
  SoSFEnum shapeType;
  SoSFEnum faceType;
  SoSFFloat creaseAngle;


#if !defined(COIN_EXCLUDE_SOACTION)
  virtual void doAction(SoAction *action);
#endif // !COIN_EXCLUDE_SOACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  virtual void GLRender(SoGLRenderAction * action);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
  virtual void callback(SoCallbackAction * action);
#endif // COIN_EXCLUDE_SOCALLBACKACTION
#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
  virtual void getBoundingBox(SoGetBoundingBoxAction *action);
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
#if !defined(COIN_EXCLUDE_SOPICKACTION)
  virtual void pick(SoPickAction * action);
#endif // !COIN_EXCLUDE_SOPICKACTION
};

#endif // !__SOSHAPEHINTS_H__
