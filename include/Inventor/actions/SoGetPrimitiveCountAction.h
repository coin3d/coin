#ifndef COIN_SOGETPRIMITIVECOUNTACTION_H
#define COIN_SOGETPRIMITIVECOUNTACTION_H

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
#include <Inventor/elements/SoDecimationTypeElement.h>

class SoGetPrimitiveCountActionP;
class SbViewportRegion;

class COIN_DLL_API SoGetPrimitiveCountAction : public SoAction {
  typedef SoAction inherited;

  SO_ACTION_HEADER(SoGetPrimitiveCountAction);

public:
  static void initClass(void);

  SoGetPrimitiveCountAction(void);
  SoGetPrimitiveCountAction(const SbViewportRegion & vp);

  virtual ~SoGetPrimitiveCountAction(void);

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

private:
  void commonConstructor(const SbViewportRegion & vp);
  SbLazyPimplPtr<SoGetPrimitiveCountActionP> pimpl;

  // NOT IMPLEMENTED:
  SoGetPrimitiveCountAction(const SoGetPrimitiveCountAction & rhs);
  SoGetPrimitiveCountAction & operator = (const SoGetPrimitiveCountAction & rhs);
}; // SoGetPrimitiveCountAction

#endif // !COIN_SOGETPRIMITIVECOUNTACTION_H
