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

#ifndef _SO_FONT_NAME_ELEMENT_H_
#define _SO_FONT_NAME_ELEMENT_H_

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOFONTNAMEELEMENT)
#error "Configuration settings disable this class!"
#endif // COIN_EXCLUDE_SOFONTNAMEELEMENT

#include <Inventor/SbName.h>
#include <Inventor/elements/SoReplacedElement.h>

class SoFontNameElement : public SoReplacedElement {
  typedef SoReplacedElement inherited;

//$ BEGIN TEMPLATE ElementHeader( SoFontNameElement )
public:
  static SoType classTypeId;
  static SoType getClassTypeId(void);
  static void * createInstance(void);
public:
  static int getClassStackIndex(void);
  virtual ~SoFontNameElement(void);

  static void initClass(void);
  static void cleanClass(void);

protected:
  SoFontNameElement(void);
  static int classStackIndex;
//$ END TEMPLATE ElementHeader

public:
  virtual void init(SoState * state);

  static void set(SoState * const state, SoNode * const node,
		  const SbName fontName);
  static const SbName get(SoState * const state);
  static const SbName getDefault();

  virtual SbBool matches(SoElement * element) const;
  virtual SoElement * copyMatchInfo() const;

  virtual void print(FILE * file) const;

protected:
  SbName fontName;
  static const SbName defaultFontName;

}; // class SoFontNameElement

#endif // ! _SO_FONT_NAME_ELEMENT_H_
