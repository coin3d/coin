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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

/*!
  \class SoActionMethodList Inventor/lists/SoActionMethodList.h
  \brief The SoActionMethodList class contains routines to call for an action,
  one for each node type.

  FIXME: write doc.
*/

#include <Inventor/lists/SoActionMethodList.h>

#include <Inventor/nodes/SoNode.h>

#include <assert.h>

/*!
  The constructor.  The \a parentList argument is the parent action's
  action method list.  It can obviously be NULL for action method lists that
  are not based on inheriting from a parent action.
*/

SoActionMethodList::SoActionMethodList(SoActionMethodList * const parentList)
  : SbPList(0), parent(parentList), numValidTypes(0)
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
  const int index = nodeType.getData();
  if ((*this)[ index ] == NULL)
    this->numValidTypes++;
  (*this)[ index ] = method;
}

/*!
  This method must be called before using the list.  It fills in NULL
  entries with their parents' method.
*/

void
SoActionMethodList::setUp(void)
{
  if (! parent)
    return;

  const int max = parent->getLength();
  for (int i = 0; i < max; i++) {
    if ((*this)[i] == NULL) {
      (*this)[i] = (*parent)[i];
      this->numValidTypes++;
    }
  }
}

/*!
  This method returns the action method for type from the parent node.
*/

SoActionMethod
SoActionMethodList::parentMethod(const SoType nodeType) const
{
  if (!parent) return NULL; // maybe return dummyAction?
  return (*parent)[ nodeType.getData() ];
}

/*!
  This method is just a static dummy method that does nothing.
*/

void
SoActionMethodList::dummyAction(SoAction * const,
				SoNode * const)
{
}
