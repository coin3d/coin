#ifndef COIN_SOENGINELIST_H
#define COIN_SOENGINELIST_H

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

#include <Inventor/lists/SoBaseList.h>

class SoEngine;

class COIN_DLL_API SoEngineList : public SoBaseList {
  typedef SoBaseList inherited;

public:
  SoEngineList(void);
  SoEngineList(const int size);
  SoEngineList(const SoEngineList & el);
  ~SoEngineList();

  void append(SoEngine * const ptr);
  SoEngine * operator[](const int i) const;
  SoEngineList & operator=(const SoEngineList & el);
};

#endif // !COIN_SOENGINELIST_H
