#ifndef COIN_SOGLUPDATEAREAELEMENT_H
#define COIN_SOGLUPDATEAREAELEMENT_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/elements/SoSubElement.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SbVec2s.h>

class COIN_DLL_API SoGLUpdateAreaElement : public SoElement {
  typedef SoElement inherited;

  SO_ELEMENT_HEADER(SoGLUpdateAreaElement);
public:
  static void initClass(void);
protected:
  virtual ~SoGLUpdateAreaElement();

public:
  virtual void init(SoState * state);
  virtual void push(SoState * state);
  virtual void pop(SoState * state,
                   const SoElement * prevTopElement);

  virtual SbBool matches(const SoElement * element) const;
  virtual SoElement * copyMatchInfo() const;

  static void set(SoState * const state, const SbVec2f & origin,
                   const SbVec2f & size);

  static SbBool get(SoState * const state, SbVec2f & origin,
                    SbVec2f & size);

  static SbVec2f getDefaultOrigin();
  static SbVec2f getDefaultSize();

protected:
  SbVec2f origin;
  SbVec2f size;

private:
  SbBool isDefault(void) const;
  void updategl(void);
  SbBool scissorstate;
  SbVec2s screenorigin, screensize;  
};

#endif // !COIN_SOGLUPDATEAREAELEMENT_H
