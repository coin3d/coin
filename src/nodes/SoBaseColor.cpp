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
  \class SoBaseColor SoBaseColor.h Inventor/nodes/SoBaseColor.h
  \brief The SoBaseColor class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoBaseColor.h>



#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLDiffuseColorElement.h>
#include <Inventor/elements/SoDiffuseColorElement.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoPickAction.h>


/*!
  \var SoMFColor SoBaseColor::rgb
  FIXME: write documentation for field
*/

// *************************************************************************

SO_NODE_SOURCE(SoBaseColor);

/*!
  Constructor.
*/
SoBaseColor::SoBaseColor()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoBaseColor);

  SO_NODE_ADD_FIELD(rgb, (SbColor(0.8f, 0.8f, 0.8f)));
}

/*!
  Destructor.
*/
SoBaseColor::~SoBaseColor()
{
}

/*!
  Does initialization common for all objects of the
  SoBaseColor class. This includes setting up the
  type system, among other things.
*/
void
SoBaseColor::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoBaseColor);

  SO_ENABLE(SoGLRenderAction, SoGLDiffuseColorElement);

  SO_ENABLE(SoCallbackAction, SoDiffuseColorElement);
  SO_ENABLE(SoPickAction, SoDiffuseColorElement);
  SO_ENABLE(SoCallbackAction, SoDiffuseColorElement);
}

/*!
  FIXME: write function documentation
*/
void
SoBaseColor::GLRender(SoGLRenderAction * action)
{
  SoBaseColor::doAction(action);
}

/*!
  FIXME: write doc
*/
void
SoBaseColor::doAction(SoAction *action)
{
  if (!rgb.isIgnored()) {
    SoDiffuseColorElement::set(action->getState(),
                               this,
                               rgb.getNum(),
                               rgb.getValues(0));
  }
}

/*!
  FIXME: write doc
*/
void
SoBaseColor::callback(SoCallbackAction *action)
{
  SoBaseColor::doAction(action);
}

void
SoBaseColor::pick(SoPickAction *action)
{
  SoBaseColor::doAction(action);
}
