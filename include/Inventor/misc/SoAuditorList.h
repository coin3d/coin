#ifndef COIN_MISC_SOAUDITORLIST_H
#define COIN_MISC_SOAUDITORLIST_H

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

// NB: make sure the ifdef-test above wrapping this includefile is
// _not_ checking on and setting the same define-string as the other
// SoAuditorList.h file in the include/Inventor/lists/ directory.

// This include file is part of the Coin library solely for purposes
// of being backwards compatible with Open Inventor.

// The real class definition include file has been moved to the lists
// directory.
#if defined(COIN_INTERNAL)
#error Do not include Inventor/misc/SoAuditorList.h internally (use Inventor/lists/SoAuditorList.h).
#endif // COIN_INTERNAL
#include <Inventor/lists/SoAuditorList.h>

#endif // !COIN_MISC_SOAUDITORLIST_H
