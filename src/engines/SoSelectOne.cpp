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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
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
#if !defined(COIN_EXCLUDE_SOMFBITMASK) && !defined(COIN_EXCLUDE_SOSFBITMASK)
    SoMFBitMask::getClassTypeId(),SoSFBitMask::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFBITMASK && !COIN_EXCLUDE_SOSFBITMASK
#if !defined(COIN_EXCLUDE_SOMFBOOL) && !defined(COIN_EXCLUDE_SOSFBOOL)
    SoMFBool::getClassTypeId(),SoSFBool::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFBOOL && !COIN_EXCLUDE_SOSFBOOL
#if !defined(COIN_EXCLUDE_SOMFCOLOR) && !defined(COIN_EXCLUDE_SOSFCOLOR)
    SoMFColor::getClassTypeId(),SoSFColor::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFCOLOR && !COIN_EXCLUDE_SOSFCOLOR
#if !defined(COIN_EXCLUDE_SOMFENGINE) && !defined(COIN_EXCLUDE_SOSFENGINE)
    SoMFEngine::getClassTypeId(),SoSFEngine::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFENGINE && !COIN_EXCLUDE_SOSFENGINE
#if !defined(COIN_EXCLUDE_SOMFENUM) && !defined(COIN_EXCLUDE_SOSFENUM)
    SoMFEnum::getClassTypeId(),SoSFEnum::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFENUM && !COIN_EXCLUDE_SOSFENUM
#if !defined(COIN_EXCLUDE_SOMFFLOAT) && !defined(COIN_EXCLUDE_SOSFFLOAT)
    SoMFFloat::getClassTypeId(),SoSFFloat::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFFLOAT && !COIN_EXCLUDE_SOSFFLOAT
#if !defined(COIN_EXCLUDE_SOMFINT32) && !defined(COIN_EXCLUDE_SOSFINT32)
    SoMFInt32::getClassTypeId(),SoSFInt32::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFINT32 && !COIN_EXCLUDE_SOSFINT32
#if !defined(COIN_EXCLUDE_SOMFMATRIX) && !defined(COIN_EXCLUDE_SOSFMATRIX)
    SoMFMatrix::getClassTypeId(),SoSFMatrix::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFMATRIX && !COIN_EXCLUDE_SOSFMATRIX
#if !defined(COIN_EXCLUDE_SOMFNAME) && !defined(COIN_EXCLUDE_SOSFNAME)
    SoMFName::getClassTypeId(),SoSFName::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFNAME && !COIN_EXCLUDE_SOSFNAME
#if !defined(COIN_EXCLUDE_SOMFNODE) && !defined(COIN_EXCLUDE_SOSFNODE)
    SoMFNode::getClassTypeId(),SoSFNode::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFNODE && !COIN_EXCLUDE_SOSFNODE
#if !defined(COIN_EXCLUDE_SOMFPATH) && !defined(COIN_EXCLUDE_SOSFPATH)
    SoMFPath::getClassTypeId(),SoSFPath::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFPATH && !COIN_EXCLUDE_SOSFPATH
#if !defined(COIN_EXCLUDE_SOMFPLANE) && !defined(COIN_EXCLUDE_SOSFPLANE)
    SoMFPlane::getClassTypeId(),SoSFPlane::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFPLANE && !COIN_EXCLUDE_SOSFPLANE
#if !defined(COIN_EXCLUDE_SOMFROTATION) && !defined(COIN_EXCLUDE_SOSFROTATION)
    SoMFRotation::getClassTypeId(),SoSFRotation::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFROTATION && !COIN_EXCLUDE_SOSFROTATION
#if !defined(COIN_EXCLUDE_SOMFSHORT) && !defined(COIN_EXCLUDE_SOSFSHORT)
    SoMFShort::getClassTypeId(),SoSFShort::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFSHORT && !COIN_EXCLUDE_SOSFSHORT
#if !defined(COIN_EXCLUDE_SOMFSTRING) && !defined(COIN_EXCLUDE_SOSFSTRING)
    SoMFString::getClassTypeId(),SoSFString::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFSTRING && !COIN_EXCLUDE_SOSFSTRING
#if !defined(COIN_EXCLUDE_SOMFTIME) && !defined(COIN_EXCLUDE_SOSFTIME)
    SoMFTime::getClassTypeId(),SoSFTime::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFTIME && !COIN_EXCLUDE_SOSFTIME
#if !defined(COIN_EXCLUDE_SOMFUINT32) && !defined(COIN_EXCLUDE_SOSFUINT32)
    SoMFUInt32::getClassTypeId(),SoSFUInt32::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFUINT32 && !COIN_EXCLUDE_SOSFUINT32
#if !defined(COIN_EXCLUDE_SOMFUSHORT) && !defined(COIN_EXCLUDE_SOSFUSHORT)
    SoMFUShort::getClassTypeId(),SoSFUShort::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFUSHORT && !COIN_EXCLUDE_SOSFUSHORT
#if !defined(COIN_EXCLUDE_SOMFVEC2F) && !defined(COIN_EXCLUDE_SOSFVEC2F)
    SoMFVec2f::getClassTypeId(),SoSFVec2f::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFVEC2F && !COIN_EXCLUDE_SOSFVEC2F
#if !defined(COIN_EXCLUDE_SOMFVEC3F) && !defined(COIN_EXCLUDE_SOSFVEC3F)
    SoMFVec3f::getClassTypeId(),SoSFVec3f::getClassTypeId(),
#endif // !COIN_EXCLUDE_SOMFVEC3F && !COIN_EXCLUDE_SOSFVEC3F
#if !defined(COIN_EXCLUDE_SOMFVEC4F) && !defined(COIN_EXCLUDE_SOSFVEC4F)
    SoMFVec4f::getClassTypeId(),SoMFVec4f::getClassTypeId()
#endif // !COIN_EXCLUDE_SOMFVEC4F && !COIN_EXCLUDE_SOSFVEC4F
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
  SO_ENGINE_INIT_ABSTRACT_CLASS(SoSelectOne,SoEngine,"Engine");
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
