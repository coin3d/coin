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

#ifndef __SOFONTNAMEELEMENT_H__
#define __SOFONTNAMEELEMENT_H__

#include <Inventor/SbName.h>
#include <Inventor/elements/SoReplacedElement.h>


class SoFontNameElement : public SoReplacedElement {
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
  static const SbName get(SoState * const state);
  static const SbName getDefault();

  virtual SbBool matches(SoElement * element) const;
  virtual SoElement * copyMatchInfo() const;

  virtual void print(FILE * file) const;

protected:
  SbName fontName;

private:
  static void clean(void);
  static SbName * defaultfontname;
};

#endif // !__SOFONTNAMEELEMENT_H__
