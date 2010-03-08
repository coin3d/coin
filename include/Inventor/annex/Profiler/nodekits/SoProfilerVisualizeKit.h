#ifndef COIN_SOPROFILERVISUALIZEKIT
#define COIN_SOPROFILERVISUALIZEKIT

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
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/fields/SoMFNode.h>
#include <Inventor/tools/SbPimplPtr.h>

struct SoProfilerVisualizeKitP;

class COIN_DLL_API SoProfilerVisualizeKit : public SoBaseKit {
  typedef SoBaseKit inherited;
  SO_KIT_HEADER(SoProfilerVisualizeKit);
  SO_KIT_CATALOG_ENTRY_HEADER(top);
  SO_KIT_CATALOG_ENTRY_HEADER(pretree);
  SO_KIT_CATALOG_ENTRY_HEADER(visualtree);

public:
  static void initClass(void);
  SoProfilerVisualizeKit(void);

  /// Set this to be the root of the scenegraph we visualize.
  SoSFNode root;

  /// Set this to be the stats field.
  SoSFTrigger statsTrigger;
  SoSFNode stats;

  // Fields picked up from SoProfilerStats follow here.
  SoMFNode separatorsWithGLCaches;

protected:
  virtual ~SoProfilerVisualizeKit(void);

private:
  /// NOT IMPLEMENTED
  SoProfilerVisualizeKit(const SoProfilerVisualizeKit&);
  /// NOT IMPLEMENTED
  SoProfilerVisualizeKit &operator=(const SoProfilerVisualizeKit&);
  SbPimplPtr<SoProfilerVisualizeKitP> pimpl;
};

#endif //!COIN_SOPROFILERVISUALIZEKIT
