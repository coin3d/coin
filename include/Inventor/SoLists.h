/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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

#ifndef COIN_SOLISTS_H
#define COIN_SOLISTS_H

#if defined(COIN_INTERNAL)
#error "Do not include SoLists.h internally."
#endif // COIN_INTERNAL

// This file's sole purpose is to provide compatibility with the way
// Open Inventor lays out its header files for the list classes.

#include <Inventor/lists/SbIntList.h>
#include <Inventor/lists/SbPList.h>
#include <Inventor/lists/SoBaseList.h>
#include <Inventor/lists/SoEnabledElementsList.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/lists/SoFieldList.h>
#include <Inventor/lists/SoNodeList.h>
#include <Inventor/lists/SoTypeList.h>

#endif // !COIN_SOLISTS_H
