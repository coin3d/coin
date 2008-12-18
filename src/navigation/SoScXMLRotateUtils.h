#ifndef COIN_SOSCXMLROTATEUTILS_H
#define COIN_SOSCXMLROTATEUTILS_H

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

// SetRotate    - initialize the rotate mode
// UpdateRotate - move camera
// EndRotate    - figure out if this ends in a spin

class RotateData;

// *************************************************************************

class SoScXMLRotateInvoke : public SoScXMLNavigationInvoke {
  typedef SoScXMLNavigationInvoke inherited;
  SCXML_OBJECT_ABSTRACT_HEADER(SoScXMLRotateInvoke);

public:
  static void initClass(void);

protected:
  static RotateData * getRotateData(const ScXMLStateMachine * statemachine);
  static void freeRotateData(const ScXMLStateMachine * statemachine);

private:
  typedef std::map<const ScXMLStateMachine *, RotateData *> RotateDataMap;
  typedef std::pair<const ScXMLStateMachine *, RotateData *> RotateDataEntry;

  static RotateDataMap rotatedatamap;

}; // SoScXMLRotateInvoke

// *************************************************************************

class SoScXMLSetRotate : public SoScXMLRotateInvoke {
  typedef SoScXMLRotateInvoke inherited;
  SCXML_OBJECT_HEADER(SoScXMLSetRotate);

public:
  static void initClass(void);

  virtual void invoke(ScXMLStateMachine * statemachine);

protected:

}; // SoScXMLSetRotate

// *************************************************************************

class SoScXMLUpdateRotate : public SoScXMLRotateInvoke {
  typedef SoScXMLRotateInvoke inherited;
  SCXML_OBJECT_HEADER(SoScXMLUpdateRotate);

public:
  static void initClass(void);

  virtual void invoke(ScXMLStateMachine * statemachine);

protected:

}; // SoScXMLUpdateRotate

// *************************************************************************

// triggers a spin event if appropriate

class SoScXMLEndRotate : public SoScXMLRotateInvoke {
  typedef SoScXMLRotateInvoke inherited;
  SCXML_OBJECT_HEADER(SoScXMLEndRotate);

public:
  static void initClass(void);

  SoScXMLEndRotate(void);

  virtual SbBool handleXMLAttributes(void);

  virtual void invoke(ScXMLStateMachine * statemachine);

protected:
  SbBool sendspinstart;

}; // SoScXMLEndRotate

// *************************************************************************

#endif // !COIN_SOSCXMLROTATEUTILS_H
