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

#ifndef _SO_FLOAT_ELEMENT_H_
#define _SO_FLOAT_ELEMENT_H_

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOFLOATELEMENT)
#error "Configuration settings disable this class!"
#endif // COIN_EXCLUDE_SOFLOATELEMENT

#include <Inventor/elements/SoElement.h>

class SoFloatElement : public SoElement {
  typedef SoElement inherited;

//$ BEGIN TEMPLATE AbstractElementHeader( SoFloatElement )
public:
  static  SoType classTypeId;
  static  SoType getClassTypeId(void);
public:
  virtual ~SoFloatElement(void);
  static  int getClassStackIndex(void);

  static  void initClass(void);
  static  void cleanClass(void);

protected:
          SoFloatElement(void);
  static  int classStackIndex;
//$ END TEMPLATE AbstractElementHeader

public:
  virtual void init( SoState * state );

  virtual SbBool matches( const SoElement * element ) const;
  virtual SoElement * copyMatchInfo( void ) const;

  virtual void print( FILE * file ) const;

  static  void set( const int stackIndex, SoState * const state,
                    SoNode * const node, const float value );
  static  void set( const int stackIndex, SoState * const state,
                    const float value );
 
  static  float get( const int stackIndex, SoState * const state );

  virtual void setElt(float value);

protected:
          float data;

}; // class SoFloatElement

#endif // ! _SO_FLOAT_ELEMENT_H_
