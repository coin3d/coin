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

#ifndef COIN_SOFIELDCONVERTER_H
#define COIN_SOFIELDCONVERTER_H

#include <Inventor/engines/SoSubEngine.h>

class SoEngineOutput;


class COIN_DLL_EXPORT SoFieldConverter : public SoEngine {
  typedef SoEngine inherited;
  SO_ENGINE_ABSTRACT_HEADER(SoFieldConverter);

public:
  static void initClass(void);
  static void initClasses(void);
  virtual SoField * getInput(SoType type) = 0;
  virtual SoEngineOutput * getOutput(SoType type) = 0;
  virtual SoField * getConnectedInput(void);
  int getForwardConnections(SoFieldList & l) const;

protected:
  SoFieldConverter(void);
  virtual ~SoFieldConverter();
};

#endif // !COIN_SOFIELDCONVERTER_H
