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

#ifndef __SOTEXTURECOORDINATEELEMENT_H__
#define __SOTEXTURECOORDINATEELEMENT_H__

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOTEXTURECOORDINATEELEMENT)
#error "Configuration settings disable this class!"
#endif // COIN_EXCLUDE_SOTEXTURECOORDINATEELEMENT

#include <Inventor/elements/SoReplacedElement.h>

#include <Inventor/SbVec2f.h>
#include <Inventor/SbVec4f.h>

/*
 * TODO
 * - conversion between 2D and 4D (like Open Inventor)
 */ 

typedef const SbVec4f & SoTextureCoordinateFunctionCB(
    void *userdata, const SbVec3f & point, const SbVec3f & normal );

class SoTextureCoordinateElement : public SoReplacedElement {
  typedef SoReplacedElement inherited;

//$ BEGIN TEMPLATE ElementHeader( SoTextureCoordinateElement )
public:
  static SoType classTypeId;
  static SoType getClassTypeId(void);
  static void * createInstance(void);
public:
  static int getClassStackIndex(void);
  virtual ~SoTextureCoordinateElement(void);

  static void initClass(void);
  static void cleanClass(void);

protected:
  SoTextureCoordinateElement(void);
  static int classStackIndex;
//$ END TEMPLATE ElementHeader

public:
  enum CoordType {
    DEFAULT = 0,
    EXPLICIT = 1,
    FUNCTION = 2
  };

  virtual void init( SoState * state );

  static void setDefault(SoState * const state, SoNode * const node);
  static void setFunction(SoState * const state, SoNode * const node,
			  SoTextureCoordinateFunctionCB * const func,
			  void * const userdata);

  static void set2(SoState * const state, SoNode * const node,
		   const int32_t numCoords, const SbVec2f * const coords );
  static void set4(SoState * const state, SoNode * const node,
		   const int32_t numCoords, const SbVec4f * const coords);

  static const SbVec2f *getArrayPtr2(SoState * const state);
  static const SbVec4f *getArrayPtr4(SoState * const state);
  
  static CoordType getType(SoState * const state);
  virtual CoordType getType() const;

  static const SoTextureCoordinateElement *getInstance(SoState * const state);

  const SbVec4f &get(const SbVec3f & point,
		     const SbVec3f & normal) const;
  int32_t getNum() const;
  SbBool is2D() const;

  const SbVec2f &get2( const int index ) const;
  const SbVec4f &get4( const int index ) const;
 
protected:
  CoordType whatKind;
  SoTextureCoordinateFunctionCB *funcCB;
  void *funcCBData;
  int32_t numCoords;
  const SbVec2f *coords2;
  const SbVec4f *coords4;
  SbBool coordsAre2D;
  SbVec2f convert2;
  SbVec4f convert4;

};

#endif // !__SOTEXTURECOORDINATEELEMENT_H__
