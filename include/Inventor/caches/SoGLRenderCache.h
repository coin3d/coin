#ifndef COIN_SOGLRENDERCACHE_H
#define COIN_SOGLRENDERCACHE_H

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

#include <Inventor/caches/SoCache.h>
#include <Inventor/elements/SoGLLazyElement.h>

class SoGLDisplayList;
class SoGLRenderCacheP;


class COIN_DLL_API SoGLRenderCache : public SoCache {
  typedef SoCache inherited;

public:
  SoGLRenderCache(SoState * state);
  virtual ~SoGLRenderCache();

  void open(SoState * state);
  void close(void);
  void call(SoState * state);

  int getCacheContext(void) const;

  virtual SbBool isValid(const SoState * state) const;
  virtual void addNestedCache(SoGLDisplayList * child);

  SoGLLazyElement::GLState * getPreLazyState(void);
  SoGLLazyElement::GLState * getPostLazyState(void);

protected:
  virtual void destroy(SoState *state);

private:
  SoGLRenderCacheP * pimpl;
};

#endif // !COIN_SOGLRENDERCACHE
