#ifndef COIN_SOVECTORIZEPSACTION_H
#define COIN_SOVECTORIZEPSACTION_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

// *************************************************************************

#include <HardCopy/SoVectorizeAction.h>
#include <HardCopy/SoPSVectorOutput.h>

class SoVectorizePSActionP;

// *************************************************************************

class COIN_DLL_API SoVectorizePSAction : public SoVectorizeAction {

  SO_ACTION_HEADER(SoVectorizePSAction);

public:
  SoVectorizePSAction(void);
  virtual ~SoVectorizePSAction();

  static void initClass(void);

  void setDefault2DFont(const SbString & fontname);
  const SbString & getDefault2DFont(void) const;

  void setGouraudThreshold(const double eps);

  SoPSVectorOutput * getOutput(void) const;
  SoPSVectorOutput * getPSOutput(void) const;

protected:

  virtual void printHeader(void) const;
  virtual void printFooter(void) const;
  virtual void printBackground(void) const;
  virtual void printItem(const SoVectorizeItem * item) const;
  virtual void printViewport(void) const;

private:

  SbVec2f convertToPS(const SbVec2f & mm) const;
  float convertToPS(const float mm) const;

  void updateLineAttribs(const SoVectorizeLine * line) const;
  void updateFont(const SbString & fontname, const float fontsize) const;
  void printSetdash(uint16_t pattern) const;

  void printCircle(const SbVec3f & v, const SbColor & c, const float radius) const;
  void printTriangle(const SbVec3f * v, const SbColor * c) const;
  void printTriangle(const SoVectorizeTriangle * item) const;
  void printLine(const SoVectorizeLine * item) const;
  void printPoint(const SoVectorizePoint * item) const;
  void printText(const SoVectorizeText * item) const;
  void printImage(const SoVectorizeImage * item) const;

  SoVectorizePSActionP * pimpl;
  friend class SoVectorizePSActionP;
};

// *************************************************************************

#endif //!COIN_SOVECTORIZEPSACTION_H
