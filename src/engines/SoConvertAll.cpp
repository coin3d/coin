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

#include <Inventor/engines/SoConvertAll.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/lists/SoFieldList.h>
#include <Inventor/SoDB.h>
#include <Inventor/fields/SoFields.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#define SO_CONVERTALL_ADD_INPUT(memberName,defaultValue) \
SO_ENGINE_ADD_INPUT(memberName,defaultValue) \
if (firstInstance) \
  inputDict.enter((unsigned long)memberName.getTypeId().getKey(),&memberName);

#define SO_CONVERTALL_ADD_OUTPUT(memberName,outputType) \
SO_ENGINE_ADD_OUTPUT(memberName,outputType) \
if (firstInstance) \
  outputDict.enter((unsigned long)outputType::getClassTypeId().getKey(), \
                   &memberName);

#define SO_CONVERTALL_SFMF(sfield,mfield) \
SoDB::addConverter(sfield::getClassTypeId(),mfield::getClassTypeId(), \
                   getClassTypeId()); \
SoDB::addConverter(mfield::getClassTypeId(),sfield::getClassTypeId(), \
                   getClassTypeId())

#define SO_CONVERTALL_OUTPUT_SCALAR(input_,fromType_) \
else if (this->fromType==fromType_::getClassTypeId()) \
  SO_ENGINE_OUTPUT(this->output,fromType_,setValue(input_->getValue()));


// OBSOLETED?? 19990925 mortene.
#if 0

#define SO_CONVERTALL_CONVERT_SCALAR(input_) \
if (this->useInput==&input_) { \
  if (0); \
  SO_CONVERTALL_OUTPUT_SCALAR(input_,SoSFBool) \
  SO_CONVERTALL_OUTPUT_SCALAR(input_,SoSFFloat) \
  SO_CONVERTALL_OUTPUT_SCALAR(input_,SoSFInt32) \
  SO_CONVERTALL_OUTPUT_SCALAR(input_,SoSFShort) \
  SO_CONVERTALL_OUTPUT_SCALAR(input_,SoSFUInt32) \
  SO_CONVERTALL_OUTPUT_SCALAR(input_,SoSFUShort) \
  break; \
}


#define SO_CONVERTALL_CONVERT_SCALAR(fromType_) \
 if (this->fromType==fromType_::getClassTypeId) { \
 if (0); \
 SO_CONVERTALL_OUTPUT_SCALAR(fromType,SoSFBool) \
 SO_CONVERTALL_OUTPUT_SCALAR(fromType,SoSFFloat) \
 SO_CONVERTALL_OUTPUT_SCALAR(fromType,SoSFInt32) \
 SO_CONVERTALL_OUTPUT_SCALAR(fromType,SoSFShort) \
 SO_CONVERTALL_OUTPUT_SCALAR(fromType,SoSFUInt32) \
 SO_CONVERTALL_OUTPUT_SCALAR(fromType,SoSFUShort) \
 break; \
 }

#define SO_CONVERTALL_OUTPUT_SCALAR(fromType_,toType_) \
 else if (this->toType==toType::getClassTypeId()) \
 SO_ENGINE_OUTPUT(this->output,fromType_,setValue(((toType_ *)input)->getValue()));

#define SO_CONVERTALL_OUTPUT_MSCALAR(type) \
 if (this->toType.getKey()==type::getClassTypeId().getKey()) { \
 SO_ENGINE_OUTPUT(this->output,type,setNum(input->getNum())); \
 for (int i=0;i<input->getNum();i++) \
   SO_ENGINE_OUTPUT(this->output,type,setValue(input->get1Value(i))); \
 break; \
 }

#endif // OBSOLETED??

SO_ENGINE_ABSTRACT_SOURCE(SoConvertAll);

