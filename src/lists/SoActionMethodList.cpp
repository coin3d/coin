/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoActionMethodList SoActionMethodList.h Inventor/lists/SoActionMethodList.h
  \brief The SoActionMethodList class contains function pointers for action methods.
  \ingroup actions

  An SoActionMethodList contains one function pointer per node
  type. Each action contains an SoActioMethodList to know which
  functions to call during scene graph traversal.
*/

#include <Inventor/lists/SoActionMethodList.h>
#include <Inventor/lists/SoTypeList.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/actions/SoAction.h>
#include <Inventor/nodes/SoNode.h>
#include <assert.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#if COIN_THREADSAFE
#include <Inventor/threads/SbMutex.h>
#endif // COIN_THREADSAFE

#ifndef DOXYGEN_SKIP_THIS

class SoActionMethodListP {
public:
  SoActionMethodList * parent;
  int setupnumtypes;
  SbList <SoType> addedtypes;
  SbList <SoActionMethod> addedmethods;

#ifdef COIN_THREADSAFE
  SbMutex mutex;
#endif // COIN_THREADSAFE
  void lock(void) {
#ifdef COIN_THREADSAFE
    this->mutex.lock();
#endif
  }
  void unlock(void) {
#ifdef COIN_THREADSAFE
    this->mutex.unlock();
#endif
  }
};

#endif // DOXYGEN_SKIP_THIS

#undef THIS
#define THIS this->pimpl

/*!
  The constructor.  The \a parentlist argument is the parent action's
  action method list.  It can be \c NULL for action method lists that
  are not based on inheriting from a parent action.
*/
SoActionMethodList::SoActionMethodList(SoActionMethodList * const parentlist)
{
  THIS = new SoActionMethodListP;
  THIS->parent = parentlist;
  THIS->setupnumtypes = 0;
}

/*!
  Destructor.
*/
SoActionMethodList::~SoActionMethodList()
{
  delete THIS;
}

// Documented in superclass. Overridden from parent to cast from \c
// void pointer.
SoActionMethod &
SoActionMethodList::operator[](const int index)
{
  return (SoActionMethod&)SbPList::operator[](index);
}

/*!
  Add a function pointer to a node type's action method.
*/
void
SoActionMethodList::addMethod(const SoType node, const SoActionMethod method)
{
  assert(node != SoType::badType());
  THIS->lock();
  THIS->addedtypes.append(node);
  THIS->addedmethods.append(method);
  THIS->setupnumtypes = 0; // force a new setUp
  THIS->unlock();
}

/*!
  This method must be called as the last initialization step before
  using the list. It fills in \c NULL entries with the parent's
  method.
*/
void
SoActionMethodList::setUp(void)
{
  THIS->lock();
  if (THIS->setupnumtypes != SoType::getNumTypes()) {
    THIS->setupnumtypes = SoType::getNumTypes();
    this->truncate(0);

    SoTypeList derivedtypes;
    int i, n = THIS->addedtypes.getLength();
    for (i = 0; i < n; i++) {
      SoType type = THIS->addedtypes[i];
      const SoActionMethod method = THIS->addedmethods[i];
      (*this)[(int)type.getData()] = method;
      
      // also set this method for all nodes that inherits this node
      derivedtypes.truncate(0);
      int numderived = SoType::getAllDerivedFrom(THIS->addedtypes[i], derivedtypes);
      for (int j = 0; j < numderived; j++) {
        int idx = (int) derivedtypes[j].getData();
        (*this)[idx] = method;
      }
    }
    
    // fill in nullAction for all nodetypes with method == NULL
    derivedtypes.truncate(0);
    (void) SoType::getAllDerivedFrom(SoNode::getClassTypeId(), derivedtypes);
    
    for (i = 0; i < this->getLength(); i++) {
      if ((*this)[i] == NULL) (*this)[i] = SoAction::nullAction;
    }
    for (i = this->getLength(); i < derivedtypes.getLength(); i++) {
      this->append((void*) SoAction::nullAction);
    }
    
    // fill in empty slots with parent method
    if (THIS->parent) {
      THIS->parent->setUp();
      n = THIS->parent->getLength();
      for (i = 0; i < n; i++) {
        if ((*this)[i] == SoAction::nullAction) (*this)[i] = (*(THIS->parent))[i];
      }
    }
  }
  THIS->unlock();
}

#undef THIS
