/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
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
#include <Inventor/SoFullPath.h>
#include <../tidbits.h> // coin_atexit()
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
//
// "extern C" wrapper is needed with the OSF1/cxx compiler (probably a
// bug in the compiler, but it doesn't seem to hurt to do this
// anyway).
extern "C" {
static int
compare_paths(const void * v0, const void * v1)
{
  SoPath * p0 = *((SoPath**)v0);
  SoPath * p1 = *((SoPath**)v1);

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
}

/*!
  Sort paths in list according to how early they are run into when
  traversing the scene graph.
*/
void
SoPathList::sort(void)
{
  qsort((void **)this->getArrayPtr(), this->getLength(), sizeof(void *), compare_paths);
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
  SoFullPath ** array = (SoFullPath**) this->getArrayPtr();
  
  for (int i = this->getLength()-2; i >= 0; i--) {
    SoFullPath * p = array[i];
    // if fork is at tail of first path, remove next path
    if (p->findFork(array[i+1]) == p->getLength()-1) {
      this->remove(i + 1);
      // get array pointer again even though it shouldn't change, but
      // it might do in the future so...
      array = (SoFullPath**) this->getArrayPtr();
    }
  }
}
