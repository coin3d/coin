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
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

#ifndef __SOCALCULATOR_H__
#define __SOCALCULATOR_H__

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/fields/SoMFFloat.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoMFString.h>

#if defined(COIN_EXCLUDE_SOCALCULATOR)
#error "Configuration settings disrespected!"
#endif // COIN_EXCLUDE_SOCALCULATOR

class SoCalculator : public SoEngine {
  typedef SoEngine inherited;

  SO_ENGINE_HEADER( SoCalculator );
  
public:
  // inputs
  SoMFFloat a, b, c, d, e, f, g, h;
  SoMFVec3f A, B, C, D, E, F, G, H;
  SoMFString expression;

  // outputs
  SoEngineOutput oa, ob, oc, od; // (SoMFFloat)
  SoEngineOutput oA, oB, oC, oD; // (SoMFVec3f)

  SoCalculator(void);

public:
  static void initClass(void);

protected:
  virtual void inputChanged(SoField * which);
  virtual ~SoCalculator(void);

private:
  virtual void evaluate(void);
};

#endif // !__SOCALCULATOR_H__
