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

#ifndef COIN_SOSUBEVENT_H
#define COIN_SOSUBEVENT_H

#include <Inventor/events/SoEvent.h>


#define SO_EVENT_HEADER() \
private: \
  static SoType classTypeId; \
public: \
  static SoType getClassTypeId(void); \
  virtual SoType getTypeId(void) const


#define SO_EVENT_SOURCE(_class_)


#define SO_EVENT_INIT_CLASS(_class_, _parentclass_)

#endif // !COIN_SOSUBEVENT_H
