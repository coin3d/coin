#ifndef COIN_SOFULLPATH_H
#define COIN_SOFULLPATH_H

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

#include <Inventor/SoPath.h>

class COIN_DLL_API SoFullPath : public SoPath {
  typedef SoPath inherited;
  friend class SoTempPath;

private:
  SoFullPath(const int approxLength);
  virtual ~SoFullPath(void);

public:
  void pop(void);
  SoNode * getTail(void) const;
  SoNode * getNodeFromTail(const int index) const;
  int getIndexFromTail(const int index) const;
  int getLength(void) const;
};

// inline methods

inline void
SoFullPath::pop(void)
{
  this->truncate(this->nodes.getLength() - 1);
}

#endif // !COIN_SOFULLPATH_H
