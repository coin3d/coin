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
  enum Flags {RETRIGGERABLE=1, HOLD_FINAL=2};

  SoSFTime timeIn;
  SoSFTime duration;
  SoSFTrigger trigger;
  SoSFBitMask flags;
  SoSFBool disable;

  SoEngineOutput timeOut;  //SoSFTime
  SoEngineOutput isActive; //SoSFBool
  SoEngineOutput ramp;     //SoSFFloat

  SoOneShot();

  static void initClass();

protected:
  ~SoOneShot();

private:
  virtual void evaluate();
  virtual void inputChanged(SoField * which);

  SbTime holdDuration;
  SbTime startTime;
  SbBool running;
};

#endif // !COIN_SOONESHOT_H
