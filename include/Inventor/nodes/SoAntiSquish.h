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
