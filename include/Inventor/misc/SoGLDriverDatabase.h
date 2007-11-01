#ifndef SOGLDRIVERDATABASE_H
#define SOGLDRIVERDATABASE_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2007 by Systems in Motion.  All rights reserved.
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
 *  See http://www.coin3d.org/ for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/C/glue/gl.h>
#include <Inventor/lists/SbList.h>

class SoGLDriver;
class SoGLDriverDatabaseP;
class SbName;

class SoGLDriverDatabase {
public:
  static SbBool isSupported(const cc_glglue * context, const SbName & feature);
  static SbBool isBroken(const cc_glglue * context, const SbName & feature);
  static SbBool isSlow(const cc_glglue * context, const SbName & feature);
  static SbBool isFast(const cc_glglue * context, const SbName & feature);

private:
  static SoGLDriverDatabaseP * pimpl(void);
};

#endif // SOGLDRIVERDATABASE_H
