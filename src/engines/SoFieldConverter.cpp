/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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

#include <Inventor/engines/SoFieldConverter.h>
#if !defined(COIN_EXCLUDE_SOCONVERTALL)
#include <Inventor/engines/SoConvertAll.h>
#endif // !COIN_EXCLUDE_SOCONVERTALL

SO_ENGINE_ABSTRACT_SOURCE(SoFieldConverter);

SoFieldConverter::SoFieldConverter()
{
  this->connectedInput=NULL;
}

void
SoFieldConverter::initClass()
{
  SO_ENGINE_INIT_ABSTRACT_CLASS(SoFieldConverter,SoEngine,"Engine")
  SoFieldConverter::initConverters();
}

void
SoFieldConverter::initConverters()
{
#if !defined(COIN_EXCLUDE_SOCONVERTALL)
  SoConvertAll::initClass();
#endif // !COIN_EXCLUDE_SOCONVERTALL
}

SoFieldConverter::~SoFieldConverter()
{
}

void
SoFieldConverter::setConnectedInput(SoField *input)
{
  this->connectedInput=input;
}

SoField *
SoFieldConverter::getConnectedInput()
{
  return this->connectedInput;
}
