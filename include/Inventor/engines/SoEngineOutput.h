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

#ifndef COIN_SOENGINEOUTPUT_H
#define COIN_SOENGINEOUTPUT_H

#include <Inventor/SoType.h>
#include <Inventor/lists/SoFieldList.h>

class SoNotList;


class COIN_DLL_EXPORT SoEngineOutput {
public:
  SoEngineOutput(void);
  virtual ~SoEngineOutput();

  SoType getConnectionType(void) const;
  int getForwardConnections(SoFieldList & fl) const;
  void enable(const SbBool flag);
  SbBool isEnabled(void) const;
  class SoEngine * getContainer(void) const;

  void setContainer(SoEngine * engine);
  void addConnection(SoField * f);
  void removeConnection(SoField * f);
  int getNumConnections(void) const;
  SoField * operator[](int i) const;

  void prepareToWrite(void) const;
  void doneWriting(void) const;

  void touchSlaves(SoNotList * nl, SbBool donotify);

private:
  SbBool enabled;
  SoEngine * container;
  SoFieldList slaves;
  SbDict notifyflags;
};

#endif // !COIN_SOENGINEOUTPUT_H
