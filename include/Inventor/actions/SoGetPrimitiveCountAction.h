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

#ifndef COIN_SOGETPRIMITIVECOUNTACTION_H
#define COIN_SOGETPRIMITIVECOUNTACTION_H

#include <Inventor/actions/SoAction.h>
#include <Inventor/actions/SoSubAction.h>
#include <Inventor/elements/SoDecimationTypeElement.h>


class SoGetPrimitiveCountAction : public SoAction {
  typedef SoAction inherited;

  SO_ACTION_HEADER(SoGetPrimitiveCountAction);

public:
  SoGetPrimitiveCountAction(void);
  virtual ~SoGetPrimitiveCountAction();

  static void initClass(void);

  int getTriangleCount(void) const;
  int getLineCount(void) const;
  int getPointCount(void) const;
  int getTextCount(void) const;
  int getImageCount(void) const;
  SbBool containsNoPrimitives(void);
  SbBool containsNonTriangleShapes(void);

  SbBool is3DTextCountedAsTriangles(void);
  void setCount3DTextAsTriangles(const SbBool flag);

  SbBool canApproximateCount(void);
  void setCanApproximate(const SbBool flag);

  void setDecimationValue(SoDecimationTypeElement::Type type,
                          float percentage = 1.0);
  SoDecimationTypeElement::Type getDecimationType(void);
  float getDecimationPercentage(void);

  void addNumTriangles(const int num);
  void addNumLines(const int num);
  void addNumPoints(const int num);
  void addNumText(const int num);
  void addNumImage(const int num);
  void incNumTriangles(void);
  void incNumLines(void);
  void incNumPoints(void);
  void incNumText(void);
  void incNumImage(void);

protected:
  virtual void beginTraversal(SoNode * node);

private:
  int numtris;
  int numlines;
  int numpoints;
  int numtexts;
  int numimages;

  SbBool textastris;
  SbBool approx;
  SbBool nonvertexastris;
  SoDecimationTypeElement::Type decimationtype;
  float decimationpercentage;
};

#endif // !COIN_SOGETPRIMITIVECOUNTACTION_H
