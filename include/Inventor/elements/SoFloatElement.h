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

#ifndef COIN_SOFLOATELEMENT_H
#define COIN_SOFLOATELEMENT_H

#include <Inventor/elements/SoSubElement.h>


class COIN_DLL_EXPORT SoFloatElement : public SoElement {
  typedef SoElement inherited;

  SO_ELEMENT_ABSTRACT_HEADER(SoFloatElement);
public:
  static void initClass(void);
protected:
  virtual ~SoFloatElement();

public:
  virtual void init(SoState * state);

  virtual SbBool matches(const SoElement * element) const;
  virtual SoElement * copyMatchInfo(void) const;

  virtual void print(FILE * file) const;

  static  void set(const int stackIndex, SoState * const state,
                    SoNode * const node, const float value);
  static  void set(const int stackIndex, SoState * const state,
                    const float value);

  static  float get(const int stackIndex, SoState * const state);

  virtual void setElt(float value);

protected:
          float data;

};

#endif // !COIN_SOFLOATELEMENT_H
