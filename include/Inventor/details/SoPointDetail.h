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

#ifndef COIN_SOPOINTDETAIL_H
#define COIN_SOPOINTDETAIL_H

#include <Inventor/details/SoSubDetail.h>


class COIN_DLL_EXPORT SoPointDetail : public SoDetail {
  typedef SoDetail inherited;

  SO_DETAIL_HEADER(SoPointDetail);

public:
  SoPointDetail(void);
  virtual ~SoPointDetail();
  static void initClass(void);
  virtual SoDetail * copy(void) const;

  int getCoordinateIndex(void) const;
  int getMaterialIndex(void) const;
  int getNormalIndex(void) const;
  int getTextureCoordIndex(void);

  void setCoordinateIndex(const int idx);
  void setMaterialIndex(const int idx);
  void setNormalIndex(const int idx);
  void setTextureCoordIndex(const int idx);

private:
  int coordindex;
  int matindex;
  int normindex;
  int texcoordindex;
};

#endif // COIN_SOPOINTDETAIL_H
