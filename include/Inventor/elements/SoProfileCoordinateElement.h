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

#ifndef __SOPROFILECOORDINATEELEMENT_H__
#define __SOPROFILECOORDINATEELEMENT_H__

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOPROFILECOORDINATEELEMENT)
#error "Configuration settings disable this class!"
#endif // COIN_EXCLUDE_SOPROFILECOORDINATEELEMENT

#include <Inventor/elements/SoReplacedElement.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SbVec3f.h>
#include <assert.h>

/*
 * TODO
 * - conversion between coord2 and coord3 (done in Inventor)
 */

class SoProfileCoordinateElement : public SoReplacedElement {
  typedef SoReplacedElement inherited;

//$ BEGIN TEMPLATE ElementHeader( SoProfileCoordinateElement )
public:
  static SoType classTypeId;
  static SoType getClassTypeId(void);
  static void * createInstance(void);
public:
  static int getClassStackIndex(void);
  virtual ~SoProfileCoordinateElement(void);

  static void initClass(void);
  static void cleanClass(void);

protected:
  SoProfileCoordinateElement(void);
  static int classStackIndex;
//$ END TEMPLATE ElementHeader

public:
  virtual void init( SoState * state );

  virtual void push( SoState * state );
  virtual void pop( SoState * state,
		    const SoElement * prevTopElement );

  static void set2( SoState * const state, SoNode * const node,
		    const int32_t numCoords, const SbVec2f * const coords );
  static void set3( SoState * const state, SoNode * const node,
		    const int32_t numCoords, const SbVec3f * const coords );
  static const SoProfileCoordinateElement *
             getInstance( SoState * const state );
  int32_t getNum( void ) const;
  const SbVec2f & get2( const int index ) const;
  const SbVec3f & get3( const int index ) const;

  SbBool is2D( void ) const;

  static SbVec2f getDefault2( void );
  static SbVec3f getDefault3( void );

  virtual void print( FILE * file ) const;

protected:
  int32_t numCoords;
  const SbVec2f * coords2;
  const SbVec3f * coords3;
  SbBool coordsAre2D;

  static SbVec2f initdefaultcoords;

}; // class SoProfileCoordinateElement

#endif // !__SOPROFILECOORDINATEELEMENT_H__
