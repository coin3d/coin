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

#ifndef COIN_SOENGINEOUTPUT_H
#define COIN_SOENGINEOUTPUT_H

#include <Inventor/SoType.h>
#include <Inventor/lists/SoFieldList.h>

class SoNotList;
class SoFieldContainer;
class SoEngine;
class SoNodeEngine;

class COIN_DLL_API SoEngineOutput {
public:
  SoEngineOutput(void);
  virtual ~SoEngineOutput();

  SoType getConnectionType(void) const;
  int getForwardConnections(SoFieldList & fl) const;
  void enable(const SbBool flag);
  SbBool isEnabled(void) const;
  SoEngine * getContainer(void) const;
  SoNodeEngine * getNodeContainer(void) const;
  SbBool isNodeEngineOutput(void) const;

  void setContainer(SoEngine * engine);
  void setNodeContainer(SoNodeEngine * nodeengine);
  void addConnection(SoField * f);
  void removeConnection(SoField * f);
  int getNumConnections(void) const;
  SoField * operator[](int i) const;

  void prepareToWrite(void) const;
  void doneWriting(void) const;

  void touchSlaves(SoNotList * nl, SbBool donotify);

  SoFieldContainer * getFieldContainer(void);

private:
  
  SbBool enabled;
  SoEngine * container;
  SoFieldList slaves;

  // FIXME: kill for Coin v2 (can't break ABI compatibility for Coin
  // v1.x.y releases).  20010910 mortene.
  SbDict notifyflags;
};

#endif // !COIN_SOENGINEOUTPUT_H
