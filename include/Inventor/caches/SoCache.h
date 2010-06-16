#ifndef COIN_SOCACHE_H
#define COIN_SOCACHE_H

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

class SoState;
class SoElement;
class SoCacheP;

#include <Inventor/SbBasic.h>
#include <stddef.h> // for NULL

class COIN_DLL_API SoCache {
public:
  SoCache(SoState * const state);

  void ref(void);
  void unref(SoState * state = NULL);

  void addElement(const SoElement * const elem);

  virtual void addCacheDependency(const SoState * state,
                                  SoCache * cache);
  virtual SbBool isValid(const SoState * state) const;
  const SoElement * getInvalidElement(const SoState * const state) const;
  void invalidate(void);

protected:
  virtual void destroy(SoState * state);
  virtual ~SoCache();

private:
  SoCacheP * pimpl;
};

#endif // !COIN_SOCACHE_H
