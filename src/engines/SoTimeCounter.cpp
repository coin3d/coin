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

/*!
  \class SoTimeCounter SoTimeCounter.h Inventor/engines/SoTimeCounter.h
  \brief The SoTimeCounter class is an integer counter engine.
  \ingroup engines

  FIXME: doc
*/

#include <Inventor/engines/SoTimeCounter.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/SoDB.h>
#include <coindefs.h>

SO_ENGINE_SOURCE(SoTimeCounter);

/*!
  Default constructor.
*/
SoTimeCounter::SoTimeCounter()
{
  SO_ENGINE_CONSTRUCTOR(SoTimeCounter);
  
  SO_ENGINE_ADD_INPUT(timeIn, (SbTime::zero()));
  SO_ENGINE_ADD_INPUT(min, (0));
  SO_ENGINE_ADD_INPUT(max, (1));
  SO_ENGINE_ADD_INPUT(step, (1));
  SO_ENGINE_ADD_INPUT(on, (TRUE));
  SO_ENGINE_ADD_INPUT(frequency, (1.0f));
  SO_ENGINE_ADD_INPUT(duty, (1.0f));
  SO_ENGINE_ADD_INPUT(reset, (0));
  SO_ENGINE_ADD_INPUT(syncIn, ());

  SO_ENGINE_ADD_OUTPUT(output, SoSFShort);
  SO_ENGINE_ADD_OUTPUT(syncOut, SoSFTrigger);

  SoField *realtime=SoDB::getGlobalField("realTime");
  this->timeIn.connectFrom(realtime);
}

// overloaded from parent
void
SoTimeCounter::initClass()
{
  SO_ENGINE_INTERNAL_INIT_CLASS(SoTimeCounter);
}


// private destructor
SoTimeCounter::~SoTimeCounter()
{
}

// doc in parent
void
SoTimeCounter::evaluate()
{
  COIN_STUB();
}

// doc in parent
void
SoTimeCounter::inputChanged(SoField *which)
{
  COIN_STUB();
}
