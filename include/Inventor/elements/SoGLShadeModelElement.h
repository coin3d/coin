/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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

#ifndef COIN_SOGLSHADEMODELELEMENT_H
#define COIN_SOGLSHADEMODELELEMENT_H

#include <Inventor/elements/SoSubElement.h>


class COIN_DLL_EXPORT SoGLShadeModelElement : public SoElement {
  typedef SoElement inherited;

  SO_ELEMENT_HEADER(SoGLShadeModelElement);
public:
  static void initClass(void);
protected:
  virtual ~SoGLShadeModelElement();

public:
  virtual void init(SoState * state);
  virtual void push(SoState * state);
  virtual void pop(SoState * state,
                   const SoElement * prevTopElement);

  virtual SbBool matches(const SoElement * element) const;
  virtual SoElement * copyMatchInfo() const;
  
  static void set(SoState * state, const SbBool flat);
  
  void evaluate(void) const;
  void forceSend(const SbBool flat) const;
  
  static const SoGLShadeModelElement * getInstance(SoState * state);
  
private:
  SbBool flat;
  SbBool glflat;
  void updategl(const SbBool flatshade);
};

#endif // !COIN_SOGLSHADEMODELELEMENT_H
