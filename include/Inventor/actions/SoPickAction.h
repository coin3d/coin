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

#ifndef COIN_SOPICKACTION_H
#define COIN_SOPICKACTION_H

#include <Inventor/actions/SoAction.h>
#include <Inventor/actions/SoSubAction.h>
#include <Inventor/SbViewportRegion.h>


class COIN_DLL_EXPORT SoPickAction : public SoAction {
  typedef SoAction inherited;

  SO_ACTION_HEADER(SoPickAction);

public:
  static void initClass(void);

  void setViewportRegion(const SbViewportRegion & newregion);
  const SbViewportRegion & getViewportRegion(void);

void enableCulling(const SbBool flag);
  SbBool isCullingEnabled() const;

protected:
  SoPickAction(const SbViewportRegion & viewportregion);
  virtual ~SoPickAction();

  virtual void beginTraversal(SoNode * node);

  SbViewportRegion vpRegion;

private:
  SbBool cullingenabled;
};

#endif // !COIN_SOPICKACTION_H
