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

#ifndef COIN_SBPROJECTOR_H
#define COIN_SBPROJECTOR_H

#include <Inventor/SbVec3f.h>
#include <Inventor/SbLine.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/SbViewVolume.h>

class SbVec2f;


class COIN_DLL_EXPORT SbProjector {
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
};

#endif // !COIN_SBPROJECTOR_H
