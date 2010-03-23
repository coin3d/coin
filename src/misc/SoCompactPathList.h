#ifndef COIN_SOCOMPACTPATHLIST_H
#define COIN_SOCOMPACTPATHLIST_H

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

#include <Inventor/SbBasic.h>
#include <Inventor/lists/SbList.h>

class SoPathList;

#ifndef COIN_INTERNAL
#error this is a private header file
#endif // !COIN_INTERNAL

// SoCompactPathList is an internal class in Coin and should not be
// used by external applications. It's used only to optimize
// SoPathList traversing. Let us know if you still need to use this
// class for some reason and we might add it as a part of the public
// Coin API.

class SoCompactPathList {
public:
  SoCompactPathList(const SoPathList & list);
  ~SoCompactPathList();

  void reset(void);

  void getChildren(int & numindices, const int *& indices);
  SbBool push(int childindex);
  void pop(void);

  int getDepth(void) const;

private:
  int * lookuptable;
  SbList <int> stack;
  int lookupidx;
  int lookupsize;

  int getNumIndices(void);
  int getStartIndex(void);

  int getChildIndex(const int child);
  int createLookupTable(int curslot, int depth,
                        const SoPathList & list,
                        int firstpath, int numpaths);
};

#endif /* COIN_SOCOMPACTPATHLIST_H */
