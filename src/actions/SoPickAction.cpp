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
  \class SoPickAction SoPickAction.h Inventor/actions/SoPickAction.h
  \brief The SoPickAction class is the base class for picking actions.
  \ingroup actions

  The basis for all interaction features that Coin provides for the
  application programmer is the pick actions. Draggers, manipulators,
  SoSelection nodes, etc all make use of the functionality provided by
  the pick actions for selecting and manipulating scene geometry in
  various ways.

  This class is not supposed to be used directly by the application
  programmer, as it is more a place-holder for the common interface of
  picking operations. It does not contain any actual code for doing
  scenegraph picks.

  Coin provides a fully functional picking action for the application
  programmer to use through the SoRayPickAction class (which inherits
  the SoPickAction class).

  \sa SoSelection
*/

#include <Inventor/actions/SoPickAction.h>
#include <Inventor/actions/SoSubActionP.h>

#include <Inventor/elements/SoDecimationPercentageElement.h>
#include <Inventor/elements/SoDecimationTypeElement.h>
#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/lists/SoEnabledElementsList.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/nodes/SoNode.h>

/*!
  \var SbViewportRegion SoPickAction::vpRegion
  The viewport region used by pick actions.
*/


SO_ACTION_SOURCE(SoPickAction);


// Override from parent class.
void
SoPickAction::initClass(void)
{
  SO_ACTION_INIT_CLASS(SoPickAction, SoAction);

  SO_ENABLE(SoPickAction, SoViewportRegionElement);
  SO_ENABLE(SoPickAction, SoDecimationTypeElement);
  SO_ENABLE(SoPickAction, SoDecimationPercentageElement);
  SO_ENABLE(SoPickAction, SoLazyElement);
  SO_ENABLE(SoPickAction, SoCacheElement);
}


/*!
  Constructor.
*/
SoPickAction::SoPickAction(const SbViewportRegion & viewportregion)
  : vpRegion(viewportregion), cullingenabled(TRUE)
{
  SO_ACTION_CONSTRUCTOR(SoPickAction);
}

/*!
  Destructor.
*/
SoPickAction::~SoPickAction(void)
{
}

/*!
  Set a new viewport region to replace the one passed in with the
  constructor.
*/
void
SoPickAction::setViewportRegion(const SbViewportRegion & newregion)
{
  this->vpRegion = newregion;
}

/*!
  Returns the viewport region used by the action.
 */
const SbViewportRegion &
SoPickAction::getViewportRegion(void)
{
  return this->vpRegion;
}

// Documented in superclass. Overrides parent traversal to set up the
// state element for the viewport region.
void
SoPickAction::beginTraversal(SoNode * node)
{
  this->getState()->push();
  SoViewportRegionElement::set(this->getState(), this->vpRegion);
  inherited::beginTraversal(node);
  this->getState()->pop();
}

/*!
  Don't calculate bounding boxes and try to do culling when picking.

  This can provide a speed-up in cases where the default setting of \c
  on would be inefficient.
*/
void
SoPickAction::enableCulling(const SbBool flag)
{
  this->cullingenabled = flag;
}

/*!
  Returns the current state of the culling flag.
*/
SbBool
SoPickAction::isCullingEnabled(void) const
{
  return this->cullingenabled;
}
