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
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOLINEDETAIL_H
#define COIN_SOLINEDETAIL_H

#include <Inventor/details/SoPointDetail.h>


class COIN_DLL_API SoLineDetail : public SoDetail {
  typedef SoDetail inherited;

  SO_DETAIL_HEADER(SoLineDetail);

public:
  SoLineDetail(void);
  virtual ~SoLineDetail();

  static void initClass(void);
  virtual SoDetail * copy(void) const;

  const SoPointDetail * getPoint0(void) const;
  const SoPointDetail * getPoint1(void) const;

  int getLineIndex(void) const;
  int getPartIndex(void) const;

  void setPoint0(const SoPointDetail * const detail);
  void setPoint1(const SoPointDetail * const detail);
  void setLineIndex(const int idx);
  void setPartIndex(const int idx);

  void incLineIndex(void);
  void incPartIndex(void);

private:
  SoPointDetail points[2];
  int lineindex;
  int partindex;
};

#endif // COIN_SOLINEDETAIL_H
