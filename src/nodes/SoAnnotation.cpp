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
  \class SoAnnotation SoAnnotation.h Inventor/nodes/SoAnnotation.h
  \brief The SoAnnotation node draws all its child geometry on top of other geometry.
  \ingroup nodes

  This group-type node uses delayed rendering in combination with
  Z-buffer disabling to let its children transparently render their
  geometry on top of the other geometry in the scene.

  Since the Z-buffer needs to be disabled, the childrens' geometry
  will not be rendered back-to-front sorted, but rather in the order
  they are present in the scenegraph.

  <b>FILE FORMAT/DEFAULTS:</b>
  \code
    Annotation {
        renderCaching AUTO
        boundingBoxCaching AUTO
        renderCulling AUTO
        pickCulling AUTO
    }
    \endcode
*/

// FIXME: consider adding a lazy GL depth buffer element. 200YMMDD pederb.

#include <Inventor/nodes/SoAnnotation.h>

#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/system/gl.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H
#include "nodes/SoSubNodeP.h"


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

// Doc in superclass.
void
SoAnnotation::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoAnnotation, SO_FROM_INVENTOR_1);
}

// Doc in superclass.
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

// Doc in superclass.
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

// Doc in superclass.
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

// Doc in superclass.
void
SoAnnotation::GLRenderOffPath(SoGLRenderAction *)
{
  // should never render, this is a separator node
}

#ifdef COIN_TEST_SUITE

BOOST_AUTO_TEST_CASE(initialized)
{
  SoAnnotation * node = new SoAnnotation;
  assert(node);
  node->ref();
  BOOST_CHECK_MESSAGE(node->getTypeId() != SoType::badType(),
                      "missing class initialization");
  node->unref();
}

#endif // COIN_TEST_SUITE
