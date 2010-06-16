#ifndef COIN_SOSCXMLNAVIGATIONTARGET_H
#define COIN_SOSCXMLNAVIGATIONTARGET_H

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

#include <Inventor/scxml/ScXMLEventTarget.h>

#include <Inventor/tools/SbPimplPtr.h>

class SbVec2f;
class SbVec3f;
class SbRotation;
class SoCamera;
class ScXMLEvent;
class ScXMLStateMachine;
class SoScXMLStateMachine;

#define COIN_NAVIGATION_EVENT_PREFIX "x-coin-navigation"

class COIN_DLL_API SoScXMLNavigationTarget : public ScXMLEventTarget {
  typedef ScXMLEventTarget inherited;
  SCXML_OBJECT_ABSTRACT_HEADER(SoScXMLNavigationTarget)

public:
  static void initClass(void);
  static void cleanClass(void);

  class COIN_DLL_API Data { // virtual base for subclasses using getSessionData()...
  public:
    virtual ~Data(void);
  };

protected:
  SoScXMLNavigationTarget(void);
  virtual ~SoScXMLNavigationTarget(void);

  typedef Data * NewDataFunc(void);
  Data * getSessionData(SbName sessionid, NewDataFunc * constructor);
  void freeSessionData(SbName sessionid);

  static SbName getSessionId(const ScXMLEvent * event);
  static ScXMLStateMachine * getStateMachine(const ScXMLEvent * event, SbName sessionid);
  static SoScXMLStateMachine * getSoStateMachine(const ScXMLEvent * event, SbName sessionid);
  static SoCamera * getActiveCamera(const ScXMLEvent * event, SbName sessionid);

  static SbBool getEventDouble(const ScXMLEvent * event, const char * label, double & dbl_out, SbBool required = TRUE);
  static SbBool getEventString(const ScXMLEvent * event, const char * label, SbString & str_out, SbBool required = TRUE);
  static SbBool getEventSbBool(const ScXMLEvent * event, const char * label, SbBool & bool_out, SbBool required = TRUE);
  static SbBool getEventSbVec2f(const ScXMLEvent * event, const char * label, SbVec2f & vec_out, SbBool required = TRUE);
  static SbBool getEventSbVec3f(const ScXMLEvent * event, const char * label, SbVec3f & vec_out, SbBool required = TRUE);
  static SbBool getEventSbRotation(const ScXMLEvent * event, const char * label, SbRotation & rot_out, SbBool required = TRUE);

private:
  class PImpl;
  SbPimplPtr<PImpl> pimpl;

}; // SoScXMLNavigationTarget

#endif // !COIN_SOSCXMLNAVIGATIONTARGET_H
