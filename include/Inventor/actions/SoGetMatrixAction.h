#ifndef COIN_SOGETMATRIXACTION_H
#define COIN_SOGETMATRIXACTION_H

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

#include <Inventor/actions/SoAction.h>
#include <Inventor/actions/SoSubAction.h>
#include <Inventor/tools/SbLazyPimplPtr.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/SbRotation.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/SbVec3f.h>

class SoGetMatrixActionP;

class COIN_DLL_API SoGetMatrixAction : public SoAction {
  typedef SoAction inherited;

  SO_ACTION_HEADER(SoGetMatrixAction);

public:
  static void initClass(void);

  SoGetMatrixAction(const SbViewportRegion & region);
  virtual ~SoGetMatrixAction(void);

  void setViewportRegion(const SbViewportRegion & region);
  const SbViewportRegion & getViewportRegion(void) const;

  SbMatrix & getMatrix(void);
  SbMatrix & getInverse(void);
  SbMatrix & getTextureMatrix(void);
  SbMatrix & getTextureInverse(void);

protected:
  virtual void beginTraversal(SoNode * node);

private:
  SbMatrix matrix, invmatrix;
  SbMatrix texmatrix, invtexmatrix;
  SbViewportRegion viewportregion;

private:
  SbLazyPimplPtr<SoGetMatrixActionP> pimpl;

  // NOT IMPLEMENTED:
  SoGetMatrixAction(const SoGetMatrixAction & rhs);
  SoGetMatrixAction & operator = (const SoGetMatrixAction & rhs);
}; // SoGetMatrixAction

#endif // !COIN_SOGETMATRIXACTION_H
