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

#ifndef COIN_SOVRMLINTERPOUTPUTLIST_H
#define COIN_SOVRMLINTERPOUTPUTLIST_H

#include <Inventor/lists/SbList.h>

class SoVRMLInterpOutput;


class COIN_DLL_EXPORT SoVRMLInterpOutputList : public SbList<SoVRMLInterpOutput *> {
public:
  SoVRMLInterpOutputList(void) : SbList<SoVRMLInterpOutput *>() { }
  SoVRMLInterpOutputList(const int sizehint) : SbList<SoVRMLInterpOutput *>(sizehint) { }
  SoVRMLInterpOutputList(const SoVRMLInterpOutputList & l) : SbList<SoVRMLInterpOutput *>(l) { }
};

#endif // !COIN_SOVRMLINTERPOUTPUTLIST_H
