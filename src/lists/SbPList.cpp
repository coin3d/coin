/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

/*!
  \class SbPList Inventor/lists/SbPList.h
  \brief The SbPList class is a container class for pointer lists.

  The SbPList class is a wrapper for void * pointer lists.  The main feature
  is that the SbPList object arrays grow dynamically as you append more
  pointers to the list.  The actual growing technique used is to double the
  list size when it becomes too small.

  There are also other array-related convenience methods; e.g. finding
  indices, inserting pointers, removing pointers (and shrink array),
  copying arrays, etc.
*/


#include <Inventor/lists/SbPList.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG
#include <assert.h>

/*!
  \var SbPList::itemBuffer

  FIXME: write doc.
*/

/*!
  \var SbPList::itemBufferSize

  FIXME: write doc.
*/

/*!
  \var SbPList::numItems

  FIXME: write doc.
*/

/*!
  \var SbPList::defaultItem

  FIXME: write doc.
*/

/*!
  This constructor initializes the allocated size for the list to \a initSize.

  It improves performance if you know the approximate total size of the list
  in advance before adding list elements.  A good idea is to use a high
  estimate to avoid
*/

SbPList::SbPList(const int initSize)
{
  assert (initSize >= 0);

  this->itemBufferSize = SbMax(2, initSize);
  this->itemBuffer = new void * [ this->itemBufferSize ];
  this->numItems = 0;
  this->defaultItem = NULL;
}

/*!
  Copy constructor. Creates a copy of the given list.
*/

SbPList::SbPList(const SbPList & list)
{
  this->itemBufferSize = -1;
  this->itemBuffer = NULL;
  this->copy(list);
}

/*!
  The destructor.
*/

SbPList::~SbPList()
{
  delete[] this->itemBuffer;
}

/*!
  Appends the pointer to the end of the list.
*/

void
SbPList::append(void * const item)
{
  if (this->numItems >= this->itemBufferSize)
    this->makeRoom(this->numItems+1);
  this->itemBuffer[this->numItems++] = (void *) item;
}

/*!
  Search for the first occurence of the given pointer.

  \return index of given pointer or -1 if not found.
*/

int
SbPList::find(void * const item) const
{
  for (int i = 0; i < this->numItems; i++)
    if (this->itemBuffer[ i ] == item)
      return i;
  return -1;
}

/*!
  Insert the given pointer before the pointer with the given index.

  Negative indices are clamped to 0.
  If the index exceeds the length of the list, the list is expanded and
  padded with NULL's if necessary. Note that in this case, the pointer
  at index \a addBefore is assumed to be NULL and hence the list will
  end with a NULL pointer.
*/

void
SbPList::insert(void * const item, int insertBefore)
{
#if COIN_DEBUG
  if (insertBefore<0) {
    insertBefore=0;
    SoDebugError::postWarning("SbPList::insert",
                              "Negative index encountered and clamped "
                              "to zero.");
  }
#endif // COIN_DEBUG
  //Pad with NULLs if necessary.
  if (insertBefore > this->numItems) {
    this->makeDefaultRoom(insertBefore+2);
    this->itemBuffer[insertBefore]=item;
    this->numItems = insertBefore+2;
  }
  else {
    if (this->numItems >= this->itemBufferSize)
      this->makeRoom(this->numItems+1);
    for (int i = this->numItems; i > insertBefore; i--)
      this->itemBuffer[ i ] = this->itemBuffer[ i - 1 ];
    this->itemBuffer[ insertBefore ] = (void *) item;
    this->numItems++;
  }
}

/*!
  Remove the pointer with index \a index, and copy the rest of the
  list one index down to fill the "hole":
*/

void
SbPList::remove(int index)
{
#if COIN_DEBUG
  if (index<0 ||index>=this->numItems) {
    SoDebugError::postWarning("SbPList::remove",
                              "index %d is out of bounds (0..%d)",
                              index, SbMax(this->numItems-1, 0));
    return;
  }
#endif // COIN_DEBUG
  this->numItems--;
  for (int i = index; i < this->numItems; i++)
    this->itemBuffer[ i ] = this->itemBuffer[ i + 1 ];
}

/*!
  Return the number of pointers in the list.
*/

//$ EXPORT INLINE
int
SbPList::getLength(void) const
{
  return this->numItems;
}

