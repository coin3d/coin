#ifndef CC_FIFOP_H
#define CC_FIFOP_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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

#ifndef COIN_INTERNAL
#error You have tried to use one of the private Coin header files
#endif /* !COIN_INTERNAL */

#include <Inventor/C/threads/common.h>
#include <Inventor/C/threads/mutexp.h>
#include <Inventor/C/threads/condvarp.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ********************************************************************** */

typedef struct cc_fifo_item cc_fifo_item;

struct cc_fifo_item {
  cc_fifo_item * next;
  void *         item;
  uint32_t       type;
};

struct cc_fifo {
  cc_mutex       access;

  cc_fifo_item * head;
  cc_fifo_item * tail;
  cc_fifo_item * free;

  unsigned int   elements;

  cc_mutex       sleepmutex; /* can access be used instead? */
  cc_condvar     sleep;
};

/* ********************************************************************** */

void cc_fifo_struct_init(cc_fifo * fifo);
void cc_fifo_struct_clean(cc_fifo * fifo);

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! CC_FIFOP_H */
