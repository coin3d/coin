#ifndef COIN_SCXML_H
#define COIN_SCXML_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2009 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/SbBasic.h>
#include <Inventor/SoType.h>

// xmlns values (only document root scxml element is checked)

// default (and fallback) - uses just the base types
#define SCXML_DEFAULT_NS "http://www.coin3d.org/scxml/default/1.0"

// namespace value that integrates with Coin types
#define SCXML_COIN_NS "http://www.coin3d.org/scxml/coin/1.0"

class ScXMLStateMachine;

class COIN_DLL_API ScXML {
public:
  static void initClasses(void);

  static ScXMLStateMachine * readFile(const char * filename);
  static ScXMLStateMachine * readBuffer(const char * bufferdata);

}; // ScXML

#endif // !COIN_SCXML_H