/*!
  Removes all pointers from (and including) the given index to
  the end of the list.
*/

void
SbPList::truncate(const int idx)
{
#if COIN_DEBUG
  if (idx<0 || (idx && idx>=this->numItems)) {
    SoDebugError::postWarning("SbPList::truncate",
                              "index %d is out of bounds (0..%d)",
                              idx, SbMax(this->numItems-1, 0));
    return;
  }
#endif // COIN_DEBUG

  this->numItems = idx; // do not free memory yet...
}

/*!
  Copy from the given list.
*/

void
SbPList::copy(const SbPList & list)
{
  if (this == &list) return;

  if (this->itemBufferSize != list.numItems) {
    delete [] this->itemBuffer;
    this->itemBufferSize = SbMax(2, list.numItems);
    this->itemBuffer = new void * [ this->itemBufferSize ];
  }

  const int n = list.numItems;
  this->numItems = n;
  for (int i = 0; i < n; i++) this->itemBuffer[i] = list.itemBuffer[i];
  this->defaultItem = list.defaultItem;
}

/*!
  Assign from the given list. Basically the same as SbPList::copy().
*/

SbPList &
SbPList::operator = (const SbPList & list)
{
  copy(list);
  return *this;
}

/*!
  Return the pointer at the given index.
*/

void * &
SbPList::operator [] (int index) const
{
#if COIN_DEBUG
  if (index<0) {
    SoDebugError::postWarning("SbPList::operator[]",
                              "index %d is less than zero", index);
    static void * p = NULL;
    return p;
  }
#endif // COIN_DEBUG
  if (this->numItems<=index) this->makeDefaultRoom(index+1);
  return this->itemBuffer[index];
}

/*!
  Equality operator. Check if the contents of two lists are equal.
*/

SbBool
SbPList::operator == (const SbPList & list) const
{
  if (this == &list) return TRUE;
  if (this->numItems != list.numItems) return FALSE;
  for (int i=0;i<this->numItems;i++)
    if (this->itemBuffer[i]!=list.itemBuffer[i])
      return FALSE;
  return TRUE;
}

/*!
  Inequality operator. Opposite of ==.
*/

SbBool
SbPList::operator != (const SbPList & list) const
{
  return !operator==(list);
}

/*!
  This method returns the element at indaxe \a index.
*/

//$ EXPORT INLINE
void *
SbPList::get(const int index) const
{
  assert(index >= 0 && index < this->numItems); // range [0, N-1]
  return this->itemBuffer[ index ];
}

/*!
  This method sets the pointer at index \a index to \a item.
*/

//$ EXPORT INLINE
void
SbPList::set(const int index, void * const item)
{
  assert(index >= 0 && index < this->numItems); // range[ 0, N-1]
  this->itemBuffer[ index ] = (void *) item;
}

/*!
  This method is to be removed.
*/

//$ EXPORT INLINE
void
SbPList::setDefault(const void * const defaultItem)
{
  this->defaultItem = (void *)defaultItem;
}

/*!
  This method is to be removed.
*/

//$ EXPORT INLINE
const void *
SbPList::getDefault() const
{
  return this->defaultItem;
}

  // FIXME: I don't think this method is a very good idea -- it can
  // (and has) lead to _extremely_ hard to find bugs. egcs v1.0.2 at
  // least managed to read SbPList::append((uint32_t)0) as a call to
  // this method, resulting in a bogus SbPList constructor
  // call. 19990418 mortene.

#if 0 // disabled
/*!
  This method is disabled.
*/

void
SbPList::append(const SbPList & list)
{
  if (this == &list) {
    SbPList copy(list);
    const int items = copy.getLength();
    for (int i = 0; i < items; i++)
      append(copy.itemBuffer[ i ]);
  } else {
    const int items = list.getLength();
    for (int i = 0; i < items; i++)
      append(list.itemBuffer[ i ]);
  }
}
#endif // disabled

/*!
  This method moves the last pointer to index \a index and shortens the list
  length by one.
*/

void
SbPList::removeFast(int index)
{
#if COIN_DEBUG
  if (index<0 ||index>=this->numItems) {
    SoDebugError::postWarning("SbPList::removeFast",
                              "index %d is out of bounds (0..%d)",
                              index, SbMax(this->numItems-1, 0));
    return;
  }
#endif // COIN_DEBUG
  this->itemBuffer[ index ] = this->itemBuffer[--this->numItems];
}

