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
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOACTIONMETHODLIST_H
#define COIN_SOACTIONMETHODLIST_H

#include <Inventor/lists/SbPList.h>
#include <Inventor/SoType.h>

class SoAction;
class SoNode;

typedef void (* SoActionMethod)(SoAction *, SoNode *);

class COIN_DLL_EXPORT SoActionMethodList : public SbPList {
  typedef SbPList inherited;

public:
  SoActionMethodList(SoActionMethodList * const parentlist);

  SoActionMethod & operator[](const int index);

  void addMethod(const SoType node, const SoActionMethod method);
  void setUp(void);

private:
  SoActionMethodList * parent;
  int setupNumTypes;
};

#endif // !COIN_SOACTIONMETHODLIST_H
