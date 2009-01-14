#ifndef COIN_SOSCXMLSPINUTILS_H
#define COIN_SOSCXMLSPINUTILS_H

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

// SetSpin    - initialize the rotate mode
// UpdateSpin - move camera
// EndSpin

#include "navigation/SoScXMLNavigationInvoke.h"

#include <map>
#include <boost/intrusive_ptr.hpp>

#include <Inventor/SbTime.h>
#include <Inventor/SbRotation.h>
#include <Inventor/nodes/SoCamera.h>

class SpinData {
public:
  SpinData(void)
    : spinning(FALSE)
  {
  }

  SbBool spinning;

  boost::intrusive_ptr<SoCamera> camera;
  SbTime updatetime;
  SbRotation spinrotation;

};

// *************************************************************************

class SoScXMLSpinInvoke : public SoScXMLNavigationInvoke {
  typedef SoScXMLNavigationInvoke inherited;
  SCXML_OBJECT_ABSTRACT_HEADER(SoScXMLSpinInvoke);

public:
  static void initClass(void);

protected:
  friend class SoScXMLEndRotate;
  static SpinData * getSpinData(const ScXMLStateMachine * statemachine);
  static void freeSpinData(const ScXMLStateMachine * statemachine);

private:
  typedef std::map<const ScXMLStateMachine *, SpinData *> SpinDataMap;
  typedef std::pair<const ScXMLStateMachine *, SpinData *> SpinDataEntry;

  static SpinDataMap spindatamap;

}; // SoScXMLSpinInvoke

// *************************************************************************

class SoScXMLSetSpin : public SoScXMLSpinInvoke {
  typedef SoScXMLSpinInvoke inherited;
  SCXML_OBJECT_HEADER(SoScXMLSetSpin);

public:
  static void initClass(void);

  virtual void invoke(ScXMLStateMachine * statemachine);

}; // SoScXMLSetSpin

// *************************************************************************

class SoScXMLUpdateSpin : public SoScXMLSpinInvoke {
  typedef SoScXMLSpinInvoke inherited;
  SCXML_OBJECT_HEADER(SoScXMLUpdateSpin);

public:
  static void initClass(void);

  virtual void invoke(ScXMLStateMachine * statemachine);

}; // SoScXMLUpdateSpin

// *************************************************************************

class SoScXMLEndSpin : public SoScXMLSpinInvoke {
  typedef SoScXMLSpinInvoke inherited;
  SCXML_OBJECT_HEADER(SoScXMLEndSpin);

public:
  static void initClass(void);

  virtual void invoke(ScXMLStateMachine * statemachine);

}; // SoScXMLEndSpin

// *************************************************************************

#endif // !COIN_SOSCXMLSPINUTILS_H
