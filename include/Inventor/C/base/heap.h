#ifndef CC_HEAP_H
#define CC_HEAP_H

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

#include <Inventor/C/basic.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

  typedef int cc_heap_compare_cb(void * closure, void * o1, void * o2);

  typedef struct cc_heap cc_heap;

  cc_heap * cc_heap_construct(unsigned int size, 
                              cc_heap_compare_cb * comparecb,
                              void * compareclosure,
                              SbBool support_remove);
  
  void cc_heap_destruct(cc_heap * h);
  void cc_heap_clear(cc_heap * h);

  void cc_heap_add(cc_heap * h, void * o);
  void * cc_heap_get_top(cc_heap * h);
  void * cc_heap_extract_top(cc_heap * h);
  int cc_heap_remove(cc_heap * h, void * o);
  unsigned int cc_heap_elements(cc_heap * h);
  SbBool cc_heap_empty(cc_heap * h);

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! CC_HEAP_H */
