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
  \class SoChildList Inventor/misc/SoChildList.h
  \brief The SoChildList class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/misc/SoChildList.h>
#include <Inventor/nodes/SoNode.h>
#if !defined(COIN_EXCLUDE_SOACTION)
#include <Inventor/actions/SoAction.h>
#endif // !COIN_EXCLUDE_SOACTION

/*!
  A constructor.
*/

SoChildList::SoChildList(SoNode * const parent)
  : SoNodeList()
{
  this->parent = parent;
}

/*!
  A constructor.
*/

SoChildList::SoChildList(SoNode * const parent,
			 const int size)
  : SoNodeList(size)
{
  this->parent = parent;
}

/*!
  A constructor.
*/

SoChildList::SoChildList(SoNode * const parent,
			 const SoChildList & list)
  : SoNodeList(0)
{
  this->parent = parent;
  copy(list);
}

/*!
  The destructor.
*/

SoChildList::~SoChildList()
{
  truncate(0);
}

/*!
  FIXME: write doc.
*/

void
SoChildList::append(SoNode * const node)
{
  SoNodeList::append(node);
  parent->startNotify();
//    node->ref();
}

/*!
  FIXME: write doc.
*/

void
SoChildList::insert(SoNode * const node,
		    const int addBefore)
{
  SoNodeList::insert(node, addBefore);
  parent->startNotify();
//    ptr->ref();
}

/*!
  FIXME: write doc.
*/

void
SoChildList::remove(const int index)
{
//    if ((*this)[which] != NULL) (*this)[which]->unref();
  SoNodeList::remove(index);
  parent->startNotify();
}

/*!
  FIXME: write doc.
*/

void
SoChildList::truncate(const int length)
{
//   for (int i = length; i < getLength(); i++) {
//     if ((*this)[i] != NULL) (*this)[i]->unref();
//   }
  if (length!=this->getLength()) {
    SoNodeList::truncate(length);
    parent->startNotify();
  }
}

/*!
  FIXME: write doc.
*/

void
SoChildList::copy(const SoChildList & list)
{
  truncate(0);
  const int max = list.getLength();
  for (int i = 0; i < max; i++) {
    append((SoNode *) list.get(i));
  }
  this->auditors = list.auditors;
  parent->startNotify();
}

/*!
  FIXME: write doc.
*/

void
SoChildList::set(const int index,
		 SoNode * const node)
{
  SoBaseList::set(index, (SoBase *)node);
  parent->startNotify();
//    node->ref();
}

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc.
*/

void
SoChildList::traverse(SoAction * const action,
		      const int first,
		      const int last)
{
  for (int i = first; i <= last && !action->hasTerminated(); i++) {
    action->traverse((SoNode *)this->get(i));
  }
}

/*!
  FIXME: write doc.
*/

void
SoChildList::traverse(
    SoAction * const action)
{
    traverse(action, 0, getLength() - 1);
}

/*!
  FIXME: write doc.
*/

void
SoChildList::traverse(
    SoAction * const action,
    const int index)
{
    traverse(action, index, index);
}

#endif // !COIN_EXCLUDE_SOACTION

/*!
  FIXME: write doc.
*/

void
SoChildList::addPathAuditor(SoPath * const path)
{
  this->auditors.append(path);
}

/*!
  FIXME: write doc.
*/

void
SoChildList::removePathAuditor(SoPath * const path)
{
  const int index = this->auditors.find(path);
  if (index >= 0)
    this->auditors.remove(index);
}

/*!
  A constructor (copy).  Private to prevent usage.
*/

SoChildList::SoChildList(const SoChildList &)
{
  assert(0); // behaviour not defined.
}