/*!
  This method clears the list.
*/

void
SbPList::clear(const int newSize)
{
  truncate(0);
  int size = SbMax(2, newSize);
  if (this->itemBufferSize != size) {
    delete [] this->itemBuffer;
    this->itemBuffer = new void * [ size ];
    this->itemBufferSize = size;
  }
}

/*!
  This method appends an item to the end of the list.
*/

//$ EXPORT INLINE
void
SbPList::push(void * const item)
{
  append(item);
}

/*!
  This method returns the last item in the list and shortens the list length
  by one.
*/

void *
SbPList::pop()
{
#if COIN_DEBUG
  if (this->numItems<=0) {
    SoDebugError::postWarning("SbPList::pop",
                              "List is empty. Returning default value.");
    return this->defaultItem;
  }
#endif // COIN_DEBUG
  return this->itemBuffer[--this->numItems];
}

/*!
  This operator returns the pointer to the list array.
*/

//$ EXPORT INLINE
SbPList::operator void **()
{
  return this->itemBuffer;
}

/*!
  This operator returns the pointer to the list array.
*/

//$ EXPORT INLINE
SbPList::operator const void **() const
{
  return (const void **) this->itemBuffer;
}

/*!
  This method returns the last element of the list.
*/

//$ EXPORT INLINE
void *
SbPList::getLastElement() const
{
#if COIN_DEBUG
  if (this->numItems<=0) {
    SoDebugError::postWarning("SbPList::getLastElement",
                              "List is empty. Returning default value.");
    return this->defaultItem;
  }
#endif // COIN_DEBUG
  return this->itemBuffer[ this->numItems - 1 ];
}

/*!
  This method sets the last element of the list.
*/

void
SbPList::setLastElement(void * const item)
{
#if COIN_DEBUG
  if (this->numItems<=0) {
    SoDebugError::postWarning("SbPList::getLastElement",
                              "List is empty. Doing nothing.");
    return;
  }
#endif // COIN_DEBUG
  this->itemBuffer[ this->numItems - 1 ] = (void *) item;
}

/*!
  This method reallocates the list to the exact size of the list to save
  memory.  Bear in mind that a lot of unnecessary reallocations might
  fragment the memory, which may lead to poor performance.
*/

void
SbPList::fit()
{
  if (this->numItems < this->itemBufferSize) {
    int size = SbMax(2, this->numItems);
    void * * newItemBuffer = new void * [ size ];
    for (int i = 0; i < this->numItems; i++)
      newItemBuffer[ i ] = this->itemBuffer[ i ];
    delete [] this->itemBuffer;
    this->itemBuffer = newItemBuffer;
    this->itemBufferSize = size;
  }
}

/*!
  This method is for debug purposes.
*/

void
SbPList::print(FILE * fp) const
{
  fprintf(fp, "void * - %p: (%d/%d) - itemSize = %d\n",
           this->itemBuffer, this->numItems, this->itemBufferSize,
           sizeof(void *));
#if COIN_DEBUG
  for (int i=0; i < this->getLength();i++)
    fprintf( fp, "%p ", (*this)[i] );
  fprintf( fp, "\n" );
#endif // COIN_DEBUG
}

//
// private members
//

/*!
  FIXME: write doc.
*/

void
SbPList::makeRoom(int newSize)
{
  assert(this->itemBufferSize > 0);
  while (newSize > this->itemBufferSize) this->itemBufferSize <<= 1;
  void **newItemBuffer = new void *[this->itemBufferSize];
  const int n = this->numItems;
  for (int i = 0; i < n; i++)
    newItemBuffer[i] = this->itemBuffer[i];
  delete [] this->itemBuffer;
  this->itemBuffer = newItemBuffer;
}

/*!
  FIXME: write doc.
*/

void
SbPList::makeDefaultRoom(int newSize) const
{
  //Cast away constness. Not good, but OIV's design stink a bit here.
  SbPList * thisobj=(SbPList *)this;

  thisobj->makeRoom(newSize);
  for (int i=thisobj->numItems;i<newSize;i++)
    thisobj->itemBuffer[i]=thisobj->defaultItem;
  thisobj->numItems=newSize;
}
