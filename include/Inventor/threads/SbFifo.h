#ifndef COIN_SBFIFO_H
#define COIN_SBFIFO_H

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

#include <Inventor/C/threads/fifo.h>

class SbFifo {
public:
  SbFifo(void) { this->fifo = cc_fifo_new(); }
  ~SbFifo(void) { cc_fifo_delete(this->fifo); }

  void assign(void * ptr, uint32_t type)
    { cc_fifo_assign(this->fifo, ptr, type); }
  void retrieve(void *& ptr, uint32_t &type)
    { cc_fifo_retrieve(this->fifo, &ptr, &type); }
  SbBool tryRetrieve(void *& ptr, uint32_t & type)
    { return cc_fifo_try_retrieve(this->fifo, &ptr, &type); }

  unsigned int size(void) const { return cc_fifo_size(this->fifo); }

  void lock(void) const { cc_fifo_lock(this->fifo); }
  void unlock(void) const { cc_fifo_unlock(this->fifo); }

  // lock/unlock only needed around the following operations:
  SbBool peek(void *& item, uint32_t & type) const
    { return cc_fifo_peek(this->fifo, &item, &type); }
  SbBool contains(void * item) const
    { return cc_fifo_contains(this->fifo, item); }
  SbBool reclaim(void * item)
    { return cc_fifo_reclaim(this->fifo, item); }

private:
  cc_fifo * fifo;
};

#endif // !COIN_SBFIFO_H
