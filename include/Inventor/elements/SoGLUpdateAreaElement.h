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

#ifndef __SOGLUPDATEAREAELEMENT_H__
#define __SOGLUPDATEAREAELEMENT_H__

#include <Inventor/elements/SoElement.h>
#include <Inventor/SbVec2f.h>


class SoGLUpdateAreaElement : public SoElement {
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
};

#endif // !__SOGLUPDATEAREAELEMENT_H__
