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

#ifndef COIN_SOSTATE_H
#define COIN_SOSTATE_H

#include <Inventor/SbBasic.h>
#include <stdio.h>

class SoAction;
class SoTypeList;
class SoElement;


class SoState {
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

  SoElement * getElementNoPush(const int stackIndex) const;

private:
  SoAction * const action;
  SoElement ** stack;
  SoElement ** initial;
  int numStacks;
  int depth;
};

#endif // !COIN_SOSTATE_H