void
SoConvertAll::initClass()
{
  SO_ENGINE_INTERNAL_INIT_ABSTRACT_CLASS(SoConvertAll);

  SoType SFTypes[] = {
    SoSFBitMask::getClassTypeId(),
    SoSFBool::getClassTypeId(),
    SoSFColor::getClassTypeId(),
    SoSFEngine::getClassTypeId(),
    SoSFEnum::getClassTypeId(),
    SoSFFloat::getClassTypeId(),
    SoSFImage::getClassTypeId(),
    SoSFInt32::getClassTypeId(),
    SoSFMatrix::getClassTypeId(),
    SoSFName::getClassTypeId(),
    SoSFNode::getClassTypeId(),
    SoSFPath::getClassTypeId(),
    SoSFPlane::getClassTypeId(),
    SoSFRotation::getClassTypeId(),
    SoSFShort::getClassTypeId(),
    SoSFString::getClassTypeId(),
    SoSFTime::getClassTypeId(),
    SoSFTrigger::getClassTypeId(),
    SoSFUInt32::getClassTypeId(),
    SoSFUShort::getClassTypeId(),
    SoSFVec2f::getClassTypeId(),
    SoSFVec3f::getClassTypeId(),
    SoSFVec4f::getClassTypeId(),
    SoType::badType()
  };

  SoType MFTypes[] = {
    SoMFBitMask::getClassTypeId(),
    SoMFBool::getClassTypeId(),
    SoMFColor::getClassTypeId(),
    SoMFEngine::getClassTypeId(),
    SoMFEnum::getClassTypeId(),
    SoMFFloat::getClassTypeId(),
    SoMFInt32::getClassTypeId(),
    SoMFMatrix::getClassTypeId(),
    SoMFName::getClassTypeId(),
    SoMFNode::getClassTypeId(),
    SoMFPath::getClassTypeId(),
    SoMFPlane::getClassTypeId(),
    SoMFRotation::getClassTypeId(),
    SoMFShort::getClassTypeId(),
    SoMFString::getClassTypeId(),
    SoMFTime::getClassTypeId(),
    SoMFUInt32::getClassTypeId(),
    SoMFUShort::getClassTypeId(),
    SoMFVec2f::getClassTypeId(),
    SoMFVec3f::getClassTypeId(),
    SoMFVec4f::getClassTypeId(),
    SoType::badType()
  };

  SoType scalarTypes[] = {
    SoSFBool::getClassTypeId(),
    SoSFFloat::getClassTypeId(),
    SoSFInt32::getClassTypeId(),
    SoSFShort::getClassTypeId(),
    SoSFUInt32::getClassTypeId(),
    SoSFUShort::getClassTypeId(),
    SoType::badType()
  };


  int i = 0;
  while (SFTypes[i] != SoType::badType()) {
    if (SFTypes[i] != SoSFString::getClassTypeId()) {
      SoDB::addConverter(SFTypes[i],
                         SoSFString::getClassTypeId(),
                         SoConvertAll::getClassTypeId());
      SoDB::addConverter(SoSFString::getClassTypeId(),
                         SFTypes[i],
                         SoConvertAll::getClassTypeId());
    }
    i++;
  }

  i = 0;
  while (MFTypes[i] != SoType::badType()) {
    SoDB::addConverter(MFTypes[i],
                       SoSFString::getClassTypeId(),
                       getClassTypeId());
    SoDB::addConverter(SoSFString::getClassTypeId(),
                       MFTypes[i],
                       getClassTypeId());
    i++;
  }


  int j = 0;
  while (scalarTypes[j] != SoType::badType()) {
    i = 0;
    while (scalarTypes[i] != SoType::badType()) {
      if (i!=j)
        SoDB::addConverter(scalarTypes[j],scalarTypes[i],getClassTypeId());
      i++;
    }
    j++;
  }

  //Color <=> Vec3f
  SoDB::addConverter(SoSFVec3f::getClassTypeId(),
                     SoSFColor::getClassTypeId(),
                     getClassTypeId());
  SoDB::addConverter(SoSFColor::getClassTypeId(),
                     SoSFVec3f::getClassTypeId(),
                     getClassTypeId());

  //Float <=> Time
  SoDB::addConverter(SoSFFloat::getClassTypeId(),
                     SoSFTime::getClassTypeId(),
                     getClassTypeId());
  SoDB::addConverter(SoSFTime::getClassTypeId(),
                     SoSFFloat::getClassTypeId(),
                     getClassTypeId());

  //Matrix <=> Rotation
  SoDB::addConverter(SoSFMatrix::getClassTypeId(),
                     SoSFRotation::getClassTypeId(),
                     getClassTypeId());
  SoDB::addConverter(SoSFRotation::getClassTypeId(),
                     SoSFMatrix::getClassTypeId(),
                     getClassTypeId());

  //SF <-> MF
  SO_CONVERTALL_SFMF(SoSFBitMask,SoMFBitMask);
  SO_CONVERTALL_SFMF(SoSFBool,SoMFBool);
  SO_CONVERTALL_SFMF(SoSFColor,SoMFColor);
  SO_CONVERTALL_SFMF(SoSFEngine,SoMFEngine);
  SO_CONVERTALL_SFMF(SoSFEnum,SoMFEnum);
  SO_CONVERTALL_SFMF(SoSFFloat,SoMFFloat);
  SO_CONVERTALL_SFMF(SoSFInt32,SoMFInt32);
  SO_CONVERTALL_SFMF(SoSFMatrix,SoMFMatrix);
  SO_CONVERTALL_SFMF(SoSFName,SoMFName);
  SO_CONVERTALL_SFMF(SoSFNode,SoMFNode);
  SO_CONVERTALL_SFMF(SoSFPath,SoMFPath);
  SO_CONVERTALL_SFMF(SoSFPlane,SoMFPlane);
  SO_CONVERTALL_SFMF(SoSFRotation,SoMFRotation);
  SO_CONVERTALL_SFMF(SoSFShort,SoMFShort);
  SO_CONVERTALL_SFMF(SoSFTime,SoMFTime);
  SO_CONVERTALL_SFMF(SoSFUInt32,SoMFUInt32);
  SO_CONVERTALL_SFMF(SoSFUShort,SoMFUShort);
  SO_CONVERTALL_SFMF(SoSFVec2f,SoMFVec2f);
  SO_CONVERTALL_SFMF(SoSFVec3f,SoMFVec3f);
  SO_CONVERTALL_SFMF(SoSFVec4f,SoMFVec4f);
}

