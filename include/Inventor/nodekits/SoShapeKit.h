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

#ifndef COIN_SOSHAPEKIT_H
#define COIN_SOSHAPEKIT_H

#include <Inventor/nodekits/SoSeparatorKit.h>


class COIN_DLL_EXPORT SoShapeKit : public SoSeparatorKit {
  typedef SoSeparatorKit inherited;

  SO_KIT_HEADER(SoShapeKit);

  SO_KIT_CATALOG_ENTRY_HEADER(coordinate3);
  SO_KIT_CATALOG_ENTRY_HEADER(coordinate4);
  SO_KIT_CATALOG_ENTRY_HEADER(localTransform);
  SO_KIT_CATALOG_ENTRY_HEADER(materialBinding);
  SO_KIT_CATALOG_ENTRY_HEADER(normal);
  SO_KIT_CATALOG_ENTRY_HEADER(normalBinding);
  SO_KIT_CATALOG_ENTRY_HEADER(profileCoordinate2);
  SO_KIT_CATALOG_ENTRY_HEADER(profileCoordinate3);
  SO_KIT_CATALOG_ENTRY_HEADER(profileList);
  SO_KIT_CATALOG_ENTRY_HEADER(shape);
  SO_KIT_CATALOG_ENTRY_HEADER(shapeHints);
  SO_KIT_CATALOG_ENTRY_HEADER(shapeSeparator);
  SO_KIT_CATALOG_ENTRY_HEADER(textureCoordinate2);
  SO_KIT_CATALOG_ENTRY_HEADER(textureCoordinateBinding);
  SO_KIT_CATALOG_ENTRY_HEADER(textureCoordinateFunction);

public:
  SoShapeKit(void);

  static void initClass(void);

protected:
  virtual ~SoShapeKit();

  virtual void setDefaultOnNonWritingFields(void);
};

#endif // !COIN_SOSHAPEKIT_H
