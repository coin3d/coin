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

#ifndef __SOPICKRAYELEMENT_H__
#define __SOPICKRAYELEMENT_H__

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOPICKRAYELEMENT)
#error "Configuration settings disable this class!"
#endif // COIN_EXCLUDE_SOPICKRAYELEMENT

#include <Inventor/elements/SoElement.h>
#include <Inventor/SbViewVolume.h>

class SoPickRayElement : public SoElement {
  typedef SoElement inherited;

//$ BEGIN TEMPLATE ElementHeader(SoPickRayElement)
public:
  static SoType classTypeId;
  static SoType getClassTypeId(void);
  static void * createInstance(void);
public:
  static int getClassStackIndex(void);
  virtual ~SoPickRayElement(void);

  static void initClass(void);

protected:
  SoPickRayElement(void);
  static int classStackIndex;
//$ END TEMPLATE ElementHeader

public:
  virtual void init(SoState * state);

  virtual SbBool matches(const SoElement * element) const;
  virtual SoElement *copyMatchInfo() const;

  static void set(SoState * const state, const SbViewVolume & volume);
  static const SbViewVolume &get(SoState * const state);

  virtual void print(FILE * file) const;

protected:
  SbViewVolume volume;
};

#endif // !__SOPICKRAYELEMENT_H__
