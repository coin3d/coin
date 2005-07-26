#ifndef COIN_SOSCRIPTENGINE_H
#define COIN_SOSCRIPTENGINE_H

/**************************************************************************\
 *  
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *    
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/
  
#include <Inventor/SbBasic.h>
#include <Inventor/SbName.h>
#include <Inventor/fields/SoField.h>

class COIN_DLL_API SoScriptEngine {

public:
/*  SoScriptEngine(void) = 0;*/
//  virtual ~SoScriptEngine() = 0;

  // FIXME: should all these functions be const? 20050719 erikgors.
  // FIXME: what about the return value from executing a script?
  // 20050719 erikgors.
  virtual SbBool executeScript(const SbName & name, const SbString & script) const = 0;
  virtual SbBool executeFile(const SbName & filename) const = 0;
  virtual SbBool executeFunction(const SbName &name, int argc, const SoField * argv, SoField * rval=NULL) const = 0;

  virtual SbBool setScriptField(const SbName & name, const SoField * f) const = 0;
  virtual SbBool getScriptField(const SbName & name, SoField * f) const = 0;
  virtual SbBool unsetScriptField(const SbName & name) const = 0;
  virtual SbBool hasScriptField(const SbName & name) const = 0;
};

#endif // !COIN_SOSCRIPTENGINE_H
