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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

#ifndef _SO_DIFFUSE_COLOR_ELEMENT_H_
#define _SO_DIFFUSE_COLOR_ELEMENT_H_

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SODIFFUSECOLORELEMENT)
#error "Configuration settings disrespected -- do not include this file!"
#endif // COIN_EXCLUDE_SODIFFUSECOLORELEMENT

#include <Inventor/elements/SoReplacedElement.h>

class SbColor;

class SoDiffuseColorElement : public SoReplacedElement {
  typedef SoReplacedElement inherited;

//$ BEGIN TEMPLATE ElementHeader( SoDiffuseColorElement )
public:
  static SoType classTypeId;
  static SoType getClassTypeId(void);
  static void * createInstance(void);
public:
  static int getClassStackIndex(void);
  virtual ~SoDiffuseColorElement(void);

  static void initClass(void);
  static void cleanClass(void);

protected:
  SoDiffuseColorElement(void);
  static int classStackIndex;
//$ END TEMPLATE ElementHeader

public:
  virtual void init(SoState * state);

  static void set(SoState * const state, SoNode * const node,
		  const int32_t numColors, const SbColor * const colors);  

  static void set(SoState * const state, SoNode * const node,
		  const int32_t numColors, const uint32_t * const colors);

  static const SoDiffuseColorElement * getInstance(SoState *state);

  int32_t getNum() const;
  const SbColor &get(const int index) const;
  SbBool isPacked() const;

protected:
  friend class SoMaterialBundle;
  virtual void setElt(const int32_t numColors, const SbColor * const colors);
  virtual void setElt(const int32_t numColors, 
		      const uint32_t * const packedcolors);
  
  int numColors;
  const SbColor *colors;
  const uint32_t *packedColors;

}; // class SoDiffuseColorElement

#endif // ! _SO_DIFFUSE_COLOR_ELEMENT_H_
