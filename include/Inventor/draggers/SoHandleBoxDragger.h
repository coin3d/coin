#ifndef COIN_SOHANDLEBOXDRAGGER_H
#define COIN_SOHANDLEBOXDRAGGER_H

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

#include <Inventor/draggers/SoDragger.h>
#include <Inventor/fields/SoSFVec3f.h>

class SoSensor;
class SoFieldSensor;
class SbPlaneProjector;
class SbLineProjector;

class COIN_DLL_API SoHandleBoxDragger : public SoDragger {
  typedef SoDragger inherited;

  SO_KIT_HEADER(SoHandleBoxDragger);

  SO_KIT_CATALOG_ENTRY_HEADER(arrow1);
  SO_KIT_CATALOG_ENTRY_HEADER(arrow1Switch);
  SO_KIT_CATALOG_ENTRY_HEADER(arrow2);
  SO_KIT_CATALOG_ENTRY_HEADER(arrow2Switch);
  SO_KIT_CATALOG_ENTRY_HEADER(arrow3);
  SO_KIT_CATALOG_ENTRY_HEADER(arrow3Switch);
  SO_KIT_CATALOG_ENTRY_HEADER(arrow4);
  SO_KIT_CATALOG_ENTRY_HEADER(arrow4Switch);
  SO_KIT_CATALOG_ENTRY_HEADER(arrow5);
  SO_KIT_CATALOG_ENTRY_HEADER(arrow5Switch);
  SO_KIT_CATALOG_ENTRY_HEADER(arrow6);
  SO_KIT_CATALOG_ENTRY_HEADER(arrow6Switch);
  SO_KIT_CATALOG_ENTRY_HEADER(arrowTranslation);
  SO_KIT_CATALOG_ENTRY_HEADER(drawStyle);
  SO_KIT_CATALOG_ENTRY_HEADER(extruder1);
  SO_KIT_CATALOG_ENTRY_HEADER(extruder1Active);
  SO_KIT_CATALOG_ENTRY_HEADER(extruder1Switch);
  SO_KIT_CATALOG_ENTRY_HEADER(extruder2);
  SO_KIT_CATALOG_ENTRY_HEADER(extruder2Active);
  SO_KIT_CATALOG_ENTRY_HEADER(extruder2Switch);
  SO_KIT_CATALOG_ENTRY_HEADER(extruder3);
  SO_KIT_CATALOG_ENTRY_HEADER(extruder3Active);
  SO_KIT_CATALOG_ENTRY_HEADER(extruder3Switch);
  SO_KIT_CATALOG_ENTRY_HEADER(extruder4);
  SO_KIT_CATALOG_ENTRY_HEADER(extruder4Active);
  SO_KIT_CATALOG_ENTRY_HEADER(extruder4Switch);
  SO_KIT_CATALOG_ENTRY_HEADER(extruder5);
  SO_KIT_CATALOG_ENTRY_HEADER(extruder5Active);
  SO_KIT_CATALOG_ENTRY_HEADER(extruder5Switch);
  SO_KIT_CATALOG_ENTRY_HEADER(extruder6);
  SO_KIT_CATALOG_ENTRY_HEADER(extruder6Active);
  SO_KIT_CATALOG_ENTRY_HEADER(extruder6Switch);
  SO_KIT_CATALOG_ENTRY_HEADER(surroundScale);
  SO_KIT_CATALOG_ENTRY_HEADER(translator1);
  SO_KIT_CATALOG_ENTRY_HEADER(translator1Active);
  SO_KIT_CATALOG_ENTRY_HEADER(translator1Switch);
  SO_KIT_CATALOG_ENTRY_HEADER(translator2);
  SO_KIT_CATALOG_ENTRY_HEADER(translator2Active);
  SO_KIT_CATALOG_ENTRY_HEADER(translator2Switch);
  SO_KIT_CATALOG_ENTRY_HEADER(translator3);
  SO_KIT_CATALOG_ENTRY_HEADER(translator3Active);
  SO_KIT_CATALOG_ENTRY_HEADER(translator3Switch);
  SO_KIT_CATALOG_ENTRY_HEADER(translator4);
  SO_KIT_CATALOG_ENTRY_HEADER(translator4Active);
  SO_KIT_CATALOG_ENTRY_HEADER(translator4Switch);
  SO_KIT_CATALOG_ENTRY_HEADER(translator5);
  SO_KIT_CATALOG_ENTRY_HEADER(translator5Active);
  SO_KIT_CATALOG_ENTRY_HEADER(translator5Switch);
  SO_KIT_CATALOG_ENTRY_HEADER(translator6);
  SO_KIT_CATALOG_ENTRY_HEADER(translator6Active);
  SO_KIT_CATALOG_ENTRY_HEADER(translator6Switch);
  SO_KIT_CATALOG_ENTRY_HEADER(uniform1);
  SO_KIT_CATALOG_ENTRY_HEADER(uniform1Active);
  SO_KIT_CATALOG_ENTRY_HEADER(uniform1Switch);
  SO_KIT_CATALOG_ENTRY_HEADER(uniform2);
  SO_KIT_CATALOG_ENTRY_HEADER(uniform2Active);
  SO_KIT_CATALOG_ENTRY_HEADER(uniform2Switch);
  SO_KIT_CATALOG_ENTRY_HEADER(uniform3);
  SO_KIT_CATALOG_ENTRY_HEADER(uniform3Active);
  SO_KIT_CATALOG_ENTRY_HEADER(uniform3Switch);
  SO_KIT_CATALOG_ENTRY_HEADER(uniform4);
  SO_KIT_CATALOG_ENTRY_HEADER(uniform4Active);
  SO_KIT_CATALOG_ENTRY_HEADER(uniform4Switch);
  SO_KIT_CATALOG_ENTRY_HEADER(uniform5);
  SO_KIT_CATALOG_ENTRY_HEADER(uniform5Active);
  SO_KIT_CATALOG_ENTRY_HEADER(uniform5Switch);
  SO_KIT_CATALOG_ENTRY_HEADER(uniform6);
  SO_KIT_CATALOG_ENTRY_HEADER(uniform6Active);
  SO_KIT_CATALOG_ENTRY_HEADER(uniform6Switch);
  SO_KIT_CATALOG_ENTRY_HEADER(uniform7);
  SO_KIT_CATALOG_ENTRY_HEADER(uniform7Active);
  SO_KIT_CATALOG_ENTRY_HEADER(uniform7Switch);
  SO_KIT_CATALOG_ENTRY_HEADER(uniform8);
  SO_KIT_CATALOG_ENTRY_HEADER(uniform8Active);
  SO_KIT_CATALOG_ENTRY_HEADER(uniform8Switch);

public:
  static void initClass(void);
  SoHandleBoxDragger(void);

