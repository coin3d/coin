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

#ifndef COIN_SOCLIPPLANEMANIP_H
#define COIN_SOCLIPPLANEMANIP_H

#include <Inventor/nodes/SoClipPlane.h>
#include <Inventor/fields/SoSFVec3f.h>

class SbBox3f;
class SoChildList;
class SoDragger;
class SoFieldSensor;
class SoPath;
class SoSensor;


class COIN_DLL_EXPORT SoClipPlaneManip : public SoClipPlane {
  typedef SoClipPlane inherited;

  SO_NODE_HEADER(SoClipPlaneManip);

public:
  static void initClass(void);
  SoClipPlaneManip(void);

  SoDragger * getDragger(void);
  void setValue(const SbBox3f & box, const SbVec3f & planenormal,
                float draggerscalefactor);
  SbBool replaceNode(SoPath * p);
  SbBool replaceManip(SoPath * p, SoClipPlane * newone) const;

  virtual void doAction(SoAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void getMatrix(SoGetMatrixAction * action);
  virtual void handleEvent(SoHandleEventAction * action);
  virtual void pick(SoPickAction * action);
  virtual void search(SoSearchAction * action);

  virtual SoChildList * getChildren(void) const;

  SoSFVec3f draggerPosition;

protected:
  ~SoClipPlaneManip();

  void setDragger(SoDragger * newdragger);

  virtual void copyContents(const SoFieldContainer * fromfc, SbBool copyconnections);
  static void valueChangedCB(void * f, SoDragger * d);
  static void fieldSensorCB(void * f, SoSensor * d);
  static void transferFieldValues(const SoClipPlane * from, SoClipPlane * to);

  SoFieldSensor * planeFieldSensor;
  SoFieldSensor * onFieldSensor;
  SoFieldSensor * draggerPosFieldSensor;
  SoChildList * children;

private:
  void attachSensors(const SbBool onoff);

};

#endif // !COIN_SOCLIPPLANEMANIP_H
