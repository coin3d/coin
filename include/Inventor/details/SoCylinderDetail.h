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

#ifndef COIN_SOCYLINDERDETAIL_H
#define COIN_SOCYLINDERDETAIL_H

#include <Inventor/details/SoSubDetail.h>


class COIN_DLL_EXPORT SoCylinderDetail : public SoDetail {
  typedef SoDetail inherited;

  SO_DETAIL_HEADER(SoCylinderDetail);

public:
  SoCylinderDetail(void);
  virtual ~SoCylinderDetail();

  static void initClass(void);
  virtual SoDetail * copy(void) const;

  void setPart(const int part);
  int getPart(void) const;

protected:
  int part;
};

#endif // COIN_SOCYLINDERDETAIL_H
