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
