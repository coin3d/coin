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
  this->initialize(inputtype);
}

// Default constructor. Leaves engine in invalid state. Should only be
// used from import code or copy code.
SoSelectOne::SoSelectOne(void)
{
}

// Set up the input and output fields of the engine. This is done from
// either the non-default constructor or the readInstance() import
// code.
void
SoSelectOne::initialize(const SoType inputfieldtype)
{
  // FIXME: be robust wrt invalid types. 20000919 mortene.

  SO_ENGINE_INTERNAL_CONSTRUCTOR(SoSelectOne);
  SO_ENGINE_ADD_INPUT(index, (0));

  // Instead of SO_ENGINE_ADD_INPUT().
  this->input = (SoMField *)inputfieldtype.createInstance();
  this->input->setNum(0);
  this->input->setContainer(this);
  this->dynamicinput = new SoFieldData(SoSelectOne::inputdata);
  this->dynamicinput->addField(this, "input", this->input);

  // FIXME: couldn't this be extracted by the use of
  // SoMField::getClassTypeId().getAllDerivedFrom() or something?
  // 19990523 mortene.
  SoType types[42]={
    SoMFBitMask::getClassTypeId(),SoSFBitMask::getClassTypeId(),
    SoMFBool::getClassTypeId(),SoSFBool::getClassTypeId(),
    SoMFColor::getClassTypeId(),SoSFColor::getClassTypeId(),
    SoMFEngine::getClassTypeId(),SoSFEngine::getClassTypeId(),
    SoMFEnum::getClassTypeId(),SoSFEnum::getClassTypeId(),
    SoMFFloat::getClassTypeId(),SoSFFloat::getClassTypeId(),
    SoMFInt32::getClassTypeId(),SoSFInt32::getClassTypeId(),
    SoMFMatrix::getClassTypeId(),SoSFMatrix::getClassTypeId(),
    SoMFName::getClassTypeId(),SoSFName::getClassTypeId(),
    SoMFNode::getClassTypeId(),SoSFNode::getClassTypeId(),
    SoMFPath::getClassTypeId(),SoSFPath::getClassTypeId(),
    SoMFPlane::getClassTypeId(),SoSFPlane::getClassTypeId(),
    SoMFRotation::getClassTypeId(),SoSFRotation::getClassTypeId(),
    SoMFShort::getClassTypeId(),SoSFShort::getClassTypeId(),
    SoMFString::getClassTypeId(),SoSFString::getClassTypeId(),
    SoMFTime::getClassTypeId(),SoSFTime::getClassTypeId(),
    SoMFUInt32::getClassTypeId(),SoSFUInt32::getClassTypeId(),
    SoMFUShort::getClassTypeId(),SoSFUShort::getClassTypeId(),
    SoMFVec2f::getClassTypeId(),SoSFVec2f::getClassTypeId(),
    SoMFVec3f::getClassTypeId(),SoSFVec3f::getClassTypeId(),
    SoMFVec4f::getClassTypeId(),SoSFVec4f::getClassTypeId()
  };

  // FIXME: man, this is horrendous. Swap with a dict. 20000919 mortene.
  SoType outputtype;
  for (int i=0;i<42;i+=2) {
    if (inputfieldtype==types[i]) {
      outputtype=types[i+1];
      break;
    }
  }

  // Instead of SO_ENGINE_ADD_OUTPUT().
  this->output = new SoEngineOutput;
  this->dynamicoutput = new SoEngineOutputData(SoSelectOne::outputdata);
  this->dynamicoutput->addOutput(this, "output", this->output, outputtype);
  this->output->setContainer(this);
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
    // FIXME: this is a very suboptimal way of doing the
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
  if (inputtype == SoType::badType() ||
      !inputtype.isDerivedFrom(SoMField::getClassTypeId())) {
    SoReadError::post(in, "Type \"%s\" for input field is not valid.",
                      fieldname.getString());
    return FALSE;
  }

  this->initialize(inputtype);
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
