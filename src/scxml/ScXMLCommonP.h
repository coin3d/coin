#ifndef COIN_SCXMLCOMMONP_H
#define COIN_SCXMLCOMMONP_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <cstring>
#include <vector>
#include <algorithm>

#include "coindefs.h"
#include "SbBasicP.h"

// *************************************************************************

#define SCXML__SET_ATTRIBUTE_VALUE(_ptr_, _name_, _value_)              \
  do {                                                                  \
    if ((_ptr_ != NULL) &&                                              \
        (_ptr_ != this->getXMLAttribute(_name_))) {                     \
      delete [] _ptr_;                                                  \
    }                                                                   \
    _ptr_ = NULL;                                                       \
    if (_value_ != NULL) {                                              \
      if (_value_ == this->getXMLAttribute(_name_)) {                   \
        _ptr_ = const_cast<char *>(_value_);                            \
      } else {                                                          \
        _ptr_ = new char [std::strlen(_value_) + 1];                    \
        std::strcpy(_ptr_, _value_);                                    \
      }                                                                 \
    }                                                                   \
  } while (FALSE)

#define SCXML__CLEAR_STD_VECTOR(_listobj_, _elementtype_)               \
  do {                                                                  \
    std::vector<_elementtype_>::iterator it = _listobj_.begin();        \
    while (it != _listobj_.end()) {                                     \
      delete *it;                                                       \
      ++it;                                                             \
    }                                                                   \
    _listobj_.clear();                                                  \
  } while ( FALSE )


#define SCXML_SINGLE_OBJECT_API_IMPL(classname, objtype, pointer, singular) \
void                                                                    \
classname::SO__CONCAT(set,singular)(objtype * obj)                      \
{                                                                       \
  pointer.reset(obj);                                                   \
  obj->setContainer(this);                                              \
}                                                                       \
                                                                        \
objtype *                                                               \
classname::SO__CONCAT(get,singular)(void) const                         \
{                                                                       \
  return pointer.get();                                                 \
}

#define SCXML_LIST_OBJECT_API_IMPL(classname, objtype, objlist, singular, plural) \
                                                                        \
int                                                                     \
classname::SO__CONCAT(getNum,plural)(void) const                        \
{                                                                       \
  return static_cast<int>(objlist.size());                              \
}                                                                       \
                                                                        \
objtype *                                                               \
classname::SO__CONCAT(get,singular)(int idx) const                      \
{                                                                       \
  assert(idx >= 0 && idx < static_cast<int>(objlist.size()));           \
  return objlist.at(idx);                                               \
}                                                                       \
                                                                        \
void                                                                    \
classname::SO__CONCAT(add,singular)(objtype * obj)                      \
{                                                                       \
  objlist.push_back(obj);                                               \
  obj->setContainer(this);                                              \
}                                                                       \
                                                                        \
void                                                                    \
classname::SO__CONCAT(remove,singular)(objtype * obj)                   \
{                                                                       \
  std::vector<objtype *>::iterator it =                                 \
    std::find(objlist.begin(), objlist.end(), obj);                     \
  assert(it != objlist.end());                                          \
  objlist.erase(it);                                                    \
  obj->setContainer(NULL);                                              \
}                                                                       \
                                                                        \
void                                                                    \
classname::SO__CONCAT(clearAll,plural)(void)                            \
{                                                                       \
  std::vector<objtype *>::iterator it = objlist.begin();                \
  while (it != objlist.end()) {                                         \
    (*it)->setContainer(NULL);                                          \
    ++it;                                                               \
  }                                                                     \
  objlist.clear();                                                      \
}

#endif // !COIN_SCXMLCOMMONP_H
