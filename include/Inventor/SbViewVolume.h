#ifndef COIN_SBVIEWVOLUME_H
#define COIN_SBVIEWVOLUME_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

#include <stdio.h>

#include <Inventor/SbBasic.h>
#include <Inventor/SbVec3f.h>

class SbBox3f;
class SbLine;
class SbMatrix;
class SbPlane;
class SbRotation;
class SbVec2f;
class SbVec3f;

class COIN_DLL_API SbViewVolume {
public:
  enum ProjectionType { ORTHOGRAPHIC = 0, PERSPECTIVE = 1 };

public:
  SbViewVolume(void);
  ~SbViewVolume(void);
  void getMatrices(SbMatrix& affine, SbMatrix& proj) const;
  SbMatrix getMatrix(void) const;
  SbMatrix getCameraSpaceMatrix(void) const;
  void projectPointToLine(const SbVec2f& pt, SbLine& line) const;
  void projectPointToLine(const SbVec2f& pt,
                          SbVec3f& line0, SbVec3f& line1) const;
  void projectToScreen(const SbVec3f& src, SbVec3f& dst) const;
  SbPlane getPlane(const float distFromEye) const;
  SbVec3f getSightPoint(const float distFromEye) const;
  SbVec3f getPlanePoint(const float distFromEye,
                        const SbVec2f& normPoint) const;
  SbRotation getAlignRotation(SbBool rightAngleOnly = FALSE) const;
  float getWorldToScreenScale(const SbVec3f& worldCenter,
                              float normRadius) const;
  SbVec2f projectBox(const SbBox3f& box) const;
  SbViewVolume narrow(float left, float bottom,
                      float right, float top) const;
  SbViewVolume narrow(const SbBox3f& box) const;
  void ortho(float left, float right,
             float bottom, float top,
             float nearval, float farval);
  void perspective(float fovy, float aspect,
                   float nearval, float farval);
  void rotateCamera(const SbRotation& q);
  void translateCamera(const SbVec3f& v);
  SbVec3f zVector(void) const;
  SbViewVolume zNarrow(float nearval, float farval) const;
  void scale(float factor);
  void scaleWidth(float ratio);
  void scaleHeight(float ratio);
  ProjectionType getProjectionType(void) const;
  const SbVec3f& getProjectionPoint(void) const;
  const SbVec3f& getProjectionDirection(void) const;
  float getNearDist(void) const;
  float getWidth(void) const;
  float getHeight(void) const;
  float getDepth(void) const;

  void print(FILE * fp) const;
  void getViewVolumePlanes(SbPlane planes[6]) const;
  void transform(const SbMatrix &matrix);
  SbVec3f getViewUp(void) const;

public:
  // Warning! It's extremely bad design to keep these data members
  // public, but we have no choice since this is how it's done in
  // the original SGI Open Inventor. We've seen example code that
  // use these variables directly so we'll have to be compatible
  // here. Please don't use these variables directly unless you're
  // very sure about what you're doing.
  ProjectionType type;
  SbVec3f projPoint;
  SbVec3f projDir;
  float nearDist;
  float nearToFar;
  SbVec3f llf;
  SbVec3f lrf;
  SbVec3f ulf;

private:

    SbVec3f     llfO;                   // x = -w, y = -w, z = -w
    SbVec3f     lrfO;                   // x =  w, y = -w, z = -w
    SbVec3f     ulfO;                   // x = -w, y =  w, z = -w

  static SbMatrix getOrthoProjection(const float left, const float right,
                                     const float bottom, const float top,
                                     const float nearval, const float farval);
  static SbMatrix getPerspectiveProjection(const float left, const float right,
                                           const float bottom, const float top,
                                           const float nearval, const float farval);
  void getPlaneRectangle(const float depth, SbVec3f & lowerleft,
                         SbVec3f & lowerright, SbVec3f & upperleft,
                         SbVec3f & upperright) const;
};

#endif // !COIN_SBVIEWVOLUME_H
