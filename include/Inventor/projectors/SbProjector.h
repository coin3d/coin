/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

#ifndef __SBPROJECTOR_H__
#define __SBPROJECTOR_H__

#include <Inventor/SbVec3f.h>
#include <Inventor/SbLine.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/SbViewVolume.h>

class SbVec2f;


class SbProjector {
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

#endif // !__SBPROJECTOR_H__
