#ifndef COIN_SOSCXMLSEEKUTILS_H
#define COIN_SOSCXMLSEEKUTILS_H

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

// SetSeek    - initialize the rotate mode
// UpdateSeek - move camera
// EndSeek    - figure out if this ends in a spin

class SbMatrix;
class SoNode;
class SoCamera;

class SoSearchAction;
class SoGetMatrixAction;
class SoRayPickAction;

class SeekData;

// *************************************************************************

class SoScXMLSeekInvoke : public SoScXMLNavigationInvoke {
  typedef SoScXMLNavigationInvoke inherited;
  SCXML_OBJECT_ABSTRACT_HEADER(SoScXMLSeekInvoke);

public:
  static void initClass(void);

protected:
  static SeekData * getSeekData(const ScXMLStateMachine * statemachine);
  static void freeSeekData(const ScXMLStateMachine * statemachine);

private:
  typedef std::map<const ScXMLStateMachine *, SeekData *> SeekDataMap;
  typedef std::pair<const ScXMLStateMachine *, SeekData *> SeekDataEntry;

  static SeekDataMap seekdatamap;

}; // SoScXMLSeekInvoke

// *************************************************************************

class SoScXMLSetSeek : public SoScXMLSeekInvoke {
  typedef SoScXMLSeekInvoke inherited;
  SCXML_OBJECT_HEADER(SoScXMLSetSeek);

public:
  static void initClass(void);

  SoScXMLSetSeek(void);
  virtual ~SoScXMLSetSeek(void);

  virtual void invoke(ScXMLStateMachine * statemachine);

protected:
  SoSearchAction * searchaction;
  SoGetMatrixAction * getmatrixaction;
  SoRayPickAction * raypickaction;

private:
  void getCameraCoordinateSystem(SoCamera * cameraarg,
                                 SoNode * root,
                                 SbMatrix & matrix,
                                 SbMatrix & inverse);

}; // SoScXMLSetSeek

// *************************************************************************

class SoScXMLUpdateSeek : public SoScXMLSeekInvoke {
  typedef SoScXMLSeekInvoke inherited;
  SCXML_OBJECT_HEADER(SoScXMLUpdateSeek);

public:
  static void initClass(void);

  virtual void invoke(ScXMLStateMachine * statemachine);

}; // SoScXMLUpdateSeek

// *************************************************************************

class SoScXMLEndSeek : public SoScXMLSeekInvoke {
  typedef SoScXMLSeekInvoke inherited;
  SCXML_OBJECT_HEADER(SoScXMLEndSeek);

public:
  static void initClass(void);

  virtual void invoke(ScXMLStateMachine * statemachine);

}; // SoScXMLEndSeek

// *************************************************************************

#endif // !COIN_SOSCXMLSEEKUTILS_H
