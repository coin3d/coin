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
#include <Inventor/actions/SoAction.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

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
SoChildList::SoChildList(SoNode * const parent, const int size)
  : SoNodeList(size)
{
  this->parent = parent;
}

/*!
  A constructor.
*/
SoChildList::SoChildList(SoNode * const parent, const SoChildList & list)
  : SoNodeList(0)
{
  this->parent = parent;
  this->copy(list);
}

/*!
  The destructor.
*/
SoChildList::~SoChildList()
{
  this->truncate(0);
}

/*!
  FIXME: write doc.
*/
void
SoChildList::append(SoNode * const node)
{
  node->addAuditor(this->parent, SoNotRec::PARENT);
  SoNodeList::append(node);
  this->parent->startNotify();
}

/*!
  FIXME: write doc.
*/
void
SoChildList::insert(SoNode * const node, const int addBefore)
{
  assert(addBefore <= this->getLength());
  node->addAuditor(this->parent, SoNotRec::PARENT);
  SoNodeList::insert(node, addBefore);
  this->parent->startNotify();
}

/*!
  FIXME: write doc.
*/
void
SoChildList::remove(const int index)
{
  assert(index < this->getLength());
  this->get(index)->removeAuditor(this->parent, SoNotRec::PARENT);
  SoNodeList::remove(index);
  this->parent->startNotify();
}

/*!
  FIXME: write doc.
*/
void
SoChildList::truncate(const int length)
{
  const int n = this->getLength();
  if (length != n) {
    for (int i = length; i < n; i++) {
      this->get(i)->removeAuditor(this->parent, SoNotRec::PARENT);
    }
    SoNodeList::truncate(length);
    this->parent->startNotify();
  }
}

/*!
  FIXME: write doc.
*/
void
SoChildList::copy(const SoChildList & list)
{
  if (this == &list) return;
  this->truncate(0);
  const int n = list.getLength();
  for (int i = 0; i < n; i++) {
    this->append((SoNode *) list.get(i));
  }
  this->auditors = list.auditors;
  this->parent->startNotify();
}

/*!
  FIXME: write doc.
*/
void
SoChildList::set(const int index, SoNode * const node)
{
  assert(index < this->getLength());
  this->get(index)->removeAuditor(this->parent, SoNotRec::PARENT);
  node->addAuditor(this->parent, SoNotRec::PARENT);
  SoBaseList::set(index, (SoBase *)node);
  this->parent->startNotify();
}

/*!
  FIXME: write doc.
*/
void
SoChildList::traverse(SoAction * const action, const int first, const int last)
{
  for (int i = first; i <= last && !action->hasTerminated(); i++) {
    action->traverse((SoNode *)this->get(i));
  }
}

/*!
  FIXME: write doc.
*/
void
SoChildList::traverse(SoAction * const action)
{
  this->traverse(action, 0, this->getLength() - 1);
}

/*!
  FIXME: write doc.
*/
void
SoChildList::traverse(SoAction * const action, const int index)
{
  this->traverse(action, index, index);
}

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
  if (index >= 0) this->auditors.remove(index);
}
