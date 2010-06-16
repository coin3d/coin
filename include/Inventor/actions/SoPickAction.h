#ifndef COIN_SOPICKACTION_H
#define COIN_SOPICKACTION_H

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
#include <Inventor/SbViewportRegion.h>

class SoPickActionP;

class COIN_DLL_API SoPickAction : public SoAction {
  typedef SoAction inherited;

  SO_ACTION_HEADER(SoPickAction);

public:
  static void initClass(void);

  void setViewportRegion(const SbViewportRegion & newregion);
  const SbViewportRegion & getViewportRegion(void);

  void enableCulling(const SbBool flag);
  SbBool isCullingEnabled(void) const;

protected:
  SoPickAction(const SbViewportRegion & viewportregion);
  virtual ~SoPickAction(void);

  virtual void beginTraversal(SoNode * node);

  SbViewportRegion vpRegion;

private:
  SbBool cullingenabled;

private:
  SbLazyPimplPtr<SoPickActionP> pimpl;

  // NOT IMPLEMENTED:
  SoPickAction(const SoPickAction & rhs);
  SoPickAction & operator = (const SoPickAction & rhs);
}; // SoPickAction

#endif // !COIN_SOPICKACTION_H
