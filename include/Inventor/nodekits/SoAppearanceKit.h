#ifndef COIN_SOAPPEARANCEKIT_H
#define COIN_SOAPPEARANCEKIT_H

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

#include <Inventor/nodekits/SoSubKit.h>
#include <Inventor/nodekits/SoBaseKit.h>

class COIN_DLL_API SoAppearanceKit : public SoBaseKit {
  typedef SoBaseKit inherited;

  SO_KIT_HEADER(SoAppearanceKit);

  SO_KIT_CATALOG_ENTRY_HEADER(complexity);
  SO_KIT_CATALOG_ENTRY_HEADER(drawStyle);
  SO_KIT_CATALOG_ENTRY_HEADER(environment);
  SO_KIT_CATALOG_ENTRY_HEADER(font);
  SO_KIT_CATALOG_ENTRY_HEADER(lightModel);
  SO_KIT_CATALOG_ENTRY_HEADER(material);
  SO_KIT_CATALOG_ENTRY_HEADER(texture2);

public:
  SoAppearanceKit(void);

  static void initClass(void);

protected:
  virtual ~SoAppearanceKit();
};

#endif // !COIN_SOAPPEARANCEKIT_H
