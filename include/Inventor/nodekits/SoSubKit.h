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


#define SO_KIT_HEADER(_kitclass_) \
  SO_NODE_HEADER(_kitclass_); \



#define SO_KIT_CATALOG_ENTRY_HEADER(_entry_) \
protected: SoSFNode _entry_; \



#define SO_KIT_SOURCE(_class_) \
  SO_NODE_SOURCE(_class_)



#define SO_KIT_INIT_CLASS(_class_, _parentclass_, _parentname_) \
  do { \
    SO_NODE_INIT_CLASS(_class_, _parentclass_, _parentname_); \
  } while (0)


#if defined(__SOLIB_INTERNAL__)
#define SO_KIT_INTERNAL_INIT_CLASS(_class_) \
  do { \
    SO_NODE_INTERNAL_INIT_CLASS(_class_); \
  } while (0)
#endif // INTERNAL macro definition


#define SO_KIT_CONSTRUCTOR(_class_) \
  do { \
    SO_NODE_CONSTRUCTOR(_class_); \
  } while (0)


#if defined(__SOLIB_INTERNAL__)
#define SO_KIT_INTERNAL_CONSTRUCTOR(_class_) \
  do { \
    SO_NODE_INTERNAL_CONSTRUCTOR(_class_); \
  } while (0)
#endif // INTERNAL macro definition


#define SO_KIT_INIT_INSTANCE()


#endif // !__SOSUBKIT_H__
