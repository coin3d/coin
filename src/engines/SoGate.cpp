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
#include <Inventor/errors/SoReadError.h>
#include <assert.h>
#include <Inventor/engines/SoSubEngineP.h>

/*!
  Default constructor. Used when reading engine from file.
*/
SoGate::SoGate(void)
{
  this->commonConstructor();
}

/*!
  Constructor. The type of the input/output is specified in \a type.
*/
SoGate::SoGate(SoType type)
{
  this->commonConstructor();
  this->initInputOutput(type);
}


// doc from parent
void
SoGate::initClass()
{
  SO_ENGINE_INTERNAL_INIT_CLASS(SoGate);
}

void
SoGate::commonConstructor(void)
{
  this->gateInputData = new SoFieldData;
  this->gateOutputData = new SoEngineOutputData;

  this->enable.setValue(FALSE);
  this->enable.setContainer(this);
  this->trigger.setContainer(this);
  this->typeField.setValue(SbName(""));
  this->typeField.setContainer(this);
  this->gateInputData->addField(this, "enable", &this->enable);
  this->gateInputData->addField(this, "trigger", &this->trigger);
  this->gateInputData->addField(this, "type", &this->typeField);

  this->output = new SoEngineOutput;
  this->output->enable(this->enable.getValue());
  this->output->setContainer(this);
}

// called from constructor or readInstance() to init input/output
void
SoGate::initInputOutput(const SoType type)
{
  this->input = (SoMField *) type.createInstance();
  this->input->setNum(0);
  this->input->setContainer(this);

  this->gateInputData->addField(this, "input", this->input);
  this->gateOutputData->addOutput(this, "output", this->output, type);
}

/*!
  Destructor.
*/
SoGate::~SoGate()
{
  delete this->gateInputData;
  delete this->gateOutputData;
  delete this->input;
  delete this->output;
}

// doc from parent
void
SoGate::evaluate()
{
  SbString valueString;

  this->input->get(valueString);
  SO_ENGINE_OUTPUT((*output),SoField,set(valueString.getString()));
}

// doc from parent
void
SoGate::inputChanged(SoField *which)
{
  if (which == &this->enable) {
    this->output->enable(this->enable.getValue());
    if (this->enable.getValue()) this->evaluate();
  }
  else if (which == &this->trigger) {
    this->output->enable(TRUE);
    this->evaluate();
    this->output->enable(FALSE);
  }
}

/*!
  Overloaded to initialize type of gate after reading.
*/
SbBool
SoGate::readInstance(SoInput *in, unsigned short flags)
{
  this->typeField.setValue(SbName(""));
  SbBool ret = inherited::readInstance(in, flags);
  if (ret) {
    if (this->typeField.getValue() != SbName("")) {
      SoType type = SoType::fromName(this->typeField.getValue());
      if (type.isDerivedFrom(SoMField::getClassTypeId())) {
        this->initInputOutput(type);
      }
      else if (type != SoType::badType()) {
        SoReadError::post(in,  "The type ``%s'' is not a type of MField", type.getName().getString());
        ret = FALSE;
      }
    }
    else {
      SoReadError::post(in, "SoGate is missing type field");
      ret = FALSE;
    }
  }
  return ret;
}

/*!
  Overloaded to write type of gate.
*/
void
SoGate::writeInstance(SoOutput *out)
{
  this->typeField.setValue(this->input->getClassTypeId().getName());
  inherited::writeInstance(out);
}

/*
 * We cannot use the SO_ENGINE_SOURCE macro, since we need to make
 * an instance of SoFieldData and SoEngineOutputData for every
 * SoGate instance, since the input and output is allocated in the
 * constructor. This makes it impossible to inherit this class,
 * but I guess that is the case in OIV too. pederb, 20000331
 */

SoType SoGate::classTypeId = SoType::badType();
SoType SoGate::getClassTypeId(void) { return SoGate::classTypeId; }
SoType SoGate::getTypeId(void) const { return SoGate::classTypeId; }

unsigned int SoGate::classinstances = 0;
SoFieldData * SoGate::inputdata = NULL;
const SoFieldData ** SoGate::parentinputdata = NULL;
SoEngineOutputData * SoGate::outputdata = NULL;
const SoEngineOutputData ** SoGate::parentoutputdata = NULL;

const SoFieldData **
SoGate::getInputDataPtr(void)
{
  assert(0 && "no static fielddata available for SoGate");
  return NULL;
}

const SoFieldData *
SoGate::getFieldData(void) const
{
  return this->gateInputData;
}

const SoEngineOutputData **
SoGate::getOutputDataPtr(void)
{
  assert(0 && "no static outputdata available for SoGate");
  return NULL;
}

const SoEngineOutputData *
SoGate::getOutputData(void) const
{
  return this->gateOutputData;
}

void *
SoGate::createInstance(void)
{
  return new SoGate;
}
