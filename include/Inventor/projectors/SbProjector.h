#ifndef COIN_SBPROJECTOR_H
#define COIN_SBPROJECTOR_H

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
