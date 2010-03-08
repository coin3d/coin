#ifndef COIN_SOSHUTTLE_H
#define COIN_SOSHUTTLE_H

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

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFBool.h>

class SoElapsedTime;
class SoCalculator;
class SoInterpolateVec3f;

class COIN_DLL_API SoShuttle : public SoTranslation {
  typedef SoTranslation inherited;

  SO_NODE_HEADER(SoShuttle);

public:
  static void initClass(void);
  SoShuttle(void);

  SoSFVec3f translation0;
  SoSFVec3f translation1;
  SoSFFloat speed;
  SoSFBool on;

  virtual void write(SoWriteAction * action);

protected:
  virtual ~SoShuttle();

private:
  virtual SoNode * copy(SbBool copyconnections = FALSE) const;
  void deconnectInternalEngines(void);
  void reconnectInternalEngines(void);

  SoInterpolateVec3f * interpolator;
  SoCalculator * calculator;
  SoElapsedTime * timer;
};

#endif // !COIN_SOSHUTTLE_H
