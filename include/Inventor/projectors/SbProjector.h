#ifndef COIN_SBPROJECTOR_H
#define COIN_SBPROJECTOR_H

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

#include <Inventor/SbVec3f.h>
#include <Inventor/SbLine.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/SbViewVolume.h>

class SbVec2f;

class COIN_DLL_API SbProjector {
public:
  virtual SbVec3f project(const SbVec2f & point) = 0;
  virtual void setViewVolume(const SbViewVolume & vol);
  const SbViewVolume & getViewVolume(void) const;
  virtual void setWorkingSpace(const SbMatrix & space);
  const SbMatrix & getWorkingSpace(void) const;
  virtual SbProjector * copy(void) const = 0;

protected:
  SbProjector(void);
  virtual ~SbProjector() { }

  SbLine getWorkingLine(const SbVec2f & point) const;

  SbViewVolume viewVol;
  SbMatrix worldToWorking, workingToWorld;
  
  float findVanishingDistance(void) const;
  SbBool verifyProjection(const SbVec3f & projpt) const;
};

#endif // !COIN_SBPROJECTOR_H
