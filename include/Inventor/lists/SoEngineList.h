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

#ifndef _SO_ENGINE_LIST_H_
#define _SO_ENGINE_LIST_H_

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOENGINE)
#error Configuration settings disrespected -- do not include this file!
#endif // COIN_EXCLUDE_SOENGINE

#include <Inventor/lists/SoBaseList.h>

class SoEngine;

class SoEngineList : public SoBaseList {
  typedef SoBaseList inherited;
public:
  SoEngineList(void);
  SoEngineList(const int size);
  SoEngineList(const SoEngineList &l);
  ~SoEngineList(void);
  void append(SoEngine * const ptr);
  SoEngine *operator [](const int i) const;
  SoEngineList &operator =(const SoEngineList &l);
};

#endif // ! _SO_ENGINE_LIST_H_
