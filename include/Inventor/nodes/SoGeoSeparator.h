#ifndef COIN_SOGEOSEPARATOR_H
#define COIN_SOGEOSEPARATOR_H

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

#include <Inventor/tools/SbLazyPimplPtr.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/fields/SoSFVec3d.h>
#include <Inventor/fields/SoMFString.h>

class SoState;
class SoGeoSeparatorP;

class COIN_DLL_API SoGeoSeparator : public SoSeparator {
  typedef SoSeparator inherited;

  SO_NODE_HEADER(SoGeoSeparator);

public:
  static void initClass(void);
  SoGeoSeparator(void);

  SoSFVec3d geoCoords;
  SoMFString geoSystem;

  virtual void callback(SoCallbackAction * action);
  virtual void GLRenderBelowPath(SoGLRenderAction * action);
  virtual void GLRenderInPath(SoGLRenderAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void getMatrix(SoGetMatrixAction * action);
  virtual void rayPick(SoRayPickAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

protected:
  virtual ~SoGeoSeparator(void);

private:
  SoGeoSeparator(const SoGeoSeparator & rhs);
  SoGeoSeparator & operator = (const SoGeoSeparator & rhs);

  void applyTransformation(SoAction * action);
  SbMatrix getTransform(SoState * state) const;

  SbLazyPimplPtr<SoGeoSeparatorP> pimpl;

};


#endif // COIN_SOGEOSEPARATOR_H
