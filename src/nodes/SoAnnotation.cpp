/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoAnnotation SoAnnotation.h Inventor/nodes/SoAnnotation.h
  \brief The SoAnnotation node draws all its children geometry on top of other geometry.
  \ingroup nodes

  FIXME: write class doc
*/

// FIXME: consider adding a lazy GL depth buffer element.

#include <Inventor/nodes/SoAnnotation.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/elements/SoCacheElement.h>

#include <Inventor/actions/SoGLRenderAction.h>
#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#if HAVE_WINDOWS_H
#include <windows.h> // *sigh* needed for gl.h
#endif // HAVE_WINDOWS_H
#include <GL/gl.h>


SO_NODE_SOURCE(SoAnnotation);


/*!
  Constructor.
*/
SoAnnotation::SoAnnotation()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoAnnotation);
}

/*!
  Destructor.
*/
SoAnnotation::~SoAnnotation()
{
}

/*!
  Does initialization common for all objects of the
  SoAnnotation class. This includes setting up the
  type system, among other things.
*/
void
SoAnnotation::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoAnnotation);
}

/*!
  FIXME: write function documentation
*/
void
SoAnnotation::GLRender(SoGLRenderAction * action)
{
  switch (action->getCurPathCode()) {
  case SoAction::NO_PATH:
  case SoAction::BELOW_PATH:
    this->GLRenderBelowPath(action);
    break;
  case SoAction::OFF_PATH:
    // do nothing. Separator will reset state.
    break;
  case SoAction::IN_PATH:
    this->GLRenderInPath(action);
    break;
  }
}

/*!
  FIXME: write doc
*/
void
SoAnnotation::GLRenderBelowPath(SoGLRenderAction * action)
{
  if (action->isRenderingDelayedPaths()) {
    SbBool zbenabled = glIsEnabled(GL_DEPTH_TEST);
    if (zbenabled) glDisable(GL_DEPTH_TEST);
    inherited::GLRenderBelowPath(action);
    if (zbenabled) glEnable(GL_DEPTH_TEST);
  }
  else {
    SoCacheElement::invalidate(action->getState());
    action->addDelayedPath(action->getCurPath()->copy());
  }
}

/*!
  FIXME: write doc
*/
void
SoAnnotation::GLRenderInPath(SoGLRenderAction * action)
{
  if (action->isRenderingDelayedPaths()) {
    SbBool zbenabled = glIsEnabled(GL_DEPTH_TEST);
    if (zbenabled) glDisable(GL_DEPTH_TEST);
    inherited::GLRenderInPath(action);
    if (zbenabled) glEnable(GL_DEPTH_TEST);
  }
  else {
    SoCacheElement::invalidate(action->getState());
    action->addDelayedPath(action->getCurPath()->copy());
  }
}

/*!
  FIXME: write doc
*/
void
SoAnnotation::GLRenderOffPath(SoGLRenderAction *)
{
  // should never render, this is a separator node
}
