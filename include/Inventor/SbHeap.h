/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
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

#ifndef COIN_SBHEAP_H
#define COIN_SBHEAP_H

#include <Inventor/SbBasic.h>
#include <Inventor/lists/SbList.h>
#include <stddef.h>

typedef struct
{
  float (*eval_func)(void*);
  int (*get_index_func)(void*);
  void (*set_index_func)(void*, int);
} SbHeapFuncs;

class COIN_DLL_API SbHeap {
public:
  SbHeap(const SbHeapFuncs &SbHeapFuncs,
         const int initsize = 1024);
  ~SbHeap();

  void emptyHeap(void);
  int size(void) const;
  int add(void *obj);
  void remove(const int pos);
  void remove(void *obj);
  void *extractMin();
  void *getMin();
  void *operator[](const int idx);

  void newWeight(void *obj, int hpos = -1);
  SbBool buildHeap(SbBool (*progresscb)(float percentage, void *data) = NULL,
                   void *data = NULL);
  SbBool traverseHeap(SbBool (*func)(void *, void *), void *userdata) const;

private:
  SbHeapFuncs funcs;
  SbList <void*> heap;

  int heapInsert(void *obj);
  void *heapExtractMin(void);
  void heapReserve(const int newsize);
  void heapify(const int idx);
};

#endif // COIN_SBHEAP_H
