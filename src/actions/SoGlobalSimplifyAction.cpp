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
  \class SoGlobalSimplifyAction Inventor/include/SoGlobalSimplifyAction.h
  \brief The SoGlobalSimplifyAction class is for globally simplifying the
  geometry of a scene graph, globally.
*/

#include <Inventor/SbName.h>
#include <Inventor/actions/SoGlobalSimplifyAction.h>
#include <Inventor/lists/SoEnabledElementsList.h>

// *************************************************************************

//$ BEGIN TEMPLATE ActionSource(SoGlobalSimplifyAction)

SoType SoGlobalSimplifyAction::classTypeId = SoType::badType();

/*!
  Returns the unique type identifier for the classname class.
*/
SoType
SoGlobalSimplifyAction::getClassTypeId(void)
{
  return classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoGlobalSimplifyAction::getTypeId(void) const
{
  return classTypeId;
}

// static variables
SoEnabledElementsList * SoGlobalSimplifyAction::enabledElements;
SoActionMethodList * SoGlobalSimplifyAction::methods;

/*!
  \fn SoGlobalSimplifyAction::enabledElements
  FIXME: write doc.
*/

/*!
  \fn SoGlobalSimplifyAction::methods
  FIXME: write doc.
*/

/*!
  This method returns the list of enabled elements for the given action class.
*/
const SoEnabledElementsList &
SoGlobalSimplifyAction::getEnabledElements(void) const
{
  return *enabledElements;
}

/*!
  This method adds a method to be perfomed by the action class on the given
  node type.
*/
void 
SoGlobalSimplifyAction::addMethod(const SoType type, SoActionMethod method)
{
  methods->addMethod(type, method);
}

/*!
  This method enables an element in the state stack for the action class.
*/
void 
SoGlobalSimplifyAction::enableElement(const SoType type, const int stackIndex)
{
  enabledElements->enable(type, stackIndex);
}
//$ END TEMPLATE ActionSource

// *************************************************************************

/*!
  This static method initializes the static data for the
  SoGlobalSimplifyAction class.
*/

void
SoGlobalSimplifyAction::initClass(void)
{
//$ BEGIN TEMPLATE InitActionSource(SoGlobalSimplifyAction)
  assert(SoGlobalSimplifyAction::getClassTypeId() == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoGlobalSimplifyAction::classTypeId = 
      SoType::createType(inherited::getClassTypeId(),
                         "SoGlobalSimplifyAction");
  enabledElements = new SoEnabledElementsList(inherited::enabledElements);
  methods = new SoActionMethodList(inherited::methods);
//$ END TEMPLATE InitActionSource
}

/*!
  This static method cleans up the static data for the
  SoGlobalSimplifyAction class.
*/

void
SoGlobalSimplifyAction::cleanClass(void)
{
}

// *************************************************************************

/*!
  A constructor.
*/

SoGlobalSimplifyAction::SoGlobalSimplifyAction(void)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  The destructor.
*/

SoGlobalSimplifyAction::~SoGlobalSimplifyAction(void)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  This method is called at the beginning of the traversal.
*/

void
SoGlobalSimplifyAction::beginTraversal(SoNode * /* node */)
{
  assert(0 && "FIXME: not implemented");
}

