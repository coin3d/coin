/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

#ifndef COIN_SOTRIGGERANY_H
#define COIN_SOTRIGGERANY_H

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/fields/SoSFTrigger.h>


class COIN_DLL_API SoTriggerAny : public SoEngine {
  typedef SoEngine inherited;

  SO_ENGINE_HEADER(SoTriggerAny);

public:
  static void initClass(void);
  SoTriggerAny(void);

  SoSFTrigger input0;
  SoSFTrigger input1;
  SoSFTrigger input2;
  SoSFTrigger input3;
  SoSFTrigger input4;
  SoSFTrigger input5;
  SoSFTrigger input6;
  SoSFTrigger input7;
  SoSFTrigger input8;
  SoSFTrigger input9;

  SoEngineOutput output;  // SoSFTrigger

protected:
  ~SoTriggerAny();

private:
  virtual void evaluate(void);
};

#endif // !COIN_SOTRIGGERANY_H
