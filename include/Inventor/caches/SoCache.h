#ifndef COIN_SOCACHE_H
#define COIN_SOCACHE_H

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
