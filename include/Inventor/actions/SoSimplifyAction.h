#ifndef COIN_SOSIMPLIFYACTION_H
#define COIN_SOSIMPLIFYACTION_H

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

#include <Inventor/actions/SoAction.h>
#include <Inventor/actions/SoSubAction.h>

class SoSimplifyActionP;

class COIN_DLL_API SoSimplifyAction : public SoAction {
  typedef SoAction inherited;

  SO_ACTION_HEADER(SoSimplifyAction);

public:
  static void initClass(void);

  SoSimplifyAction(void);
  virtual ~SoSimplifyAction(void);

  virtual void apply(SoNode * root);
  virtual void apply(SoPath * path);
  virtual void apply(const SoPathList & pathlist, SbBool obeysrules = FALSE);

protected:
  virtual void beginTraversal(SoNode * node);

private:
  SbPimplPtr<SoSimplifyActionP> pimpl;

  // NOT IMPLEMENTED:
  SoSimplifyAction(const SoSimplifyAction & rhs);
  SoSimplifyAction & operator = (const SoSimplifyAction & rhs);
}; // SoSimplifyAction

#endif // !COIN_SOSIMPLIFYACTION_H
