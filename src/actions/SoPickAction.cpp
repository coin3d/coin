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

#include <Inventor/SbName.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/actions/SoSubAction.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/lists/SoEnabledElementsList.h>
#include <Inventor/nodes/SoNode.h>

#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/elements/SoDecimationTypeElement.h>
#include <Inventor/elements/SoDecimationPercentageElement.h>
#include <Inventor/elements/SoLazyElement.h>

// *************************************************************************

//$ BEGIN TEMPLATE ActionSource(SoPickAction)

SoType SoPickAction::classTypeId = SoType::badType();

/*!
  Returns the unique type identifier for the classname class.
*/
SoType
SoPickAction::getClassTypeId(void)
{
  return classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoPickAction::getTypeId(void) const
{
  return classTypeId;
}

#include <assert.h>

// static variables
SoEnabledElementsList * SoPickAction::enabledElements;
SoActionMethodList * SoPickAction::methods;

/*!
  \fn SoPickAction::enabledElements
  FIXME: write doc.
*/

/*!
  \fn SoPickAction::methods
  FIXME: write doc.
*/

/*!
  This method returns the list of enabled elements for the given action class.
*/
const SoEnabledElementsList &
SoPickAction::getEnabledElements(void) const
{
  assert(enabledElements);
  return *enabledElements;
}

/*!
  This method adds a method to be perfomed by the action class on the given
  node type.
*/
void
SoPickAction::addMethod(const SoType type, SoActionMethod method)
{
  assert(methods);
  methods->addMethod(type, method);
}

/*!
  This method enables an element in the state stack for the action class.
*/
void
SoPickAction::enableElement(const SoType type, const int stackIndex)
{
  assert(enabledElements);
  enabledElements->enable(type, stackIndex);
}
//$ END TEMPLATE ActionSource

// *************************************************************************

/*!
  This static method is for initializing static data for the
  SoPickAction class.
*/

void
SoPickAction::initClass(void)
{
//$ BEGIN TEMPLATE InitActionSource(SoPickAction)
  assert(SoPickAction::getClassTypeId() == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoPickAction::classTypeId =
      SoType::createType(inherited::getClassTypeId(),
                         "SoPickAction");
  enabledElements = new SoEnabledElementsList(inherited::enabledElements);
  methods = new SoActionMethodList(inherited::methods);
//$ END TEMPLATE InitActionSource

  ENABLE_ELEMENT(SoViewportRegionElement);
  ENABLE_ELEMENT(SoDecimationTypeElement);
  ENABLE_ELEMENT(SoDecimationPercentageElement);
  ENABLE_ELEMENT(SoLazyElement);
}

// *************************************************************************

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
