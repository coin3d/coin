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

#ifndef __SOANTISQUISH_H__
#define __SOANTISQUISH_H__

#include <Inventor/nodes/SoTransformation.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/SbMatrix.h>

#if defined(COIN_EXCLUDE_SOANTISQUISH)
#error Configuration settings disrespected -- do not include this file!
#endif // COIN_EXCLUDE_SOANTISQUISH


// *************************************************************************

class SoAntiSquish : public SoTransformation {
  typedef SoTransformation inherited;

//$ BEGIN TEMPLATE NodeHeader(SoAntiSquish)
private:
  static SoType classTypeId;

public:
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
  static void * createInstance(void);
public:
  static void initClass(void);
  static void cleanClass(void);

  SoAntiSquish(void);
protected:
  virtual ~SoAntiSquish();
//$ END TEMPLATE NodeHeader

public:
  enum Sizing {
    X, Y, Z, AVERAGE_DIMENSION, BIGGEST_DIMENSION, SMALLEST_DIMENSION,
    LONGEST_DIAGONAL
  };

  SoSFEnum sizing;
  SoSFBool recalcAlways;

  void recalc(void);
#if !defined(COIN_EXCLUDE_SOACTION)
  void doAction(SoAction * action);
#endif // !COIN_EXCLUDE_SOACTION

protected:
#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
  virtual void callback(SoCallbackAction * action);
#endif // !COIN_EXCLUDE_SOCALLBACKACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  virtual void GLRender(SoGLRenderAction * action);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
#if !defined(COIN_EXCLUDE_SOGETMATRIXACTION)
  virtual void getMatrix(SoGetMatrixAction * action);
#endif // !COIN_EXCLUDE_SOGETMATRIXACTION
#if !defined(COIN_EXCLUDE_SOPICKACTION)
  virtual void pick(SoPickAction * action);
#endif // !COIN_EXCLUDE_SOPICKACTION

  SbMatrix getUnsquishingMatrix(SbMatrix squishedMatrix,
				SbBool doInverse, SbMatrix &inverseAnswer);
};

#endif // !__SOANTISQUISH_H__
