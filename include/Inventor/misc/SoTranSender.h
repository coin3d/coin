#ifndef COIN_SOTRANSENDER_H
#define COIN_SOTRANSENDER_H

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

#include <Inventor/SbBasic.h>

class SoOutput;
class SoNode;


class COIN_DLL_API SoTranSender {

public:
  SoTranSender(SoOutput * output);
  ~SoTranSender();

  SoOutput * getOutput(void) const;
  void insert(SoNode * node);
  void insert(SoNode * node, SoNode * parent, int n);
  void remove(SoNode * parent, int n);
  void replace(SoNode * parent, int n, SoNode * newnode);
  void modify(SoNode * node);
  void prepareToSend(void);

private:
  // Placeholder for any data for the instance. Just added for the
  // sake of making it possible to implement this class after release
  // without breaking ABI compatibility.
  void * pimpl;
};

#endif // !COIN_SOTRANSENDER_H
