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

#ifndef __SONORMALELEMENT_H__
#define __SONORMALELEMENT_H__

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SONORMALELEMENT)
#error "Configuration settings disable this class!"
#endif // COIN_EXCLUDE_SONORMALELEMENT

#include <Inventor/elements/SoReplacedElement.h>
#include <Inventor/SbVec3f.h>
#include <assert.h>

class SoNormalElement : public SoReplacedElement {
  typedef SoReplacedElement inherited;

//$ BEGIN TEMPLATE ElementHeader(SoNormalElement)
public:
  static SoType classTypeId;
  static SoType getClassTypeId(void);
  static void * createInstance(void);
public:
  static int getClassStackIndex(void);
  virtual ~SoNormalElement(void);

  static void initClass(void);

protected:
  SoNormalElement(void);
  static int classStackIndex;
//$ END TEMPLATE ElementHeader

public:
  virtual void init(SoState * state);

  static void set(SoState * const state, SoNode * const node,
                  const int32_t numNormals, const SbVec3f * const normals,
                  const SbBool normalsAreUnitLength = FALSE);

  static const SoNormalElement *getInstance(SoState * const state);
  int32_t getNum() const;
  const SbVec3f & get(const int index) const;
  const SbVec3f *getArrayPtr() const;

  SbBool areNormalsUnitLength() const;
  
  
  virtual void print(FILE * file) const;

protected:
  int32_t numNormals;
  const SbVec3f *normals;
  SbBool unitLength;

};

#endif // !__SONORMALELEMENT_H__
