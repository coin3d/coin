#ifndef COIN_SOPROFILERELEMENT_H
#define COIN_SOPROFILERELEMENT_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/elements/SoElement.h>
#include <Inventor/elements/SoSubElement.h>

#include <Inventor/annex/Profiler/SbProfilingData.h>

// *************************************************************************

class COIN_DLL_API SoProfilerElement : public SoElement {
  typedef SoElement inherited;
  SO_ELEMENT_HEADER(SoProfilerElement);

public:
  static void initClass(void);

  static SoProfilerElement * get(SoState * state);

  virtual SbBool matches(const SoElement * element) const;
  virtual SoElement * copyMatchInfo(void) const;

  SbProfilingData & getProfilingData(void);
  const SbProfilingData & getProfilingData(void) const;

protected:
  virtual ~SoProfilerElement(void);

  SbProfilingData data;

private:
  virtual void push(SoState * state);
  virtual void pop(SoState * state, const SoElement * elt);

}; // SoProfilerElement

#endif // !COIN_SOPROFILERELEMENT_H
