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
  \class SoSelectOne SoSelectOne.h Inventor/engines/SoSelectOne.h
  \brief The SoSelectOne class is used to select one value from a set of values.
  \ingroup engines

  FIXME: doc
*/

#include <Inventor/engines/SoSelectOne.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/fields/SoFields.h>
#include <Inventor/engines/SoSubEngineP.h>

SO_ENGINE_SOURCE(SoSelectOne);

/*!
  Constructor. Sets the type of values to be selected.
*/
SoSelectOne::SoSelectOne(SoType inputType)
{
  SO_ENGINE_INTERNAL_CONSTRUCTOR(SoSelectOne);

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

  //Instead of SO_ENGINE_ADD_OUTPUT()
  this->output=new SoEngineOutput;
#if 0 // obsoleted. FIXME: reimplement, 20000309 pederb
  SoType outputType;
  for (int i=0;i<42;i+=2) {
    if (inputType==types[i]) {
      outputType=types[i+1];
      break;
    }
  }
  this->output->setType(outputType);
#endif // obsoleted
  this->output->setContainer(this);

  // FIXME: ivestigate whether output should be added to outputdata somehow
  // pederb, 20000309
}

// overloaded from parent
void
SoSelectOne::initClass()
{
  SO_ENGINE_INTERNAL_INIT_CLASS(SoSelectOne);
}

//
// private members
//
// dummy default constructor
SoSelectOne::SoSelectOne(void)
{
  assert(FALSE && "default constructor shouldn't be used");
  // FIXME: ..or used when reading engine from file? 20000324 mortene.
}

SoSelectOne::~SoSelectOne()
{
  delete this->input;
  delete this->output;
}

// overloaded from parent
void
SoSelectOne::evaluate()
{
  SbString valueString;
  int idx=this->index.getValue();

  this->input->get1(idx,valueString);
  SO_ENGINE_OUTPUT((*output),SoSField,set(valueString.getString()));
}
