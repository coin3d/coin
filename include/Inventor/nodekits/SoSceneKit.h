/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOSCENEKIT_H
#define COIN_SOSCENEKIT_H

#include <Inventor/nodekits/SoSubKit.h>
#include <Inventor/nodekits/SoBaseKit.h>


class COIN_DLL_API SoSceneKit : public SoBaseKit {
  typedef SoBaseKit inherited;

  SO_KIT_HEADER(SoSceneKit);

  SO_KIT_CATALOG_ENTRY_HEADER(cameraList);
  SO_KIT_CATALOG_ENTRY_HEADER(childList);
  SO_KIT_CATALOG_ENTRY_HEADER(lightList);
  SO_KIT_CATALOG_ENTRY_HEADER(topSeparator);

public:
  static void initClass(void);
  SoSceneKit(void);

  int getCameraNumber(void);
  void setCameraNumber(int camNum);
  virtual SbBool affectsState(void) const;

protected:
  virtual ~SoSceneKit();
};

#endif // !COIN_SOSCENEKIT_H
