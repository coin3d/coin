#ifndef COIN_SOPROFILERTOPENGINE_H
#define COIN_SOPROFILERTOPENGINE_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/engines/SoEngine.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoMFName.h>
#include <Inventor/fields/SoMFEnum.h>
#include <Inventor/fields/SoMFTime.h>
#include <Inventor/fields/SoMFUInt32.h>
#include <Inventor/tools/SbPimplPtr.h>

class SoProfilerTopEngineP;

class COIN_DLL_API SoProfilerTopEngine : public SoEngine {
  typedef SoEngine inherited;
  SO_ENGINE_HEADER(SoProfilerTopEngine);

public:
  static void initClass(void);
  SoProfilerTopEngine(void);

  enum Column {
    NAME,
    COUNT,
    TIME_SECS,
    TIME_SECS_MAX,
    TIME_SECS_AVG,
    TIME_MSECS,
    TIME_MSECS_MAX,
    TIME_MSECS_AVG,
    TIME_PERCENT,
    TIME_PERCENT_MAX,
    TIME_PERCENT_AVG
  };

  enum SortOrder {
    TIME_DEC,
    TIME_MAX_DEC,
    TIME_AVG_DEC,
    COUNT_DEC_TIME_DEC,
    COUNT_DEC_TIME_MAX_DEC,
    COUNT_DEC_TIME_AVG_DEC,
    ALPHANUMERIC_INC
  };

  // inputs
  SoMFName statisticsNames;
  SoMFTime statisticsTimings;
  SoMFTime statisticsTimingsMax;
  SoMFUInt32 statisticsCounts;

  // output-related
  SoMFEnum columns;   // [ NAME, COUNT, TIME_MSECS, TIME_PERCENT ]
  SoSFEnum sortOrder; // TIME_DEC
  SoSFInt32 maxLines; // 16
  SoSFFloat decay;    // 0.0  ([0.0 - 1.0>)

  // result
  SoEngineOutput prettyText; // SoMFString

protected:
  virtual ~SoProfilerTopEngine(void);

  virtual void evaluate(void);

private:
  SbPimplPtr<SoProfilerTopEngineP> pimpl;

  SoProfilerTopEngine(const SoProfilerTopEngine & rhs); // disable
  SoProfilerTopEngine & operator = (const SoProfilerTopEngine & rhs); // disable

}; // SoProfilerTopEngine

#endif // !COIN_SOPROFILERTOPENGINE_H
