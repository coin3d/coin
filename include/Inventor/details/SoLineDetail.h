#ifndef COIN_SOLINEDETAIL_H
#define COIN_SOLINEDETAIL_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

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

#endif // !COIN_SOLINEDETAIL_H
