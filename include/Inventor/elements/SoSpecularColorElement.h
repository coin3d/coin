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

#ifndef _SO_SPECULAR_COLOR_ELEMENT_H_
#define _SO_SPECULAR_COLOR_ELEMENT_H_

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOSPECULARCOLORELEMENT)
#error "Configuration settings disable this class!"
#endif // COIN_EXCLUDE_SOSPECULARCOLORELEMENT

#include <Inventor/elements/SoReplacedElement.h>

class SbColor;

class SoSpecularColorElement : public SoReplacedElement {
  typedef SoReplacedElement inherited;

//$ BEGIN TEMPLATE ElementHeader( SoSpecularColorElement )
public:
  static SoType classTypeId;
  static SoType getClassTypeId(void);
  static void * createInstance(void);
public:
  static int getClassStackIndex(void);
  virtual ~SoSpecularColorElement(void);

  static void initClass(void);
  static void cleanClass(void);

protected:
  SoSpecularColorElement(void);
  static int classStackIndex;
//$ END TEMPLATE ElementHeader

public:
  virtual void init(SoState * state);
  static void set(SoState * const state, SoNode * const node,
		  const int32_t numColors, const SbColor * const colors);  

  static const SoSpecularColorElement * getInstance(SoState *state);

  int32_t getNum() const;
  const SbColor &get(const int index) const;

protected:
  virtual void setElt(const int32_t numColors, const SbColor * const colors);
  int numColors;
  const SbColor *colors;

};

#endif // ! _SO_SPECULAR_COLOR_ELEMENT_H_