  SoSFVec3f scaleFactor;
  SoSFVec3f translation;

protected:
  ~SoHandleBoxDragger();
  virtual SbBool setUpConnections(SbBool onoff, SbBool doitalways = FALSE);
  virtual void setDefaultOnNonWritingFields(void);

  static void startCB(void * f, SoDragger * d);
  static void motionCB(void * f, SoDragger * d);
  static void finishCB(void * f, SoDragger * d);
  static void metaKeyChangeCB(void * f, SoDragger * d);
  static void fieldSensorCB(void * f, SoSensor * s);
  static void valueChangedCB(void * f, SoDragger * d);

  void dragStart(void);
  void drag(void);
  void dragFinish(void);
  void setAllPartsActive(SbBool onoroff);

  SoFieldSensor * translFieldSensor;
  SoFieldSensor * scaleFieldSensor;

private:

  void updateSwitches();
  void updateArrows();
  SoNode *getNodeFieldNode(const char *fieldname);
  SbPlaneProjector *planeProj;
  SbLineProjector *lineProj;
  int whatkind;
  int whatnum;
  int constraintState;
  SbBool ctrlDown;
  SbVec3f worldRestartPt;
  SbVec3f ctrlOffset;

  void getSurroundScaleMatrices(SbMatrix &matrix, SbMatrix &inv);
  SbVec3f getDraggerCenter();
  SbVec3f calcCtrlOffset(const SbVec3f startpt);
};

#endif // !COIN_SOHANDLEBOXDRAGGER_H
