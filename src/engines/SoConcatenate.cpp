/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoConcatenate SoConcatenate.h Inventor/engines/SoConcatenate.h
  \brief The SoConcatenate class is used to concatenate several inputs into one output.
  \ingroup engines

  Takes all the values from the 10 input multivalue fields in turn and
  concatenates them into the multivalue output.


  Note that this engine's output field deviates a little from the
  "standard" output mechanism of the majority of engine classes: the
  SoConcatenate::output is not a permanent SoEngineOutput instance,
  but a \e pointer to a SoEngineOutput instance.  The reason for this
  is that it is necessary to allocate the output field dynamically to
  make it match what the SoConcatenate::input is connected to since
  the type of the SoConcatenate::output always should be the same as
  the type of the SoConcatenate::input.
*/

#include <Inventor/engines/SoConcatenate.h>
#include <Inventor/engines/SoSubEngineP.h>

#include <Inventor/SbString.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/lists/SoEngineOutputList.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  \var SoMField * SoConcatenate::input[10]
  The multivalue input fields which we will concatenate into the
  output.
*/
/*!
  \var SoEngineOutput * SoConcatenate::output

  (SoMField) This is the field output containing the concatenated
  values of all the input fields.

  The type of the field will of course match the type of the input
  field.
*/

// Can't use the standard SO_ENGINE_SOURCE macro, as this engine
// doesn't keep a class-global set of inputs and outputs: we need to
// make an instance of SoFieldData and SoEngineOutputData for every
// instance of the class, since the input and output fields are
// dynamically allocated.
SO_INTERNAL_ENGINE_SOURCE_DYNAMIC_IO(SoConcatenate);


/**************************************************************************/

// Default constructor. Leaves engine in invalid state. Should only be
// used from import code or copy code.
SoConcatenate::SoConcatenate(void)
{
  this->input[0] = NULL;
}

/*!
  Constructor. The type of the input/output is specified in \a type.
*/
SoConcatenate::SoConcatenate(SoType type)
{
  this->input[0] = NULL;
  if (!this->initialize(type)) {
#if COIN_DEBUG
    SoDebugError::post("SoConcatenate::SoConcatenate",
                       "invalid type '%s' for input fields",
                       type == SoType::badType() ? "badType" :
                       type.getName().getString());
#endif // COIN_DEBUG
  }
}


// doc from parent
void
SoConcatenate::initClass(void)
{
  SO_ENGINE_INTERNAL_INIT_CLASS(SoConcatenate);
}

// Set up the input and output fields of the engine. This is done from
// either the non-default constructor or the readInstance() import
// code.
SbBool
SoConcatenate::initialize(const SoType inputfieldtype)
{
  assert(this->input[0] == NULL);

  SO_ENGINE_INTERNAL_CONSTRUCTOR(SoConcatenate);

  if (inputfieldtype.isDerivedFrom(SoMField::getClassTypeId()) &&
      inputfieldtype.canCreateInstance()) {
    // Instead of SO_ENGINE_ADD_INPUT().
    this->dynamicinput = new SoFieldData(SoConcatenate::inputdata);
    for (int i=0; i < SoConcatenate::NUMINPUTS; i++) {
      this->input[i] = (SoMField *)inputfieldtype.createInstance();
      this->input[i]->setNum(0);
      this->input[i]->setContainer(this);
      SbString s = "input";
      s.addIntString(i);
      this->dynamicinput->addField(this, s.getString(), this->input[i]);
    }
  }
  else {
    return FALSE;
  }

  // Instead of SO_ENGINE_ADD_OUTPUT().
  this->output = new SoEngineOutput;
  this->dynamicoutput = new SoEngineOutputData(SoConcatenate::outputdata);
  this->dynamicoutput->addOutput(this, "output", this->output, inputfieldtype);
  this->output->setContainer(this);

  return TRUE;
}

/*!
  Destructor.
*/
SoConcatenate::~SoConcatenate()
{
  delete this->dynamicinput;
  delete this->dynamicoutput;

  for (int i=0; i < SoConcatenate::NUMINPUTS; i++) delete this->input[i];
  delete this->output;
}

/*!
  Overloaded to initialize type of input before reading.
*/
SbBool
SoConcatenate::readInstance(SoInput * in, unsigned short flags)
{
  // This code is identical to readInstance() of SoSelectOne and
  // SoGate, so migrate changes.

  SbName tmp;
  if (!in->read(tmp) || tmp != "type") {
    SoReadError::post(in, "\"type\" keyword is missing.");
    return FALSE;
  }
  SbName fieldname;
  if (!in->read(fieldname)) {
    SoReadError::post(in, "Couldn't read input type for engine.");
    return FALSE;
  }
  SoType inputtype = SoType::fromName(fieldname);
  if (!this->initialize(inputtype)) {
    SoReadError::post(in, "Type \"%s\" for input field is not valid.",
                      fieldname.getString());
    return FALSE;
  }

  return SoEngine::readInstance(in, flags);
}

/*!
  Overloaded to write type of inputs.
*/
void
SoConcatenate::writeInstance(SoOutput * out)
{
  // This code is identical to writeInstance() of SoSelectOne and
  // SoGate, so migrate changes.

  if (this->writeHeader(out, FALSE, TRUE)) return;

  SbBool binarywrite = out->isBinary();

  if (!binarywrite) out->indent();
  out->write("type");
  if (!binarywrite) out->write(' ');
  out->write(this->input[0]->getTypeId().getName());
  if (binarywrite) out->write((unsigned int)0);
  else out->write('\n');

  this->getFieldData()->write(out, this);
  this->writeFooter(out);
}

// overloaded from parent
void
SoConcatenate::copyContents(const SoFieldContainer * from,
                            SbBool copyconnections)
{
  this->initialize(((SoConcatenate *)from)->input[0]->getTypeId());
  inherited::copyContents(from, copyconnections);
}

// documented in superclass
void
SoConcatenate::evaluate(void)
{
  int n = 0, i, j;
  SbString value;

  for (i = 0; i < SoConcatenate::NUMINPUTS; i++)
    n += this->input[i]->getNum();

  SO_ENGINE_OUTPUT((*output),SoMField,setNum(n));

  n = 0;
  for (i = 0; i < SoConcatenate::NUMINPUTS; i++) {
    for (j = 0; j < this->input[i]->getNum(); j++) {
      this->input[i]->get1(j, value);
      // FIXME: this is an amazingly ineffective way of copying the
      // values. 20000920 mortene.
      SO_ENGINE_OUTPUT((*output), SoMField, set1(n, value.getString()));
      n++;
    }
  }
}
