/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
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