SoConvertAll::SoConvertAll(const SoType fType,const SoType tType)
{
  SO_ENGINE_CONSTRUCTOR(SoConvertAll);

#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoConvertAll::SoConvertAll",
                         "from: %s, to: %s",
                         fType.getName().getString(),
                         tType.getName().getString());
#endif // debug

  this->input=(SoField *)fType.createInstance();
  
  //FIXME: ?
  //  this->input->setValue defaultValue;
  this->input->setContainer(this);
  this->output.setContainer(this);
  SoConvertAll::outputdata->addOutput(this, "output", &this->output, tType);

  this->fromType=fType;
  this->toType=tType;
}

SoConvertAll::~SoConvertAll()
{
  delete input;
}

SoField *
SoConvertAll::getInput(SoType /* type */)
{
  return input;
}

SoEngineOutput *
SoConvertAll::getOutput(SoType /* type */)
{
  return &output;
}

void
SoConvertAll::evaluate()
{
  for (int i = 0 ; i < this->output.getNumConnections(); i++)
    this->input->convertTo(this->output[i]);



//   if (this->output.getConnectionType()==SoSFString::getClassTypeId())
//     this->convert2String();

//   if (this->input->getTypeId()==SoSFString::getClassTypeId())
//     this->convertFromString();
}

// void
// SoConvertAll::convert2String()
// {
//   SbString str;

//   this->input->get(str);

//   SO_ENGINE_OUTPUT(this->output,SoSFString,setValue(str));
// }

// void
// SoConvertAll::convertFromString()
// {
//   SbString str;

//   this->input->get(str);

//   for (int _i=0;_i<this->output.getNumConnections();_i++)
//     ((SoField *)(this->output)[_i])->set(str.getString());
// }

// void
// SoConvertAll::convertScalar()
// {
//   while (1) {
//     SO_CONVERTALL_CONVERT_SCALAR(sfbool,
//     SO_CONVERTALL_CONVERT_SCALAR(SoSFBool);
//     SO_CONVERTALL_CONVERT_SCALAR(SoSFFloat);
//     SO_CONVERTALL_CONVERT_SCALAR(SoSFLong);
//     SO_CONVERTALL_CONVERT_SCALAR(SoSFShort);
//     SO_CONVERTALL_CONVERT_SCALAR(SoSFULong);
//     SO_CONVERTALL_CONVERT_SCALAR(SoSFUShort);

// //     SO_CONVERTALL_CONVERT_MSCALAR(SoMFBool);
// //     SO_CONVERTALL_CONVERT_MSCALAR(SoMFFloat);
// //     SO_CONVERTALL_CONVERT_MSCALAR(SoMFLong);
// //     SO_CONVERTALL_CONVERT_MSCALAR(SoMFShort);
// //     SO_CONVERTALL_CONVERT_MSCALAR(SoMFULong);
// //     SO_CONVERTALL_CONVERT_MSCALAR(SoMFUShort);
//     break;
//   }
// }
