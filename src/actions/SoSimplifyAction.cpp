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
  \class SoSimplifyAction Inventor/include/SoSimplifyAction.h
  \brief The SoSimplifyAction class is the base class for the simplify
  action classes.
*/

#include <Inventor/SbName.h>
#include <Inventor/actions/SoSimplifyAction.h>
#include <Inventor/lists/SoEnabledElementsList.h>

// *************************************************************************

//$ BEGIN TEMPLATE ActionSource( SoSimplifyAction )
//$ BEGIN TEMPLATE ActionClassTypeSource( SoSimplifyAction )

SoType SoSimplifyAction::classTypeId = SoType::badType();

/*!
  Returns the unique type identifier for the classname class.
*/
SoType
SoSimplifyAction::getClassTypeId(void)
{
  return classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoSimplifyAction::getTypeId(void) const
{
  return classTypeId;
}
//$ END TEMPLATE ActionClassTypeSource

#include <assert.h>

// static variables
SoEnabledElementsList * SoSimplifyAction::enabledElements;
SoActionMethodList * SoSimplifyAction::methods;

/*!
  \fn SoSimplifyAction::enabledElements
  FIXME: write doc.
*/

/*!
  \fn SoSimplifyAction::methods
  FIXME: write doc.
*/

/*!
  This method returns the list of enabled elements for the given action class.
*/
const SoEnabledElementsList &
SoSimplifyAction::getEnabledElements(void) const
{
  assert(enabledElements);
  return *enabledElements;
}

/*!
  This method adds a method to be perfomed by the action class on the given
  node type.
*/
void 
SoSimplifyAction::addMethod(const SoType type, SoActionMethod method)
{
  assert(methods);
  methods->addMethod(type, method);
}

/*!
  This method enables an element in the state stack for the action class.
*/
void 
SoSimplifyAction::enableElement(const SoType type, const int stackIndex)
{
  assert(enabledElements);
  enabledElements->enable(type, stackIndex);
}
//$ END TEMPLATE ActionSource

// *************************************************************************

/*!
  This static method is for initializing the static data for the
  SoSimplifyAction class.
*/

void
SoSimplifyAction::initClass(void)
{
//$ BEGIN TEMPLATE InitActionSource( SoSimplifyAction )
  assert(SoSimplifyAction::getClassTypeId() == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoSimplifyAction::classTypeId = 
      SoType::createType(inherited::getClassTypeId(),
                         "SoSimplifyAction");
  enabledElements = new SoEnabledElementsList(inherited::enabledElements);
  methods = new SoActionMethodList(inherited::methods);
//$ END TEMPLATE InitActionSource
}

/*!
  This static method cleans up static data for the SoSimplifyAction class.
*/

void
SoSimplifyAction::cleanClass(void)
{
}

// *************************************************************************

/*!
  A constructor.
*/

SoSimplifyAction::SoSimplifyAction(void)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  The destructor.
*/

SoSimplifyAction::~SoSimplifyAction(void)
{
}

/*!
  This method is called before a scene graph traversal.
*/

void
SoSimplifyAction::beginTraversal(SoNode * /* node */)
{
  assert(0 && "FIXME: not implemented");
}

