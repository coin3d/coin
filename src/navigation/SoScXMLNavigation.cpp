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

#include <Inventor/navigation/SoScXMLNavigation.h>

/*!
  \class SoScXMLNavigation SoScXMLNavigation.h Inventor/scxml/SoScXMLNavigation.h
  \brief Static class for some static init/cleanup/synchronization functions.

  \ingroup navigation
*/

#include <cassert>
#include "tidbitsp.h"

#include <Inventor/misc/CoinResources.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/navigation/SoScXMLRotateTarget.h>
#include <Inventor/navigation/SoScXMLPanTarget.h>
#include <Inventor/navigation/SoScXMLZoomTarget.h>
#include <Inventor/navigation/SoScXMLDollyTarget.h>
#include <Inventor/navigation/SoScXMLSpinTarget.h>
#include <Inventor/navigation/SoScXMLSeekTarget.h>
#include <Inventor/navigation/SoScXMLMiscTarget.h>
#include <Inventor/navigation/SoScXMLFlightControlTarget.h>
#include <Inventor/navigation/SoScXMLMotionTarget.h>
#include <Inventor/C/threads/mutex.h>
#include "threads/threadsutilp.h"
#include "navigation/common-xml.cpp"
#include "navigation/examiner-xml.cpp"
#include "navigation/plane-xml.cpp"

class SoScXMLNavigation::PImpl {
public:
  static void * syncmutex;
};

void * SoScXMLNavigation::PImpl::syncmutex = NULL;

void
SoScXMLNavigation::initClasses(void)
{
  assert(PImpl::syncmutex == NULL);
  CC_MUTEX_CONSTRUCT(PImpl::syncmutex);

  SoScXMLNavigationTarget::initClass();
  SoScXMLRotateTarget::initClass();
  SoScXMLSpinTarget::initClass();
  SoScXMLPanTarget::initClass();
  SoScXMLZoomTarget::initClass();
  SoScXMLDollyTarget::initClass();
  SoScXMLSeekTarget::initClass();
  SoScXMLMiscTarget::initClass();
  SoScXMLFlightControlTarget::initClass();
  SoScXMLMotionTarget::initClass();

  CoinResources::set("coin:scxml/navigation/common.xml",
                     SbByteBuffer(sizeof(common_xml)-1,&common_xml[0]));

  CoinResources::set("coin:scxml/navigation/examiner.xml",
                     SbByteBuffer(sizeof(examiner_xml)-1,&examiner_xml[0]));

  CoinResources::set("coin:scxml/navigation/plane.xml",
                     SbByteBuffer(sizeof(plane_xml)-1,&plane_xml[0]));

  // launch services
  SoScXMLRotateTarget::constructSingleton();
  SoScXMLPanTarget::constructSingleton();
  SoScXMLZoomTarget::constructSingleton();
  SoScXMLDollyTarget::constructSingleton();
  SoScXMLSpinTarget::constructSingleton();
  SoScXMLSeekTarget::constructSingleton();
  SoScXMLMiscTarget::constructSingleton();
  SoScXMLFlightControlTarget::constructSingleton();
  SoScXMLMotionTarget::constructSingleton();

  coin_atexit(reinterpret_cast<coin_atexit_f*>(SoScXMLNavigation::cleanClasses), CC_ATEXIT_NORMAL);
}

void
SoScXMLNavigation::cleanClasses(void)
{
  SoScXMLMotionTarget::destructSingleton();
  SoScXMLFlightControlTarget::destructSingleton();
  SoScXMLMiscTarget::destructSingleton();
  SoScXMLSeekTarget::destructSingleton();
  SoScXMLSpinTarget::destructSingleton();
  SoScXMLDollyTarget::destructSingleton();
  SoScXMLZoomTarget::destructSingleton();
  SoScXMLPanTarget::destructSingleton();
  SoScXMLRotateTarget::destructSingleton();

  SoScXMLRotateTarget::cleanClass();
  SoScXMLSpinTarget::cleanClass();
  SoScXMLPanTarget::cleanClass();
  SoScXMLDollyTarget::cleanClass();
  SoScXMLZoomTarget::cleanClass();
  SoScXMLSeekTarget::cleanClass();
  SoScXMLMiscTarget::cleanClass();
  SoScXMLFlightControlTarget::cleanClass();
  SoScXMLMotionTarget::cleanClass();
  SoScXMLNavigationTarget::cleanClass();

  CC_MUTEX_DESTRUCT(PImpl::syncmutex);
  PImpl::syncmutex = NULL;
}

void
SoScXMLNavigation::syncLock(void)
{
  CC_MUTEX_LOCK(PImpl::syncmutex);
}

void
SoScXMLNavigation::syncUnlock(void)
{
  CC_MUTEX_UNLOCK(PImpl::syncmutex);
}
