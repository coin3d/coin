#ifndef COIN_SOCYLINDERDETAIL_H
#define COIN_SOCYLINDERDETAIL_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/details/SoSubDetail.h>

class COIN_DLL_API SoCylinderDetail : public SoDetail {
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

#endif // !COIN_SOCYLINDERDETAIL_H
