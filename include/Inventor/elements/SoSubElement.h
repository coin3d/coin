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

#ifndef __SOSUBELEMENT_H__
#define __SOSUBELEMENT_H__

/*
 * NOTES
 *
 * This file contains macro definitions with common declarations and
 * definitions used in Coin elements.
 *
 * During development, actual source code is used through a template system
 * instead of macros, so these macros aren't inserted since they aren't
 * in use yet.
 *
 * FIXME: document macros. 19991209 mortene.
 *
 */

#define SO_ELEMENT_ABSTRACT_HEADER(_class_) \
private: \
  static SoType classTypeId; \
protected: \
  _class_(void); \
  static int classStackIndex; \
public: \
  static SoType getClassTypeId(void); \
  static int getClassStackIndex(void)


#define SO_ELEMENT_HEADER(_class_) \
  SO_ELEMENT_ABSTRACT_HEADER(_class_); \
public: \
  static void * createInstance(void)


#define SO_ELEMENT_ABSTRACT_SOURCE(_class_)
#define SO_ELEMENT_SOURCE(_class_)

#define SO_ELEMENT_INIT_ABSTRACT_CLASS(_class_, _parent_)
#define SO_ELEMENT_INIT_CLASS(_class_, _parent_)

#endif // !__SOSUBELEMENT_H__
