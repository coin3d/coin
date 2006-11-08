#ifndef COIN_SOCOMPACTPATHLIST_H
#define COIN_SOCOMPACTPATHLIST_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

#include <Inventor/SbBasic.h>

class SoPathList;

class COIN_DLL_API SoCompactPathList {
public:
  SoCompactPathList(const SoPathList & list);
  ~SoCompactPathList();
  
  void reset(void);

  void getChildren(int & numindices, const int *& indices);
  SbBool push(int childindex);
  void pop(void);

private:
  int * lookuptable;
  SbList <int> stack;
  int	lookupidx;
  int lookupsize;

  int	getNumIndices(void); 
  int	getStartIndex(void);

  int	getChildIndex(const int child);  
  int	createLookupTable(int curslot, int depth,
                        const SoPathList & list,
                        int firstpath, int numpaths);
};

#endif /* COIN_SOCOMPACTPATHLIST_H */
