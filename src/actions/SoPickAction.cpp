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
  \class SoPickAction Inventor/actions/SoPickAction.h
  \brief The SoPickAction class is for picking nodes in a scene graph.
*/

#include <Inventor/actions/SoPickAction.h>
#include <Inventor/actions/SoSubAction.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/lists/SoEnabledElementsList.h>
#include <Inventor/nodes/SoNode.h>

#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/elements/SoDecimationTypeElement.h>
#include <Inventor/elements/SoDecimationPercentageElement.h>
#include <Inventor/elements/SoLazyElement.h>



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
  A constructor.
*/

SoPickAction::SoPickAction(const SbViewportRegion & viewportRegion)
  : vpRegion(viewportRegion),
    cullingEnabled(TRUE)
{
  SO_ACTION_CONSTRUCTOR(SoPickAction);

  static int first = 1;
  if (first) {
    SO_ACTION_ADD_METHOD(SoNode, SoNode::pickS);
  }
}

/*!
  The destructor.
*/

SoPickAction::~SoPickAction(void)
{
}

void
SoPickAction::beginTraversal(SoNode *node)
{
  this->getState()->push();
  SoViewportRegionElement::set(this->getState(), this->vpRegion);
  inherited::beginTraversal(node);
  this->getState()->pop();
}

void
SoPickAction::enableCulling(const SbBool flag)
{
  this->cullingEnabled = flag;
}

SbBool
SoPickAction::isCullingEnabled() const
{
  return this->cullingEnabled;
}
