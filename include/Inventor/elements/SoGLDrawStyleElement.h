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

#ifndef COIN_SOGLDRAWSTYLEELEMENT_H
#define COIN_SOGLDRAWSTYLEELEMENT_H

#include <Inventor/elements/SoDrawStyleElement.h>


class COIN_DLL_EXPORT SoGLDrawStyleElement : public SoDrawStyleElement {
  typedef SoDrawStyleElement inherited;

  SO_ELEMENT_HEADER(SoGLDrawStyleElement);
public:
  static void initClass(void);
protected:
  virtual ~SoGLDrawStyleElement();

public:
  virtual void init(SoState * state);

  virtual void push(SoState * state);
  virtual void pop(SoState * state,
                   const SoElement * prevTopElement);

protected:
  virtual void setElt(int32_t style);

private:
  void updategl();

};

#endif // !COIN_SOGLDRAWSTYLEELEMENT_H
