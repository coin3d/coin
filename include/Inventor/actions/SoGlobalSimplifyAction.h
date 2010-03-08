#ifndef COIN_SOGLOBALSIMPLIFYACTION_H
#define COIN_SOGLOBALSIMPLIFYACTION_H

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

// FIXME: this class has not yet been implemented in Coin. this is a
// placeholder / stub / skeleton code, not yet built (nor installed,
// obviously).  --mortene.

#include <Inventor/actions/SoSimplifyAction.h>
#include <Inventor/tools/SbLazyPimplPtr.h>

class SoGlobalSimplifyActionP;

class COIN_DLL_API SoGlobalSimplifyAction : public SoSimplifyAction {
  typedef SoSimplifyAction inherited;

  SO_ACTION_HEADER(SoGlobalSimplifyAction);

public:
  static void initClass(void);

  SoGlobalSimplifyAction(void);
  virtual ~SoGlobalSimplifyAction(void);

protected:
  virtual void beginTraversal(SoNode * node);

private:
  SbLazyPimplPtr<SoGlobalSimplifyActionP> pimpl;

  // NOT IMPLEMENTED:
  SoGlobalSimplifyAction(const SoGlobalSimplifyAction & rhs);
  SoGlobalSimplifyAction & operator = (const SoGlobalSimplifyAction & rhs);
}; // SoGlobalSimplifyAction

#endif // !COIN_SOGLOBALSIMPLIFYACTION_H
