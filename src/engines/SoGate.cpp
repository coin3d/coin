/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoGate SoGate.h Inventor/engines/SoGate.h
  \brief The SoGate class is used to selectively copy values from input to output.
  \ingroup engines

  This engine will forward values from the SoGate::input field to the
  SoGate::output field when the SoGate::enable field is \c TRUE.


  Note that this engine's output field deviates a little from the
  "standard" output mechanism of the majority of engine classes: the
  SoGate::output is not a permanent SoEngineOutput instance, but a \e
  pointer to a SoEngineOutput instance.  The reason for this is that
  it is necessary to allocate the output field dynamically to make it
  match what the SoGate::input is connected to since the type of the
  SoGate::output always should be the same as the type of the
  SoGate::input.
*/

#include <Inventor/engines/SoGate.h>
#include <Inventor/engines/SoSubEngineP.h>

#include <Inventor/SbString.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <assert.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


/*!
  \var SoMField * SoGate::input
  The multivalue input field which we will forward to the output when
  SoGate::enable is \c TRUE.
*/
/*!
  \var SoSFBool SoGate::enable
  Set whether or not to forward from input to output field.
*/
/*!
  \var SoSFTrigger SoGate::trigger
  Copy the current values of the input field once to the output field.
*/
/*!
  \var SoEngineOutput * SoGate::output

  (SoMField) This is the field output containing the values of
  SoGate::input.

  The type of the field will of course match the type of the input
  field.
*/

// Can't use the standard SO_ENGINE_SOURCE macro, as this engine
// doesn't keep a class-global set of inputs and outputs: we need to
// make an instance of SoFieldData and SoEngineOutputData for every
// instance of the class, since the input and output fields are
// dynamically allocated.
SO_INTERNAL_ENGINE_SOURCE_DYNAMIC_IO(SoGate);


/**************************************************************************/

// Default constructor. Leaves engine in invalid state. Should only be
// used from import code or copy code.
SoGate::SoGate(void)
{
  this->input = NULL;
}

/*!
  Constructor. The type of the input/output is specified in \a type.
*/
SoGate::SoGate(SoType type)
{
  this->input = NULL;
  if (!this->initialize(type)) {
#if COIN_DEBUG
    SoDebugError::post("SoGate::SoGate",
                       "invalid type '%s' for input field",
                       type == SoType::badType() ? "badType" :
                       type.getName().getString());
#endif // COIN_DEBUG
  }
}


// doc from parent
void
SoGate::initClass(void)
{
  SO_ENGINE_INTERNAL_INIT_CLASS(SoGate);
}

// Set up the input and output fields of the engine. This is done from
// either the non-default constructor or the readInstance() import
// code.
SbBool
SoGate::initialize(const SoType inputfieldtype)
{
  assert(this->input == NULL);

  SO_ENGINE_INTERNAL_CONSTRUCTOR(SoGate);
  SO_ENGINE_ADD_INPUT(trigger, ());
  SO_ENGINE_ADD_INPUT(enable, (FALSE));

  if (inputfieldtype.isDerivedFrom(SoMField::getClassTypeId()) &&
      inputfieldtype.canCreateInstance()) {
    // Instead of SO_ENGINE_ADD_INPUT().
    this->input = (SoMField *)inputfieldtype.createInstance();
    this->input->setNum(0);
    this->input->setContainer(this);
    this->dynamicinput = new SoFieldData(SoGate::inputdata);
    this->dynamicinput->addField(this, "input", this->input);
  }
  else {
    return FALSE;
  }

  // Instead of SO_ENGINE_ADD_OUTPUT().
  this->output = new SoEngineOutput;
  this->dynamicoutput = new SoEngineOutputData(SoGate::outputdata);
  this->dynamicoutput->addOutput(this, "output", this->output, inputfieldtype);
  this->output->setContainer(this);

  return TRUE;
}

/*!
  Destructor.
*/
SoGate::~SoGate()
{
  delete this->dynamicinput;
  delete this->dynamicoutput;
  delete this->input;
  delete this->output;
}

// doc from parent
void
SoGate::evaluate(void)
{
  // Force update of slave fields.
  this->output->enable(TRUE);

  // FIXME: this is an amazingly ineffective way of copying the
  // values. 20000920 mortene.
  SbString valuestring;
  this->input->get(valuestring);
  SO_ENGINE_OUTPUT((*output), SoField, set(valuestring.getString()));

  // No more updates until either the SoGate::enable field or the
  // SoGate::trigger field is touched.
  if (!this->enable.getValue()) this->output->enable(FALSE);
}

// doc from parent
void
SoGate::inputChanged(SoField * which)
{
  if (which == &this->enable) {
    SbBool enableval = this->enable.getValue();
    if (this->output->isEnabled() != enableval)
      this->output->enable(enableval);
  }
  else if (which == &this->trigger) {
    this->output->enable(TRUE);
  }
  // Changes to the input field are handled automatically according to
  // the value of the SoGate::enable field.
}

// Documented in superclass. Overridden to initialize type of gate
// before reading.
SbBool
SoGate::readInstance(SoInput * in, unsigned short flags)
{
  // This code is identical to readInstance() of SoSelectOne and
  // SoConcatenate, so migrate changes.

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

// Documented in superclass. Overridden to write type of gate.
void
SoGate::writeInstance(SoOutput * out)
{
  // This code is identical to writeInstance() of SoSelectOne and
  // SoConcatenate, so migrate changes.

  if (this->writeHeader(out, FALSE, TRUE)) return;

  SbBool binarywrite = out->isBinary();

  if (!binarywrite) out->indent();
  out->write("type");
  if (!binarywrite) out->write(' ');
  out->write(this->input->getTypeId().getName());
  if (binarywrite) out->write((unsigned int)0);
  else out->write('\n');

  this->getFieldData()->write(out, this);
  this->writeFooter(out);
}

// Documented in superclass.
void
SoGate::copyContents(const SoFieldContainer * from, SbBool copyconnections)
{
  this->initialize(((SoGate *)from)->input->getTypeId());
  inherited::copyContents(from, copyconnections);
}
