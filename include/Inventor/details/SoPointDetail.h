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

#ifndef __SOPOINTDETAIL_H__
#define __SOPOINTDETAIL_H__

#include <Inventor/details/SoDetail.h>

class SoPointDetail : public SoDetail {
  typedef SoDetail inherited;
//$ BEGIN TEMPLATE DetailHeader(SoPointDetail)
public:
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId(void);
  static void initClass(void);
private:
  static SoType classTypeId;
//$ END TEMPLATE DetailHeader
public:
  SoPointDetail();
  virtual ~SoPointDetail();
  virtual SoDetail *copy() const;

  int getCoordinateIndex() const;
  int getMaterialIndex() const;
  int getNormalIndex() const;
  int getTextureCoordIndex();

  void setCoordinateIndex(const int idx);
  void setMaterialIndex(const int idx);
  void setNormalIndex(const int idx);
  void setTextureCoordIndex(const int idx);

private:
  int coordIndex;
  int matIndex;
  int normIndex;
  int texCoordIndex;
};

#endif // __SOPOINTDETAIL_H__
