#ifndef COIN_SOSCROLLINGGRAPHKIT_H
#define COIN_SOSCROLLINGGRAPHKIT_H

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

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodekits/SoSubKit.h>

#include <Inventor/tools/SbPimplPtr.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/fields/SoMFFloat.h>
#include <Inventor/fields/SoMFColor.h>
#include <Inventor/fields/SoMFName.h>

class SoSensor;
class SoScrollingGraphKitP;

class SoScrollingGraphKit : public SoBaseKit {
  typedef SoBaseKit inherited;
  SO_KIT_HEADER(SoScrollingGraphKit);
  SO_KIT_CATALOG_ENTRY_HEADER(scene);

public:
  static void initClass(void);

  SoScrollingGraphKit(void);

  enum GraphicsType {
    LINES,
    STACKED_BARS,
    DEFAULT_GRAPHICS = STACKED_BARS
  };

  enum RangeType {
    ABSOLUTE_ACCUMULATIVE,
    //ABSOLUTE_OVERWRITE,
    //RELATIVE_ACCUMULATIVE,
    //RELATIVE_OVERWRITE,
    DEFAULT_RANGETYPE = ABSOLUTE_ACCUMULATIVE
  };

  // config
  SoSFEnum graphicsType;
  SoSFEnum rangeType;
  SoSFTime seconds; // seconds to pass over graph area (20)
  SoMFColor colors; // rotating color list

  // geometry
  SoSFVec3f viewportSize; // input
  SoSFVec3f position;     // input
  SoSFVec3f size;         // input

  // the dynamic inputs
  SoMFName addKeys;
  SoMFFloat addValues; // input

protected:
  virtual ~SoScrollingGraphKit(void);

  static void addValuesCB(void * closure, SoSensor * sensor);

private:
  SbPimplPtr<SoScrollingGraphKitP> pimpl;

  // NOT IMPLEMENTED
  SoScrollingGraphKit(const SoScrollingGraphKit &);
  SoScrollingGraphKit & operator = (const SoScrollingGraphKit &);

}; // SoScrollingGraphKit

#endif // !COIN_SOSCROLLINGGRAPHKIT_H
