/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoState Inventor/misc/SoState.h
  \brief The SoState class is the container class for the Coin
  graph traversal state data.

  The SoState class is used by actions derived from the SoAction class. It
  manages the state data as stacks of elements derived from SoElement.
*/

#include <Inventor/SbName.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/lists/SoTypeList.h>
#include <Inventor/elements/SoElement.h>

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

#endif // DOXYGEN_SKIP_THIS

/*!
  The constructor.  The \a theAction argument is the action object the state
  is part of, and the \a enabledElements argument is an SoTypeList of the
  elements that are enabled for this action.

  The constructor pushes a default element onto the indexes of all the
  enabled element stacks.  SoElement::push() is not called on the initial
  elements in the SoState stacks, but SoElement::init() is.
*/

SoState::SoState(SoAction * const theAction,
                 const SoTypeList & enabledElements)
  : action(theAction), depth(0), cacheopen(FALSE)
{
  int i;

  this->numStacks = SoElement::getNumStackIndices() ;

  this->stack = new SoElement * [this->numStacks];
  this->initial = new SoElement * [this->numStacks];

  for (i = 0; i < this->numStacks; i++) {
    this->initial[i] = NULL;
    this->stack[i] = NULL;
  }

  const int numElements = enabledElements.getLength();
  for (i = 0; i < numElements; i++) {
    SoType type = enabledElements[ i ];
    assert(type.isBad() || type.canCreateInstance());
    if (!type.isBad()) {
      SoElement * const element = (SoElement *) type.createInstance();
      element->setDepth(this->depth);
      const int stackIndex = element->getStackIndex();
      this->stack[ stackIndex ] = element;
      this->initial[ stackIndex ] = element;
      element->init(this); // called for first element in state stack
    }
  }
  this->pushstore = new sostate_pushstore;
}

/*!
  The destructor.

  Note that when destruction happens, lagging events caused by lazy evaluation
  won't be performed.
*/

SoState::~SoState(void)
{
  for (int i = 0; i < this->numStacks; i++) {
    SoElement * elem = this->initial[i];
    SoElement * next;
    while (elem) {
      next = elem->nextup;
      delete elem;
      elem = next;
    }
  }

  delete[] this->initial;
  delete[] this->stack;

  sostate_pushstore * item = this->pushstore;
  while (item->prev) item = item->prev; // go to first item
  while (item) {
    sostate_pushstore * next = item->next;
    delete item;
    item = next;
  }
}

/*!
  This method returns the pointer to the action instance given to the
  constructor.
*/

SoAction *
SoState::getAction(void) const
{
  return this->action;
}

/*!
  This method returns a modifyable instance of the element on the top of
  the stack with the given stackIndex.  Because of lazy programming,
  this function may need to do some work, so SoState::getConstElement()
  should be used instead whenever possible.
*/

SoElement *
SoState::getElement(const int stackIndex)
{
  SoElement * element = this->stack[ stackIndex ];
  assert(element);

#if 0 // debug
  SoDebugError::postInfo("SoState::getElement",
                         "stackIndex: %d, element: %p ('%s'), "
                         "stackdepth: %d, pushstack: %s",
                         stackIndex, element,
                         element->getTypeId().getName().getString(),
                         element->getDepth(),
                         (element->getDepth() < this->depth) ?
                         "yes" : "no");
#endif // debug

  if (element->getDepth() < this->depth) { // create elt of correct depth
    SoElement * next = element->nextup;
    if (! next) { // allocate new element
      next = (SoElement *) element->getTypeId().createInstance();
      next->nextdown = element;
      element->nextup = next;
    }
    next->setDepth(this->depth);
    next->push(this);
    this->stack[ stackIndex ] = next;
    element = next;
    this->pushstore->elements.append(stackIndex);
  }
  assert(element != NULL);
  return element;
}

/*!
  This method returns a pointer to the top element of the given element
  stack.  The element is read-only and must not be changed under any
  circumstances or strange side-effect will occur.
*/

const SoElement *
SoState::getConstElement(const int stackIndex) const
{
  assert(stack[stackIndex]);
  return stack[stackIndex];
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
  if (this->pushstore->next == NULL) {
    sostate_pushstore * store = new sostate_pushstore;
    store->prev = this->pushstore;
    this->pushstore->next = store;
  }
  this->pushstore = this->pushstore->next;
  this->pushstore->elements.truncate(0);
  this->depth++;
}

/*!
  This method pops the state to restore it to a previous state.
  Pops are performed eagerly but the code is very tight so there is
  no reason to worry about efficiency.
*/

void
SoState::pop(void)
{
  this->depth--;
  int n = this->pushstore->elements.getLength();
  if (n) {
    const int * array = this->pushstore->elements.getArrayPtr();
    for (int i = 0; i < n; i++) {
      int idx = array[i];
      SoElement * elem = this->stack[idx];
      SoElement * prev = elem->nextdown;
      assert(prev);
      prev->pop(this, elem);
      this->stack[idx] = prev;
    }
  }
  this->pushstore->elements.truncate(0);
  this->pushstore = this->pushstore->prev;
}

/*!
  This method is just for debugging purposes.
*/

void
SoState::print(FILE * const file) const
{
  fprintf(file, "SoState[%p]: depth = %d\n", this, this->depth);
  fprintf(file, "  enabled elements {\n");
  for (int i = 0; i < this->numStacks; i++) {
    SoElement * element;
    if ((element = this->stack[i]) != NULL)
      fprintf(file, "    %s\n",
               element->getTypeId().getName().getString());
  }
  fprintf(file, "  }\n");
}

/*!
  This method returns TRUE if the element with the given element stack
  index is enabled, and FALSE otherwise.
*/

SbBool
SoState::isElementEnabled(const int stackIndex) const
{
  return (this->stack[ stackIndex ] != NULL);
}

/*!
  This method returns the current depth of the state stack.

  The depth is "virtual", not necessarily physical.
*/

int
SoState::getDepth(void) const
{
  return this->depth;
}

/*!
  Controls whether a cache is open.
*/
void
SoState::setCacheOpen(const SbBool open)
{
  this->cacheopen = open;
}

/*!
  Returns whether a cache is open.
*/
SbBool
SoState::isCacheOpen(void) const
{
  return this->cacheopen;
}

/*!
  This method returns a pointer to a writable element without
  checking for state depth.  Use with care.
*/

SoElement *
SoState::getElementNoPush(const int stackIndex) const
{
  return this->stack[ stackIndex ];
}
