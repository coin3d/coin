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

#ifndef COIN_SOONESHOT_H
#define COIN_SOONESHOT_H

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/fields/SoSFBitMask.h>


class COIN_DLL_EXPORT SoOneShot : public SoEngine {
  typedef SoEngine inherited;

  SO_ENGINE_HEADER(SoOneShot);

public:
  static void initClass(void);
  SoOneShot(void);

  enum Flags {RETRIGGERABLE=1, HOLD_FINAL=2};

  SoSFTime timeIn;
  SoSFTime duration;
  SoSFTrigger trigger;
  SoSFBitMask flags;
  SoSFBool disable;

  SoEngineOutput timeOut;  // SoSFTime
  SoEngineOutput isActive; // SoSFBool
  SoEngineOutput ramp;     // SoSFFloat

protected:
  ~SoOneShot();

private:
  virtual void evaluate(void);
  virtual void inputChanged(SoField * which);
  virtual void writeInstance(SoOutput * out);

  SbTime holdduration;
  float holdramp;
  SbTime starttime;
  SbBool running;
};

#endif // !COIN_SOONESHOT_H
