#ifndef COIN_SOLIGHTPATH_H
#define COIN_SOLIGHTPATH_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/lists/SbList.h>

class SoNode;
class SoTempPath;

class COIN_DLL_API SoLightPath {
public:
  SoLightPath(SoNode *node, const int approxlength);
  SoLightPath(const int approxlength);
  ~SoLightPath();

  void setHead(SoNode * const node);
  void append(const int childindex);
  void push(const int childindex);
  void pop(void);
  void setTail(const int childindex);
  SoNode *getTail(void) const;
  SoNode *getHead(void) const;
  SoNode *getNode(const int index) const;
  int getIndex(const int index) const;
  int getFullLength() const;
  void truncate(const int startindex);

  void makeTempPath(SoTempPath *path) const;
private:
  SoNode *headnode;
  SbList <int> indices;
};

#endif // !COIN_SOLIGHTPATH_H
