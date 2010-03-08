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

// IMPORTANT NOTE: this file should not be compiled/linked into Coin,
// it was put here to run the C++ compiler over the C++ wrapper headers
// for syntax/sanity checking...

#include <Inventor/threads/SbThread.h>
#include <Inventor/threads/SbMutex.h>
#include <Inventor/threads/SbRWMutex.h>
#include <Inventor/threads/SbCondVar.h>
/* #include <Inventor/threads/SbBarrier.h> */
#include <Inventor/threads/SbStorage.h>
#include <Inventor/threads/SbTypedStorage.h>
#include <Inventor/threads/SbFifo.h>

int
main(int argc, char ** argv)
{
  return 0;
}
