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

/*!
  \class SoNodeKit SoNodeKit.h Inventor/nodekits/SoNodeKit.h
  \brief The SoNodeKit class is used to initialize the nodekit classes.
  \ingroup nodekits

  The sole function of SoNodeKit is to be just a placeholder for the
  toplevel initialization code for all nodekit-related classes.

  \sa SoBaseKit
*/

#include <Inventor/nodekits/SoNodeKit.h>
#include <Inventor/SoDB.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/nodekits/SoAppearanceKit.h>
#include <Inventor/nodekits/SoCameraKit.h>
#include <Inventor/nodekits/SoLightKit.h>
#include <Inventor/nodekits/SoNodeKitListPart.h>
#include <Inventor/nodekits/SoSceneKit.h>
#include <Inventor/nodekits/SoShapeKit.h>
#include <Inventor/nodekits/SoWrapperKit.h>


SbBool SoNodeKit::isinitialized = FALSE;


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
  if (SoNodeKit::isinitialized) return;
  SoNodeKit::isinitialized = TRUE;

  if (!SoDB::isInitialized()) SoDB::init();

  SoNodeKitListPart::initClass();

  SoBaseKit::initClass();
  SoAppearanceKit::initClass();
  SoCameraKit::initClass();
  SoLightKit::initClass();
  SoSceneKit::initClass();
  SoSeparatorKit::initClass();
  SoShapeKit::initClass();
  SoWrapperKit::initClass();

  SoType type = SoBaseKit::getClassTypeId();
  SoRayPickAction::addMethod(type, SoNode::rayPickS);
}
