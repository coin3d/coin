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
  \class SoCacheHint SoCacheHint.h Inventor/nodes/SoCacheHint.h
  \brief The SoCacheHint class is a node containing hints about how to cache geometry.
  \ingroup nodes

  The SoCacheHint node is used to set up clues to the rendering
  subsystem about how Coin should cache vertex data.

  Please note that this is an experimental class. The API might change
  a lot before/if it's included in any official Coin release.

  <b>FILE FORMAT/DEFAULTS:</b>
  \code
    CacheHint {
        memValue 0.5
        gfxValue 0.5
    }
  \endcode
*/

/*!
  \var SoSFFloat SoCacheHint::memValue

  Sets the value for main memory usage. Should be a number between 0
  and 1. A higher value will use more memory for caching.  Default
  value is 0.5

*/

/*!
  \var SoSFFloat SoCacheHint::gfxValue

  Sets the value for gfx memory usage. Should be a number between 0
  and 1. A higher value will use more memory for caching.  Default
  value is 0.5

*/

#include <Inventor/nodes/SoCacheHint.h>

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/elements/SoCacheHintElement.h>

#include "nodes/SoSubNodeP.h"

// *************************************************************************

SO_NODE_SOURCE(SoCacheHint);

/*!
  Constructor.
*/
SoCacheHint::SoCacheHint(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoCacheHint);
  SO_NODE_ADD_FIELD(memValue, (0.5f));
  SO_NODE_ADD_FIELD(gfxValue, (0.5f));
}

/*!
  Destructor.
*/
SoCacheHint::~SoCacheHint()
{
}

// doc in super
void
SoCacheHint::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoCacheHint, SO_FROM_COIN_2_4);
  
  SO_ENABLE(SoGLRenderAction, SoCacheHintElement);
}

void
SoCacheHint::doAction(SoAction * action)
{
  SoState * state = action->getState();
  SoCacheHintElement::set(state, this, 
                          this->memValue.getValue(),
                          this->gfxValue.getValue());
}

void
SoCacheHint::GLRender(SoGLRenderAction * action)
{
  SoCacheHint::doAction(action);
}

void
SoCacheHint::callback(SoCallbackAction * action)
{
  // do nothing
  SoNode::callback(action);
}

void
SoCacheHint::pick(SoPickAction * action)
{
  // do nothing
  SoNode::pick(action);
}

void
SoCacheHint::getBoundingBox(SoGetBoundingBoxAction * action)
{
  // do nothing
  SoNode::getBoundingBox(action);
}
