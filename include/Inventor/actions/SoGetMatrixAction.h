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
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOGETMATRIXACTION_H
#define COIN_SOGETMATRIXACTION_H

#include <Inventor/actions/SoAction.h>
#include <Inventor/actions/SoSubAction.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/SbRotation.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/SbVec3f.h>


class COIN_DLL_EXPORT SoGetMatrixAction : public SoAction {
  typedef SoAction inherited;

  SO_ACTION_HEADER(SoGetMatrixAction);

public:
  SoGetMatrixAction(const SbViewportRegion & region);
  virtual ~SoGetMatrixAction();

  static void initClass(void);

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
};

#endif // !COIN_SOGETMATRIXACTION_H
