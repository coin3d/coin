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

#ifndef __SOGLSHADEMODELELEMENT_H__
#define __SOGLSHADEMODELELEMENT_H__

#include <Inventor/elements/SoElement.h>

#if defined(COIN_EXCLUDE_SOGLSHADEMODELELEMENT)
#error "Configuration settings disrespected -- do not include this file!"
#endif /// defined(COIN_EXCLUDE_SOGLSHADEMODELELEMENT)


class SoGLShadeModelElement : public SoElement {
  typedef SoElement inherited;

//$ BEGIN TEMPLATE ElementHeader( SoGLShadeModelElement )
public:
  static SoType classTypeId;
  static SoType getClassTypeId(void);
  static void * createInstance(void);
public:
  static int getClassStackIndex(void);
  virtual ~SoGLShadeModelElement(void);

  static void initClass(void);
  static void cleanClass(void);

protected:
  SoGLShadeModelElement(void);
  static int classStackIndex;
//$ END TEMPLATE ElementHeader

public:
  virtual void init(SoState * state);
  virtual void push(SoState * state);
  virtual void pop(SoState * state,
		   const SoElement * prevTopElement);

  virtual SbBool matches(const SoElement * element) const;
  virtual SoElement * copyMatchInfo() const;

  static void setMaterial(SoState * const state,
			  const SbBool perVertex);
  static void setNormal(SoState * const state,
			const SbBool perVertex);
  virtual void print(FILE * file) const;

  void evaluate() const;
  void forceSend(const SbBool flat) const;
  
private:
  SbBool matPerVertex;
  SbBool normPerVertex;
  SbBool glflat;
  
  void updategl(const SbBool flat);
};

#endif // !__SOGLSHADEMODELELEMENT_H__
