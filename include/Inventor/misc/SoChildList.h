#ifndef COIN_SOCHILDLIST_H
#define COIN_SOCHILDLIST_H

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

#include <Inventor/lists/SoNodeList.h>

class SoPath;
class SoAction;

class COIN_DLL_API SoChildList : public SoNodeList {
  typedef SoNodeList inherited;

public:
  SoChildList(SoNode * const parent);
  SoChildList(SoNode * const parent, const int size);
  SoChildList(SoNode * const parent, const SoChildList & cl);
  ~SoChildList();

  void append(SoNode * const node);
  void insert(SoNode * const ptr, const int addbefore);
  void remove(const int index);
  void truncate(const int length);
  void copy(const SoChildList & cl);
  void set(const int index, SoNode * const node);

  void traverseInPath(SoAction * const action, const int numindices,
                      const int * indices);
  void traverse(SoAction * const action);
  void traverse(SoAction * const action, const int index);
  void traverse(SoAction * const action, SoNode * node);
  void traverse(SoAction * const action, const int first, const int last);
  
public:
  void addPathAuditor(SoPath * const path);
  void removePathAuditor(SoPath * const path);

private:
  SoNode * parent;
  SbList<SoPath *> auditors;
};

#endif // !COIN_SOCHILDLIST_H
