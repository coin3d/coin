/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
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

#include <Inventor/nodes/SoAnnotation.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/actions/SoGLRenderAction.h>
#ifdef _WIN32
#include <windows.h>
#endif // _WIN32
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
  if (action->isRenderingDelayedPaths()) {
    inherited::GLRender(action);
  }
  else {
    action->addDelayedPath((SoPath*)action->getCurPath());
  }
}

/*!
  FIXME: write doc
*/
void
SoAnnotation::GLRenderBelowPath(SoGLRenderAction * action)
{
  if (action->isRenderingDelayedPaths()) {
    inherited::GLRenderBelowPath(action);
  }
  else {
    action->addDelayedPath((SoPath*)action->getCurPath());
  }
}

/*!
  FIXME: write doc
*/
void
SoAnnotation::GLRenderInPath(SoGLRenderAction * action)
{
  if (action->isRenderingDelayedPaths()) {
    inherited::GLRenderInPath(action);
  }
}

/*!
  FIXME: write doc
*/
void
SoAnnotation::GLRenderOffPath(SoGLRenderAction *)
{
  // should never render
}
