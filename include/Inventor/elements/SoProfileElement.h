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

#ifndef _SO_PROFILE_ELEMENT_H_
#define _SO_PROFILE_ELEMENT_H_

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOPROFILEELEMENT)
#error "Configuration settings disrespected -- this class should not be in use!"
#endif // COIN_EXCLUDE_SOPROFILEELEMENT

#include <Inventor/elements/SoAccumulatedElement.h>
#include <Inventor/lists/SoNodeList.h>

class SoProfile;

class SoProfileElement : public SoAccumulatedElement {
  typedef SoAccumulatedElement inherited;

//$ BEGIN TEMPLATE ElementHeader( SoProfileElement )
public:
  static SoType classTypeId;
  static SoType getClassTypeId(void);
  static void * createInstance(void);
public:
  static int getClassStackIndex(void);
  virtual ~SoProfileElement(void);

  static void initClass(void);
  static void cleanClass(void);

protected:
  SoProfileElement(void);
  static int classStackIndex;
//$ END TEMPLATE ElementHeader

public:
  enum Profile {
    START_FIRST = 0,
    START_NEW = 1,
    ADD_TO_CURRENT = 2
  };

  virtual void init( SoState * state );

  virtual void push( SoState * state );
  virtual void pop( SoState * state,
              const SoElement * prevTopElement );

  static  void add( SoState * const state, SoProfile * const profile );
  static  const SoNodeList & get( SoState * const state );

  virtual void print( FILE * file ) const;

protected:
  SoNodeList profiles;

}; // class SoProfileElement

#endif // ! _SO_PROFILE_ELEMENT_H_
