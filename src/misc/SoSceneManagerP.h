#ifndef COIN_SOSCENEMANAGERP_H
#define COIN_SOSCENEMANAGERP_H

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

#include <Inventor/SoSceneManager.h>
#include <Inventor/elements/SoLazyElement.h>

class SoCamera;
class SbMatrix;
class SoSearchAction;

class SoSceneManagerP {
public:
  SoSceneManagerP(SoSceneManager * publ);
  ~SoSceneManagerP();

  SoCamera * searchForCamera(SoNode * root,
                             SoCamera * defcamera = NULL);
  static void renderCB(void * userdata, class SoRenderManager * mgr);

  SoCamera * camera;
  SoNode * scene;
  SoSceneManagerRenderCB * rendercb;
  void * rendercbdata;
  SoSearchAction * searchaction;
  SoSceneManager * publ;
  class SoRenderManager * rendermanager;
  class SoEventManager * eventmanager;
  SbColor backgroundcolor;
};

#endif // COIN_SOSCENEMANAGERP_H
