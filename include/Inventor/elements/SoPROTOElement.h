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
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOPROTOELEMENT_H
#define COIN_SOPROTOELEMENT_H

#include <Inventor/elements/SoSubElement.h>

class COIN_DLL_EXPORT SoPROTOElement : public SoElement {
  typedef SoElement inherited;

  SO_ELEMENT_HEADER(SoPROTOElement);
public:
  static void initClass(void);
protected:
  virtual ~SoPROTOElement();

public:
  virtual void init(SoState * state);

  virtual void push(SoState * state);
  virtual void pop(SoState * state,
              const SoElement * prevTopElement);

  virtual SbBool matches(const SoElement * element) const;
  virtual SoElement * copyMatchInfo(void) const;

  // FIXME: quick hack. 19980810 mortene.
  class SoPROTO *nodeptr;
  // larsa: SoElement got dataNode member for this :(
  // - might fix this later
};

#endif // !COIN_SOPROTOELEMENT_H
