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
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOCOMPUTEBOUNDINGBOX_H
#define COIN_SOCOMPUTEBOUNDINGBOX_H

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoSFPath.h>
#include <Inventor/SbViewportRegion.h>

class SoGetBoundingBoxAction;


class COIN_DLL_EXPORT SoComputeBoundingBox : public SoEngine {
  typedef SoEngine inherited;

  SO_ENGINE_HEADER(SoComputeBoundingBox);

public:
  SoSFNode node;
  SoSFPath path;

  SoEngineOutput min;           // SoSFVec3f
  SoEngineOutput max;           // SoSFVec3f
  SoEngineOutput boxCenter;     // SoSFVec3f
  SoEngineOutput objectCenter;  // SoSFVec3f

  SoComputeBoundingBox(void);
  static void initClass(void);

  void setViewportRegion(const SbViewportRegion & vpr);
  const SbViewportRegion & getViewportRegion(void) const;

protected:
  virtual ~SoComputeBoundingBox();

private:
  virtual void evaluate(void);
  SoGetBoundingBoxAction * bboxaction;
};

#endif // !COIN_SOCOMPUTEBOUNDINGBOX_H
