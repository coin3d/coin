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
  \class SoChildList SoChildList.h Inventor/misc/SoChildList.h
  \brief The SoChildList class is a container for node children.
  \ingroup general

  This class does automatic notification on the parent nodes upon
  adding or removing children.

  Methods for action traversal of the children are also provided.
*/

#include <Inventor/misc/SoChildList.h>
#include <Inventor/actions/SoAction.h>
#include <Inventor/nodes/SoNode.h>



/*!
  Default constructor, sets parent container and initializes a minimal
  list.
*/
SoChildList::SoChildList(SoNode * const parent)
  : SoNodeList()
{
  this->parent = parent;
}

/*!
  Constructor with hint about list size.

  \sa SoNodeList::SoNodeList(const int)
*/
SoChildList::SoChildList(SoNode * const parent, const int size)
  : SoNodeList(size)
{
  this->parent = parent;
}

/*!
  Copy constructor.

  \sa SoNodeList::SoNodeList(const SoNodeList &)
*/
SoChildList::SoChildList(SoNode * const parent, const SoChildList & cl)
  : SoNodeList()
{
  this->parent = parent;
  this->copy(cl);
}

/*!
  Destructor.
*/
SoChildList::~SoChildList()
{
  this->truncate(0);
}

/*!
  Append a new \a node instance as a child of our parent container.

  Automatically notifies parent node and any SoPath instances auditing
  paths with nodes from this list.
*/
void
SoChildList::append(SoNode * const node)
{
  node->addAuditor(this->parent, SoNotRec::PARENT);
  SoNodeList::append(node);

  this->parent->startNotify();

  // Doesn't need to notify SoPath auditors, as adding a new node at
  // _the end_ won't affect any path "passing through" this childlist.
}

/*!
  Insert a new \a node instance as a child of our parent container at
  position \a addbefore.

  Automatically notifies parent node and any SoPath instances auditing
  paths with nodes from this list.
*/
void
SoChildList::insert(SoNode * const node, const int addbefore)
{
  assert(addbefore <= this->getLength());
  node->addAuditor(this->parent, SoNotRec::PARENT);
  SoNodeList::insert(node, addbefore);

  this->parent->startNotify();
  for (int i=0; i < this->auditors.getLength(); i++)
    this->auditors[i]->insertIndex(this->parent, addbefore);
}

/*!
  Remove the child node pointer at \a index.

  Automatically notifies parent node and any SoPath instances auditing
  paths with nodes from this list.
*/
void
SoChildList::remove(const int index)
{
  assert(index >= 0 && index < this->getLength());
  SoNodeList::operator[](index)->removeAuditor(this->parent, SoNotRec::PARENT);
  SoNodeList::remove(index);

  this->parent->startNotify();
  for (int i=0; i < this->auditors.getLength(); i++)
    this->auditors[i]->removeIndex(this->parent, index);
}

/*!
  Overloaded from superclass to handle notification.
*/
void
SoChildList::truncate(const int length)
{
  const int n = this->getLength();
  assert(length >= 0 && length <= n);

  if (length != n) {
    for (int i = length; i < n; i++)
      SoNodeList::operator[](i)->removeAuditor(this->parent, SoNotRec::PARENT);

    SoNodeList::truncate(length);

    this->parent->startNotify();
    for (int k=0; k < this->auditors.getLength(); k++) {
      for (int j=length; j < n; j++) {
        this->auditors[k]->removeIndex(this->parent, j);
      }
    }
  }
}

/*!
  Copy contents of \a cl into this list. Overloaded from superclass to
  handle notification.

  \sa SoBaseList::copy(const SoBaseList &)
*/
void
SoChildList::copy(const SoChildList & cl)
{
  if (this == &cl) return;

  // Call truncate() explicitly here to get the path notification.
  this->truncate(0);
  SoBaseList::copy(cl);

  // Note: we are not copying the path auditor list. This is the
  // correct behavior, so don't try to "fix" it.

  this->parent->startNotify();
}

/*!
  Set the child \a node pointer at \a index in the list. Overloaded
  from superclass to handle notification.
*/
void
SoChildList::set(const int index, SoNode * const node)
{
  assert(index >= 0 && index < this->getLength());
  SoNodeList::operator[](index)->removeAuditor(this->parent, SoNotRec::PARENT);
  SoBaseList::set(index, (SoBase *)node);
  node->addAuditor(this->parent, SoNotRec::PARENT);

  this->parent->startNotify();
  for (int i=0; i < this->auditors.getLength(); i++)
    this->auditors[i]->replaceIndex(this->parent, index, node);
}

/*!
  Traverse child nodes in the list from index \a first up to and
  including index \a last, or until the SoAction::hasTerminated() flag
  of \a action has been set.
*/
void
SoChildList::traverse(SoAction * const action, const int first, const int last)
{
  for (int i = first; i <= last && !action->hasTerminated(); i++)
    action->traverse(SoNodeList::operator[](i));
}

/*!
  Traverse all nodes in the list, invoking their methods for the given
  \a action.
*/
void
SoChildList::traverse(SoAction * const action)
{
  this->traverse(action, 0, this->getLength() - 1);
}

/*!
  Traverse the node at \a index (and possibly its children, if its a
  group node), applying the nodes' method for the given \a action.
*/
void
SoChildList::traverse(SoAction * const action, const int index)
{
  this->traverse(action, index, index);
}

/*!
  Traverse the \a node (and possibly its children, if its a group
  node), applying the nodes' method for the given \a action.
*/
void
SoChildList::traverse(SoAction * const action, SoNode * node)
{
  int idx = this->find(node);
  assert(idx != -1);
  this->traverse(action, idx);
}

/*!
  Notify \a path whenever this list of node children changes.
*/
void
SoChildList::addPathAuditor(SoPath * const path)
{
  this->auditors.append(path);
}

/*!
  Remove \a path as an auditor for our list of node children.
*/
void
SoChildList::removePathAuditor(SoPath * const path)
{
  const int index = this->auditors.find(path);
  assert(index != -1);
  this->auditors.remove(index);
}
