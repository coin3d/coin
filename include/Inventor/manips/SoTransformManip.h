/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

#ifndef COIN_SOTRANSFORMMANIP_H
#define COIN_SOTRANSFORMMANIP_H

#include <Inventor/nodes/SoTransform.h>

class SoChildList;
class SoDragger;
class SoFieldSensor;
class SoPath;
class SoSensor;


class SoTransformManip : public SoTransform {
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
  ~SoTransformManip();

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
};

#endif // !COIN_SOTRANSFORMMANIP_H
