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

#ifndef __SONODEKITPATH_H__
#define __SONODEKITPATH_H__

#include <Inventor/SoPath.h>

class SoNode;
class SoBaseKit;

class SoNodeKitPath : public SoPath
{
  typedef SoPath inherited;

public:
  int getLength(void) const;
  SoNode * getTail(void) const;
  SoNode * getNode(int i) const;
  SoNode * getNodeFromTail(int i) const;
  void truncate(int start);
  void pop(void);
  void append(SoBaseKit * childKit);
  void append(const SoNodeKitPath * fromPath);
  SbBool containsNode(SoBaseKit * node) const;
  int findFork(const SoNodeKitPath * path) const;

  friend int operator==(const SoNodeKitPath & p1, const SoNodeKitPath & p2);

protected:
  SoNodeKitPath(int approxLength);
  virtual ~SoNodeKitPath();
};

#endif // !__SONODEKITPATH_H__
