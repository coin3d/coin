/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

// FIXME: this implements a C ADT for a dynamic array of void
// pointers. It's basically just a quick hack of wrapping the C ADT
// around our SbPList C++ class. We should really correct this to be
// the other way around: we should make a generic C ADT for a dynamic
// array, which is then wrapped by SbList.
//
// Don't make this class public until the C rewrite happens, and the
// API has been audited. This ADT must have been reimplemented a
// zillion times around in various projects, so check out how our API
// design fares versus others.
//
// 20030604 mortene.

#include <Inventor/C/base/dynarray.h>
#include <Inventor/lists/SbPList.h>

struct cc_dynarray {
  SbPList plist;
};

cc_dynarray *
cc_dynarray_new(void)
{
  return new struct cc_dynarray;
}

cc_dynarray *
cc_dynarray_duplicate(const cc_dynarray * src)
{
  cc_dynarray * p = cc_dynarray_new();
  p->plist = src->plist;
  return p;
}

void
cc_dynarray_destruct(cc_dynarray * arr)
{
  delete arr;
}

void
cc_dynarray_fit(cc_dynarray * arr)
{
  arr->plist.fit();
}

void
cc_dynarray_append(cc_dynarray * arr, void * item)
{
  arr->plist.append(item);
}

int
cc_dynarray_find(const cc_dynarray * arr, void * item)
{
  return arr->plist.find(item);
}

void
cc_dynarray_insert(cc_dynarray * arr, void * item, int idx)
{
  arr->plist.insert(item, idx);
}

void
cc_dynarray_remove(cc_dynarray * arr, void * item)
{
  arr->plist.removeItem(item);
}

void
cc_dynarray_remove_idx(cc_dynarray * arr, int idx)
{
  arr->plist.remove(idx);
}

void
cc_dynarray_removefast(cc_dynarray * arr, int idx)
{
  arr->plist.removeFast(idx);
}

unsigned int
cc_dynarray_length(const cc_dynarray * arr)
{
  return arr->plist.getLength();
}

void
cc_dynarray_truncate(cc_dynarray * arr, unsigned int len)
{
  arr->plist.truncate(len);
}

void **
cc_dynarray_get_arrayptr(const cc_dynarray * arr)
{
  return arr->plist.getArrayPtr();
}

void *
cc_dynarray_get(const cc_dynarray * arr, unsigned int idx)
{
  return arr->plist[idx];
}

SbBool
cc_dynarray_eq(const cc_dynarray * arr1, const cc_dynarray * arr2)
{
  return (arr1->plist == arr2->plist);
}

void
cc_dynarray_set(cc_dynarray * arr, unsigned int idx, void * item)
{
  return arr->plist.set(idx, item);
}
