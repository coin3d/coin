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

#ifndef _SO_ANNO_TEXT3_FONT_SIZE_HINT_ELEMENT_H_
#define _SO_ANNO_TEXT3_FONT_SIZE_HINT_ELEMENT_H_

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOANNOTEXT3FONTSIZEHINTELEMENT)
#error "Configuration settings disable this class!"
#endif // COIN_EXCLUDE_SOANNOTEXT3FONTSIZEHINTELEMENT

#include <Inventor/elements/SoInt32Element.h>

class SoAnnoText3FontSizeHintElement : public SoInt32Element {
  typedef SoInt32Element inherited;

//$ BEGIN TEMPLATE ElementHeader( SoAnnoText3FontSizeHintElement )
public:
  static SoType classTypeId;
  static SoType getClassTypeId(void);
  static void * createInstance(void);
public:
  static int getClassStackIndex(void);
  virtual ~SoAnnoText3FontSizeHintElement(void);

  static void initClass(void);
  static void cleanClass(void);

protected:
  SoAnnoText3FontSizeHintElement(void);
  static int classStackIndex;
//$ END TEMPLATE ElementHeader

public:
  enum FontSizeHint {
    ANNOTATION, FIT_TEXT_VECTOR
  };
  
  virtual void init( SoState * state );
  static void set( SoState * const state, const FontSizeHint hint );
  static FontSizeHint get( SoState * const state );
  static FontSizeHint getDefault( void );

  static void set(SoState * const state, SoNode * const node,
		  const FontSizeHint hint);

};

#endif // ! _SO_ANNO_TEXT3_FONT_SIZE_HINT_ELEMENT_H_
