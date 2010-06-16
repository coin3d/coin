#ifndef COIN_SOTRANSFORMMANIP_H
#define COIN_SOTRANSFORMMANIP_H

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

#include <Inventor/nodes/SoTransform.h>
#include <Inventor/tools/SbLazyPimplPtr.h>
#ifndef COIN_INTERNAL
#include <Inventor/draggers/SoDragger.h>
#endif // !COIN_INTERNAL

class SoChildList;
class SoDragger;
class SoFieldSensor;
class SoPath;
class SoSensor;
class SoTransformManipP;

class COIN_DLL_API SoTransformManip : public SoTransform {
  typedef SoTransform inherited;

  SO_NODE_HEADER(SoTransformManip);

public:
  static void initClass(void);
  SoTransformManip(void);

  SoDragger * getDragger(void);
  SbBool replaceNode(SoPath * p);
  SbBool replaceManip(SoPath * p, SoTransform * newone) const;

  virtual void doAction(SoAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void getMatrix(SoGetMatrixAction * action);
  virtual void handleEvent(SoHandleEventAction * action);
  virtual void pick(SoPickAction * action);
  virtual void search(SoSearchAction * action);

  virtual SoChildList * getChildren(void) const;

protected:
  virtual ~SoTransformManip(void);

  void setDragger(SoDragger * newdragger);

  virtual void copyContents(const SoFieldContainer * fromfc,
                            SbBool copyconnections);

  static void transferFieldValues(const SoTransform * from, SoTransform * to);
  static void valueChangedCB(void * f, SoDragger * d);
  static void fieldSensorCB(void * f, SoSensor * d);

  SoFieldSensor * rotateFieldSensor;
  SoFieldSensor * translFieldSensor;
  SoFieldSensor * scaleFieldSensor;
  SoFieldSensor * centerFieldSensor;
  SoFieldSensor * scaleOrientFieldSensor;
  SoChildList * children;

  void attachSensors(const SbBool onoff);

private:
  SbLazyPimplPtr<SoTransformManipP> pimpl;

  // NOT IMPLEMENTED:
  SoTransformManip(const SoTransformManip & rhs);
  SoTransformManip & operator = (const SoTransformManip & rhs);
}; // SoTransformManip

#endif // !COIN_SOTRANSFORMMANIP_H
