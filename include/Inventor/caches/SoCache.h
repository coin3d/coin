/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

#ifndef __SOCACHE_H__
#define __SOCACHE_H__

class SoState;
class SoElement;

#include <Inventor/SbBasic.h>
#include <Inventor/lists/SbPList.h>

class SoCache {
public:
  SoCache(SoState * const state);
  
  void ref();
  void unref(SoState *state = NULL);

  void addElement(const SoElement * const elem);
  
  virtual void addCacheDependency(const SoState *state,
				  SoCache *cache);
  virtual SbBool isValid(const SoState *state) const;
  const SoElement *getInvalidElement(const SoState * const state) const;
  void invalidate();

protected:
  virtual void destroy(SoState *state);
  virtual ~SoCache();

private:
  SbPList elements;
  unsigned char *elementFlags;
  int refCount;
  SbBool invalidated;
  int stateDepth;
};

#endif // !__SOCACHE_H__
