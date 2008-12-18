#ifndef COIN_SOSCXMLPANUTILS_H
#define COIN_SOSCXMLPANUTILS_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2008 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include "navigation/SoScXMLNavigationInvoke.h"

#include <map>

// SetPan    - initialize the rotate mode
// UpdatePan - move camera
// EndPan    - figure out if this ends in a spin

class PanData;

// *************************************************************************

class SoScXMLPanInvoke : public SoScXMLNavigationInvoke {
  typedef SoScXMLNavigationInvoke inherited;
  SCXML_OBJECT_ABSTRACT_HEADER(SoScXMLPanInvoke);

public:
  static void initClass(void);

protected:
  static PanData * getPanData(const ScXMLStateMachine * statemachine);
  static void freePanData(const ScXMLStateMachine * statemachine);

private:
  typedef std::map<const ScXMLStateMachine *, PanData *> PanDataMap;
  typedef std::pair<const ScXMLStateMachine *, PanData *> PanDataEntry;

  static PanDataMap pandatamap;

}; // SoScXMLPanInvoke

// *************************************************************************

class SoScXMLSetPan : public SoScXMLPanInvoke {
  typedef SoScXMLPanInvoke inherited;
  SCXML_OBJECT_HEADER(SoScXMLSetPan);

public:
  static void initClass(void);

  virtual void invoke(ScXMLStateMachine * statemachine);

}; // SoScXMLSetPan

// *************************************************************************

class SoScXMLUpdatePan : public SoScXMLPanInvoke {
  typedef SoScXMLPanInvoke inherited;
  SCXML_OBJECT_HEADER(SoScXMLUpdatePan);

public:
  static void initClass(void);

  virtual void invoke(ScXMLStateMachine * statemachine);

}; // SoScXMLUpdatePan

// *************************************************************************

class SoScXMLEndPan : public SoScXMLPanInvoke {
  typedef SoScXMLPanInvoke inherited;
  SCXML_OBJECT_HEADER(SoScXMLEndPan);

public:
  static void initClass(void);

  virtual void invoke(ScXMLStateMachine * statemachine);

}; // SoScXMLEndPan

// *************************************************************************

#endif // !COIN_SOSCXMLPANUTILS_H
