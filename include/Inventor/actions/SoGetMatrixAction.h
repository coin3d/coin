/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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

#ifndef COIN_SOGETMATRIXACTION_H
#define COIN_SOGETMATRIXACTION_H

#include <Inventor/actions/SoAction.h>
#include <Inventor/actions/SoSubAction.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/SbRotation.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/SbVec3f.h>


class SoGetMatrixAction : public SoAction {
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
