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

#ifndef COIN_SOREPLACEDELEMENT_H
#define COIN_SOREPLACEDELEMENT_H

#include <Inventor/elements/SoElement.h>
#include <Inventor/system/inttypes.h>


class SoReplacedElement : public SoElement {
  typedef SoElement inherited;

  SO_ELEMENT_ABSTRACT_HEADER(SoReplacedElement);
public:
  static void initClass(void);
protected:
  virtual ~SoReplacedElement();

public:
  virtual void init(SoState * state);
  virtual SbBool matches(const SoElement * element) const;

  virtual SoElement * copyMatchInfo() const;

  uint32_t getNodeId() const;

  virtual void print(FILE * file) const;

protected:
  static  SoElement * getElement(SoState * const state,
                                 const int stackIndex, SoNode * const node);

  uint32_t nodeId;
};

#endif // !COIN_SOREPLACEDELEMENT_H
