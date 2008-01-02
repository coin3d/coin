#ifndef COIN_SBPIMPLPTR_HPP
#define COIN_SBPIMPLPTR_HPP

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2008 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#ifndef COIN_SBPIMPLPTR_H
#error do not include Inventor/tools/SbPimplPtr.hpp directly, use Inventor/tools/SbPimplPtr.h
#endif // !COIN_SBPIMPLPTR_H

/* ********************************************************************** */

template <typename T>
SbPimplPtr<T>::SbPimplPtr(void)
: ptr(NULL)
{
  this->set(this->getNew());
}

template <typename T>
SbPimplPtr<T>::SbPimplPtr(T * initial)
{
  this->ptr = initial;
}

template <typename T>
SbPimplPtr<T>::SbPimplPtr(const SbPimplPtr<T> & copy)
{
  *this = copy;
}

template <typename T>
SbPimplPtr<T>::~SbPimplPtr(void)
{
  this->set(NULL);
}

template <typename T>
void
SbPimplPtr<T>::set(T * value)
{
  if (this->ptr) {
    delete this->ptr;
  }
  this->ptr = value;
}

template <typename T>
T &
SbPimplPtr<T>::get(void) const
{
  return *(this->ptr);
}

template <typename T>
T *
SbPimplPtr<T>::getNew(void) const
{
  return new T;
}

template <typename T>
SbPimplPtr<T> &
SbPimplPtr<T>::operator = (const SbPimplPtr<T> & copy)
{
  this->get() = copy.get();
  return *this;
}

template <typename T>
SbBool
SbPimplPtr<T>::operator == (const SbPimplPtr<T> & rhs) const
{
  return this->get() == rhs.get();
}

template <typename T>
SbBool
SbPimplPtr<T>::operator != (const SbPimplPtr<T> & rhs) const
{
  return !(*this == rhs);
}

template <typename T>
const T *
SbPimplPtr<T>::operator -> (void) const
{
  return &(this->get());
}

template <typename T>
T *
SbPimplPtr<T>::operator -> (void)
{
  return &(this->get());
}

/* ********************************************************************** */

#endif // !COIN_SBPIMPLPTR_HPP
