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

#include <Inventor/engines/SoConvertAll.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/SoDB.h>
#include <Inventor/fields/SoFields.h>

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
#if !defined(COIN_EXCLUDE_SOSFBITMASK)
    SoSFBitMask::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOSFBITMASK
#if !defined(COIN_EXCLUDE_SOSFBOOL)
    SoSFBool::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOSFBOOL
#if !defined(COIN_EXCLUDE_SOSFCOLOR)
    SoSFColor::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOSFCOLOR
#if !defined(COIN_EXCLUDE_SOSFENGINE)
    SoSFEngine::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOSFENGINE
#if !defined(COIN_EXCLUDE_SOSFENUM)
    SoSFEnum::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOSFENUM
#if !defined(COIN_EXCLUDE_SOSFFLOAT)
    SoSFFloat::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOSFFLOAT
#if !defined(COIN_EXCLUDE_SOSFIMAGE)
    SoSFImage::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOSFIMAGE
#if !defined(COIN_EXCLUDE_SOSFINT32)
    SoSFInt32::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOSFINT32
#if !defined(COIN_EXCLUDE_SOSFMATRIX)
    SoSFMatrix::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOSFMATRIX
#if !defined(COIN_EXCLUDE_SOSFNAME)
    SoSFName::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOSFNAME
#if !defined(COIN_EXCLUDE_SOSFNODE)
    SoSFNode::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOSFNODE
#if !defined(COIN_EXCLUDE_SOSFPATH)
    SoSFPath::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOSFPATH
#if !defined(COIN_EXCLUDE_SOSFPLANE)
    SoSFPlane::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOSFPLANE
#if !defined(COIN_EXCLUDE_SOSFROTATION)
    SoSFRotation::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOSFROTATION
#if !defined(COIN_EXCLUDE_SOSFSHORT)
    SoSFShort::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOSFSHORT
#if !defined(COIN_EXCLUDE_SOSFSTRING)
    SoSFString::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOSFSTRING
#if !defined(COIN_EXCLUDE_SOSFTIME)
    SoSFTime::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOSFTIME
#if !defined(COIN_EXCLUDE_SOSFTRIGGER)
    SoSFTrigger::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOSFTRIGGER
#if !defined(COIN_EXCLUDE_SOSFUINT32)
    SoSFUInt32::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOSFUINT32
#if !defined(COIN_EXCLUDE_SOSFUSHORT)
    SoSFUShort::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOSFUSHORT
#if !defined(COIN_EXCLUDE_SOSFVEC2F)
    SoSFVec2f::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOSFVEC2F
#if !defined(COIN_EXCLUDE_SOSFVEC3F)
    SoSFVec3f::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOSFVEC3F
