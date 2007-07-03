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

#include <Inventor/nodes/SoViewerNavigationMode.h>
#include <Inventor/navigation/SoNavigation.h>
#include <Inventor/navigation/SoNavigationSystem.h>
#include <Inventor/navigation/SoNavigationMode.h>
#include <Inventor/navigation/SoCenterMode.h>
#include <Inventor/navigation/SoIdleMode.h>
#include <Inventor/navigation/SoOrthoCenterMode.h>
#include <Inventor/navigation/SoPanMode.h>
#include <Inventor/navigation/SoPickMode.h>
#include <Inventor/navigation/SoPitchMode.h>
#include <Inventor/navigation/SoRollMode.h>
#include <Inventor/navigation/SoRotateMode.h>
#include <Inventor/navigation/SoYawMode.h>
#include <Inventor/navigation/SoZoomMode.h>

void
SoNavigation::init(void)
{
  SoViewerNavigationMode::initClass();
  SoNavigationSystem::initClass();
  SoNavigationMode::initClass();
  SoCenterMode::initClass();
  SoIdleMode::initClass();
  SoOrthoCenterMode::initClass();
  SoPanMode::initClass();
  SoPickMode::initClass();
  SoPitchMode::initClass();
  SoRollMode::initClass();
  SoRotateMode::initClass();
  SoYawMode::initClass();
  SoZoomMode::initClass();
}
