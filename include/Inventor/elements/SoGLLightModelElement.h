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

#ifndef __SOGLLIGHTMODELELEMENT_H__
#define __SOGLLIGHTMODELELEMENT_H__

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOGLLIGHTMODELELEMENT)
#error "Configuration settings disrespected -- do not include this file!"
#endif // COIN_EXCLUDE_SOGLLIGHTMODELELEMENT

#include <Inventor/elements/SoLightModelElement.h>

class SoGLLightModelElement : public SoLightModelElement {
  typedef SoLightModelElement inherited;

//$ BEGIN TEMPLATE ElementHeader(SoGLLightModelElement)
public:
  static SoType classTypeId;
  static SoType getClassTypeId(void);
  static void * createInstance(void);
public:
  static int getClassStackIndex(void);
  virtual ~SoGLLightModelElement(void);

  static void initClass(void);

protected:
  SoGLLightModelElement(void);
  static int classStackIndex;
//$ END TEMPLATE ElementHeader

public:
  virtual void init(SoState * state);
  virtual void push(SoState * state);
  virtual void pop(SoState * state,
		   const SoElement * prevTopElement);

  void evaluate() const; // lazy update

  // force this value without changing state
  void forceSend(const Model model) const; 

  static const SoGLLightModelElement *getInstance(SoState *state);

protected:
  virtual void setElt(int32_t value);

private:
  void updategl();
  Model current;

};

#endif // !__SOGLLIGHTMODELELEMENT_H__
