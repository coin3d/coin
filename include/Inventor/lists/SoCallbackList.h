#ifndef COIN_LISTS_SOCALLBACKLIST_H
#define COIN_LISTS_SOCALLBACKLIST_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

// NB: make sure the ifdef-test above wrapping this includefile is
// _not_ checking on and setting the same define-string as the other
// SoCallbackList.h file in misc/.

#include <Inventor/lists/SbPList.h>

typedef void SoCallbackListCB(void * userdata, void * callbackdata);

class COIN_DLL_API SoCallbackList {
public:
  SoCallbackList(void);
  ~SoCallbackList();

  void addCallback(SoCallbackListCB * f, void * userData = NULL);
  void removeCallback(SoCallbackListCB * f, void * userdata = NULL);

  void clearCallbacks(void);
  int getNumCallbacks(void) const;

  void invokeCallbacks(void * callbackdata);

private:
  SbPList funclist;
  SbPList datalist;
};

#endif // !COIN_LISTS_SOCALLBACKLIST_H
