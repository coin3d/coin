#ifndef COIN_SOACTIONMETHODLIST_H
#define COIN_SOACTIONMETHODLIST_H

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

#include <Inventor/lists/SbPList.h>
#include <Inventor/SoType.h>

class SoAction;
class SoNode;

typedef void (* SoActionMethod)(SoAction *, SoNode *);

class COIN_DLL_API SoActionMethodList : public SbPList {
  typedef SbPList inherited;

public:
  SoActionMethodList(SoActionMethodList * const parentlist);
  ~SoActionMethodList();

  SoActionMethod & operator[](const int index);

  void addMethod(const SoType node, const SoActionMethod method);
  void setUp(void);

private:
  class SoActionMethodListP * pimpl;
};

#endif // !COIN_SOACTIONMETHODLIST_H
