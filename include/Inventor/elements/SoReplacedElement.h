/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
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

#ifndef COIN_SOREPLACEDELEMENT_H
#define COIN_SOREPLACEDELEMENT_H

#include <Inventor/elements/SoSubElement.h>
#include <Inventor/system/inttypes.h>


class COIN_DLL_API SoReplacedElement : public SoElement {
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
