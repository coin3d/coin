#ifndef COIN_SOTEXTDETAIL_H
#define COIN_SOTEXTDETAIL_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

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

#endif // !COIN_SOTEXTDETAIL_H
