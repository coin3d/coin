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

#ifndef __SOELAPSEDTIME_H__
#define __SOELAPSEDTIME_H__

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFTrigger.h>


class SoElapsedTime : public SoEngine {
  typedef SoEngine inherited;
  SO_ENGINE_HEADER(SoElapsedTime);

public:
  SoSFTime timeIn;
  SoSFFloat speed;
  SoSFBool on;
  SoSFBool pause;
  SoSFTrigger reset;

  SoEngineOutput timeOut; // SoSFTime

  SoElapsedTime();

  static void initClass();

protected:
  ~SoElapsedTime();

private:
  virtual void evaluate();
  virtual void inputChanged(SoField * which);

  SbTime pauseTime;
  SbTime lastTime;
  SbTime currTime;
  enum {RUNNING, STOPPED, PAUSED} status;
  SbBool firstTime;
};

#endif // !__SOELAPSEDTIME_H__
