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

#ifndef __SORAYPICKACTION_H__
#define __SORAYPICKACTION_H__

#include <Inventor/actions/SoPickAction.h>
#include <Inventor/SbViewVolume.h>
#include <Inventor/SbLine.h>
#include <Inventor/SbPlane.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/lists/SoPickedPointList.h>

class SbVec2s;
class SbVec2f;
class SbVec3f;
class SbViewportRegion;
class SoPickedPoint;


class SoRayPickAction : public SoPickAction {
  typedef SoPickAction inherited;

//$ BEGIN TEMPLATE ActionHeader(SoRayPickAction)
private:
  static SoType classTypeId;

public:
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId(void);

protected:
  virtual const SoEnabledElementsList & getEnabledElements(void) const;
  static SoEnabledElementsList * enabledElements;
  static SoActionMethodList * methods;

public:
  static void addMethod(const SoType type, SoActionMethod method);
  static void enableElement(const SoType type, const int stackIndex);

  static void initClass(void);

  virtual ~SoRayPickAction();
//$ END TEMPLATE ActionHeader

public:
  SoRayPickAction(const SbViewportRegion & viewportRegion);

  void setPoint(const SbVec2s & viewportPoint);
  void setNormalizedPoint(const SbVec2f & normPoint);
  void setRadius(const float radiusInPixels);
  void setRay(const SbVec3f & start, const SbVec3f & direction,
              float nearDistance = -1.0,
              float farDistance = -1.0);
  void setPickAll(const SbBool flag);
  SbBool isPickAll() const;
  const SoPickedPointList & getPickedPointList() const;
  SoPickedPoint *getPickedPoint(const int index = 0) const;


  void computeWorldSpaceRay();
  SbBool hasWorldSpaceRay() const;
  void setObjectSpace();
  void setObjectSpace(const SbMatrix &matrix);
  SbBool intersect(const SbVec3f &v0,
                   const SbVec3f &v1,
                   const SbVec3f &v2,
                   SbVec3f &intersection, SbVec3f &barycentric,
                   SbBool &front) const;
  SbBool intersect(const SbVec3f &v0, const SbVec3f &v1,
                   SbVec3f &intersection) const;
  SbBool intersect(const SbVec3f &point) const;
  SbBool intersect(const SbBox3f &box,
                   const SbBool useFullViewVolume = TRUE);
  const SbViewVolume &getViewVolume();
  const SbLine &getLine();
  SbBool isBetweenPlanes(const SbVec3f &intersection) const;
  SoPickedPoint *addIntersection(const SbVec3f &objectSpacePoint);

protected:

  virtual void beginTraversal(SoNode *node);

private:

  void cleanupPickedPoints(void);
  void setFlag(const unsigned int flag);
  void clearFlag(const unsigned int flag);
  SbBool isFlagSet(const unsigned int flag) const;
  void calcObjectSpaceData();
  void calcMatrices();
  float calcRayRadius(const float radiusInPixels);

  SbViewVolume osVolume;
  SbLine osLine;
  SbPlane nearPlane;
  SbVec2s vpPoint;
  SbVec2f normvpPoint;
  SbVec3f rayStart;
  SbVec3f rayDirection;
  float rayRadiusStart;
  float rayRadiusDelta;
  float rayNear;
  float rayFar;
  float radiusInPixels;

  float currentPickDistance;

  SbLine wsLine;
  SbMatrix obj2World;
  SbMatrix world2Obj;
  SbMatrix extraMatrix;

  SoPickedPointList pickedPointList;

  unsigned int flags;
};

#endif // !__SORAYPICKACTION_H__
