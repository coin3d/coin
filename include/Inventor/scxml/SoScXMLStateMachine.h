#ifndef COIN_SOSCXMLSTATEMACHINE_H
#define COIN_SOSCXMLSTATEMACHINE_H

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

#include <Inventor/scxml/ScXMLStateMachine.h>

class SoEvent;
class SoNode;
class SoCamera;
class SbViewportRegion;

class COIN_DLL_API SoScXMLStateMachine : public ScXMLStateMachine {
  typedef ScXMLStateMachine inherited;
  SCXML_OBJECT_HEADER(SoScXMLStateMachine)

public:
  static void initClass(void);
  static void cleanClass(void);

  SoScXMLStateMachine(void);
  virtual ~SoScXMLStateMachine(void);

  virtual void setSceneGraphRoot(SoNode * root);
  virtual SoNode * getSceneGraphRoot(void) const;

  virtual void setActiveCamera(SoCamera * camera);
  virtual SoCamera * getActiveCamera(void) const;

  virtual void setViewportRegion(const SbViewportRegion & vp);
  virtual const SbViewportRegion & getViewportRegion(void) const;

  virtual void preGLRender(void);
  virtual void postGLRender(void);

  virtual SbBool processSoEvent(const SoEvent * event);

  virtual const char * getVariable(const char * key) const;

private:
  SoScXMLStateMachine(const SoScXMLStateMachine & rhs); // N/A
  SoScXMLStateMachine & operator = (const SoScXMLStateMachine & rhs); // N/A

  class PImpl;
  SbPimplPtr<PImpl> pimpl;

}; // SoScXMLStateMachine

#endif // !COIN_SOSCXMLSTATEMACHINE_H
