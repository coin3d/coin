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

#ifndef COIN_SOTEXTDETAIL_H
#define COIN_SOTEXTDETAIL_H

#include <Inventor/details/SoSubDetail.h>


class COIN_DLL_EXPORT SoTextDetail : public SoDetail {
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
