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
  \class SoPickAction SoPickAction.h Inventor/actions/SoPickAction.h
  \brief The SoPickAction class is the base class for picking actions.
  \ingroup actions

  The basis for all interaction features that Coin provides for the
  application programmer is the pick actions. Draggers, manipulators,
  SoSelection nodes, etc all make use of the functionality provided by
  the pick actions for selecting and manipulating scene geometry in
  various ways.

  This class is not supposed to be used directly by the application
  programmer, see SoRayPickAction.

  \sa SoSelection
*/

#include <Inventor/actions/SoPickAction.h>
#include <Inventor/actions/SoSubActionP.h>

#include <Inventor/elements/SoDecimationPercentageElement.h>
#include <Inventor/elements/SoDecimationTypeElement.h>
#include <Inventor/elements/SoLazyElement.h>
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
}


/*!
  Constructor.
*/
SoPickAction::SoPickAction(const SbViewportRegion & viewportregion)
  : vpRegion(viewportregion), cullingenabled(TRUE)
{
  SO_ACTION_CONSTRUCTOR(SoPickAction);

  SO_ACTION_ADD_METHOD_INTERNAL(SoNode, SoNode::pickS);
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

/*!
  Overloads parent traversal to set up the state element for the
  viewport region.
*/
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
