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

#ifndef __SOTEXTOUTLINEENABLEDELEMENT_H__
#define __SOTEXTOUTLINEENABLEDELEMENT_H__

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOTEXTOUTLINEENABLEDELEMENT)
#error "Configuration settings disable this class!"
#endif // COIN_EXCLUDE_SOTEXTOUTLINEENABLEDELEMENT

#include <Inventor/elements/SoInt32Element.h>

class SoTextOutlineEnabledElement : public SoInt32Element {
  typedef SoInt32Element inherited;

//$ BEGIN TEMPLATE ElementHeader(SoTextOutlineEnabledElement)
public:
  static SoType classTypeId;
  static SoType getClassTypeId(void);
  static void * createInstance(void);
public:
  static int getClassStackIndex(void);
  virtual ~SoTextOutlineEnabledElement(void);

  static void initClass(void);
  static void cleanClass(void);

protected:
  SoTextOutlineEnabledElement(void);
  static int classStackIndex;
//$ END TEMPLATE ElementHeader

public:
  virtual void init(SoState * state);

  virtual void push(SoState * state);
  virtual void pop(SoState * state, const SoElement * prevTopElement);

  static void set(SoState * const state, SoNode * const node,
		  const SbBool enabled);
  static void set(SoState * const state, const SbBool enabled);
  static SbBool get(SoState * const state);
  static SbBool getDefault(void);
};

#endif // !__SOTEXTOUTLINEENABLEDELEMENT_H__
