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
  \class SoGraphVerifyAction Inventor/actions/SoGraphVerifyAction.h
  \brief The SoGraphVerifyAction is for verifying the data in a scene graph.
*/

#include <Inventor/actions/SoGraphVerifyAction.h>

// *************************************************************************

//$ BEGIN TEMPLATE ActionSource( SoGraphVerifyAction )
//$ BEGIN TEMPLATE ActionClassTypeSource( SoGraphVerifyAction )

SoType SoGraphVerifyAction::classTypeId = SoType::badType();

/*!
  Returns the unique type identifier for the classname class.
*/
SoType
SoGraphVerifyAction::getClassTypeId(void)
{
  return classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoGraphVerifyAction::getTypeId(void) const
{
  return classTypeId;
}
//$ END TEMPLATE ActionClassTypeSource

#include <assert.h>

// static variables
SoEnabledElementsList * SoGraphVerifyAction::enabledElements;
SoActionMethodList * SoGraphVerifyAction::methods;

/*!
  \fn SoGraphVerifyAction::enabledElements
  FIXME: write doc.
*/

/*!
  \fn SoGraphVerifyAction::methods
  FIXME: write doc.
*/

/*!
  This method returns the list of enabled elements for the given action class.
*/
const SoEnabledElementsList &
SoGraphVerifyAction::getEnabledElements(void) const
{
  assert(enabledElements);
  return *enabledElements;
}

/*!
  This method adds a method to be perfomed by the action class on the given
  node type.
*/
void 
SoGraphVerifyAction::addMethod(const SoType type, SoActionMethod method)
{
  assert(methods);
  methods->addMethod(type, method);
}

/*!
  This method enables an element in the state stack for the action class.
*/
void 
SoGraphVerifyAction::enableElement(const SoType type, const int stackIndex)
{
  assert(enabledElements);
  enabledElements->enable(type, stackIndex);
}
//$ END TEMPLATE ActionSource

// *************************************************************************

/*!
  This static method initializes the static data for the
  SoGraphVerifyAction class.
*/

void
SoGraphVerifyAction::initClass(void)
{
}

/*!
  This static method cleans up the static data for the
  SoGraphVerifyAction class.
*/

void
SoGraphVerifyAction::cleanClass(void)
{
}

// *************************************************************************

/*!
  FIXME: write doc.
*/

void
SoGraphVerifyAction::~SoGraphVerifyAction(void)
{
}

// *************************************************************************

