/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include "misc/SoCompactPathList.h"

#include <assert.h>

#include <Inventor/SoFullPath.h>
#include <Inventor/lists/SoPathList.h>
#include <Inventor/errors/SoDebugError.h>

/*!
  \class SoCompactPathList Inventor/misc/SoCompactPathList.h
  \brief The SoCompactPathList class is used to optimize SoPathList traversal.

  This class creates a lookup table which is used while doing SoPathList traversal.
  For each node in the paths, it stores the following information:

  1. the number of distinct children at that level
  2. a child index to each distinct child to be used for IN_PATH travesal
  3. an index to each child's lookup table position

  1. and 2. are used for optimized IN_PATH action traversal. 3. is
  used internally to traverse this lookup table.

*/

#define FULL_PATH(list, i) ((SoFullPath *) list[i])

/*!
  Constructor.
*/
SoCompactPathList::SoCompactPathList(const SoPathList & list)
  : stack(256)
{
  assert(list.getLength());
  SoNode * head = FULL_PATH(list, 0)->getHead();
  int numnodes = 0;

  for (int i = 0; i < list.getLength(); i++) {
    assert(FULL_PATH(list, i)->getHead() == head);
    numnodes += FULL_PATH(list, i)->getLength() - 1;
  }
  // 3 entries for each node + one extra for the root. This is a
  // worst-case size, but memory usage isn't an issue for this class
  // (memory usage will be small compared to memory used by the paths)
  this->lookupsize = 3 * numnodes + 1;

  this->lookuptable = new int[this->lookupsize];
  this->createLookupTable(0, 1, list, 0, list.getLength());
  this->reset();
}

/*!
  Destructor.
*/
SoCompactPathList::~SoCompactPathList()
{
  delete[] this->lookuptable;
}

/*!
  Reset path list.
*/
void
SoCompactPathList::reset()
{
  this->stack.truncate(0);
  this->lookupidx = 0;
}

/*!
  Returns the children to be used for IN_PATH traversal.
*/
void
SoCompactPathList::getChildren(int & numindices, const int *& indices)
{
  assert(this->lookupidx >= 0);
  numindices = this->getNumIndices();
  indices = this->lookuptable + this->getStartIndex();
}

/*!
  Push \a childindex to the current path.
 */
SbBool
SoCompactPathList::push(int childindex)
{
  assert(this->lookupidx >= 0); // this function should be used only for IN_PATH traversal

  // push current node to be able to restore it in pop()
  this->stack.push(this->lookupidx);

  int i;
  const int n = this->getNumIndices();
  const int idx = this->getStartIndex();

  // check if childindex is IN_PATH or not
  for (i = 0; i < n; i++) {
    if (this->lookuptable[idx + i] == childindex) break;
  }

  if (i < n) { // IN_PATH
    // get ready to traverse the child
    this->lookupidx = this->getChildIndex(i);
    assert(this->lookupidx < this->lookupsize);
  }
  else { // OFF_PATH
    this->lookupidx = -1;
  }
  return (this->lookupidx >= 0);
}

/*!
  Pop the current node off the path.
*/
void
SoCompactPathList::pop()
{
  this->lookupidx = this->stack.pop();
}

/*!
  Precalculate tables for each node in the paths.
*/
int
SoCompactPathList::createLookupTable(int curidx, int depth,
                                     const SoPathList & list,
                                     int firstpath, int numpaths)
{
  // When we get to the tail, store a 0 for no children. The traversal
  // will switch to BELOW_PATH when this happens.
  if (depth >= FULL_PATH(list, firstpath)->getLength()) {
    this->lookuptable[curidx] = 0;
    return curidx + 1;
  }

  int i;
  // count the number of IN_PATH indices that we need to create and
  // fill in first part of the lookup table. We do this by finding the
  // number of distinct children in all the paths we're traversing.
  int numchildren = 0;
  int prevchildidx = -1;

  for (i = 0; i < numpaths; i++) {
    int childidx = FULL_PATH(list, firstpath + i)->getIndex(depth);
    if (childidx != prevchildidx) {
      // fill in the IN_PATH table indices
      this->lookuptable[curidx + 1 + numchildren] = childidx;
      numchildren++;
      prevchildidx = childidx;
    }
  }
  // store the size of this node's tables
  this->lookuptable[curidx] = numchildren;

  // Find next free position in lookup table. We need to store 1 +
  // 2*numchildren integers for this node (size + IN_PATH table and
  // lookup table position for each child)
  int nextidx = curidx + 1 + 2 * numchildren;

  // Recurse and fill in info for each child in the lookup table
  int curchild = 0;
  i = 0;

  while (i < numpaths) {
    int startpath = i + firstpath;
    int childidx = FULL_PATH(list, firstpath + i)->getIndex(depth);
    int pathcounter = 1;
    i++;
    // find all paths that go through childidx
    while ((i < numpaths) && (FULL_PATH(list, firstpath + i)->getIndex(depth) == childidx)) {
      i++;
      pathcounter++;
    }
    this->lookuptable[curidx + 1 + numchildren + curchild] = nextidx;
    nextidx = this->createLookupTable(nextidx, depth + 1, list,
                                      startpath, pathcounter);
    curchild++;
  }
  assert(curchild == numchildren);
  return nextidx;
}


#undef FULL_PATH

/*!
  Returns the number of IN_PATH children for the current node.
*/
int
SoCompactPathList::getNumIndices(void)
{
  assert(this->lookupidx >= 0);
  return this->lookuptable[this->lookupidx];
}

/*!
  Returns the position of the IN_PATH table.
*/
int
SoCompactPathList::getStartIndex(void)
{
  assert(this->lookupidx >= 0);
  return this->lookupidx + 1;
}

/*!
  Returns the lookup table position for \a child.
*/
int
SoCompactPathList::getChildIndex(const int child)
{
  const int idx = this->getStartIndex() + this->getNumIndices() + child;
  assert(idx < this->lookupsize);
  return this->lookuptable[idx];
}

/*!
  Returns the depth of the current node.
*/
int
SoCompactPathList::getDepth(void) const
{
  return 1 + this->stack.getLength();
}
