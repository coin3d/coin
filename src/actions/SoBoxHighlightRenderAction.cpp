/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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
  \class SoBoxHighlightRenderAction Inventor/include/actions/SoBoxHighlightRenderAction.h
  \brief The SoBoxHighlightRenderAction class is a render action class for highlighting selections.
*/

#include <Inventor/SbName.h>
#include <Inventor/actions/SoBoxHighlightRenderAction.h>
#include <Inventor/lists/SoEnabledElementsList.h>

// *************************************************************************

//$ BEGIN TEMPLATE ActionSource(SoBoxHighlightRenderAction)

SoType SoBoxHighlightRenderAction::classTypeId = SoType::badType();

/*!
  Returns the unique type identifier for the classname class.
*/
SoType
SoBoxHighlightRenderAction::getClassTypeId(void)
{
  return classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoBoxHighlightRenderAction::getTypeId(void) const
{
  return classTypeId;
}

// static variables
SoEnabledElementsList * SoBoxHighlightRenderAction::enabledElements;
SoActionMethodList * SoBoxHighlightRenderAction::methods;

/*!
  \fn SoBoxHighlightRenderAction::enabledElements
  FIXME: write doc.
*/

/*!
  \fn SoBoxHighlightRenderAction::methods
  FIXME: write doc.
*/

/*!
  This method returns the list of enabled elements for the given action class.
*/
const SoEnabledElementsList &
SoBoxHighlightRenderAction::getEnabledElements(void) const
{
  return *enabledElements;
}

/*!
  This method adds a method to be perfomed by the action class on the given
  node type.
*/
void 
SoBoxHighlightRenderAction::addMethod(const SoType type, SoActionMethod method)
{
  methods->addMethod(type, method);
}

/*!
  This method enables an element in the state stack for the action class.
*/
void 
SoBoxHighlightRenderAction::enableElement(const SoType type, const int stackIndex)
{
  enabledElements->enable(type, stackIndex);
}
//$ END TEMPLATE ActionSource

// *************************************************************************

/*!
  This method initializes static data for the SoBoxHightlightRenderAction
  class.
*/

void
SoBoxHighlightRenderAction::initClass(void)
{
//$ BEGIN TEMPLATE InitActionSource(SoBoxHighlightRenderAction)
  assert(SoBoxHighlightRenderAction::getClassTypeId() == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoBoxHighlightRenderAction::classTypeId = 
      SoType::createType(inherited::getClassTypeId(),
                         "SoBoxHighlightRenderAction");
  enabledElements = new SoEnabledElementsList(inherited::enabledElements);
  methods = new SoActionMethodList(inherited::methods);
//$ END TEMPLATE InitActionSource
}

/*!
  This method cleans up the static data for the SoBoxHightlightRenderAction
  class.
*/

void
SoBoxHighlightRenderAction::cleanClass(void)
{
}

// *************************************************************************

/*!
  A constructor.
*/

SoBoxHighlightRenderAction::SoBoxHighlightRenderAction(void)
  : inherited(SbViewportRegion())
{
  assert(0 && "FIXME: not implemented");
}

/*!
  A constructor.
*/

SoBoxHighlightRenderAction::SoBoxHighlightRenderAction(const SbViewportRegion & viewportRegion)
  : inherited(viewportRegion)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  The destructor.
*/

SoBoxHighlightRenderAction::~SoBoxHighlightRenderAction(void)
{
  assert(0 && "FIXME: not implemented");
}

