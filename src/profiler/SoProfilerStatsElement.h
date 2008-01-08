#ifndef COIN_SOPROFILERSTATSELEMENT_H
#define COIN_SOPROFILERSTATSELEMENT_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2008 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#ifndef COIN_INTERNAL
#error this is a private header file
#endif // COIN_INTERNAL

#include <Inventor/elements/SoElement.h>
#include <Inventor/tools/SbPimplPtr.h>
#include <Inventor/elements/SoSubElement.h>

class SoState;
class SoProfilerStats;
class SoProfilerStatsElementP;

class COIN_DLL_API SoProfilerStatsElement : public SoElement {
  typedef SoElement inherited;
  SO_ELEMENT_HEADER(SoProfilerStatsElement);
public:
  static void initClass();
  static SoProfilerStatsElement * get(SoState *state);
  static void set(SoState * state, SoProfilerStats * stats);
  
  const SoProfilerStats * stats() const;
protected:
  virtual ~SoProfilerStatsElement();
  virtual SbBool matches(const SoElement*) const;
  virtual SoElement * copyMatchInfo() const;
  
private:
  /// NOT IMPLEMENTED
  SoProfilerStatsElement(const SoProfilerStatsElement&);

  /// NOT IMPLEMENTED
  SoProfilerStatsElement &operator=(const SoProfilerStatsElement &);
  
  SbPimplPtr<SoProfilerStatsElementP> pimpl;
};


#endif //!COIN_SOPROFILERSTATSELEMENT_H
