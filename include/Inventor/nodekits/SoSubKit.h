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

#ifndef __SOSUBKIT_H__
#define __SOSUBKIT_H__

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/nodekits/SoNodekitCatalog.h>


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
  static const SoNodekitCatalog ** parentcatalogptr; \

  

#define SO_KIT_CATALOG_ENTRY_HEADER(_entry_) \
protected: SoSFNode _entry_; \



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
} \


#define SO_KIT_IS_FIRST_INSTANCE() \
  SO_NODE_IS_FIRST_INSTANCE()


#define SO_KIT_INIT_CLASS(_class_, _parentclass_, _parentname_) \
  do { \
    SO_NODE_INIT_CLASS(_class_, _parentclass_, _parentname_); \
  } while (0)


#if defined(__SOLIB_INTERNAL__)
#define SO_KIT_INTERNAL_INIT_CLASS(_class_) \
  do { \
    SO_NODE_INTERNAL_INIT_CLASS(_class_); \
    _class_::parentcatalogptr = inherited::getClassNodekitCatalogPtr(); \
  } while (0)
#endif // INTERNAL macro definition


#define SO_KIT_CONSTRUCTOR(_class_) \
  do { \
    SO_NODE_CONSTRUCTOR(_class_); \
    if (SO_KIT_IS_FIRST_INSTANCE()) { \
      SoType mytype = SoType::fromName(SO__QUOTE(_class_)); \
      _class_::classcatalog = (*_class_::parentcatalogptr)->clone(mytype); \
    } \
  } while (0)


#if defined(__SOLIB_INTERNAL__)
#define SO_KIT_INTERNAL_CONSTRUCTOR(_class_) \
  do { \
    SO_NODE_INTERNAL_CONSTRUCTOR(_class_); \
    if (SO_KIT_IS_FIRST_INSTANCE()) { \
      SoType mytype = SoType::fromName(SO__QUOTE(_class_)); \
      _class_::classcatalog = (*_class_::parentcatalogptr)->clone(mytype); \
    } \
  } while (0)
#endif // INTERNAL macro definition


#define SO_KIT_INIT_INSTANCE()



#endif // !__SOSUBKIT_H__
