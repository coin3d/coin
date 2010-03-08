#ifndef COIN_SBLAZYPIMPLPTR_HPP
#define COIN_SBLAZYPIMPLPTR_HPP

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

#ifndef COIN_SBLAZYPIMPLPTR_H
#error do not include Inventor/tools/SbLazyPimplPtr.hpp directly, use Inventor/tools/SbLazyPimplPtr.h
#endif // !COIN_SBLAZYPIMPLPTR_H

/* ********************************************************************** */

template <typename T>
SbLazyPimplPtr<T>::SbLazyPimplPtr(void)
: ptr(NULL)
{
}

template <typename T>
SbLazyPimplPtr<T>::SbLazyPimplPtr(T * initial)
{
  this->ptr = initial;
}

template <typename T>
SbLazyPimplPtr<T>::SbLazyPimplPtr(const SbLazyPimplPtr<T> & copy)
{
  *this = copy;
}

template <typename T>
SbLazyPimplPtr<T>::~SbLazyPimplPtr(void)
{
  this->set(NULL);
}

template <typename T>
void
SbLazyPimplPtr<T>::set(T * value)
{
  if (this->ptr) {
    delete this->ptr;
  }
  this->ptr = value;
}

template <typename T>
T &
SbLazyPimplPtr<T>::get(void) const
{
  if (this->ptr == NULL) {
    this->ptr = this->getNew();
  }
  return *(this->ptr);
}

template <typename T>
T *
SbLazyPimplPtr<T>::getNew(void) const
{
  return new T;
}

template <typename T>
SbLazyPimplPtr<T> &
SbLazyPimplPtr<T>::operator = (const SbLazyPimplPtr<T> & copy)
{
  this->get() = copy.get();
  return *this;
}

template <typename T>
SbBool
SbLazyPimplPtr<T>::operator == (const SbLazyPimplPtr<T> & rhs) const
{
  return this->get() == rhs.get();
}

template <typename T>
SbBool
SbLazyPimplPtr<T>::operator != (const SbLazyPimplPtr<T> & rhs) const
{
  return !(*this == rhs);
}

template <typename T>
const T *
SbLazyPimplPtr<T>::operator -> (void) const
{
  return &(this->get());
}

template <typename T>
T *
SbLazyPimplPtr<T>::operator -> (void)
{
  return &(this->get());
}

/* ********************************************************************** */

#endif // !COIN_SBPIMPLPTR_HPP