#if !defined(COIN_EXCLUDE_SOSFVEC4F)
    SoSFVec4f::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOSFVEC4F
    SoType::badType()
  };

  SoType MFTypes[] = {
#if !defined(COIN_EXCLUDE_SOMFBITMASK)
    SoMFBitMask::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFBITMASK
#if !defined(COIN_EXCLUDE_SOMFBOOL)
    SoMFBool::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFBOOL
#if !defined(COIN_EXCLUDE_SOMFCOLOR)
    SoMFColor::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFCOLOR
#if !defined(COIN_EXCLUDE_SOMFENGINE)
    SoMFEngine::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFENGINE
#if !defined(COIN_EXCLUDE_SOMFENUM)
    SoMFEnum::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFENUM
#if !defined(COIN_EXCLUDE_SOMFFLOAT)
    SoMFFloat::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFFLOAT
#if !defined(COIN_EXCLUDE_SOMFINT32)
    SoMFInt32::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFINT32
#if !defined(COIN_EXCLUDE_SOMFMATRIX)
    SoMFMatrix::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFMATRIX
#if !defined(COIN_EXCLUDE_SOMFNAME)
    SoMFName::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFNAME
#if !defined(COIN_EXCLUDE_SOMFNODE)
    SoMFNode::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFNODE
#if !defined(COIN_EXCLUDE_SOMFPATH)
    SoMFPath::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFPATH
#if !defined(COIN_EXCLUDE_SOMFPLANE)
    SoMFPlane::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFPLANE
#if !defined(COIN_EXCLUDE_SOMFROTATION)
    SoMFRotation::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFROTATION
#if !defined(COIN_EXCLUDE_SOMFSHORT)
    SoMFShort::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFSHORT
#if !defined(COIN_EXCLUDE_SOMFSTRING)
    SoMFString::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFSTRING
#if !defined(COIN_EXCLUDE_SOMFTIME)
    SoMFTime::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFTIME
#if !defined(COIN_EXCLUDE_SOMFUINT32)
    SoMFUInt32::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFUINT32
#if !defined(COIN_EXCLUDE_SOMFUSHORT)
    SoMFUShort::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFUSHORT
#if !defined(COIN_EXCLUDE_SOMFVEC2F)
    SoMFVec2f::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFVEC2F
#if !defined(COIN_EXCLUDE_SOMFVEC3F)
    SoMFVec3f::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFVEC3F
#if !defined(COIN_EXCLUDE_SOMFVEC4F)
    SoMFVec4f::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFVEC4F
    SoType::badType()
  };

  SoType scalarTypes[] = {
#if !defined(COIN_EXCLUDE_SOSFBOOL)
    SoSFBool::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOSFBOOL
#if !defined(COIN_EXCLUDE_SOSFFLOAT)
    SoSFFloat::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOSFFLOAT
#if !defined(COIN_EXCLUDE_SOSFINT32)
    SoSFInt32::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOSFINT32
#if !defined(COIN_EXCLUDE_SOSFSHORT)
    SoSFShort::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOSFSHORT
#if !defined(COIN_EXCLUDE_SOSFUINT32)
    SoSFUInt32::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOSFUINT32
#if !defined(COIN_EXCLUDE_SOSFUSHORT)
    SoSFUShort::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOSFUSHORT
    SoType::badType()
  };



