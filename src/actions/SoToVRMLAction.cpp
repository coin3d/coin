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
  \class SoToVRMLAction Inventor/include/SoToVRMLAction.h
  \brief The SoToVRMLAction class builds a new scene graph using only
  VRML 1.0 nodes.
*/

#include <Inventor/SbName.h>
#include <Inventor/actions/SoToVRMLAction.h>
#include <Inventor/lists/SoEnabledElementsList.h>

// *************************************************************************

//$ BEGIN TEMPLATE ActionSource( SoToVRMLAction )

SoType SoToVRMLAction::classTypeId = SoType::badType();

/*!
  Returns the unique type identifier for the classname class.
*/
SoType
SoToVRMLAction::getClassTypeId(void)
{
  return classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoToVRMLAction::getTypeId(void) const
{
  return classTypeId;
}

// static variables
SoEnabledElementsList * SoToVRMLAction::enabledElements;
SoActionMethodList * SoToVRMLAction::methods;

/*!
  \fn SoToVRMLAction::enabledElements
  FIXME: write doc.
*/

/*!
  \fn SoToVRMLAction::methods
  FIXME: write doc.
*/

/*!
  This method returns the list of enabled elements for the given action class.
*/
const SoEnabledElementsList &
SoToVRMLAction::getEnabledElements(void) const
{
  return *enabledElements;
}

/*!
  This method adds a method to be perfomed by the action class on the given
  node type.
*/
void 
SoToVRMLAction::addMethod(const SoType type, SoActionMethod method)
{
  methods->addMethod(type, method);
}

/*!
  This method enables an element in the state stack for the action class.
*/
void 
SoToVRMLAction::enableElement(const SoType type, const int stackIndex)
{
  enabledElements->enable(type, stackIndex);
}
//$ END TEMPLATE ActionSource

// *************************************************************************

/*!
  This static method initializes static data for the
  SoToVRMLAction class.
*/

void
SoToVRMLAction::initClass(
    void )
{
//$ BEGIN TEMPLATE InitActionSource( SoToVRMLAction )
  assert(SoToVRMLAction::getClassTypeId() == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoToVRMLAction::classTypeId = 
      SoType::createType(inherited::getClassTypeId(),
                         "SoToVRMLAction");
  enabledElements = new SoEnabledElementsList(inherited::enabledElements);
  methods = new SoActionMethodList(inherited::methods);
//$ END TEMPLATE InitActionSource
}

/*!
  This static method cleans up static data for the
  SoToVRMLAction class.
*/

void
SoToVRMLAction::cleanClass(
    void )
{
}

// *************************************************************************

/*!
  A constructor.
*/

SoToVRMLAction::SoToVRMLAction(void)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  The destructor.
*/

SoToVRMLAction::~SoToVRMLAction(
    void )
{
  // FIXME: implement.
}

/*!
  This method is called before the traversal is begun.
*/

void
SoToVRMLAction::beginTraversal( SoNode * node )
{
  assert(0 && "FIXME: not implemented");
}

