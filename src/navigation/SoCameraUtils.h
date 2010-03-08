#ifndef COIN_SOCAMERAUTILS_H
#define COIN_SOCAMERAUTILS_H

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

#include <Inventor/SbBasic.h>

class SoCamera;
class SbVec3f;

class SoCameraManager {
public:
  static SoCameraManager * createFor(SoCamera * camera);

  virtual ~SoCameraManager(void);

  virtual void setZoomLimits(float minzoomvalue = 0.0f, float maxzoomvalue = 0.0f);
  virtual SbBool getZoomLimits(float & minzoomvalue, float & maxzoomvalue) const;

  virtual void setZoomValue(float zoomvalue, SbBool limit = FALSE) = 0;
  virtual void adjustZoomValue(float diffvalue, SbBool limit = TRUE) = 0; // diffvalue=0.0 is identity
  virtual void adjustZoom(float factor, SbBool limit = TRUE) = 0; // factor=1.0 is identity

  virtual float getZoomValue(void) const;

  virtual void setZoomByDollyLimits(float mindollydistance = 0.0f, float maxdollydistance = 0.0f, float unitydistance = 0.0f);
  virtual SbBool getZoomByDollyLimits(float & mindollydistance, float & maxdollydistance, float & unitydistance) const;

  virtual void setZoomValueByDolly(float zoomvalue, SbBool limit = FALSE) = 0;
  virtual void adjustZoomByDollyDistance(float distance, SbBool limit = TRUE) = 0; // distance=0.0 is identity
  virtual void adjustZoomByDolly(float factor, SbBool limit = TRUE) = 0; // factor=1.0 is identity

  virtual float getZoomByDollyValue(void) const;

  SoCamera * getCamera(void) const { return this->camera; }
  SbVec3f getFocalPoint(void) const;

  virtual void copyLimits(const SoCameraManager * other);

protected:
  SoCameraManager(SoCamera * camera);

  SbBool havezoomlimits;
  float minzoom, maxzoom;
  SbBool havezoombydollylimits;
  float mindollydistance, maxdollydistance, unitydistance;

private:
  SoCamera * camera;

}; // SoCameraManager

#endif // !COIN_SOCAMERAUTILS_H
