#ifndef COIN_SOANTISQUISH_H
#define COIN_SOANTISQUISH_H

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

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoTransformation.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/SbMatrix.h>

class COIN_DLL_API SoAntiSquish : public SoTransformation {
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
  virtual void doAction(SoAction * action);

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
