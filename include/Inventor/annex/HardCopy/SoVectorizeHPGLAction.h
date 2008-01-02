#ifndef COIN_SOVECTORIZEHPGLACTION_H
#define COIN_SOVECTORIZEHPGLACTION_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2008 by Kongsberg SIM.  All rights reserved.
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

// *************************************************************************

#include <Inventor/annex/HardCopy/SoVectorizeAction.h>
#include <Inventor/annex/HardCopy/SoHPGLVectorOutput.h>

class SoVectorizeHPGLActionP;

// *************************************************************************

class COIN_DLL_API SoVectorizeHPGLAction : public SoVectorizeAction {

  SO_ACTION_HEADER(SoVectorizeHPGLAction);

public:
  SoVectorizeHPGLAction(void);
  virtual ~SoVectorizeHPGLAction();

  static void initClass(void);

  SoHPGLVectorOutput * getOutput(void) const;
  SoHPGLVectorOutput * getHPGLVectorOutput(void) const;

private:
  SoVectorizeHPGLActionP * pimpl;
  friend class SoVectorizeHPGLActionP;
};

// *************************************************************************

#endif //!COIN_SOVECTORIZEHPGLACTION_H
