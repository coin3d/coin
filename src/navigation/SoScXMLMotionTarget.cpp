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

#include <Inventor/navigation/SoScXMLMotionTarget.h>
#include "coindefs.h"

/*!
  \class SoScXMLMotionTarget SoScXMLMotionTarget.h Inventor/navigation/SoScXMLMotionTarget.h
  \brief to be used in parallel with other non-moving targets when parallel states are implemented.

  \ingroup navigation
*/

#include <cassert>

#include <Inventor/navigation/SoScXMLNavigation.h>


SCXML_OBJECT_SOURCE(SoScXMLMotionTarget);

void
SoScXMLMotionTarget::initClass(void)
{
  SCXML_OBJECT_INIT_CLASS(SoScXMLMotionTarget, SoScXMLNavigationTarget, "SoScXMLNavigationTarget");
}

void
SoScXMLMotionTarget::cleanClass(void)
{
  SoScXMLMotionTarget::classTypeId = SoType::badType();
}

SoScXMLMotionTarget * SoScXMLMotionTarget::theSingleton = NULL;

SoScXMLMotionTarget *
SoScXMLMotionTarget::constructSingleton(void)
{
  assert(SoScXMLMotionTarget::theSingleton == NULL);
  SoScXMLMotionTarget::theSingleton =
    static_cast<SoScXMLMotionTarget *>(SoScXMLMotionTarget::classTypeId.createInstance());
  return SoScXMLMotionTarget::theSingleton;
}

void
SoScXMLMotionTarget::destructSingleton(void)
{
  assert(SoScXMLMotionTarget::theSingleton != NULL);
  delete SoScXMLMotionTarget::theSingleton;
  SoScXMLMotionTarget::theSingleton = NULL;
}

SoScXMLMotionTarget *
SoScXMLMotionTarget::singleton(void)
{
  assert(SoScXMLMotionTarget::theSingleton != NULL);
  return SoScXMLMotionTarget::theSingleton;
}

SoScXMLMotionTarget::SoScXMLMotionTarget(void)
{
  this->setEventTargetType(SOSCXML_NAVIGATION_TARGETTYPE);
  this->setEventTargetName("Motion");
}

SoScXMLMotionTarget::~SoScXMLMotionTarget(void)
{
}

SbBool
SoScXMLMotionTarget::processOneEvent(const ScXMLEvent * COIN_UNUSED_ARG(event))
{
  return FALSE;
}
