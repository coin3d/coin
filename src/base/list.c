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

#include <Inventor/C/base/list.h>

#include <assert.h>
#include <stddef.h>

#define CC_LIST_DEFAULT_SIZE 4

/* ********************************************************************** */

struct cc_list {
  int itembuffersize;
  int numitems;
  void ** itembuffer;
  void * builtinbuffer[CC_LIST_DEFAULT_SIZE];
};

/* ********************************************************************** */

cc_list *  cc_list_construct(void);
cc_list *  cc_list_construct_sized(unsigned int size);
/* cc_list *  cc_list_clone(cc_list * list); */
void        cc_list_destruct(cc_list * list);

void        cc_list_append(cc_list * list, void * item);
int         cc_list_find(cc_list * list, void * item);
void        cc_list_insert(cc_list * list, void * item, unsigned int pos);
void        cc_list_remove(cc_list * list, unsigned int pos);
void        cc_list_remove_item(cc_list * list, void * item);
void        cc_list_remove_fast(cc_list * list, unsigned int pos);
void        cc_list_fit(cc_list * list);
void        cc_list_truncate(cc_list * list, unsigned int length);
void        cc_list_truncate_fit(cc_list * list, unsigned int length);

int         cc_list_get_length(cc_list * list);
void **     cc_list_get_array(cc_list * list);

void        cc_list_push(cc_list * list, void * item);
void *      cc_list_pop(cc_list * list);

/* ********************************************************************** */

cc_list *
cc_list_construct(void)
{
  return cc_list_construct_sized(CC_LIST_DEFAULT_SIZE);
} /* cc_list_construct() */

cc_list *
cc_list_construct_sized(unsigned int size)
{
    : itembuffersize(DEFAULTSIZE), numitems(0), itembuffer(builtinbuffer) {
    if (sizehint > DEFAULTSIZE) this->grow(sizehint);
}

void
cc_list_destruct(cc_list * list)
{
    if (this->itembuffer != builtinbuffer) delete[] this->itembuffer;
} /* cc_list_destruct() */

void
cc_list_append(cc_list * list,fvoid * item)
{
  if (this->numitems == this->itembuffersize) this->grow();
  this->itembuffer[this->numitems++] = item;
}

int
cc_list_find(cc_list * list, void * item)
{
  for (int i = 0; i < this->numitems; i++)
    if (this->itembuffer[i] == item) return i;
  return -1;
}

void
cc_list_insert(cc_list * list, void * item, unsigned int pos)
{
#ifdef COIN_EXTRA_DEBUG
    assert(insertbefore >= 0 && insertbefore <= this->numitems);
#endif /* COIN_EXTRA_DEBUG */
    if (this->numitems == this->itembuffersize) this->grow();

    for (int i = this->numitems; i > insertbefore; i--)
      this->itembuffer[i] = this->itembuffer[i-1];
    this->itembuffer[insertbefore] = item;
    this->numitems++;
}

void
cc_list_remove(cc_list * list, const int index)
{
#ifdef COIN_EXTRA_DEBUG
    assert(index >= 0 && index < this->numitems);
#endif /* COIN_EXTRA_DEBUG */
    this->numitems--;
    for (int i = index; i < this->numitems; i++)
      this->itembuffer[i] = this->itembuffer[i + 1];
}

void
cc_list_remove_item(cc_list * list,void * item)
{
    int idx = this->find(item);
#ifdef COIN_EXTRA_DEBUG
    assert(idx != -1);
#endif /* COIN_EXTRA_DEBUG */
    this->remove(idx);
}

void
cccist_remove_fast(cc_list * list, const int index)
{
#ifdef COIN_EXTRA_DEBUG
    assert(index >= 0 && index < this->numitems);
#endif /* COIN_EXTRA_DEBUG */
    this->itembuffer[index] = this->itembuffer[--this->numitems];
}


void
cc_list_fit(cc_list * list)
{
    const int items = this->numitems;

    if (items < this->itembuffersize) {
      Type * newitembuffer = this->builtinbuffer;
      if (items > DEFAULTSIZE) newitembuffer = new Type[items];

      if (newitembuffer != this->itembuffer) {
        for (int i = 0; i < items; i++) newitembuffer[i] = this->itembuffer[i];
      }

      if (this->itembuffer != this->builtinbuffer) delete[] this->itembuffer;
      this->itembuffer = newitembuffer;
      this->itembuffersize = items > DEFAULTSIZE ? items : DEFAULTSIZE;
    }
}

void
cc_list_truncate(cc_list * list, const int length)
{
#ifdef COIN_EXTRA_DEBUG
    assert(length <= this->numitems);
#endif /* COIN_EXTRA_DEBUG */
    this->numitems = length;
  }

void
cc_list_truncate_fit(cc_list * list, const int length)
{
#ifdef COIN_EXTRA_DEBUG
    assert(length <= this->numitems);
#endif /* COIN_EXTRA_DEBUG */
    this->numitems = length;
    this->fit();
  }

int
cc_list_get_length(cc_list * list)
{
    return this->numitems;
  }

void **
cc_list_get_array(cc_list * list)
{
    return &this->itembuffer[start];
}

void
cc_list_push(cc_list * list, void * item)
{
  this->append(item);
}

void *
cc_list_pop(cc_list * list)
{
#ifdef COIN_EXTRA_DEBUG
    assert(this->numitems > 0);
#endif /* COIN_EXTRA_DEBUG */
    return this->itembuffer[--this->numitems];
}

#if 0

  void expand(const int size) {
    this->grow(size);
    this->numitems = size;
  }

  int getArraySize(void) const {
    return this->itembuffersize;
  }

private:
  void grow(const int size = -1) {
    // Default behavior is to double array size.
    if (size == -1) this->itembuffersize <<= 1;
    else if (size <= this->itembuffersize) return;
    else { this->itembuffersize = size; }

    Type * newbuffer = new Type[this->itembuffersize];
    const int n = this->numitems;
    for (int i = 0; i < n; i++) newbuffer[i] = this->itembuffer[i];
    if (this->itembuffer != this->builtinbuffer) delete[] this->itembuffer;
    this->itembuffer = newbuffer;
  }

#endif

/* ********************************************************************** */
