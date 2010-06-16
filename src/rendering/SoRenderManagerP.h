#ifndef COIN_SORENDERMANAGERP_H
#define COIN_SORENDERMANAGERP_H

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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <vector>

#ifdef COIN_THREADSAFE
#include <Inventor/threads/SbMutex.h>
#endif // COIN_THREADSAFE

#include <Inventor/system/gl.h>
#include <Inventor/SbColor4f.h>
#include <Inventor/SoRenderManager.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/misc/SoNotification.h>

class SbMatrix;
class SoNodeSensor;
class SoInfo;
class SoNode;
class SoGetBoundingBoxAction;
class SoGetMatrixAction;
class SoSearchAction;
class SbPList;

class SoRenderManagerP {
public:
  SoRenderManagerP(SoRenderManager * publ);
  ~SoRenderManagerP();

  void setClippingPlanes(void);
  static void updateClippingPlanesCB(void * closure, SoSensor * sensor);
  void getCameraCoordinateSystem(SbMatrix & matrix,
                                 SbMatrix & inverse);
  static void redrawshotTriggeredCB(void * data, SoSensor * sensor);
  static void cleanup(void);

  void lock(void) {
#ifdef COIN_THREADSAFE
    this->mutex.lock();
#endif // COIN_THREADSAFE
  }
  void unlock(void) {
#ifdef COIN_THREADSAFE
    this->mutex.unlock();
#endif // COIN_THREADSAFE
  }

  SoRenderManager * publ;
  SoNodeSensor * rootsensor;
  SoNode * scene;
  SoCamera * camera;
  float nearplanevalue;
  SbBool doublebuffer;
  SbBool isactive;
  float stereooffset;
  SoInfo * dummynode;
  uint32_t overlaycolor;
  SoColorPacker colorpacker;
  SbViewportRegion stereostencilmaskvp;
  GLubyte * stereostencilmask;
  SbColor4f backgroundcolor;
  int backgroundindex;
  SbBool texturesenabled;
  SbBool isrgbmode;
  uint32_t redrawpri;
  SoNodeSensor * clipsensor;

  SoGetBoundingBoxAction * getbboxaction;
  SoAudioRenderAction * audiorenderaction;
  SoGetMatrixAction * getmatrixaction;
  SoGLRenderAction * glaction;
  SoSearchAction * searchaction;
  SbBool deleteaudiorenderaction;
  SbBool deleteglaction;

  SoRenderManager::StereoMode stereostenciltype;
  SoRenderManager::RenderMode rendermode;
  SoRenderManager::StereoMode stereomode;
  SoRenderManager::AutoClippingStrategy autoclipping;

  SoRenderManagerRenderCB * rendercb;
  void * rendercbdata;
  SoOneShotSensor * redrawshot;

  SbPList * superimpositions;

  void invokePreRenderCallbacks(void);
  void invokePostRenderCallbacks(void);
  typedef std::pair<SoRenderManagerRenderCB *, void *> RenderCBTouple;
  std::vector<RenderCBTouple> preRenderCallbacks;
  std::vector<RenderCBTouple> postRenderCallbacks;

  // "private" data
  static SbBool touchtimer;
  static SbBool cleanupfunctionset;

#ifdef COIN_THREADSAFE
  SbMutex mutex;
#endif // COIN_THREADSAFE
};

// *************************************************************************

// This class inherits SoNodeSensor and overrides its notify() method
// to provide a means of debugging notifications on the root node.
//
// Good for debugging cases when there are continuous redraws due to
// scene graph changes we have no clue as to the source of.
//
// A sensor of this class is only made if the below debugging envvar
// is set. Otherwise, and ordinary SoNodeSensor is used instead.

class SoRenderManagerRootSensor : public SoNodeSensor {
  typedef SoNodeSensor inherited;

public:
  SoRenderManagerRootSensor(SoSensorCB * func, void * data) : inherited(func, data) { }
  virtual ~SoRenderManagerRootSensor() { }

  virtual void notify(SoNotList * l);
  static SbBool debug(void);

private:
  static int debugrootnotifications;
};

// *************************************************************************


#endif // COIN_SORENDERMANAGERP_H
