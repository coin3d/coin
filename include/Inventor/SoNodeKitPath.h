/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SONODEKITPATH_H
#define COIN_SONODEKITPATH_H

#include <Inventor/SoPath.h>

class SoNode;
class SoBaseKit;
class SoSearchAction;

class COIN_DLL_EXPORT SoNodeKitPath : public SoPath {
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

  friend COIN_DLL_EXPORT int operator==(const SoNodeKitPath & p1, const SoNodeKitPath & p2);

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

COIN_DLL_EXPORT int operator==(const SoNodeKitPath & p1, const SoNodeKitPath & p2);

#endif // !COIN_SONODEKITPATH_H
