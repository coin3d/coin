/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOSTATE_H
#define COIN_SOSTATE_H

#include <Inventor/SbBasic.h>
#include <Inventor/lists/SbList.h>
#include <stdio.h>

class SoAction;
class SoTypeList;
class SoElement;


class COIN_DLL_API SoState {
public:
  SoState(SoAction * const action,
          const SoTypeList & enabledElements);
  ~SoState(void);

  SoAction * getAction(void) const;

  SoElement * getElement(const int stackIndex);
  const SoElement * getConstElement(const int stackIndex) const;

  void push(void);
  void pop(void);

  void print(FILE * const file = stdout) const;

  SbBool isElementEnabled(const int stackIndex) const;

  int getDepth() const;

  void setCacheOpen(const SbBool flag);
  SbBool isCacheOpen(void) const;

  void lazyEvaluate(void) const;

  SoElement * getElementNoPush(const int stackIndex) const;

private:
  SoAction * const action;
  SoElement ** stack;
  SoElement ** initial;
  int numStacks;
  int depth;
  SbBool cacheopen;
  class sostate_pushstore * pushstore;
  SbList <int> lazylist;
};

// inlined methods

inline const SoElement *
SoState::getConstElement(const int stackIndex) const
{
  return stack[stackIndex];
}

#endif // !COIN_SOSTATE_H
