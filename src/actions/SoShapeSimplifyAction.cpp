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
  \class SoShapeSimplifyAction Inventor/include/SoShapeSimplifyAction.h
  \brief The SoShapeSimplifyAction class replaces complex primitives
  with simplified polygon representations.
*/

#include <Inventor/SbName.h>
#include <Inventor/actions/SoShapeSimplifyAction.h>
#include <Inventor/lists/SoEnabledElementsList.h>
#include <coindefs.h> // COIN_STUB()

// *************************************************************************

//$ BEGIN TEMPLATE ActionSource(SoShapeSimplifyAction)

SoType SoShapeSimplifyAction::classTypeId = SoType::badType();

/*!
  Returns the unique type identifier for the classname class.
*/
SoType
SoShapeSimplifyAction::getClassTypeId(void)
{
  return classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoShapeSimplifyAction::getTypeId(void) const
{
  return classTypeId;
}

#include <assert.h>

// static variables
SoEnabledElementsList * SoShapeSimplifyAction::enabledElements;
SoActionMethodList * SoShapeSimplifyAction::methods;

/*!
  \fn SoShapeSimplifyAction::enabledElements
  FIXME: write doc.
*/

/*!
  \fn SoShapeSimplifyAction::methods
  FIXME: write doc.
*/

/*!
  This method returns the list of enabled elements for the given action class.
*/
const SoEnabledElementsList &
SoShapeSimplifyAction::getEnabledElements(void) const
{
  assert(enabledElements);
  return *enabledElements;
}

/*!
  This method adds a method to be perfomed by the action class on the given
  node type.
*/
void
SoShapeSimplifyAction::addMethod(const SoType type, SoActionMethod method)
{
  assert(methods);
  methods->addMethod(type, method);
}

/*!
  This method enables an element in the state stack for the action class.
*/
void
SoShapeSimplifyAction::enableElement(const SoType type, const int stackIndex)
{
  assert(enabledElements);
  enabledElements->enable(type, stackIndex);
}
//$ END TEMPLATE ActionSource

// *************************************************************************

/*!
  This static method initializes static data for the
  SoShapeSimplifyAction class.
*/

void
SoShapeSimplifyAction::initClass(void)
{
//$ BEGIN TEMPLATE InitActionSource(SoShapeSimplifyAction)
  assert(SoShapeSimplifyAction::getClassTypeId() == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoShapeSimplifyAction::classTypeId =
      SoType::createType(inherited::getClassTypeId(),
                         "SoShapeSimplifyAction");
  enabledElements = new SoEnabledElementsList(inherited::enabledElements);
  methods = new SoActionMethodList(inherited::methods);
//$ END TEMPLATE InitActionSource
}

// *************************************************************************

/*!
  A constructor.
*/

SoShapeSimplifyAction::SoShapeSimplifyAction(void)
{
  COIN_STUB();
}

/*!
  The destructor.
*/

SoShapeSimplifyAction::~SoShapeSimplifyAction(void)
{
  COIN_STUB();
}

/*!
  This method is called at the beginning of the traversal.
*/

void
SoShapeSimplifyAction::beginTraversal(SoNode * /* node */)
{
  COIN_STUB();
}
