/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOFONTNAMEELEMENT_H
#define COIN_SOFONTNAMEELEMENT_H

#include <Inventor/SbName.h>
#include <Inventor/elements/SoReplacedElement.h>


class COIN_DLL_API SoFontNameElement : public SoReplacedElement {
  typedef SoReplacedElement inherited;

  SO_ELEMENT_HEADER(SoFontNameElement);
public:
  static void initClass(void);
protected:
  virtual ~SoFontNameElement();

public:
  virtual void init(SoState * state);

  static void set(SoState * const state, SoNode * const node,
                  const SbName fontName);
  static const SbName & get(SoState * const state);
  static SbName getDefault(void);

  virtual SbBool matches(const SoElement * element) const;
  virtual SoElement * copyMatchInfo(void) const;

  virtual void print(FILE * file) const;

protected:
  SbName fontName;

private:
  static void clean(void);
  static SbName * defaultfontname;
};

#endif // !COIN_SOFONTNAMEELEMENT_H
