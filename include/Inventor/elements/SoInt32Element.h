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

#ifndef _SO_INT32_ELEMENT_H_
#define _SO_INT32_ELEMENT_H_

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOINT32ELEMENT)
#error "Configuration settings disable this class!"
#endif // COIN_EXCLUDE_SOINT32ELEMENT

#include <Inventor/elements/SoElement.h>
#include <Inventor/system/inttypes.h>

class SoInt32Element : public SoElement {
  typedef SoElement inherited;

//$ BEGIN TEMPLATE AbstractElementHeader( SoInt32Element )
public:
  static  SoType classTypeId;
  static  SoType getClassTypeId(void);
public:
  virtual ~SoInt32Element(void);
  static  int getClassStackIndex(void);

  static  void initClass(void);
  static  void cleanClass(void);

protected:
          SoInt32Element(void);
  static  int classStackIndex;
//$ END TEMPLATE AbstractElementHeader

public: // constructor / destructor / etc.
  virtual void init( SoState * state );

  virtual SbBool matches( const SoElement * element ) const;
  virtual SoElement * copyMatchInfo( void ) const;

  virtual void print( FILE * file ) const;

  static  void set( const int index, SoState * const state,
                    SoNode * const node, const int32_t value );
  static  void set( const int index, SoState * const state,
                    const int32_t value );
  static  int32_t get( const int index, SoState * const state );
  virtual void setElt(int32_t value);

protected:
  int32_t data;

};

#endif // ! _SO_INT32_ELEMENT_H_
