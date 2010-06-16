#ifndef COIN_SOPROFILERTOPKIT
#define COIN_SOPROFILERTOPKIT

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

#include <Inventor/annex/Profiler/nodekits/SoProfilerOverlayKit.h>
#include <Inventor/fields/SoSFColor.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFVec2f.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/tools/SbPimplPtr.h>

class SoProfilerTopKitP;

class COIN_DLL_API SoProfilerTopKit : public SoProfilerOverlayKit {
  typedef SoProfilerOverlayKit inherited;
  SO_KIT_HEADER(SoProfilerTopKit);
  SO_KIT_CATALOG_ENTRY_HEADER(textSep);
  SO_KIT_CATALOG_ENTRY_HEADER(color);
  SO_KIT_CATALOG_ENTRY_HEADER(translation);
  SO_KIT_CATALOG_ENTRY_HEADER(text);
  SO_KIT_CATALOG_ENTRY_HEADER(graph);

public:
  static void initClass(void);
  SoProfilerTopKit(void);

  SoSFColor txtColor;
  SoSFInt32 lines;

  SoSFVec2f topKitSize;    // output set from internal parts
  SoSFVec3f position;      // input set from SoProfilerOverlayKit

protected:
  virtual ~SoProfilerTopKit(void);

private:
  SbPimplPtr<SoProfilerTopKitP> pimpl;
  friend class SoProfilerTopKitP;

  SoProfilerTopKit(const SoProfilerTopKit & rhs);
  SoProfilerTopKit & operator = (const SoProfilerTopKit & rhs);

}; // SoProfilerTopKit

#endif //!COIN_SOPROFILERTOPKIT
