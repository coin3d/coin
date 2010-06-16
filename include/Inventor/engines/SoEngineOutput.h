/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#ifndef COIN_SOENGINEOUTPUT_H
#define COIN_SOENGINEOUTPUT_H

#include <Inventor/SoType.h>
#include <Inventor/lists/SoFieldList.h>
#include <Inventor/lists/SbList.h>

class SoNotList;
class SoFieldContainer;
class SoEngine;
class SoNodeEngine;

class COIN_DLL_API SoEngineOutput {
public:
  SoEngineOutput(void);
  virtual ~SoEngineOutput(void);

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
  SoEngine * container; // FIXME: change to SoFieldContainer pointer
  SoFieldList slaves;
  SbList<SbBool> fieldnotiflist;
};

#endif // !COIN_SOENGINEOUTPUT_H
