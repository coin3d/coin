/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2009 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include "navigation/SoScXMLNavigation.h"

#include "navigation/SoScXMLRotateUtils.h"
#include "navigation/SoScXMLPanUtils.h"
#include "navigation/SoScXMLZoomUtils.h"
#include "navigation/SoScXMLSpinUtils.h"
#include "navigation/SoScXMLSeekUtils.h"
#include "navigation/SoScXMLViewUtils.h"

#include "navigation/examiner-xml.cpp"

#include "misc/CoinResources.h"

void
SoScXMLNavigation::initClasses(void)
{
  SoScXMLNavigationInvoke::initClass();

  SoScXMLRotateInvoke::initClass();
  SoScXMLSetRotate::initClass();
  SoScXMLUpdateRotate::initClass();
  SoScXMLEndRotate::initClass();

  SoScXMLSpinInvoke::initClass();
  SoScXMLSetSpin::initClass();
  SoScXMLUpdateSpin::initClass();
  SoScXMLEndSpin::initClass();

  SoScXMLPanInvoke::initClass();
  SoScXMLSetPan::initClass();
  SoScXMLUpdatePan::initClass();
  SoScXMLEndPan::initClass();
  
  SoScXMLZoomInvoke::initClass();
  SoScXMLSetZoom::initClass();
  SoScXMLUpdateZoom::initClass();
  SoScXMLEndZoom::initClass();
  
  SoScXMLSeekInvoke::initClass();
  SoScXMLSetSeek::initClass();
  SoScXMLUpdateSeek::initClass();
  SoScXMLEndSeek::initClass();

  SoScXMLViewAll::initClass();

  CoinResources::set("coin:scxml/navigation/examiner.xml",
                     reinterpret_cast<const char *>(&(examiner_xml[0])),
                     sizeof(examiner_xml));
}
