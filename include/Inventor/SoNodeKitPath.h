#ifndef COIN_SONODEKITPATH_H
#define COIN_SONODEKITPATH_H

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

#include <Inventor/SoPath.h>

class SoNode;
class SoBaseKit;
class SoSearchAction;

class COIN_DLL_API SoNodeKitPath : public SoPath {
  typedef SoPath inherited;

public:
  int getLength(void) const;
  SoNode * getTail(void) const;
  SoNode * getNode(const int idx) const;
  SoNode * getNodeFromTail(const int idx) const;
  void truncate(const int length);
  void pop(void);
  void append(SoBaseKit * childKit);
  void append(const SoNodeKitPath * fromPath);
  SbBool containsNode(SoBaseKit * node) const;
  int findFork(const SoNodeKitPath * path) const;

  friend COIN_DLL_API int operator==(const SoNodeKitPath & p1, const SoNodeKitPath & p2);

protected:
  SoNodeKitPath(const int approxLength);
  virtual ~SoNodeKitPath();

private:

  static void clean(void);
  static SoSearchAction *searchAction;
  SoSearchAction *getSearchAction(void);

  // these methods should not be used on an SoNodeKitPath
  void append(const int childIndex);
  void append(SoNode *childNode);
  void append(const SoPath *fromPath);
  void push(const int childIndex);
  int getIndex(const int i) const;
  int getIndexFromTail(const int i) const;
  void insertIndex(SoNode *parent,const int newIndex);
  void removeIndex(SoNode *parent,const int oldIndex);
  void replaceIndex(SoNode *parent,const int index,SoNode *newChild);
};

COIN_DLL_API int operator==(const SoNodeKitPath & p1, const SoNodeKitPath & p2);

#endif // !COIN_SONODEKITPATH_H
