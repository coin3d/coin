#ifndef COIN_SOSTATE_H
#define COIN_SOSTATE_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/SbBasic.h>
#include <stdio.h>
#include <assert.h>

class SoAction;
class SoTypeList;
class SoElement;

class COIN_DLL_API SoState {
public:
  SoState(SoAction * action,
          const SoTypeList & enabledelements);
  ~SoState(void);

  SoAction * getAction(void) const;

  SoElement * getElement(const int stackindex);
  const SoElement * getConstElement(const int stackindex) const;

  void push(void);
  void pop(void);

  void print(FILE * const file = stdout) const;

  SbBool isElementEnabled(const int stackindex) const;

  int getDepth(void) const;

  void setCacheOpen(const SbBool flag);
  SbBool isCacheOpen(void) const;

  SoElement * getElementNoPush(const int stackindex) const;

private:
  SoElement ** stack;
  int numstacks;
  SbBool cacheopen;
  class SoStateP * pimpl;  
};

// these methods are used very often, and is therefore inlined

inline SbBool
SoState::isElementEnabled(const int stackindex) const
{
  return (stackindex < this->numstacks) && (this->stack[stackindex] != NULL);
}

inline const SoElement *
SoState::getConstElement(const int stackindex) const
{
  assert(this->isElementEnabled(stackindex));
  return this->stack[stackindex];
}

inline SbBool 
SoState::isCacheOpen(void) const
{
  return this->cacheopen;
}

inline SoElement *
SoState::getElementNoPush(const int stackindex) const
{
  assert(this->isElementEnabled(stackindex));
  return this->stack[stackindex];
}


#endif // !COIN_SOSTATE_H
