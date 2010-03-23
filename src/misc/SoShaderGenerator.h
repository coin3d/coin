#ifndef COIN_SOSHADERGENERATOR_H
#define COIN_SOSHADERGENERATOR_H

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

#ifndef COIN_INTERNAL
#error this is a private header file
#endif /* !COIN_INTERNAL */

#include <Inventor/SbString.h>

class SbName;

class SoShaderGenerator {
public:

  SoShaderGenerator(void);
  ~SoShaderGenerator();

  void reset(const SbBool freeoldstrings);

  void setVersion(const SbString & str);
  void addDefine(const SbString & str, const SbBool checkexists);
  void addDeclaration(const SbString & str, const SbBool checkexists);
  void addFunction(const SbString & str, const SbBool checkexists);
  void addNamedFunction(const SbName & name, const SbBool checkexists);
  
  void addMainStatement(const SbString & str);

  const SbString & getShaderProgram(void);

private:
  SbString version;
  SbString defines;
  SbString declarations;
  SbString functions;
  SbString main;
  
  SbBool dirty;
  SbString combined;
};

#endif // COIN_SOSHADERGENERATOR_H
