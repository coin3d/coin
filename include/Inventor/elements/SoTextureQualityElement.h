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

#ifndef _SO_TEXTURE_QUALITY_ELEMENT_H_
#define _SO_TEXTURE_QUALITY_ELEMENT_H_

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOTEXTUREQUALITYELEMENT)
#error "Configuration settings disable this class!"
#endif // COIN_EXCLUDE_SOTEXTUREQUALITYELEMENT

#include <Inventor/elements/SoFloatElement.h>

class SoTextureQualityElement : public SoFloatElement {
  typedef SoFloatElement inherited;

//$ BEGIN TEMPLATE ElementHeader( SoTextureQualityElement )
public:
  static SoType classTypeId;
  static SoType getClassTypeId(void);
  static void * createInstance(void);
public:
  static int getClassStackIndex(void);
  virtual ~SoTextureQualityElement(void);

  static void initClass(void);
  static void cleanClass(void);

protected:
  SoTextureQualityElement(void);
  static int classStackIndex;
//$ END TEMPLATE ElementHeader

public:
  virtual void init( SoState * state );

  virtual void push( SoState * state );
  virtual void pop( SoState * state, const SoElement * prevTopElement );
 
  static void set( SoState * const state, SoNode * const node,
		   const float quality );
  static void set( SoState * const state, const float quality );
  static float get( SoState * const state );
  static float getDefault( void );

};

#endif // ! _SO_TEXTURE_QUALITY_ELEMENT_H_
