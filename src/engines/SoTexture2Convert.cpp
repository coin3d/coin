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

#include <Inventor/engines/SoTexture2Convert.h>
#include "engines/SoSubEngineP.h"

/*!
  \class SoTexture2Convert SoTexture2Convert.h Inventor/engines/SoTexture2Convert.h
  \brief Abstract base class for texture filtering engines.

  \ingroup engines
  \COIN_CLASS_EXTENSION
  \since Coin 3.0
*/

SO_ENGINE_ABSTRACT_SOURCE(SoTexture2Convert);

void
SoTexture2Convert::initClass(void)
{
  SO_ENGINE_INTERNAL_INIT_ABSTRACT_CLASS(SoTexture2Convert);
}

SoTexture2Convert::SoTexture2Convert(void)
{
  SO_ENGINE_INTERNAL_CONSTRUCTOR(SoTexture2Convert);

  SO_ENGINE_ADD_INPUT(sourceImage, (SbVec2s(0,0), 1, NULL));

  SO_ENGINE_ADD_OUTPUT(image, SoSFImage);
}
