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
  : SbPList(0), parent(parentlist)
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
  Overloaded from parent to cast from \c void *.
*/
const SoActionMethod
SoActionMethodList::operator[](const int index) const
{
  return (const SoActionMethod)SbPList::operator[](index);
}

/*!
  Add a function pointer to a node type's action method.
*/
void
SoActionMethodList::addMethod(const SoType node, const SoActionMethod method)
{
  // I'm a bit unsure about this one. Should I really find all nodes
  // derived from a node when adding methods, and should perhaps the
  // method for the children be overwritten even though method !=
  // NULL?  pederb 19991029

  const int index = node.getData();
  (*this)[index] = method;

  SoTypeList derivedtypes;
  int n = SoType::getAllDerivedFrom(node, derivedtypes);
  for (int i = 0; i < n; i++) {
    SoType type = derivedtypes[i];
    if ((*this)[(int)type.getData()] == NULL) {
#if 0 // debug
      {
        SbString methodname;
        if (method == SoNode::pickS) methodname = "pickS";
        else if (method == SoNode::rayPickS) methodname = "rayPickS";
        else methodname.sprintf("%p", method);

        SoDebugError::postInfo("SoActionMethodList::addMethod",
                               "(listp %p) ``%s'' overloaded with %s",
                               this, type.getName().getString(),
                               methodname.getString());
      }
#endif // debug
      (*this)[(int)type.getData()] = method;
    }
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
  if (this->parent) {
    this->parent->setUp();
    const int max = this->parent->getLength();
    for (int i = 0; i < max; i++) {
      if ((*this)[i] == NULL) (*this)[i] = (*(this->parent))[i];
    }
  }
}
