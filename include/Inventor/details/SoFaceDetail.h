/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOFACEDETAIL_H
#define COIN_SOFACEDETAIL_H

#include <Inventor/details/SoPointDetail.h>


class COIN_DLL_EXPORT SoFaceDetail : public SoDetail {
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

#endif // COIN_SOFACEDETAIL_H
