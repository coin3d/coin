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
  \class SoGate SoGate.h Inventor/engines/SoGate.h
  \brief The SoGate class is used to selectively copy values from input to output.
  \ingroup engines

  FIXME: doc
*/

#include <Inventor/engines/SoGate.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/SbString.h>
#include <coindefs.h>


SO_ENGINE_SOURCE(SoGate);


/*!
  Default constructor.
*/
SoGate::SoGate(SoType inputtype)
{
  SO_ENGINE_CONSTRUCTOR(SoGate);

  this->input = (SoMField *)inputtype.createInstance();
  SO_ENGINE_ADD_INPUT(enable,(FALSE));
  SO_ENGINE_ADD_INPUT(trigger,());

  //Instead of SO_ENGINE_ADD_OUTPUT()
  this->output=new SoEngineOutput;
#if 0 // obsoleted. FIXME: reimplement 20000309 pederb
  this->output->setType(inputtype);
#endif // obsolete
  this->output->setContainer(this);
#if 0 // FIXME: old kintel code. reimplement. pederb, 20000309
  this->outputList->append(this->output);
#endif
}

// overloaded from parent
void
SoGate::initClass()
{
  SO_ENGINE_INTERNAL_INIT_CLASS(SoGate);
}


//
// private members
//

// dummy default constructor
SoGate::SoGate(void)
{
  assert(FALSE && "default constructor shouldn't be used");
  // FIXME: ..or used when reading engine from file? 20000324 mortene.
}

SoGate::~SoGate()
{
  delete this->input;
  delete this->output;
}

// overloaded from parent
void
SoGate::evaluate()
{
  SbString valueString;

  this->input->get(valueString);
  SO_ENGINE_OUTPUT((*output),SoField,set(valueString.getString()));
}

// overloaded from parent
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
