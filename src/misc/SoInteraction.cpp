/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoInteraction SoInteraction.h Inventor/SoInteraction.h
  \brief The SoInteraction class takes care of initalizing internal classes.
  \ingroup general

  SoInteraction is present for the sole purpose of providing an
  interface to the initialization methods of the classes in Coin which
  are somehow related to user interaction, like the draggers and
  manipulators.

  It is unlikely that the application programmer should need to worry
  about this class, as SoInteraction::init() is called by the GUI
  specific initialization methods.

 */

#include <Inventor/SoInteraction.h>
#include <Inventor/SoDB.h>
#include <Inventor/draggers/SoDragger.h>
#include <Inventor/manips/SoCenterballManip.h>
#include <Inventor/manips/SoClipPlaneManip.h>
#include <Inventor/manips/SoDirectionalLightManip.h>
#include <Inventor/manips/SoHandleBoxManip.h>
#include <Inventor/manips/SoJackManip.h>
#include <Inventor/manips/SoPointLightManip.h>
#include <Inventor/manips/SoSpotLightManip.h>
#include <Inventor/manips/SoTabBoxManip.h>
#include <Inventor/manips/SoTrackballManip.h>
#include <Inventor/manips/SoTransformBoxManip.h>
#include <Inventor/manips/SoTransformerManip.h>
#include <Inventor/nodekits/SoNodeKit.h>
#include <Inventor/nodes/SoAntiSquish.h>
#include <Inventor/nodes/SoExtSelection.h>
#include <Inventor/nodes/SoSurroundScale.h>


SbBool SoInteraction::isinitialized = FALSE;


/*!
  Calls the initClass() method of these classes: SoAntiSquish,
  SoSelection, SoExtSelection, SoSurroundScale, SoInteractionKit,
  SoDragger, SoClipPlaneManip, SoDirectionalLightManip,
  SoPointLightManip, SoSpotLightManip, SoTransformManip,
  SoCenterballManip, SoHandleBoxManip, SoJackManip, SoTabBoxManip,
  SoTrackballManip, SoTransformBoxManip, SoTransformerManip.

  Note that this method calls SoDB::init() and SoNodeKit::init() to
  make sure all classes that the interaction functionality depends on
  have been initialized.


  Application programmers should usually not have to invoke this
  method directly from application code, as it is indirectly called
  from the GUI-binding libraries' init()-functions.  Only if you are
  using your own GUI-binding (and not one of Systems in Motion's SoQt,
  SoGtk, SoXt, SoWin, etc libraries) do you have to explicitly call
  SoInteraction::init().

  \code
  int main(int argc, char ** argv )
  {
    // SoQt::init() calls SoDB::init(), SoNodeKit::init() and
    // SoInteraction::init().
    QWidget * window = SoQt::init( argv[0] );

    SoSeparator * root = make_scenegraph();
    root->ref();
    /// [... etc ...] ///
  \endcode

 */
void
SoInteraction::init(void)
{
  if (SoInteraction::isinitialized) return;
  SoInteraction::isinitialized = TRUE;

  if (!SoDB::isInitialized()) SoDB::init();
  SoNodeKit::init();


  SoAntiSquish::initClass();
  SoSelection::initClass();
  SoExtSelection::initClass();
  SoSurroundScale::initClass();

  SoInteractionKit::initClass();

  SoDragger::initClass();

  SoClipPlaneManip::initClass();
  SoDirectionalLightManip::initClass();
  SoPointLightManip::initClass();
  SoSpotLightManip::initClass();
  SoTransformManip::initClass();
  SoCenterballManip::initClass();
  SoHandleBoxManip::initClass();
  SoJackManip::initClass();
  SoTabBoxManip::initClass();
  SoTrackballManip::initClass();
  SoTransformBoxManip::initClass();
  SoTransformerManip::initClass();
}
