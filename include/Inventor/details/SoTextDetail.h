/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
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

#ifndef COIN_SOTEXTDETAIL_H
#define COIN_SOTEXTDETAIL_H

#include <Inventor/details/SoSubDetail.h>


class COIN_DLL_API SoTextDetail : public SoDetail {
  typedef SoDetail inherited;

  SO_DETAIL_HEADER(SoTextDetail);

public:
  SoTextDetail(void);
  virtual ~SoTextDetail();

  static void initClass(void);
  virtual SoDetail * copy(void) const;

  int getStringIndex(void) const;
  int getCharacterIndex(void) const;
  int getPart(void) const;

  void setStringIndex(const int idx);
  void setCharacterIndex(const int idx);
  void setPart(const int idx);

private:
  int stringindex;
  int charindex;
  int part;
};

#endif // COIN_SOTEXTDETAIL_H
