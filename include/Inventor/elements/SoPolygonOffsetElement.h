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

#ifndef _SO_POLYGON_OFFSET_ELEMENT_H_
#define _SO_POLYGON_OFFSET_ELEMENT_H_

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOPOLYGONOFFSETELEMENT)
#error "Configuration settings disable this class!"
#endif // COIN_EXCLUDE_SOPOLYGONOFFSETELEMENT

#include <Inventor/elements/SoReplacedElement.h>

class SoPolygonOffsetElement : public SoReplacedElement {
  typedef SoReplacedElement inherited;

//$ BEGIN TEMPLATE ElementHeader( SoPolygonOffsetElement )
public:
  static SoType classTypeId;
  static SoType getClassTypeId(void);
  static void * createInstance(void);
public:
  static int getClassStackIndex(void);
  virtual ~SoPolygonOffsetElement(void);

  static void initClass(void);
  static void cleanClass(void);

protected:
  SoPolygonOffsetElement(void);
  static int classStackIndex;
//$ END TEMPLATE ElementHeader

public:
  enum Style {
    FILLED = 0x01,
    LINES = 0x02, 
    POINTS = 0x04
  };

  virtual void init(SoState * state);
  static void set(SoState * state, SoNode * node, float factor, float units, 
		  Style styles, SbBool on);
  static void get(SoState * state, float & factor, float & units,
		  Style & styles, SbBool & on);
  virtual void push(SoState * state);
  virtual SbBool matches(const SoElement * elt) const;
  virtual SoElement * copyMatchInfo() const;
  static void getDefault(float & factor, float & units, Style & styles,
			 SbBool & on);

protected:
  Style style;
  SbBool active;
  float offsetfactor;
  float offsetunits;

  virtual void setElt(float factor, float units, Style styles, SbBool on);

};

#endif // ! _SO_POLYGON_OFFSET_ELEMENT_H_
