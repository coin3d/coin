/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

#ifndef COIN_SOSHUTTLE_H
#define COIN_SOSHUTTLE_H

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
