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

#ifndef COIN_SOCONEDETAIL_H
#define COIN_SOCONEDETAIL_H

#include <Inventor/details/SoSubDetail.h>


class COIN_DLL_EXPORT SoConeDetail : public SoDetail {
  typedef SoDetail inherited;

  SO_DETAIL_HEADER(SoConeDetail);

public:
  SoConeDetail(void);
  virtual ~SoConeDetail();

  static void initClass(void);
  virtual SoDetail * copy(void) const;

  void setPart(const int part);
  int getPart(void) const;

protected:
  int part;
};

#endif // COIN_SOCONEDETAIL_H
