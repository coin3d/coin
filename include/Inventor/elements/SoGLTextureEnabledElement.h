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

#ifndef COIN_SOGLTEXTUREENABLEDELEMENT_H
#define COIN_SOGLTEXTUREENABLEDELEMENT_H

#include <Inventor/elements/SoInt32Element.h>


class COIN_DLL_EXPORT SoGLTextureEnabledElement : public SoInt32Element {
  typedef SoInt32Element inherited;

  SO_ELEMENT_HEADER(SoGLTextureEnabledElement);
public:
  static void initClass(void);
protected:
  virtual ~SoGLTextureEnabledElement();

public:
  virtual void init(SoState * state);

  virtual void push(SoState * state);
  virtual void pop(SoState * state,
                   const SoElement * prevTopElement);
  virtual void evaluate(void) const;
  virtual void forceSend(const SbBool onoff) const;
  static  void set(SoState * const state, SoNode * const node,
                   const SbBool enabled);
  static  void set(SoState * const state, const SbBool enabled);

  static SbBool get(SoState * const state);
  static SbBool getDefault(void);

private:
  int32_t glstate;
  void updategl(void);

};

#endif // !COIN_SOGLTEXTUREENABLEDELEMENT_H
