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
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SODETAIL_H
#define COIN_SODETAIL_H

#include <Inventor/SoType.h>


class COIN_DLL_EXPORT SoDetail {

public:
  virtual ~SoDetail();

  static void initClass(void);
  static void initClasses(void);

  virtual SoDetail * copy(void) const = 0;

  virtual SoType getTypeId(void) const = 0;
  SbBool isOfType(const SoType type) const;

  static SoType getClassTypeId(void);

protected:
  SoDetail(void);

private:
  static SoType classTypeId;
};

#endif // COIN_SODETAIL_H
