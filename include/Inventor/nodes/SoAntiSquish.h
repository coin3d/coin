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

#ifndef COIN_SOANTISQUISH_H
#define COIN_SOANTISQUISH_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoTransformation.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/SbMatrix.h>


class COIN_DLL_EXPORT SoAntiSquish : public SoTransformation {
  typedef SoTransformation inherited;

  SO_NODE_HEADER(SoAntiSquish);

public:
  static void initClass(void);
  SoAntiSquish(void);

  enum Sizing {
    X, Y, Z, AVERAGE_DIMENSION, BIGGEST_DIMENSION, SMALLEST_DIMENSION,
    LONGEST_DIAGONAL
  };

  SoSFEnum sizing;
  SoSFBool recalcAlways;

  void recalc(void);
  void doAction(SoAction * action);

protected:
  virtual ~SoAntiSquish();

  virtual void callback(SoCallbackAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void getMatrix(SoGetMatrixAction * action);
  virtual void pick(SoPickAction * action);

  SbMatrix getUnsquishingMatrix(const SbMatrix & squishedmatrix,
                                const SbBool calcinverse,
                                SbMatrix & getinverse);
private:
  SbMatrix unsquishedmatrix, inversematrix;
  SbBool matrixvalid, inversevalid;
};

#endif // !COIN_SOANTISQUISH_H
