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

#ifndef COIN_SOSUBKIT_H
#define COIN_SOSUBKIT_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/nodekits/SoNodekitCatalog.h>

#ifndef COIN_INTERNAL
// Include this header file for better Open Inventor compatibility.
#include <Inventor/nodekits/SoNodeKitListPart.h>
#endif // !COIN_INTERNAL


#define SO_KIT_HEADER(_kitclass_) \
  SO_NODE_HEADER(_kitclass_); \
public: \
  static const SoNodekitCatalog * getClassNodekitCatalog(void); \
  virtual const SoNodekitCatalog * getNodekitCatalog(void) const; \
 \
protected: \
  static const SoNodekitCatalog ** getClassNodekitCatalogPtr(void); \
 \
private: \
  static SoNodekitCatalog * classcatalog; \
  static const SoNodekitCatalog ** parentcatalogptr



#define SO_KIT_CATALOG_ENTRY_HEADER(_entry_) \
protected: SoSFNode _entry_



#define SO_KIT_SOURCE(_class_) \
SoNodekitCatalog * _class_::classcatalog = NULL; \
const SoNodekitCatalog ** _class_::parentcatalogptr = NULL; \
 \
SO_NODE_SOURCE(_class_); \
 \
const SoNodekitCatalog * \
_class_::getClassNodekitCatalog(void) \
{ \
  return _class_::classcatalog; \
} \
 \
const SoNodekitCatalog * \
_class_::getNodekitCatalog(void) const \
{ \
  return _class_::classcatalog; \
} \
 \
const SoNodekitCatalog ** \
_class_::getClassNodekitCatalogPtr(void) \
{ \
  return (const class SoNodekitCatalog **)&_class_::classcatalog; \
}


#define SO_KIT_IS_FIRST_INSTANCE() \
  SO_NODE_IS_FIRST_INSTANCE()


#define SO_KIT_INIT_CLASS(_class_, _parentclass_, _parentname_) \
  do { \
    SO_NODE_INIT_CLASS(_class_, _parentclass_, _parentname_); \
    _class_::parentcatalogptr = _parentclass_::getClassNodekitCatalogPtr(); \
  } while (0)



#define SO_KIT_CONSTRUCTOR(_class_) \
  do { \
    SO_NODE_CONSTRUCTOR(_class_); \
    if (SO_KIT_IS_FIRST_INSTANCE()) { \
      SoType mytype = SoType::fromName(SO__QUOTE(_class_)); \
      _class_::classcatalog = (*_class_::parentcatalogptr)->clone(mytype); \
    } \
  } while (0)



#define SO_KIT_ADD_CATALOG_ENTRY(_part_, _partclass_, _isdefnull_ , _parent_, _sibling_, _ispublic_) \
  do { \
    if (SO_KIT_IS_FIRST_INSTANCE()) { \
      classcatalog->addEntry(SO__QUOTE(_part_), \
                           _partclass_::getClassTypeId(), \
                           _partclass_::getClassTypeId(), \
                           _isdefnull_, \
                           SO__QUOTE(_parent_), \
                           SO__QUOTE(_sibling_), \
                           FALSE, \
                           SoType::badType(), \
                           SoType::badType(), \
                           _ispublic_); \
    } \
    SO_NODE_ADD_FIELD(_part_,(NULL)); \
  } while (0)



#define SO_KIT_ADD_CATALOG_LIST_ENTRY(_part_, _containertype_, _isdefnull_, _parent_, _sibling_, _itemtype_, _ispublic_) \
  do {  \
    if (SO_KIT_IS_FIRST_INSTANCE()) { \
      classcatalog->addEntry(SO__QUOTE(_part_), \
                             SoNodeKitListPart::getClassTypeId(), \
                             SoNodeKitListPart::getClassTypeId(), \
                             _isdefnull_, \
                             SO__QUOTE(_parent_), \
                             SO__QUOTE(_sibling_), \
                             TRUE, \
                             _containertype_::getClassTypeId(), \
                             _itemtype_::getClassTypeId(), \
                             _ispublic_); \
    } \
    SO_NODE_ADD_FIELD(_part_,(NULL)); \
  } while (0)



#define SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(_part_, _class_, _defaultclass_, _isdefnull_, _parent_, _sibling_, _ispublic_) \
  do { \
    if (SO_KIT_IS_FIRST_INSTANCE()) { \
      classcatalog->addEntry(SO__QUOTE(_part_), \
                             _class_::getClassTypeId(), \
                             _defaultclass_::getClassTypeId(), \
                             _isdefnull_, \
                             SO__QUOTE(_parent_), \
                             SO__QUOTE(_sibling_), \
                             FALSE, \
                             SoType::badType(), \
                             SoType::badType(), \
                             _ispublic_); \
    } \
    SO_NODE_ADD_FIELD(_part_,(NULL)); \
  } while (0)



#define SO_KIT_ADD_LIST_ITEM_TYPE(_part_, _listitemtype_) \
  do { \
    if (SO_KIT_IS_FIRST_INSTANCE()) { \
      classcatalog->addListItemType(SO__QUOTE(_part_), \
                                  _listitemtype_::getClassTypeId()); \
    } \
  } while (0)




#define SO_KIT_INIT_INSTANCE() \
    createFieldList(); \
    createDefaultParts()


#endif // !COIN_SOSUBKIT_H
