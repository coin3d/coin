#ifndef COIN_SOGETMATRIXACTION_H
#define COIN_SOGETMATRIXACTION_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/actions/SoAction.h>
#include <Inventor/actions/SoSubAction.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/SbRotation.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/SbVec3f.h>


class COIN_DLL_API SoGetMatrixAction : public SoAction {
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
