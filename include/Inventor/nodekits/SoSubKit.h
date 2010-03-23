#ifndef COIN_SOSUBKIT_H
#define COIN_SOSUBKIT_H

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

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/nodekits/SoNodekitCatalog.h>
#include <Inventor/C/tidbits.h>

#ifndef COIN_INTERNAL
// Include this header file for better Open Inventor compatibility.
#include <Inventor/nodekits/SoNodeKitListPart.h>
#endif // !COIN_INTERNAL


// FIXME: document all the macros, as they are part of the public
// API. 20011024 mortene.

#define PRIVATE_KIT_HEADER(_kitclass_) \
public: \
  static const SoNodekitCatalog * getClassNodekitCatalog(void); \
  virtual const SoNodekitCatalog * getNodekitCatalog(void) const; \
 \
protected: \
  static const SoNodekitCatalog ** getClassNodekitCatalogPtr(void); \
 \
private: \
  static SoNodekitCatalog * classcatalog; \
  static const SoNodekitCatalog ** parentcatalogptr; \
  static void atexit_cleanupkit(void)

#define SO_KIT_HEADER(_kitclass_) \
  SO_NODE_HEADER(_kitclass_); \
  PRIVATE_KIT_HEADER(_kitclass_)

#define SO_KIT_ABSTRACT_HEADER(_kitclass_) \
  SO_NODE_ABSTRACT_HEADER(_kitclass_); \
  PRIVATE_KIT_HEADER(_kitclass_)

#define SO_KIT_CATALOG_ENTRY_HEADER(_entry_) \
protected: SoSFNode _entry_


#define PRIVATE_KIT_SOURCE(_class_) \
SoNodekitCatalog * _class_::classcatalog = NULL; \
const SoNodekitCatalog ** _class_::parentcatalogptr = NULL; \
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
  return const_cast<const class SoNodekitCatalog **>(&_class_::classcatalog); \
} \
 \
void \
_class_::atexit_cleanupkit(void) \
{ \
   delete _class_::classcatalog; \
  _class_::classcatalog = NULL; \
  _class_::parentcatalogptr = NULL; \
}

#define SO_KIT_SOURCE(_class_) \
SO_NODE_SOURCE(_class_) \
PRIVATE_KIT_SOURCE(_class_)

#define SO_KIT_ABSTRACT_SOURCE(_class_) \
SO_NODE_ABSTRACT_SOURCE(_class_); \
PRIVATE_KIT_SOURCE(_class_)

#define SO_KIT_IS_FIRST_INSTANCE() \
   SO_NODE_IS_FIRST_INSTANCE()

#define SO_KIT_INIT_CLASS(_class_, _parentclass_, _parentname_) \
  do { \
    SO_NODE_INIT_CLASS(_class_, _parentclass_, _parentname_); \
    _class_::parentcatalogptr = _parentclass_::getClassNodekitCatalogPtr(); \
  } while (0)

#define SO_KIT_INIT_ABSTRACT_CLASS(_class_, _parentclass_, _parentname_) \
  do { \
    SO_NODE_INIT_ABSTRACT_CLASS(_class_, _parentclass_, _parentname_); \
    _class_::parentcatalogptr = _parentclass_::getClassNodekitCatalogPtr(); \
  } while (0)


#define SO_KIT_CONSTRUCTOR(_class_) \
  do { \
    SO_NODE_CONSTRUCTOR(_class_); \
    SoBase::staticDataLock(); \
    if (_class_::classcatalog == NULL) { \
      SoType mytype = SoType::fromName(SO__QUOTE(_class_)); \
      _class_::classcatalog = (*_class_::parentcatalogptr)->clone(mytype); \
      cc_coin_atexit_static_internal(_class_::atexit_cleanupkit); \
    } \
    SoBase::staticDataUnlock(); \
  } while (0)



#define SO_KIT_ADD_CATALOG_ENTRY(_part_, _partclass_, _isdefnull_ , _parent_, _sibling_, _ispublic_) \
  do { \
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
    SO_NODE_ADD_FIELD(_part_,(NULL)); \
  } while (0)



#define SO_KIT_ADD_CATALOG_LIST_ENTRY(_part_, _containertype_, _isdefnull_, _parent_, _sibling_, _itemtype_, _ispublic_) \
  do {  \
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
    SO_NODE_ADD_FIELD(_part_,(NULL)); \
  } while (0)



#define SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(_part_, _class_, _defaultclass_, _isdefnull_, _parent_, _sibling_, _ispublic_) \
  do { \
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
    SO_NODE_ADD_FIELD(_part_,(NULL)); \
  } while (0)



#define SO_KIT_ADD_LIST_ITEM_TYPE(_part_, _listitemtype_) \
  do { \
    classcatalog->addListItemType(SO__QUOTE(_part_), \
                                  _listitemtype_::getClassTypeId()); \
  } while (0)


#define SO_KIT_INIT_INSTANCE() \
  this->createFieldList(); \
  this->createDefaultParts()

#define SO_KIT_ADD_FIELD(_fieldname_, _defvalue_) \
  SO_NODE_ADD_FIELD(_fieldname_, _defvalue_)

// New for Coin-3
#define SO_KIT_ADD_EMPTY_MFIELD(_fieldname_) \
  SO_NODE_ADD_EMPTY_MFIELD(_fieldname_)

#define SO_KIT_DEFINE_ENUM_VALUE(_enumtype_, _enumvalue_) \
  SO_NODE_DEFINE_ENUM_VALUE(_enumtype_, _enumvalue_)

#define SO_KIT_SET_MF_ENUM_TYPE(_fieldname_, _enumtype_) \
  SO_NODE_SET_MF_ENUM_TYPE(_fieldname_, _enumtype_)

#define SO_KIT_SET_SF_ENUM_TYPE(_fieldname_, _enumtype_) \
  SO_NODE_SET_SF_ENUM_TYPE(_fieldname_, _enumtype_)

#define SO_KIT_CHANGE_ENTRY_TYPE(_part_, _newpartclassname_, _newdefaultpartclassname_) \
  do { \
    classcatalog->narrowTypes(SO__QUOTE(_part_), \
                              SoType::fromName(SO__QUOTE(_newpartclassname_)), \
                              SoType::fromName(SO__QUOTE(_newdefaultpartclassname_))); \
  } while (0)


#define SO_KIT_CHANGE_NULL_BY_DEFAULT(_part_, _newnullbydefault_) \
  do { \
    classcatalog->setNullByDefault(SO__QUOTE(_part_), _newnullbydefault_); \
  } while (0)

#endif // !COIN_SOSUBKIT_H
