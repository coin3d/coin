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

#include <Inventor/engines/SoGate.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/engines/SoEngineOutput.h>

SO_ENGINE_ABSTRACT_SOURCE(SoGate);

SoGate::SoGate(SoType inputType)
{
  SO_ENGINE_CONSTRUCTOR(SoGate);

  this->input=(SoMField *)inputType.createInstance();
  SO_ENGINE_ADD_INPUT(enable,(FALSE));
  SO_ENGINE_ADD_INPUT(trigger,());

  //Instead of SO_ENGINE_ADD_OUTPUT()
  this->output=new SoEngineOutput;
  this->output->setType(inputType);
  this->output->setContainer(this);
  this->outputList->append(this->output);
}

void
SoGate::initClass()
{
  SO_ENGINE_INTERNAL_INIT_ABSTRACT_CLASS(SoGate);
}

//
// private members
//
SoGate::~SoGate()
{
  delete this->input;
  delete this->output;
}

void
SoGate::evaluate()
{
  SbString valueString;

  this->input->get(valueString);
  SO_ENGINE_OUTPUT((*output),SoField,set(valueString.getString()));
}

void
SoGate::inputChanged(SoField *which)
{
  if (which==&this->enable)
    this->output->enable(this->enable.getValue());
  else if (which==&this->trigger) {
    this->output->enable(TRUE);
    this->evaluate();
    this->output->enable(FALSE);
  }
}
