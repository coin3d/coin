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

#include <Inventor/engines/SoComposeMatrix.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/SoType.h>
#include <Inventor/fields/SoMFMatrix.h>

SO_ENGINE_SOURCE(SoComposeMatrix);

SoComposeMatrix::SoComposeMatrix()
{
  SO_ENGINE_CONSTRUCTOR(SoComposeMatrix);

  SO_ENGINE_ADD_INPUT(translation,(0,0,0));
  SO_ENGINE_ADD_INPUT(rotation,(0,0,0,0));
  SO_ENGINE_ADD_INPUT(scaleFactor,(0,0,0));
  SO_ENGINE_ADD_INPUT(scaleOrientation,(0,0,0,0));
  SO_ENGINE_ADD_INPUT(center,(0,0,0));

  SO_ENGINE_ADD_OUTPUT(matrix,SoMFMatrix);
}

void
SoComposeMatrix::initClass()
{
  SO_ENGINE_INTERNAL_INIT_CLASS(SoComposeMatrix);
}

//
// private members
//
SoComposeMatrix::~SoComposeMatrix()
{
}

void
SoComposeMatrix::evaluate()
{
  int numTranslation=translation.getNum();
  int numRotation=rotation.getNum();
  int numScaleFactor=scaleFactor.getNum();
  int numScaleOrientation=scaleOrientation.getNum();
  int numCenter=center.getNum();

  int numOut=numTranslation>numRotation?numTranslation:numRotation;
  int numOut2=
    numScaleFactor>numScaleOrientation?numScaleFactor:numScaleOrientation;
  numOut2=numOut2>numCenter?numOut2:numCenter;
  numOut=numOut>numOut2?numOut:numOut2;

  SO_ENGINE_OUTPUT(matrix,SoMFMatrix,setNum(numOut));

  int i;

  for (i=0;i<numOut;i++) {
    const SbVec3f translationVal=
      i<numTranslation?translation[i]:translation[numTranslation-1];
    const SbVec3f scaleFactorVal=
      i<numScaleFactor?scaleFactor[i]:scaleFactor[numScaleFactor-1];
    const SbVec3f centerVal=i<numCenter?center[i]:center[numCenter-1];
    const SbRotation rotationVal=
      i<numRotation?rotation[i]:rotation[numRotation-1];
    const SbRotation scaleOrientationVal=
      i<numScaleOrientation?
      scaleOrientation[i]:scaleOrientation[numScaleOrientation-1];

    SbMatrix mat;
    mat.setTransform(translationVal,rotationVal,scaleFactorVal,
                     scaleOrientationVal,centerVal);
    SO_ENGINE_OUTPUT(matrix,SoMFMatrix,set1Value(i,mat));
  }
}
