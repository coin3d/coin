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
  \class SoActionMethodList SoActionMethodList.h Inventor/lists/SoActionMethodList.h
  \brief The SoActionMethodList class contains function pointers for action methods.
  \ingroup actions

  An SoActionMethodList contains one function pointer per node
  type. Each action contains an SoActioMethodList to know which
  functions to call during scene graph traversal.
*/

#include <Inventor/lists/SoActionMethodList.h>
#include <Inventor/lists/SoTypeList.h>
#include <assert.h>


/*!
  The constructor.  The \a parentlist argument is the parent action's
  action method list.  It can be \c NULL for action method lists that
  are not based on inheriting from a parent action.
*/
SoActionMethodList::SoActionMethodList(SoActionMethodList * const parentlist)
  : SbPList(0), parent(parentlist), setupNumTypes(0)
{
}

/*!
  Overloaded from parent to cast from \c void pointer.
*/
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
  (*this)[(int)node.getData()] = method;
  
  SoTypeList derivedtypes;
  const int n = SoType::getAllDerivedFrom(node, derivedtypes);
  for (int i = 0; i < n; i++) {
    int idx = (int) derivedtypes[i].getData();
    if (idx >= this->getLength() || (*this)[idx] == NULL) 
      (*this)[idx] = method;
  }
}

/*!
  This method must be called as the last initialization step before
  using the list. It fills in \c NULL entries with the parent's
  method.
*/
void
SoActionMethodList::setUp(void)
{
  if (this->setupNumTypes != SoType::getNumTypes()) {
    this->setupNumTypes = SoType::getNumTypes();
    if (this->parent) {
      this->parent->setUp();
      const int max = this->parent->getLength();
      for (int i = 0; i < max; i++) {
        if ((*this)[i] == NULL) (*this)[i] = (*(this->parent))[i];
      }
    }
  }
}
