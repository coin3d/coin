/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
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
  \class SoSelectOne SoSelectOne.h Inventor/engines/SoSelectOne.h
  \brief The SoSelectOne class is used to select one value from a set of values.
  \ingroup engines

  The output field will be the index'th value of the input multivalue
  field.
*/

#include <Inventor/engines/SoSelectOne.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/engines/SoSubEngineP.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/fields/SoFields.h>
#include <Inventor/lists/SoEngineOutputList.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  \var SoMField * SoSelectOne::input
  The multivalue input field which we will select a single value from
  for our output.
*/
/*!
  \var SoSFInt32 SoSelectOne::index
  Index of the value from the input field which will be put on the
  output.
*/
/*!
  \var SoEngineOutput * SoSelectOne::output
  (SoSField) This is the singlevalue field output containing the index'th
  value of SoSelectOne::input.

  The type of the field will of course match the type of the input field,
  i.e. if SoSelectOne::input is an SoMFFloat, SoSelectOne::output will
  be an SoSFFloat etc.
*/

// Can't use the standard SO_ENGINE_SOURCE macro, as this engine
// doesn't keep a class-global set of inputs and outputs: we need to
// make an instance of SoFieldData and SoEngineOutputData for every
// instance of the class, since the input and output fields are
// dynamically allocated.
SO_INTERNAL_ENGINE_SOURCE_DYNAMIC_IO(SoSelectOne);


/*!
  Constructor. Sets the type of the input field. The input field must
  be of type SoMField.
*/
SoSelectOne::SoSelectOne(SoType inputtype)
{
  this->input = NULL;
  if (!this->initialize(inputtype)) {
#if COIN_DEBUG
    SoDebugError::post("SoSelectOne::SoSelectOne",
                       "invalid type '%s' for input field",
                       inputtype == SoType::badType() ? "badType" :
                       inputtype.getName().getString());
#endif // COIN_DEBUG
  }
}

// Default constructor. Leaves engine in invalid state. Should only be
// used from import code or copy code.
SoSelectOne::SoSelectOne(void)
{
  this->input = NULL;
}

// Set up the input and output fields of the engine. This is done from
// either the non-default constructor or the readInstance() import
// code.
SbBool
SoSelectOne::initialize(const SoType inputfieldtype)
{
  assert(this->input == NULL);

  SO_ENGINE_INTERNAL_CONSTRUCTOR(SoSelectOne);
  SO_ENGINE_ADD_INPUT(index, (0));

  // Instead of SO_ENGINE_ADD_INPUT().
  if (inputfieldtype.isDerivedFrom(SoMField::getClassTypeId()) &&
      inputfieldtype.canCreateInstance()) {
    this->input = (SoMField *)inputfieldtype.createInstance();
    this->input->setNum(0);
    this->input->setContainer(this);
    this->dynamicinput = new SoFieldData(SoSelectOne::inputdata);
    this->dynamicinput->addField(this, "input", this->input);
  }
  else {
    return FALSE;
  }

  SbString multiname = inputfieldtype.getName().getString();
  // Built-in fields always start with the "MF", but we try to handle
  // user-defined fields aswell.
  char * ptr = strstr(multiname.getString(), "MF");
  assert(ptr != NULL && "invalid input field type");
  unsigned int offset = ptr - multiname.getString();
  SbString singlename = offset == 0 ? "" : multiname.getSubString(0, offset-1);
  singlename += 'S';
  singlename += multiname.getSubString(offset + 1);

  SoType outputtype = SoType::fromName(singlename);
  assert(outputtype != SoType::badType() &&
         outputtype.isDerivedFrom(SoSField::getClassTypeId()) &&
         "invalid input field type");

  // Instead of SO_ENGINE_ADD_OUTPUT().
  this->output = new SoEngineOutput;
  this->dynamicoutput = new SoEngineOutputData(SoSelectOne::outputdata);
  this->dynamicoutput->addOutput(this, "output", this->output, outputtype);
  this->output->setContainer(this);

  return TRUE;
}

// overloaded from parent
void
SoSelectOne::initClass(void)
{
  SO_ENGINE_INTERNAL_INIT_CLASS(SoSelectOne);
}

SoSelectOne::~SoSelectOne()
{
  delete this->dynamicinput;
  delete this->dynamicoutput;

  delete this->input;
  delete this->output;
}

// overloaded from parent
void
SoSelectOne::evaluate(void)
{
  int idx = this->index.getValue();

  if (idx == 0 && this->input->getNum() == 0) {
    // Nil is the no-op value (also the default initial value).
    SO_ENGINE_OUTPUT((*output), SoSField, setDirty(FALSE));
  }
  else if (idx >= 0 && idx < this->input->getNum()) {
    // FIXME: this is a _very_ suboptimal way of doing the
    // SoMFField->SoSFField conversion. 20000919 mortene.
    SbString valuestring;
    this->input->get1(idx, valuestring);
    SO_ENGINE_OUTPUT((*output), SoSField, set(valuestring.getString()));
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSelectOne::evaluate", "invalid index %d", idx);
  }
#endif // COIN_DEBUG
}

// overloaded from parent
SbBool
SoSelectOne::readInstance(SoInput * in, unsigned short flags)
{
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

// overloaded from parent
void
SoSelectOne::writeInstance(SoOutput * out)
{
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

// overloaded from parent
void
SoSelectOne::copyContents(const SoFieldContainer * from,
                          SbBool copyconnections)
{
  this->initialize(((SoSelectOne *)from)->input->getTypeId());
  inherited::copyContents(from, copyconnections);
}
