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
#include <Inventor/SoOutput.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/engines/SoSubEngineP.h>
#include <Inventor/fields/SoFields.h>
#include <Inventor/lists/SoEngineOutputList.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*** SO_ENGINE_SOURCE replacement start *************************************/

// Can not use the standard SO_ENGINE_SOURCE macro, as SoSelectOne
// engines doesn't keep a class-global set of inputs and outputs.

PRIVATE_ENGINE_TYPESYSTEM_SOURCE(SoSelectOne);

unsigned int SoSelectOne::classinstances = 0;
SoFieldData * SoSelectOne::inputdata = NULL;
const SoFieldData ** SoSelectOne::parentinputdata = NULL;
SoEngineOutputData * SoSelectOne::outputdata = NULL;
const SoEngineOutputData ** SoSelectOne::parentoutputdata = NULL;

const SoFieldData ** SoSelectOne::getInputDataPtr(void)
{
  assert(0 && "function not in use for SoSelectOne");
  return NULL;
}

const SoFieldData *
SoSelectOne::getFieldData(void) const
{
  return this->dynamicinput;
}

const SoEngineOutputData **
SoSelectOne::getOutputDataPtr(void)
{
  assert(0 && "function not in use for SoSelectOne");
  return NULL;
}

const SoEngineOutputData *
SoSelectOne::getOutputData(void) const
{
  return this->dynamicoutput;
}

void *
SoSelectOne::createInstance(void)
{
  return new SoSelectOne;
}

/*** SO_ENGINE_SOURCE replacement end ***************************************/



/*!
  Constructor. Sets the type of the input field. The input field must
  be of type SoMField.
*/
SoSelectOne::SoSelectOne(SoType inputtype)
{
  SO_ENGINE_INTERNAL_CONSTRUCTOR(SoSelectOne);

  SO_ENGINE_ADD_INPUT(index, (0));

  // Instead of SO_ENGINE_ADD_INPUT().
  this->input = (SoMField *)inputtype.createInstance();
  this->input->setNum(0);
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
    if (inputtype==types[i]) {
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

// dummy default constructor
SoSelectOne::SoSelectOne(void)
{
  assert(FALSE && "default constructor shouldn't be used");
  // FIXME: ..or used when reading engine from file? 20000324 mortene.
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
