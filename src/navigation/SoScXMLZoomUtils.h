#ifndef COIN_SOSCXMLZOOMUTILS_H
#define COIN_SOSCXMLZOOMUTILS_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2009 by Kongsberg SIM.  All rights reserved.
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

// SetZoom    - initialize the rotate mode
// UpdateZoom - move camera
// EndZoom    - figure out if this ends in a spin

class ZoomData;

// *************************************************************************

class SoScXMLZoomInvoke : public SoScXMLNavigationInvoke {
  typedef SoScXMLNavigationInvoke inherited;
  SCXML_OBJECT_ABSTRACT_HEADER(SoScXMLZoomInvoke);

public:
  static void initClass(void);

protected:
  static ZoomData * getZoomData(const ScXMLStateMachine * statemachine);
  static void freeZoomData(const ScXMLStateMachine * statemachine);

private:
  typedef std::map<const ScXMLStateMachine *, ZoomData *> ZoomDataMap;
  typedef std::pair<const ScXMLStateMachine *, ZoomData *> ZoomDataEntry;

  static ZoomDataMap zoomdatamap;

}; // SoScXMLZoomInvoke

// *************************************************************************

class SoScXMLSetZoom : public SoScXMLZoomInvoke {
  typedef SoScXMLZoomInvoke inherited;
  SCXML_OBJECT_HEADER(SoScXMLSetZoom);

public:
  static void initClass(void);

  virtual void invoke(ScXMLStateMachine * statemachine);

}; // SoScXMLSetZoom

// *************************************************************************

class SoScXMLUpdateZoom : public SoScXMLZoomInvoke {
  typedef SoScXMLZoomInvoke inherited;
  SCXML_OBJECT_HEADER(SoScXMLUpdateZoom);

public:
  static void initClass(void);

  virtual void invoke(ScXMLStateMachine * statemachine);

}; // SoScXMLUpdateZoom

// *************************************************************************

class SoScXMLEndZoom : public SoScXMLZoomInvoke {
  typedef SoScXMLZoomInvoke inherited;
  SCXML_OBJECT_HEADER(SoScXMLEndZoom);

public:
  static void initClass(void);

  virtual void invoke(ScXMLStateMachine * statemachine);

}; // SoScXMLEndZoom

// *************************************************************************

#endif // !COIN_SOSCXMLZOOMUTILS_H
