/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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

#ifndef COIN_SOGLRENDERCACHE_H
#define COIN_SOGLRENDERCACHE_H

#include <Inventor/caches/SoCache.h>
#include <Inventor/lists/SbList.h>

class SoGLDisplayList;

class SoGLRenderCache : public SoCache {
  typedef SoCache inherited;
public:

  SoGLRenderCache(SoState * state);
  ~SoGLRenderCache();

  void open(SoState * state);
  void close(void);
  void call(SoState * state);

  int getCacheContext(void) const;

  virtual SbBool isValid(const SoState * state) const;
  virtual void addNestedCache(SoGLDisplayList * child);

protected:
  virtual void	destroy(SoState *state);

private:
  SoGLDisplayList * displaylist;
  SoState * openstate;
  SbList <SoGLDisplayList*> nestedcachelist;
};

#endif // COIN_SOGLRENDERCACHE
