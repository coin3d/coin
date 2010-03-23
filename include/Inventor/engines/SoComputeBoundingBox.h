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

#ifndef COIN_SOCOMPUTEBOUNDINGBOX_H
#define COIN_SOCOMPUTEBOUNDINGBOX_H

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoSFPath.h>
#include <Inventor/SbViewportRegion.h>

class SoGetBoundingBoxAction;


class COIN_DLL_API SoComputeBoundingBox : public SoEngine {
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
  virtual ~SoComputeBoundingBox(void);

private:
  virtual void evaluate(void);
  SoGetBoundingBoxAction * bboxaction;
};

#endif // !COIN_SOCOMPUTEBOUNDINGBOX_H
