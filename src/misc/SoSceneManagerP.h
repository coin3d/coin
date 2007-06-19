/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2007 by Systems in Motion.  All rights reserved.
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
 *  See http://www.coin3d.org/ for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#ifndef COIN_SOSCENEMANAGERP_H
#define COIN_SOSCENEMANAGERP_H

#include <Inventor/system/gl.h>
#include <Inventor/SoSceneManager.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/elements/SoLazyElement.h>

class SoCamera;
class SoInfo;

class SoSceneManagerP {
public:
  SoSceneManagerP(SoSceneManager * publ);
  ~SoSceneManagerP();

  enum {
    FLAG_RGBMODE = 0x0001,
    FLAG_ACTIVE = 0x0002
  };

  SbBool isActive(void) const { return this->flags & SoSceneManagerP::FLAG_ACTIVE; }
  static void redrawshotTriggeredCB(void * data, SoSensor * sensor);
        
  void setCamera(SoCamera * camera);
  SoCamera * getCamera(void);
  static void cleanup(void);
  static SbBool cleanupfunctionset;

  SbColor backgroundcolor;

  SoSceneManager::RenderMode rendermode;
  SoSceneManager::StereoMode stereomode;

  float stereooffset;
  SoSearchAction searchaction;
  SoCamera * camera;
  SbBool doublebuffer;

  SoSceneManagerRenderCB * rendercb;
  void * rendercbdata;
  static SbBool touchtimer;
  
  SoGLRenderAction * glaction;
  SbBool deleteglaction;
  SoAudioRenderAction *audiorenderaction;
  SbBool deleteaudiorenderaction;
  SoHandleEventAction * handleeventaction;
  SbBool deletehandleeventaction;
  
  SoNode * scene;
  SoOneShotSensor * redrawshot;
  
  SoSceneManager * publ;
  
  int backgroundindex;

  uint32_t flags;  
  uint32_t redrawpri;

  SbPList superimpositions;
};

#endif // COIN_SOSCENEMANAGERP_H
