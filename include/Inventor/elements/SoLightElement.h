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

#ifndef __SOLIGHTELEMENT_H__
#define __SOLIGHTELEMENT_H__

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOLIGHTELEMENT)
#error "Configuration settings disable this class!"
#endif // COIN_EXCLUDE_SOLIGHTELEMENT

#include <Inventor/elements/SoAccumulatedElement.h>
#include <Inventor/lists/SbPList.h>
#include <Inventor/lists/SoNodeList.h>
#include <Inventor/SbMatrix.h>

class SoLight;

class SoLightElement : public SoAccumulatedElement {
  typedef SoAccumulatedElement inherited;

//$ BEGIN TEMPLATE ElementHeader(SoLightElement)
public:
  static SoType classTypeId;
  static SoType getClassTypeId(void);
  static void * createInstance(void);
public:
  static int getClassStackIndex(void);
  virtual ~SoLightElement(void);

  static void initClass(void);

protected:
  SoLightElement(void);
  static int classStackIndex;
//$ END TEMPLATE ElementHeader

public:
  virtual void init(SoState * state);

  virtual void push(SoState * state);
  virtual void pop(SoState * state,
              const SoElement * prevTopElement);

  static  void add(SoState * const state, SoLight * const light,
              const SbMatrix & WCToVRCMatrix);
  static  const SoNodeList & getLights(SoState * const state);
  static  const SbMatrix & getMatrix(SoState * const state,
              const int index);

  virtual void print(FILE * file) const;

protected:
  SoNodeList lights;
  SbPList WCToVRCMatrices; // World Coords -> View Reference Coords
  int startIndex;

};

#endif // !__SOLIGHTELEMENT_H__
