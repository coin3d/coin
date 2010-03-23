#ifndef COIN_SOFACEDETAIL_H
#define COIN_SOFACEDETAIL_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/details/SoPointDetail.h>

class COIN_DLL_API SoFaceDetail : public SoDetail {
  typedef SoDetail inherited;

  SO_DETAIL_HEADER(SoFaceDetail);

public:
  SoFaceDetail(void);
  virtual ~SoFaceDetail();

  static void initClass(void);
  virtual SoDetail * copy(void) const;

  int getNumPoints(void) const;
  const SoPointDetail * getPoint(const int idx) const;
  SoPointDetail * getPoints(void);
  int getFaceIndex(void) const;
  int getPartIndex(void) const;

  void setNumPoints(const int num);
  void setPoint(const int idx, const SoPointDetail * const detail);
  void setFaceIndex(const int idx);
  void setPartIndex(const int idx);

  void incFaceIndex(void);
  void incPartIndex(void);

private:
  SoPointDetail * pointsarray;
  int numallocated;
  int numpoints;
  int faceindex;
  int partindex;
};

#endif // !COIN_SOFACEDETAIL_H
