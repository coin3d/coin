/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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
  \class SoPathList Inventor/lists/SoPathList.h
  \brief The SoPathList class is a container class for arrays of pointers to
  SoPath objects.

  FIXME: write doc.
*/

#include <Inventor/lists/SoPathList.h>
#include <Inventor/SoPath.h>
#include <assert.h>

/*!
  A constructor (default).
*/

SoPathList::SoPathList(void)
  : SoBaseList()
{
}

/*!
  A constructor.
*/

SoPathList::SoPathList(const int size)
  : SoBaseList(size)
{
}

/*!
  A constructor (copy).
*/

SoPathList::SoPathList(const SoPathList & l)
  : SoBaseList(l)
{
}

/*!
  The destructor.
*/

SoPathList::~SoPathList(void)
{
}

/*!
  FIXME: write doc.
*/

//
// inline only for this class
//
inline SoPath *
SoPathList::get(const int index) const
{
  return (SoPath *)SoBaseList::get(index);
}

/*!
  FIXME: write doc.
*/

inline void 
SoPathList::set(const int index, SoPath * const path)
{
  SoBaseList::set(index, path);
}

/*!
  FIXME: write doc.
*/

//$ EXPORT INLINE
void 
SoPathList::append(SoPath * const path)
{
  SoBaseList::append((SoBase *)path);
}

/*!
  FIXME: write doc.
*/

//$ EXPORT INLINE
SoPath *
SoPathList::operator [](const int i) const
{
  return (SoPath *)SoBaseList::operator[](i);
}

/*!
  FIXME: write doc.
*/

SoPathList &
SoPathList::operator =(const SoPathList & l)
{
  SoBaseList::copy(l);
  return *this;
}

/*!
  FIXME: write doc.
*/

int 
SoPathList::findPath(const SoPath & path) const
{
  int i, n = this->getLength();
  for (i = 0; i < n; i++) {
    if (*(this->get(i)) == path) return i;
  }
  return -1;

}

static int 
compare_paths(SoPath *p0, SoPath *p1)
{  
  int diff = (char*)p0->getHead() - (char*)p1->getHead();
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
  FIXME: write doc.
*/

void 
SoPathList::sort()
{
  int i, j, distance, n = this->getLength();
  SoPath *tmp;
  SoPath **array = (SoPath**) this->itemBuffer;
  
  // shell sort algorithm (O(nlog(n))
  for (distance = 1; distance <= n/9; distance = 3*distance + 1);
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
}

/*!
  List should be sorted first. Removes identical paths and
  paths that go through the tail of another path.
*/
void 
SoPathList::uniquify()
{
  int i, n = this->getLength();

  // remove identical paths
  for (i = 0; i < n-1; i++) {
    int j = i+1;
    while (compare_paths(this->get(i), this->get(j)) == 0 && j < n) {
      this->remove(j); 
      n--;
    }
  }
  
  // remove paths that go through the tail of another path
  for (i = 0; i < n; i++) {
    // got to store path to make sure the path itself is not checked
    SoPath *path = this->get(i);
    SoNode *tail = path->getTail();
    int j = 0;
    while (j < n) {
      SoPath *testpath = this->get(j);
      if (path != testpath && testpath->findNode(tail) >= 0) {
        this->remove(j);
        n--;
      }
      else j++;
    }
  }
}
