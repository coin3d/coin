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
