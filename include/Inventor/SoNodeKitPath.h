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

  friend int operator==(const SoNodeKitPath & p1, const SoNodeKitPath & p2);

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

#endif // !COIN_SONODEKITPATH_H
