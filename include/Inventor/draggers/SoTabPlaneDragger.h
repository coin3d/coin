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

#ifndef COIN_SOTABPLANEDRAGGER_H
#define COIN_SOTABPLANEDRAGGER_H

#include <Inventor/draggers/SoDragger.h>
#include <Inventor/fields/SoSFVec3f.h>

class SoSensor;
class SoFieldSensor;
class SbLineProjector;
class SbPlaneProjector;


class COIN_DLL_EXPORT SoTabPlaneDragger : public SoDragger {
  typedef SoDragger inherited;

  SO_KIT_HEADER(SoTabPlaneDragger);

  SO_KIT_CATALOG_ENTRY_HEADER(cornerScaleCoords);
  SO_KIT_CATALOG_ENTRY_HEADER(cornerScaleTab0);
  SO_KIT_CATALOG_ENTRY_HEADER(cornerScaleTab1);
  SO_KIT_CATALOG_ENTRY_HEADER(cornerScaleTab2);
  SO_KIT_CATALOG_ENTRY_HEADER(cornerScaleTab3);
  SO_KIT_CATALOG_ENTRY_HEADER(edgeScaleCoords);
  SO_KIT_CATALOG_ENTRY_HEADER(edgeScaleTab0);
  SO_KIT_CATALOG_ENTRY_HEADER(edgeScaleTab1);
  SO_KIT_CATALOG_ENTRY_HEADER(edgeScaleTab2);
  SO_KIT_CATALOG_ENTRY_HEADER(edgeScaleTab3);
  SO_KIT_CATALOG_ENTRY_HEADER(planeSwitch);
  SO_KIT_CATALOG_ENTRY_HEADER(scaleTabHints);
  SO_KIT_CATALOG_ENTRY_HEADER(scaleTabMaterial);
  SO_KIT_CATALOG_ENTRY_HEADER(scaleTabMaterialBinding);
  SO_KIT_CATALOG_ENTRY_HEADER(scaleTabNormal);
  SO_KIT_CATALOG_ENTRY_HEADER(scaleTabNormalBinding);
  SO_KIT_CATALOG_ENTRY_HEADER(scaleTabs);
  SO_KIT_CATALOG_ENTRY_HEADER(translator);


public:
  static void initClass(void);
  SoTabPlaneDragger(void);

  SoSFVec3f translation;
  SoSFVec3f scaleFactor;

  void adjustScaleTabSize(void);

protected:

  ~SoTabPlaneDragger();

  virtual void GLRender(SoGLRenderAction * action);

  virtual SbBool setUpConnections(SbBool onoff, SbBool doitalways = FALSE);
  virtual void setDefaultOnNonWritingFields(void);

  void reallyAdjustScaleTabSize(SoGLRenderAction * action);

  void getXYScreenLengths(SbVec2f & lengths, const SbMatrix & localtoscreen,
                           const SbVec2s & winsize);

  void dragStart(void);
  void drag(void);
  void dragFinish(void);

  // Lots of public/protected methods and members were removed from
  // this class as they clearly should have been private.
  // Let us know if we've removed something that you need.
  // pederb, 20000226

private:

  // static methods moved from public to private
  static void startCB(void * f, SoDragger * d);
  static void motionCB(void * f, SoDragger * d);
  static void finishCB(void * f, SoDragger * d);
  static void metaKeyChangeCB(void * f, SoDragger * d);
  static void fieldSensorCB(void * f, SoSensor * s);
  static void valueChangedCB(void * f, SoDragger * d);

  void createPrivateParts(void);
  SoNode *getNodeFieldNode(const char *fieldname);

  SoFieldSensor * scaleFieldSensor;
  SoFieldSensor * translFieldSensor;
  SbLineProjector *lineProj;
  SbPlaneProjector *planeProj;
  int whatkind;
  int constraintState;
  float prevsizex;
  float prevsizey;
  SbBool adjustTabs;
  SbVec3f worldRestartPt;
  SbVec3f scaleCenter;
};

#endif // !COIN_SOTABPLANEDRAGGER_H
