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
  \class SoCallbackAction Inventor/actions/SoCallbackAction.h
  \brief The SoCallbackAction class is a traversal action that calls
  callbacks at specific nodes.
*/

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/SbName.h>
#include <Inventor/lists/SoEnabledElementsList.h>

// *************************************************************************

//$ BEGIN TEMPLATE ActionSource( SoCallbackAction )

SoType SoCallbackAction::classTypeId = SoType::badType();

/*!
  Returns the unique type identifier for the classname class.
*/
SoType
SoCallbackAction::getClassTypeId(void)
{
  return classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoCallbackAction::getTypeId(void) const
{
  return classTypeId;
}

// static variables
SoEnabledElementsList * SoCallbackAction::enabledElements;
SoActionMethodList * SoCallbackAction::methods;

/*!
  \fn SoCallbackAction::enabledElements
  FIXME: write doc.
*/

/*!
  \fn SoCallbackAction::methods
  FIXME: write doc.
*/

/*!
  This method returns the list of enabled elements for the given action class.
*/
const SoEnabledElementsList &
SoCallbackAction::getEnabledElements(void) const
{
  return *enabledElements;
}

/*!
  This method adds a method to be perfomed by the action class on the given
  node type.
*/
void 
SoCallbackAction::addMethod(const SoType type, SoActionMethod method)
{
  methods->addMethod(type, method);
}

/*!
  This method enables an element in the state stack for the action class.
*/
void 
SoCallbackAction::enableElement(const SoType type, const int stackIndex)
{
  enabledElements->enable(type, stackIndex);
}
//$ END TEMPLATE ActionSource

// *************************************************************************

/*!
  This static method initializes all the static data for the SoCallbackAction
  class.
*/

void
SoCallbackAction::initClass(
    void )
{
//$ BEGIN TEMPLATE InitActionSource( SoCallbackAction )
  assert(SoCallbackAction::getClassTypeId() == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoCallbackAction::classTypeId = 
      SoType::createType(inherited::getClassTypeId(),
                         "SoCallbackAction");
  enabledElements = new SoEnabledElementsList(inherited::enabledElements);
  methods = new SoActionMethodList(inherited::methods);
//$ END TEMPLATE InitActionSource
}

/*!
  This static method cleans up the static data for the SoCallbackAction class.
*/

void
SoCallbackAction::cleanClass(
    void )
{
}

// *************************************************************************

/*!
  A constructor.
*/

SoCallbackAction::SoCallbackAction(
    void )
{
}

/*!
  The destructor.
*/

SoCallbackAction::~SoCallbackAction(
    void )
{
}

