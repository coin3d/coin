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
  \class SoGetPrimitiveCountAction Inventor/actions/SoGetPrimitiveCountAction.h
  \brief The SoGetPrimitiveCountAction class is for getting the number of
  primitives a subgraph consists of.

  FIXME: code MiA. 19990315 mortene.
*/

#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoSubAction.h>
#include <Inventor/SbName.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/lists/SoEnabledElementsList.h>

// *************************************************************************

//$ BEGIN TEMPLATE ActionSource( SoGetPrimitiveCountAction )

SoType SoGetPrimitiveCountAction::classTypeId = SoType::badType();

/*!
  Returns the unique type identifier for the classname class.
*/
SoType
SoGetPrimitiveCountAction::getClassTypeId(void)
{
  return classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoGetPrimitiveCountAction::getTypeId(void) const
{
  return classTypeId;
}

// static variables
SoEnabledElementsList * SoGetPrimitiveCountAction::enabledElements;
SoActionMethodList * SoGetPrimitiveCountAction::methods;

/*!
  \fn SoGetPrimitiveCountAction::enabledElements
  FIXME: write doc.
*/

/*!
  \fn SoGetPrimitiveCountAction::methods
  FIXME: write doc.
*/

/*!
  This method returns the list of enabled elements for the given action class.
*/
const SoEnabledElementsList &
SoGetPrimitiveCountAction::getEnabledElements(void) const
{
  return *enabledElements;
}

/*!
  This method adds a method to be perfomed by the action class on the given
  node type.
*/
void 
SoGetPrimitiveCountAction::addMethod(const SoType type, SoActionMethod method)
{
  methods->addMethod(type, method);
}

/*!
  This method enables an element in the state stack for the action class.
*/
void 
SoGetPrimitiveCountAction::enableElement(const SoType type, const int stackIndex)
{
  enabledElements->enable(type, stackIndex);
}
//$ END TEMPLATE ActionSource

// *************************************************************************

/*!
  This static method initializes the static data for the
  SoGetPrimitiveCountAction class.
*/

void
SoGetPrimitiveCountAction::initClass(
    void )
{
//$ BEGIN TEMPLATE InitActionSource( SoGetPrimitiveCountAction )
  assert(SoGetPrimitiveCountAction::getClassTypeId() == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoGetPrimitiveCountAction::classTypeId = 
      SoType::createType(inherited::getClassTypeId(),
                         "SoGetPrimitiveCountAction");
  enabledElements = new SoEnabledElementsList(inherited::enabledElements);
  methods = new SoActionMethodList(inherited::methods);
//$ END TEMPLATE InitActionSource

  methods->setDefault((void *)SoNode::getPrimitiveCountS);
}

/*!
  This static method cleans up the static data for the
  SoGetPrimitiveCount class.
*/

void
SoGetPrimitiveCountAction::cleanClass(
    void )
{
}

// *************************************************************************

/*!
  A constructor.
*/

SoGetPrimitiveCountAction::SoGetPrimitiveCountAction(void)
{
  SO_ACTION_CONSTRUCTOR(SoGetPrimitiveCountAction);  
}

/*!
  The destructor.
*/

SoGetPrimitiveCountAction::~SoGetPrimitiveCountAction()
{
}

/*!
  This method is called at the beginning of the traversal.
*/

void
SoGetPrimitiveCountAction::beginTraversal(SoNode * node)
{
  this->traverse( node );
}

