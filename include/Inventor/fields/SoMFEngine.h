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

#ifndef COIN_SOMFENGINE_H
#define COIN_SOMFENGINE_H

#include <Inventor/fields/SoMField.h>
#include <Inventor/fields/SoSubField.h>

class SoEngine;


class COIN_DLL_EXPORT SoMFEngine : public SoMField {
  typedef SoMField inherited;

  SO_MFIELD_HEADER(SoMFEngine, SoEngine *, SoEngine *);

public:
  static void initClass(void);

  virtual void fixCopy(SbBool copyConnections);
  virtual SbBool referencesCopy(void) const;

private:
  virtual void countWriteRefs(SoOutput * out) const;
};

#endif // !COIN_SOMFENGINE_H
