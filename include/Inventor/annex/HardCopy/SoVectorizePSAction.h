#ifndef COIN_SOVECTORIZEPSACTION_H
#define COIN_SOVECTORIZEPSACTION_H

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

// *************************************************************************

#include <Inventor/annex/HardCopy/SoVectorizeAction.h>
#include <Inventor/annex/HardCopy/SoPSVectorOutput.h>

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
  SoVectorizePSActionP * pimpl;
  friend class SoVectorizePSActionP;
};

// *************************************************************************

#endif //!COIN_SOVECTORIZEPSACTION_H
