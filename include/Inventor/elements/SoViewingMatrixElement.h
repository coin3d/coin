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

#ifndef __SOVIEWINGMATRIXELEMENT_H__
#define __SOVIEWINGMATRIXELEMENT_H__

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOVIEWINGMATRIXELEMENT)
#error "Configuration settings disable this class!"
#endif // COIN_EXCLUDE_SOVIEWINGMATRIXELEMENT

#include <Inventor/elements/SoReplacedElement.h>

#include <Inventor/SbMatrix.h>

class SoViewingMatrixElement : public SoReplacedElement {
  typedef SoReplacedElement inherited;

//$ BEGIN TEMPLATE ElementHeader(SoViewingMatrixElement)
public:
  static SoType classTypeId;
  static SoType getClassTypeId(void);
  static void * createInstance(void);
public:
  static int getClassStackIndex(void);
  virtual ~SoViewingMatrixElement(void);

  static void initClass(void);

protected:
  SoViewingMatrixElement(void);
  static int classStackIndex;
//$ END TEMPLATE ElementHeader

public:
  virtual void init(SoState * state);
  static  void set(SoState * const state, SoNode * const node,
		   const SbMatrix & ViewingMatrix);
  static  const SbMatrix &get(SoState * const state);
  virtual void print(FILE * file) const;

protected:
  virtual void setElt(const SbMatrix &matrix);
  SbMatrix viewingMatrix;

};

#endif // !__SOVIEWINGMATRIXELEMENT_H__
