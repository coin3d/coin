/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoState Inventor/misc/SoState.h
  \brief The SoState class manages the Coin scenegraph traversal state data.
  \ingroup general

  The SoState class is used by actions derived from the SoAction
  class. It manages the scenegraph state as stacks of elements (ie
  instances of classes derived from SoElement).

  For more information on the inner workings of traversal states in
  Coin, we recommend the book «The Inventor Toolmaker» (ISBN
  0-201-62493-1), also available at SGI's <a
  href="http://techpubs.sgi.com/library">online library</a>. Search
  for "Toolmaker".
*/
// FIXME: should link to example(s) sourcecode extending the library
// by setting up new elements and/or actions. 20010716 mortene.

/*!
  \fn const SoElement * SoState::getConstElement(const int stackIndex) const

  This method returns a pointer to the top element of the given element
  stack.  The element is read-only and must not be changed under any
  circumstances or strange side-effect will occur.
*/

/*!
  \fn SbBool SoState::isCacheOpen(void) const

  Returns whether a cache is open.
*/

/*!
  \fn SbBool SoState::isElementEnabled(const int stackindex) const

  This method returns TRUE if the element with the given element stack
  index is enabled, and FALSE otherwise.
*/

/*!
  SoElement * SoState::getElementNoPush(const int stackindex) const

  This method returns a pointer to a writable element without
  checking for state depth.  Use with care.
*/

#include <Inventor/misc/SoState.h>

#include <Inventor/SbName.h>
#include <Inventor/elements/SoElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/lists/SoTypeList.h>
#include <Inventor/misc/SoGL.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#include <Inventor/system/gl.h> // for glGetError() debugging


#ifndef DOXYGEN_SKIP_THIS

// Internal class used to store which elements are pushed for a depth.
// This makes it possible to avoid searching through all elements
// and testing depth in pop().
class sostate_pushstore {
public:
  sostate_pushstore(void) {
    this->next = this->prev = NULL;
  }
  SbList <int> elements;
  sostate_pushstore * next;
  sostate_pushstore * prev;
};

// class to store private data members
class SoStateP {
public:
  SoAction * action;
  SoElement ** initial;
  int depth;
  class sostate_pushstore * pushstore;
};

#endif // DOXYGEN_SKIP_THIS

#undef THIS
#define THIS this->pimpl

/*!
  The constructor.  The \a theAction argument is the action object the state
  is part of, and the \a enabledElements argument is an SoTypeList of the
  elements that are enabled for this action.

  The constructor pushes a default element onto the indexes of all the
  enabled element stacks.  SoElement::push() is not called on the initial
  elements in the SoState stacks, but SoElement::init() is.
*/

SoState::SoState(SoAction * theAction, const SoTypeList & enabledelements)
{
  THIS = new SoStateP;

  THIS->action = theAction;
  THIS->depth = 0;
  this->cacheopen = FALSE;

  int i;

  this->numstacks = SoElement::getNumStackIndices() ;

  // the stack member can be accessed from inline methods, and is
  // therefore not moved to the private class.
  this->stack = new SoElement * [this->numstacks];
  THIS->initial = new SoElement * [this->numstacks];

  for (i = 0; i < this->numstacks; i++) {
    THIS->initial[i] = NULL;
    this->stack[i] = NULL;
  }

  const int numelements = enabledelements.getLength();
  for (i = 0; i < numelements; i++) {
    SoType type = enabledelements[i];
    assert(type.isBad() || type.canCreateInstance());
    if (!type.isBad()) {
      SoElement * const element = (SoElement *) type.createInstance();
      element->setDepth(THIS->depth);
      const int stackindex = element->getStackIndex();
      this->stack[stackindex] = element;
      THIS->initial[stackindex] = element;
      element->init(this); // called for first element in state stack
    }
  }
  THIS->pushstore = new sostate_pushstore;
}

/*!
  The destructor.

  Note that when destruction happens, lagging events caused by lazy evaluation
  won't be performed.
*/

