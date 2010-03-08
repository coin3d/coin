#ifndef COIN_SOWRITEACTION_H
#define COIN_SOWRITEACTION_H

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
#include <Inventor/tools/SbLazyPimplPtr.h>

class SoWriteActionP;

class COIN_DLL_API SoWriteAction : public SoAction {
  typedef SoAction inherited;

  SO_ACTION_HEADER(SoWriteAction);

public:
  SoWriteAction(void);
  SoWriteAction(SoOutput * out);
  virtual ~SoWriteAction(void);

  static void initClass(void);

  SoOutput * getOutput(void) const;

  void continueToApply(SoNode * node);
  void continueToApply(SoPath * path);

protected:
  virtual void beginTraversal(SoNode * node);
  virtual SbBool shouldCompactPathLists(void) const;

private:
  void commonConstructor(SoOutput * out);

  SoOutput * outobj;
  SbBool localoutputalloc;
  SbBool continuing;

private:
  SbLazyPimplPtr<SoWriteActionP> pimpl;

  // NOT IMPLEMENTED:
  SoWriteAction(const SoWriteAction & rhs);
  SoWriteAction & operator = (const SoWriteAction & rhs);
}; // SoWriteAction

#endif // !COIN_SOWRITEACTION_H
