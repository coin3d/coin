#ifndef COIN_SBFIFO_H
#define COIN_SBFIFO_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.  See the file LICENSE.GPL
 *  at the root directory of this source distribution for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  www.sim.no, support@sim.no, Voice: +47 22114160, Fax: +47 22207097
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
