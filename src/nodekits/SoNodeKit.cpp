/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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
  \brief The SoNodeKit class ...
  \ingroup nodekits

  FIXME: write class doc
*/

#include <Inventor/nodekits/SoNodeKit.h>
#include <Inventor/nodekits/SoNodeKitListPart.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodekits/SoAppearanceKit.h>
#include <Inventor/nodekits/SoCameraKit.h>
#include <Inventor/nodekits/SoLightKit.h>
#include <Inventor/nodekits/SoSceneKit.h>
#include <Inventor/nodekits/SoSeparatorKit.h>
#include <Inventor/nodekits/SoShapeKit.h>
#include <Inventor/nodekits/SoWrapperKit.h>

/*!
  FIXME: write function documentation
*/
void
SoNodeKit::init(void)
{
  SoNodeKitListPart::initClass();

  SoBaseKit::initClass();
  SoAppearanceKit::initClass();
  SoCameraKit::initClass();
  SoLightKit::initClass();
  SoSceneKit::initClass();
  SoSeparatorKit::initClass();
  SoShapeKit::initClass();
  SoWrapperKit::initClass();

  // FIXME: other stuff probably missing. 19991106 mortene.
}
