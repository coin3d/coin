#ifndef COIN_SOPENDULUM_H
#define COIN_SOPENDULUM_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFBool.h>

class SoElapsedTime;
class SoCalculator;
class SoInterpolateRotation;

class COIN_DLL_API SoPendulum : public SoRotation {
  typedef SoRotation inherited;

  SO_NODE_HEADER(SoPendulum);

public:
  static void initClass(void);
  SoPendulum(void);

  SoSFRotation rotation0;
  SoSFRotation rotation1;
  SoSFFloat speed;
  SoSFBool on;

  virtual void write(SoWriteAction * action);

protected:
  virtual ~SoPendulum();

private:
  virtual SoNode * copy(SbBool copyconnections = FALSE) const;
  void deconnectInternalEngine(void);
  void reconnectInternalEngine(void);

  SoInterpolateRotation * interpolator;
  SoCalculator * calculator;
  SoElapsedTime * timer;
};

#endif // !COIN_SOPENDULUM_H
