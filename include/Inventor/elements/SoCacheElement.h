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

#ifndef __SOCACHEELEMENT_H__
#define __SOCACHEELEMENT_H__

#include <Inventor/elements/SoElement.h>

class SoCache;


class SoCacheElement : public SoElement {
  typedef SoElement inherited;

  SO_ELEMENT_HEADER(SoCacheElement);
public:
  static void initClass(void);
protected:
  virtual ~SoCacheElement();

public:
  virtual void init(SoState * state);

  virtual void push(SoState * state);
  virtual void pop(SoState * state,
                   const SoElement * prevTopElement);

  static void set(SoState * const state, SoCache * const cache);
  SoCache * getCache(void) const;
  static SbBool anyOpen(SoState * const state);
  static void invalidate(SoState * const state);
  virtual SbBool matches(const SoElement * element) const;
  virtual SoElement * copyMatchInfo(void) const;
  SoCacheElement * getNextCacheElement(void) const;

  static void addElement(SoState * const state,
                         const SoElement * const element);
  static void addCacheDependency(SoState * const state, SoCache * const cache);
  static SbBool setInvalid(const SbBool newValue);
  static SoCache * getCurrentCache(SoState * const state);

private:
  SoCache * cache;
  static SbBool invalidated;
};

#endif // !__SOCACHEELEMENT_H__
