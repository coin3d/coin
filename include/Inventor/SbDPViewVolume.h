#ifndef COIN_SBDPVIEWVOLUME_H
#define COIN_SBDPVIEWVOLUME_H

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
#include <Inventor/SbVec3d.h>

class SbBox3f;
class SbDPLine;
class SbDPMatrix;
class SbPlane;
class SbDPRotation;
class SbVec2d;
class SbViewVolume;

class COIN_DLL_API SbDPViewVolume {
public:
  enum ProjectionType { ORTHOGRAPHIC = 0, PERSPECTIVE = 1 };

public:
  SbDPViewVolume(void);
  ~SbDPViewVolume(void);
  void getMatrices(SbDPMatrix& affine, SbDPMatrix& proj) const;
  SbDPMatrix getMatrix(void) const;
  SbDPMatrix getCameraSpaceMatrix(void) const;
  void projectPointToLine(const SbVec2d& pt, SbDPLine& line) const;
  void projectPointToLine(const SbVec2d& pt,
                          SbVec3d& line0, SbVec3d& line1) const;
  void projectToScreen(const SbVec3d& src, SbVec3d& dst) const;
  SbPlane getPlane(const double distFromEye) const;
  SbVec3d getSightPoint(const double distFromEye) const;
  SbVec3d getPlanePoint(const double distFromEye,
                        const SbVec2d& normPoint) const;
  SbDPRotation getAlignRotation(SbBool rightAngleOnly = FALSE) const;
  double getWorldToScreenScale(const SbVec3d& worldCenter,
                              double normRadius) const;
  SbVec2d projectBox(const SbBox3f& box) const;
  SbDPViewVolume narrow(double left, double bottom,
                      double right, double top) const;
  SbDPViewVolume narrow(const SbBox3f& box) const;
  void ortho(double left, double right,
             double bottom, double top,
             double nearval, double farval);
  void perspective(double fovy, double aspect,
                   double nearval, double farval);
  void rotateCamera(const SbDPRotation& q);
  void translateCamera(const SbVec3d& v);
  SbVec3d zVector(void) const;
  SbDPViewVolume zNarrow(double nearval, double farval) const;
  void scale(double factor);
  void scaleWidth(double ratio);
  void scaleHeight(double ratio);
  ProjectionType getProjectionType(void) const;
  const SbVec3d& getProjectionPoint(void) const;
  const SbVec3d& getProjectionDirection(void) const;
  double getNearDist(void) const;
  double getWidth(void) const;
  double getHeight(void) const;
  double getDepth(void) const;

  void print(FILE * fp) const;
  void getViewVolumePlanes(SbPlane planes[6]) const;
  void transform(const SbDPMatrix &matrix);
  SbVec3d getViewUp(void) const;

  void copyValues(SbViewVolume & vv);

private:

  ProjectionType type;
  SbVec3d projPoint;
  SbVec3d projDir;
  double nearDist;
  double nearToFar;
  SbVec3d llf;
  SbVec3d lrf;
  SbVec3d ulf;

  static SbDPMatrix getOrthoProjection(const double left, const double right,
                                       const double bottom, const double top,
                                       const double nearval, const double farval);
  static SbDPMatrix getPerspectiveProjection(const double left, const double right,
                                             const double bottom, const double top,
                                             const double nearval, const double farval);
  void getPlaneRectangle(const double depth, SbVec3d & lowerleft,
                         SbVec3d & lowerright, SbVec3d & upperleft,
                         SbVec3d & upperright) const;
};

#endif // !COIN_SBDPVIEWVOLUME_H