#if !defined(COIN_EXCLUDE_SOSFSTRING)
  int i = 0;
  while (SFTypes[i] != SoType::badType()) {
    SoDB::addConverter(SFTypes[i],
                       SoSFString::getClassTypeId(),
                       getClassTypeId());
    SoDB::addConverter(SoSFString::getClassTypeId(),
                       SFTypes[i],
                       getClassTypeId());
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
#endif // !COIN_EXCLUDE_SOSFSTRING

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

#if !defined(COIN_EXCLUDE_SOSFVEC3F) &&  !defined(COIN_EXCLUDE_SOSFCOLOR)
  //Color <=> Vec3f
  SoDB::addConverter(SoSFVec3f::getClassTypeId(),
                     SoSFColor::getClassTypeId(),
                     getClassTypeId());
  SoDB::addConverter(SoSFColor::getClassTypeId(),
                     SoSFVec3f::getClassTypeId(),
                     getClassTypeId());
#endif // !COIN_EXCLUDE_SOSFVEC3F &&  !COIN_EXCLUDE_SOSFCOLOR


#if !defined(COIN_EXCLUDE_SOSFFLOAT) &&  !defined(COIN_EXCLUDE_SOSFTIME)
  //Float <=> Time
  SoDB::addConverter(SoSFFloat::getClassTypeId(),
                     SoSFTime::getClassTypeId(),
                     getClassTypeId());
  SoDB::addConverter(SoSFTime::getClassTypeId(),
                     SoSFFloat::getClassTypeId(),
                     getClassTypeId());
#endif // !COIN_EXCLUDE_SOSFFLOAT &&  !COIN_EXCLUDE_SOSFTIME

#if !defined(COIN_EXCLUDE_SOSFMATRIX) &&  !defined(COIN_EXCLUDE_SOSFROTATION)
  //Matrix <=> Rotation
  SoDB::addConverter(SoSFMatrix::getClassTypeId(),
                     SoSFRotation::getClassTypeId(),
                     getClassTypeId());
  SoDB::addConverter(SoSFRotation::getClassTypeId(),
                     SoSFMatrix::getClassTypeId(),
                     getClassTypeId());
#endif // !COIN_EXCLUDE_SOSFMATRIX &&  !COIN_EXCLUDE_SOSFROTATION

  //SF <-> MF
#if !defined(COIN_EXCLUDE_SOSFBITMASK) && !defined(COIN_EXCLUDE_SOMFBITMASK)
  SO_CONVERTALL_SFMF(SoSFBitMask,SoMFBitMask);
#endif // !COIN_EXCLUDE_SOSFBITMASK && !COIN_EXCLUDE_SOMFBITMASK
#if !defined(COIN_EXCLUDE_SOSFBOOL) && !defined(COIN_EXCLUDE_SOMFBOOL)
  SO_CONVERTALL_SFMF(SoSFBool,SoMFBool);
#endif // !COIN_EXCLUDE_SOSFBOOL && !COIN_EXCLUDE_SOMFBOOL
#if !defined(COIN_EXCLUDE_SOSFCOLOR) && !defined(COIN_EXCLUDE_SOMFCOLOR)
  SO_CONVERTALL_SFMF(SoSFColor,SoMFColor);
#endif // !COIN_EXCLUDE_SOSFCOLOR && !COIN_EXCLUDE_SOMFCOLOR
#if !defined(COIN_EXCLUDE_SOSFENGINE) && !defined(COIN_EXCLUDE_SOMFENGINE)
  SO_CONVERTALL_SFMF(SoSFEngine,SoMFEngine);
#endif // !COIN_EXCLUDE_SOSFENGINE && !COIN_EXCLUDE_SOMFENGINE
#if !defined(COIN_EXCLUDE_SOSFENUM) && !defined(COIN_EXCLUDE_SOMFENUM)
  SO_CONVERTALL_SFMF(SoSFEnum,SoMFEnum);
#endif // !COIN_EXCLUDE_SOSFENUM && !COIN_EXCLUDE_SOMFENUM
#if !defined(COIN_EXCLUDE_SOSFFLOAT) && !defined(COIN_EXCLUDE_SOMFFLOAT)
  SO_CONVERTALL_SFMF(SoSFFloat,SoMFFloat);
#endif // !COIN_EXCLUDE_SOSFFLOAT && !COIN_EXCLUDE_SOMFFLOAT
#if !defined(COIN_EXCLUDE_SOSFINT32) && !defined(COIN_EXCLUDE_SOMFINT32)
  SO_CONVERTALL_SFMF(SoSFInt32,SoMFInt32);
#endif // !COIN_EXCLUDE_SOSFINT32 && !COIN_EXCLUDE_SOMFINT32
#if !defined(COIN_EXCLUDE_SOSFMATRIX) && !defined(COIN_EXCLUDE_SOMFMATRIX)
  SO_CONVERTALL_SFMF(SoSFMatrix,SoMFMatrix);
#endif // !COIN_EXCLUDE_SOSFMATRIX && !COIN_EXCLUDE_SOMFMATRIX
#if !defined(COIN_EXCLUDE_SOSFNAME) && !defined(COIN_EXCLUDE_SOMFNAME)
  SO_CONVERTALL_SFMF(SoSFName,SoMFName);
#endif // !COIN_EXCLUDE_SOSFNAME && !COIN_EXCLUDE_SOMFNAME
#if !defined(COIN_EXCLUDE_SOSFNODE) && !defined(COIN_EXCLUDE_SOMFNODE)
  SO_CONVERTALL_SFMF(SoSFNode,SoMFNode);
#endif // !COIN_EXCLUDE_SOSFNODE && !COIN_EXCLUDE_SOMFNODE
#if !defined(COIN_EXCLUDE_SOSFPATH) && !defined(COIN_EXCLUDE_SOMFPATH)
  SO_CONVERTALL_SFMF(SoSFPath,SoMFPath);
#endif // !COIN_EXCLUDE_SOSFPATH && !COIN_EXCLUDE_SOMFPATH
#if !defined(COIN_EXCLUDE_SOSFPLANE) && !defined(COIN_EXCLUDE_SOMFPLANE)
  SO_CONVERTALL_SFMF(SoSFPlane,SoMFPlane);
#endif // !COIN_EXCLUDE_SOSFPLANE && !COIN_EXCLUDE_SOMFPLANE
#if !defined(COIN_EXCLUDE_SOSFROTATION) && !defined(COIN_EXCLUDE_SOMFROTATION)
  SO_CONVERTALL_SFMF(SoSFRotation,SoMFRotation);
#endif // !COIN_EXCLUDE_SOSFROTATION && !COIN_EXCLUDE_SOMFROTATION
#if !defined(COIN_EXCLUDE_SOSFSHORT) && !defined(COIN_EXCLUDE_SOMFSHORT)
  SO_CONVERTALL_SFMF(SoSFShort,SoMFShort);
#endif // !COIN_EXCLUDE_SOSFSHORT && !COIN_EXCLUDE_SOMFSHORT
#if !defined(COIN_EXCLUDE_SOSFSTRING) && !defined(COIN_EXCLUDE_SOMFSTRING)
  SO_CONVERTALL_SFMF(SoSFString,SoMFString);
#endif // !COIN_EXCLUDE_SOSFSTRING && !COIN_EXCLUDE_SOMFSTRING
#if !defined(COIN_EXCLUDE_SOSFTIME) && !defined(COIN_EXCLUDE_SOMFTIME)
  SO_CONVERTALL_SFMF(SoSFTime,SoMFTime);
#endif // !COIN_EXCLUDE_SOSFTIME && !COIN_EXCLUDE_SOMFTIME
#if !defined(COIN_EXCLUDE_SOSFUINT32) && !defined(COIN_EXCLUDE_SOMFUINT32)
  SO_CONVERTALL_SFMF(SoSFUInt32,SoMFUInt32);
#endif // !COIN_EXCLUDE_SOSFUINT32 && !COIN_EXCLUDE_SOMFUINT32
#if !defined(COIN_EXCLUDE_SOSFUSHORT) && !defined(COIN_EXCLUDE_SOMFUSHORT)
  SO_CONVERTALL_SFMF(SoSFUShort,SoMFUShort);
#endif // !COIN_EXCLUDE_SOSFUSHORT && !COIN_EXCLUDE_SOMFUSHORT
#if !defined(COIN_EXCLUDE_SOSFVEC2F) && !defined(COIN_EXCLUDE_SOMFVEC2F)
  SO_CONVERTALL_SFMF(SoSFVec2f,SoMFVec2f);
#endif // !COIN_EXCLUDE_SOSFVEC2F && !COIN_EXCLUDE_SOMFVEC2F
#if !defined(COIN_EXCLUDE_SOSFVEC3F) && !defined(COIN_EXCLUDE_SOMFVEC3F)
  SO_CONVERTALL_SFMF(SoSFVec3f,SoMFVec3f);
#endif // !COIN_EXCLUDE_SOSFVEC3F && !COIN_EXCLUDE_SOMFVEC3F
#if !defined(COIN_EXCLUDE_SOSFVEC4F) && !defined(COIN_EXCLUDE_SOMFVEC4F)
  SO_CONVERTALL_SFMF(SoSFVec4f,SoMFVec4f);
#endif // !COIN_EXCLUDE_SOSFVEC4F && !COIN_EXCLUDE_SOMFVEC4F
}

SoConvertAll::SoConvertAll(const SoType fType,const SoType tType)
{
  SO_ENGINE_CONSTRUCTOR(SoConvertAll);

  this->input=(SoField *)fType.createInstance();
  //FIXME: ?
  //  this->input->setValue defaultValue;
  this->input->setContainer(this);
  this->output.setType(tType);
  this->output.setContainer(this);
  this->outputList->append(&this->output);

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
  for (int i=0;i<this->output.getNumConnections();i++)
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