SoState::~SoState(void)
{
  for (int i = 0; i < this->numstacks; i++) {
    SoElement * elem = THIS->initial[i];
    SoElement * next;
    while (elem) {
      next = elem->nextup;
      delete elem;
      elem = next;
    }
  }

  delete[] THIS->initial;
  delete[] this->stack;

  sostate_pushstore * item = THIS->pushstore;
  while (item->prev) item = item->prev; // go to first item
  while (item) {
    sostate_pushstore * next = item->next;
    delete item;
    item = next;
  }
  delete THIS;
}

/*!
  This method returns the pointer to the action instance given to the
  constructor.
*/

SoAction *
SoState::getAction(void) const
{
  return THIS->action;
}

/*!
  This method returns a modifyable instance of the element on the top of
  the stack with the given stackindex.  Because of lazy programming,
  this function may need to do some work, so SoState::getConstElement()
  should be used instead whenever possible.
*/

SoElement *
SoState::getElement(const int stackindex)
{
  if (!this->isElementEnabled(stackindex)) return NULL;
  SoElement * element = this->stack[stackindex];

#if 0 // debug
  SoDebugError::postInfo("SoState::getElement",
                         "stackindex: %d, element: %p ('%s'), "
                         "stackdepth: %d, pushstack: %s",
                         stackindex, element,
                         element->getTypeId().getName().getString(),
                         element->getDepth(),
                         (element->getDepth() < THIS->depth) ?
                         "yes" : "no");
#endif // debug

  if (element->getDepth() < THIS->depth) { // create elt of correct depth
    SoElement * next = element->nextup;
    if (! next) { // allocate new element
      next = (SoElement *) element->getTypeId().createInstance();
      next->nextdown = element;
      element->nextup = next;
    }
    next->setDepth(THIS->depth);
    next->push(this);
    this->stack[stackindex] = next;
    element = next;
    THIS->pushstore->elements.append(stackindex);
  }
  return element;
}

/*!
  This method pushes the state one level down.  This saves the state so it can
  be changed and later restored to this state by calling SoState::pop().

  The push and pop mechanism is performed lazily for efficiency reasons (avoids
  a lot of memory allocation and copying).  Only when a state element is
  actually going to be changed, that element will be pushed for real.
*/

void
SoState::push(void)
{
  if (THIS->pushstore->next == NULL) {
    sostate_pushstore * store = new sostate_pushstore;
    store->prev = THIS->pushstore;
    THIS->pushstore->next = store;
  }
  THIS->pushstore = THIS->pushstore->next;
  THIS->pushstore->elements.truncate(0);
  THIS->depth++;
}

/*!
  This method pops the state to restore it to a previous state.
  Pops are performed eagerly but the code is very tight so there is
  no reason to worry about efficiency.
*/

void
SoState::pop(void)
{
  THIS->depth--;
  int n = THIS->pushstore->elements.getLength();
  if (n) {
    const int * array = THIS->pushstore->elements.getArrayPtr();
    for (int i = 0; i < n; i++) {
      int idx = array[i];
      SoElement * elem = this->stack[idx];
      SoElement * prev = elem->nextdown;
      assert(prev);
      prev->pop(this, elem);
      this->stack[idx] = prev;
    }
  }
  THIS->pushstore->elements.truncate(0);
  THIS->pushstore = THIS->pushstore->prev;
}

/*!
  This method is just for debugging purposes.
*/

void
SoState::print(FILE * const file) const
{
  fprintf(file, "SoState[%p]: depth = %d\n", this, THIS->depth);
  fprintf(file, "  enabled elements {\n");
  for (int i = 0; i < this->numstacks; i++) {
    SoElement * element;
    if ((element = this->stack[i]) != NULL)
      fprintf(file, "    %s\n",
               element->getTypeId().getName().getString());
  }
  fprintf(file, "  }\n");
}

/*!
  This method returns the current depth of the state stack.

  The depth is "virtual", not necessarily physical.
*/

int
SoState::getDepth(void) const
{
  return THIS->depth;
}

/*!
  Controls whether a cache is open.
*/
void
SoState::setCacheOpen(const SbBool open)
{
  this->cacheopen = open;
}

#undef THIS
