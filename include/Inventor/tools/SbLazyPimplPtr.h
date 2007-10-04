#ifndef COIN_SBLAZYPIMPLPTR_H
#define COIN_SBLAZYPIMPLPTR_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2007 by Systems in Motion.  All rights reserved.
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
 *  See http://www.coin3d.org/ for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

// The SbLazyPimplPtr<> class is loosely based on the boost::pimpl_ptr<>-
// proposal of 2005 by Asger Mangaard.
//
// It is reimplemented here as separate classes, not with policy-based
// templates, because we still try to support compilers that don't support
// template templates (e.g. Visual C++ 6).
//
// See also SbPimplPtr<>, our implementation of the immediate-creation
// policy.

#include <Inventor/SbBasic.h>

/* ********************************************************************** */

template <class T>
class COIN_DLL_API SbLazyPimplPtr {
public:
  SbLazyPimplPtr(void);
  SbLazyPimplPtr(T * initial);
  SbLazyPimplPtr(const SbLazyPimplPtr<T> & copy);
  ~SbLazyPimplPtr(void);

  void set(T * value);
  T & get(void) const;

  SbLazyPimplPtr<T> & operator = (const SbLazyPimplPtr<T> & copy);

  SbBool operator == (const SbLazyPimplPtr<T> & rhs) const;
  SbBool operator != (const SbLazyPimplPtr<T> & rhs) const;

  const T * operator -> (void) const;
  T * operator -> (void);

protected:
  T * getNew(void) const;

protected:
  mutable T * ptr;

}; // SbLazyPimplPtr

#ifdef COIN_INTERNAL
// the implementation is in the .hpp class
#include <Inventor/tools/SbLazyPimplPtr.hpp>
#endif // COIN_INTERNAL

/* ********************************************************************** */

#endif // !COIN_SBLAZYPIMPLPTR_H