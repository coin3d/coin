#ifndef COIN_SOCLIPPLANEMANIP_H
#define COIN_SOCLIPPLANEMANIP_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/nodes/SoClipPlane.h>
#include <Inventor/fields/SoSFVec3f.h>

class SbBox3f;
class SoChildList;
class SoDragger;
class SoFieldSensor;
class SoPath;
class SoSensor;


class COIN_DLL_API SoClipPlaneManip : public SoClipPlane {
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
