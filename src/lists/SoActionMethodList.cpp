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
  \class SoActionMethodList Inventor/lists/SoActionMethodList.h
  \brief The SoActionMethodList class contains routines to call for an action,
  one for each node type.

  FIXME: write doc.
*/

#include <Inventor/lists/SoActionMethodList.h>
#include <Inventor/actions/SoAction.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/SbName.h>
#include <assert.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  The constructor.  The \a parentList argument is the parent action's
  action method list.  It can obviously be NULL for action method lists that
  are not based on inheriting from a parent action.
*/

SoActionMethodList::SoActionMethodList(SoActionMethodList * const parentList)
  : SbPList(0), parent(parentList)
{
  this->setDefault((void *)dummyAction);
}

/*!
  This operator is used to get and set methods.  The list will grow
  dynamically as we access methods off the end of the list, and entries
  will be initialized to NULL.
*/

SoActionMethod &
SoActionMethodList::operator [] (const int index)
{
  return ((SoActionMethod &)((*(SbPList *)this)[index]));
}

/*!
  This operator is used to get methods.
*/

const SoActionMethod
SoActionMethodList::operator [] (const int index) const
{
  return ((const SoActionMethod)((*(const SbPList *)this)[index]));
}

/*!
  This method adds a method to the appropriate place in the list.
*/

void
SoActionMethodList::addMethod(const SoType nodeType,
			      const SoActionMethod method)
{
  //
  // I'm a bit unsure about this one. Should I really find all
  // nodes derived from a node when adding methods, and should
  // perhaps the method for the children be overwritten even
  // though method != dummyAction?  pederb 19991029 

  const int index = nodeType.getData();
  (*this)[ index ] = method;
  
  dummyList.truncate(0);
  int n = SoType::getAllDerivedFrom(nodeType, dummyList);
  for (int i = 0; i < n; i++) {
    SoType type = dummyList[i];
    if ((*this)[(int)type.getData()] == dummyAction) 
      (*this)[i] = method;
  }
}

/*!
  This method must be called before using the list.  It fills in NULL
  entries with their parents' method. Also, nodes will inherit method
  from their parent, so parent nodes are also searched for methods.
*/

void
SoActionMethodList::setUp(void)
{
  this->dummyList.clear(); // free memory
  
  if (parent) {
    parent->setUp();
    const int max = parent->getLength();
    for (int i = 0; i < max; i++) {
      if ((*this)[i] == dummyAction) {
	(*this)[i] = (*parent)[i];
      }
    }
  }
}

/*!
  This method is just a static dummy method that does nothing.
*/

void
SoActionMethodList::dummyAction(SoAction *action,
				SoNode *node)
{
#if COIN_DEBUG // debug
  SoDebugError::postInfo("SoActionMethodList::dummyAction",
			 "action: %s, node: %s",
			 action->getTypeId().getName().getString(),
			 node->getTypeId().getName().getString());
#endif // debug
}

