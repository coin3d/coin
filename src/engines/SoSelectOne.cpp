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

#include <Inventor/engines/SoSelectOne.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/fields/SoFields.h>

SO_ENGINE_ABSTRACT_SOURCE(SoSelectOne);

SoSelectOne::SoSelectOne(SoType inputType)
{
  SO_ENGINE_CONSTRUCTOR(SoSelectOne);

  SO_ENGINE_ADD_INPUT(index,(0));
  this->input=(SoMField *)inputType.createInstance();

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
    SoMFVec4f::getClassTypeId(),SoMFVec4f::getClassTypeId()
  };

  SoType outputType;
  for (int i=0;i<42;i+=2) {
    if (inputType==types[i]) {
      outputType=types[i+1];
      break;
    }
  }

  //Instead of SO_ENGINE_ADD_OUTPUT()
  this->output=new SoEngineOutput;
  this->output->setType(outputType);
  this->output->setContainer(this);
  this->outputList->append(this->output);
}

void
SoSelectOne::initClass()
{
  SO_ENGINE_INTERNAL_INIT_ABSTRACT_CLASS(SoSelectOne);
}

//
// private members
//
SoSelectOne::~SoSelectOne()
{
  delete this->input;
  delete this->output;
}

void
SoSelectOne::evaluate()
{
  SbString valueString;
  int idx=this->index.getValue();

  this->input->get1(idx,valueString);
  SO_ENGINE_OUTPUT((*output),SoSField,set(valueString.getString()));
}
