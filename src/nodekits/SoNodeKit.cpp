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

/*!
  \class SoNodeKit SoNodeKit.h Inventor/nodekits/SoNodeKit.h
  \brief The SoNodeKit class is used to initialize the nodekit classes.
  \ingroup nodekits

  The sole function of SoNodeKit is to be just a placeholder for the
  toplevel initialization code for all nodekit-related classes.

  \sa SoBaseKit
*/

#include <Inventor/nodekits/SoNodeKit.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <Inventor/SbBasic.h>
#include <Inventor/SoDB.h>
#include <Inventor/C/tidbits.h>

#ifdef HAVE_NODEKITS
#include <Inventor/nodekits/SoAppearanceKit.h>
#include <Inventor/nodekits/SoCameraKit.h>
#include <Inventor/nodekits/SoLightKit.h>
#include <Inventor/nodekits/SoNodeKitListPart.h>
#include <Inventor/nodekits/SoSceneKit.h>
#include <Inventor/nodekits/SoShapeKit.h>
#include <Inventor/nodekits/SoWrapperKit.h>
#include <ForeignFiles/SoForeignFileKit.h>
#endif // HAVE_NODEKITS

#include "tidbitsp.h"

static SbBool nodekit_isinitialized = FALSE;

static void nodekit_cleanup(void)
{
  nodekit_isinitialized = FALSE;
}

/*!
  Initialize the nodekit system.

  Note that this method is \e not called implicitly from SoDB::init().
  As a matter of fact, this method calls SoDB::init() itself to make
  sure all the underlying classes for the nodekits classes have
  been initialized.

  This method is also called from within SoInteraction::init(), as the
  interaction functionality in Coin depends on the nodekit classes.
 */
void
SoNodeKit::init(void)
{
  if (nodekit_isinitialized) return;

  if (!SoDB::isInitialized()) SoDB::init();

#ifdef HAVE_NODEKITS
  SoNodeKitListPart::initClass();

  SoBaseKit::initClass();
  SoAppearanceKit::initClass();
  SoCameraKit::initClass();
  SoLightKit::initClass();
  SoSceneKit::initClass();
  SoSeparatorKit::initClass();
  SoShapeKit::initClass();
  SoWrapperKit::initClass();
  SoForeignFileKit::initClass();
#endif // HAVE_NODEKITS

  nodekit_isinitialized = TRUE;
  cc_coin_atexit_static_internal((coin_atexit_f*) nodekit_cleanup);
}
