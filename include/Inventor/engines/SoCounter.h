/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

#ifndef __SOCOUNTER_H__
#define __SOCOUNTER_H__

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/fields/SoSFShort.h>

#if defined(COIN_EXCLUDE_SOCOUNTER)
#error "Configuration settings disrespected -- do not include this file!"
#endif // COIN_EXCLUDE_SOCOUNTER

class SoCounter : public SoEngine {
  typedef SoEngine inherited;
  SO_ENGINE_HEADER(SoCounter);

public:
  SoSFShort min;
  SoSFShort max;
  SoSFShort step;
  SoSFTrigger trigger;
  SoSFShort reset;

  SoEngineOutput output;  //SoSFShort
  SoEngineOutput syncOut; //SoSFTrigger

  SoCounter();

  static void initClass();

private:
  virtual ~SoCounter();
  virtual void evaluate();
  virtual void inputChanged(SoField * which);

  int numSteps;
  short value;
};

#endif // !__SOCOUNTER_H__
