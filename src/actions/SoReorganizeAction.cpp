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
  \class SoReorganizeAction Inventor/include/SoReorganizeAction.h
  \brief The SoReorganizeAction class ...
*/

#include <Inventor/SbName.h>
#include <Inventor/actions/SoReorganizeAction.h>
#include <Inventor/lists/SoEnabledElementsList.h>

// *************************************************************************

//$ BEGIN TEMPLATE ActionSource( SoReorganizeAction )
//$ BEGIN TEMPLATE ActionClassTypeSource( SoReorganizeAction )

SoType SoReorganizeAction::classTypeId = SoType::badType();

/*!
  Returns the unique type identifier for the classname class.
*/
SoType
SoReorganizeAction::getClassTypeId(void)
{
  return classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoReorganizeAction::getTypeId(void) const
{
  return classTypeId;
}
//$ END TEMPLATE ActionClassTypeSource

#include <assert.h>

// static variables
SoEnabledElementsList * SoReorganizeAction::enabledElements;
SoActionMethodList * SoReorganizeAction::methods;

/*!
  \fn SoReorganizeAction::enabledElements
  FIXME: write doc.
*/

/*!
  \fn SoReorganizeAction::methods
  FIXME: write doc.
*/

/*!
  This method returns the list of enabled elements for the given action class.
*/
const SoEnabledElementsList &
SoReorganizeAction::getEnabledElements(void) const
{
  assert(enabledElements);
  return *enabledElements;
}

/*!
  This method adds a method to be perfomed by the action class on the given
  node type.
*/
void 
SoReorganizeAction::addMethod(const SoType type, SoActionMethod method)
{
  assert(methods);
  methods->addMethod(type, method);
}

/*!
  This method enables an element in the state stack for the action class.
*/
void 
SoReorganizeAction::enableElement(const SoType type, const int stackIndex)
{
  assert(enabledElements);
  enabledElements->enable(type, stackIndex);
}
//$ END TEMPLATE ActionSource

// *************************************************************************

/*!
  This static method initializes the static data for the
  SoReorganizeAction class.
*/

void
SoReorganizeAction::initClass(void)
{
//$ BEGIN TEMPLATE InitActionSource( SoReorganizeAction )
  assert(SoReorganizeAction::getClassTypeId() == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoReorganizeAction::classTypeId = 
      SoType::createType(inherited::getClassTypeId(),
                         "SoReorganizeAction");
  enabledElements = new SoEnabledElementsList(inherited::enabledElements);
  methods = new SoActionMethodList(inherited::methods);
//$ END TEMPLATE InitActionSource
}

/*!
  This static method cleans up the static data for the
  SoReorganizeAction class.
*/

void
SoReorganizeAction::cleanClass(void)
{
}

// *************************************************************************

/*!
  A constructor.
*/

SoReorganizeAction::SoReorganizeAction(void)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  The destructor.
*/

SoReorganizeAction::~SoReorganizeAction(void)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  This method is called before the graph is traversed.
*/

void
SoReorganizeAction::beginTraversal(SoNode * /* node */)
{
  assert(0 && "FIXME: not implemented");
}

