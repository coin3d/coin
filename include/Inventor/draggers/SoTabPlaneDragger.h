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

#ifndef COIN_SOTABPLANEDRAGGER_H
#define COIN_SOTABPLANEDRAGGER_H

#include <Inventor/draggers/SoDragger.h>
#include <Inventor/fields/SoSFVec3f.h>

class SoSensor;
class SoFieldSensor;
class SbLineProjector;
class SbPlaneProjector;


class SoTabPlaneDragger : public SoDragger {
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
  enum State {
    INACTIVE,
    TRANSLATING,
    EDGE_SCALING,
    CORNER_SCALING,
    UNIFORM_SCALING
  };

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

  void translateStart(void);
  void translateDrag(void);
  void edgeScaleStart(void);
  void edgeScaleDrag(void);
  void cornerScaleStart(void);
  void cornerScaleDrag(void);
  void scaleUniformStart(void);
  void scaleUniformDrag(void);

  static void startCB(void * f, SoDragger * d);
  static void motionCB(void * f, SoDragger * d);
  static void finishCB(void * f, SoDragger * d);
  static void metaKeyChangeCB(void * f, SoDragger * d);
  static void fieldSensorCB(void * f, SoSensor * s);
  static void valueChangedCB(void * f, SoDragger * d);

  SbBool needScaleTabAdjustment;
  SbBool shftDown;
  SbLineProjector * lineProj;
  SbPlaneProjector * planeProj;
  SbVec3f scaleCenter;
  SbVec3f worldRestartPt;
  SoFieldSensor * scaleFieldSensor;
  SoFieldSensor * translFieldSensor;
  State currentState;
  State restartState;
  int currentScalePatch;
  int translateDir;
};

#endif // !COIN_SOTABPLANEDRAGGER_H
