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
  \class SoPathList SoPathList.h Inventor/lists/SoPathList.h
  \brief The SoPathList class is a container for pointers to SoPath objects.
  \ingroup general

  As this class inherits SoBaseList, referencing and dereferencing
  will default be done on the objects at append(), remove(), insert()
  etc.
*/

#include <Inventor/lists/SoPathList.h>
#include <Inventor/SoPath.h>
#include <assert.h>


/*!
  Default constructor.
*/
SoPathList::SoPathList(void)
  : SoBaseList()
{
}

/*!
  Constructor with a hint about the number of elements the list will
  hold.

  \sa SoBaseList::SoBaseList(const int)
*/
SoPathList::SoPathList(const int size)
  : SoBaseList(size)
{
}

/*!
  Copy constructor.

  \sa SoBaseList::SoBaseList(const SoBaseList &)
*/
SoPathList::SoPathList(const SoPathList & pl)
  : SoBaseList(pl)
{
}

/*!
  Destructor.

  \sa SoBaseList::~SoBaseList()
*/
SoPathList::~SoPathList()
{
}

/*!
  Append \a ptr to the list.

  \sa SoBaseList::append()
*/
void
SoPathList::append(SoPath * const path)
{
  SoBaseList::append((SoBase *)path);
}

/*!
  Return node pointer at index \a i.

  \sa SoBaseList::operator[]()
*/
SoPath *
SoPathList::operator[](const int i) const
{
  return (SoPath *)SoBaseList::operator[](i);
}

/*!
  Copy contents of list \a pl to this list. 

  \sa SoBaseList::operator=()
*/
SoPathList &
SoPathList::operator=(const SoPathList & pl)
{
  SoBaseList::copy(pl);
  return *this;
}

/*!
  Find and return index of first item equal to \a path.
*/
int
SoPathList::findPath(const SoPath & path) const
{
  int i, n = this->getLength();
  for (i = 0; i < n; i++) if (*(*this)[i] == path) return i;
  return -1;

}

// Return a negative number if the path pointed to by v0 is considered
// "less than" v1, a positive number if v0 is considered "larger than"
// v1 and zero if they are equal.
static int
compare_paths(const void * v0, const void * v1)
{
  SoPath * p0 = (SoPath *)v0;
  SoPath * p1 = (SoPath *)v1;

  int diff = (char *)p0->getHead() - (char *)p1->getHead();
  if (diff != 0) return diff;

  int n = SbMin(p0->getLength(), p1->getLength());
  int i;
  for (i = 1; i < n; i++) {
    diff = p0->getIndex(i) - p1->getIndex(i);
    if (diff != 0) return diff;
  }
  // shortest path first
  return p0->getLength() - p1->getLength();
}

/*!
  Sort paths in list according to how early they are run into when
  traversing the scene graph.
*/
void
SoPathList::sort(void)
{
#if 0 // OBSOLETED: why not use qsort() from the C library? 20000228 mortene.
  int i, j, distance, n = this->getLength();
  SoPath * tmp;
  void ** ptr = (void **)(*this);
  SoPath ** array = (SoPath **)ptr;

  // shell sort algorithm (O(nlog(n))
  for (distance = 1; distance <= n/9; distance = 3 * distance + 1);
  for (; distance > 0; distance /= 3) {
    for (i = distance; i < n; i++) {
      tmp = array[i];
      j = i;
      while (j >= distance && compare_paths(array[j-distance], tmp) > 0) {
        array[j] = array[j-distance];
        j -= distance;
      }
      array[j] = tmp;
    }
  }
#else
  qsort((void **)this->getArrayPtr(), this->getLength(), sizeof(void *), compare_paths);
#endif
}

/*!
  Removes identical paths and paths that go through the tail of
  another path.

  Note that this method assumes the list to be in a sorted order.

  \sa sort()
*/
void
SoPathList::uniquify(void)
{
  int i, n = this->getLength();

  // remove identical paths
  for (i = 0; i < n-1; i++) {
    int j = i+1;
    while (compare_paths((*this)[i], (*this)[j]) == 0 && j < n) {
      this->remove(j);
      n--;
    }
  }

  // remove paths that go through the tail of another path
  for (i = 0; i < n; i++) {
    // got to store path to make sure the path itself is not checked
    SoPath * path = (*this)[i];
    SoNode * tail = path->getTail();
    int j = 0;
    while (j < n) {
      SoPath * testpath = (*this)[j];
      if (path != testpath && testpath->findNode(tail) >= 0) {
        this->remove(j);
        n--;
      }
      else j++;
    }
  }
}
