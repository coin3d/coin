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

#ifndef __SOGETPRIMITIVECOUNTACTION_H__
#define __SOGETPRIMITIVECOUNTACTION_H__

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
#error "This file is excluded from Coin through the configuration system!"
#endif // COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

#include <Inventor/actions/SoAction.h>

class SoGetPrimitiveCountAction : public SoAction {
  typedef SoAction inherited;

//$ BEGIN TEMPLATE ActionHeader(SoGetPrimitiveCountAction)
private:
  static SoType classTypeId;

public:
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId(void);

protected:
  virtual const SoEnabledElementsList & getEnabledElements(void) const;
  static SoEnabledElementsList * enabledElements;
  static SoActionMethodList * methods;

public:
  static void addMethod(const SoType type, SoActionMethod method);
  static void enableElement(const SoType type, const int stackIndex);

  static void initClass(void);

  virtual ~SoGetPrimitiveCountAction();
//$ END TEMPLATE ActionHeader

public:
  SoGetPrimitiveCountAction(void);

  int getTriangleCount() const;
  int getLineCount() const;
  int getPointCount() const;
  int getTextCount() const;
  int getImageCount() const;
  SbBool containsNoPrimitives();
  SbBool containsNonTriangleShapes();

  SbBool is3DTextCountedAsTriangles();
  void setCount3DTextAsTriangles(const SbBool onOff);

  SbBool canApproximateCount();
  void setCanApproximate(const SbBool onOff);

#if 0 // not supported yet
  void setDecimationValue(SoDecimationTypeElement::Type type,
                          float percentage = 1.0);
  SoDecimationTypeElement::Type getDecimationType();
  float getDecimationPercentage();
#endif // not supported

public:
  void addNumTriangles(const int num);
  void addNumLines(const int num);
  void addNumPoints(const int num);
  void addNumText(const int num);
  void addNumImage(const int num);
  void incNumTriangles();
  void incNumLines();
  void incNumPoints();
  void incNumText();
  void incNumImage();

  // not part of OIV
  SbBool isNonVertexShapesCountedAsTriangles() const;
  void setCountNonVertexShapesAsTriangles(const SbBool onOff);

  int getCubeCount() const;
  int getCylinderCount() const;
  int getConeCount() const;
  int getSphereCount() const;

  void incNumCubes();
  void incNumCylinders();
  void incNumCones();
  void incNumSpheres();

protected:
  virtual void beginTraversal(SoNode * node);

private:
  int numTris;
  int numLines;
  int numPoints;
  int numTexts;
  int numImages;
  int numCubes;
  int numCylinders;
  int numCones;
  int numSpheres;

  SbBool textAsTris;
  SbBool approx;
  SbBool nonVertexAsTris;
};

#endif // !__SOGETPRIMITIVECOUNTACTION_H__
