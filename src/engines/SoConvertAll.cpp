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
#include <assert.h>

// FIXME:

// SoSFTime -> SoSFString
//      if (this->getValue().getValue()>31500000.0)
//        ((SoSFString *)dest)->setValue(this->getValue().formatDate());
//      else
//        ((SoSFString *)dest)->setValue(this->getValue().format());

// SoSFRotation -> SoSFMatrix
//      SbMatrix mat;
//      mat.setRotate(this->getValue());
//      ((SoSFMatrix *)dest)->setValue(mat);

// SoSFMatrix -> SoSFRotation
//      ((SoSFRotation *)dest)->setValue(this->getValue());


SbDict * SoConvertAll::converter_dict = NULL;


// FIXME: "ABSTRACT"? 20000311 mortene.
SO_ENGINE_ABSTRACT_SOURCE(SoConvertAll);



// Defines function for converting SoSFXXX -> SoMFXXX.
#define SOCONVERTALL_SINGLE2MULTI(_fromto_, _from_, _to_) \
static void _fromto_(SoField * from, SoField * to) \
{ \
  ((_to_ *)to)->setValue(((_from_ *)from)->getValue()); \
}

// Defines function for converting SoMFXXX -> SoSFXXX.
#define SOCONVERTALL_MULTI2SINGLE(_fromto_, _from_, _to_) \
static void _fromto_(SoField * from, SoField * to) \
{ \
  if (((_from_ *)from)->getNum() > 0) \
    ((_to_ *)to)->setValue((*((_from_ *)from))[0]); \
}


SOCONVERTALL_SINGLE2MULTI(SoSFBitMask_SoMFBitMask, SoSFBitMask, SoMFBitMask);
SOCONVERTALL_MULTI2SINGLE(SoMFBitMask_SoSFBitMask, SoMFBitMask, SoSFBitMask);
SOCONVERTALL_SINGLE2MULTI(SoSFBool_SoMFBool, SoSFBool, SoMFBool);
SOCONVERTALL_MULTI2SINGLE(SoMFBool_SoSFBool, SoMFBool, SoSFBool);
SOCONVERTALL_SINGLE2MULTI(SoSFColor_SoMFColor, SoSFColor, SoMFColor);
SOCONVERTALL_MULTI2SINGLE(SoMFColor_SoSFColor, SoMFColor, SoSFColor);
SOCONVERTALL_SINGLE2MULTI(SoSFEnum_SoMFEnum, SoSFEnum, SoMFEnum);
SOCONVERTALL_MULTI2SINGLE(SoMFEnum_SoSFEnum, SoMFEnum, SoSFEnum);
SOCONVERTALL_SINGLE2MULTI(SoSFFloat_SoMFFloat, SoSFFloat, SoMFFloat);
SOCONVERTALL_MULTI2SINGLE(SoMFFloat_SoSFFloat, SoMFFloat, SoSFFloat);
SOCONVERTALL_SINGLE2MULTI(SoSFInt32_SoMFInt32, SoSFInt32, SoMFInt32);
SOCONVERTALL_MULTI2SINGLE(SoMFInt32_SoSFInt32, SoMFInt32, SoSFInt32);
SOCONVERTALL_SINGLE2MULTI(SoSFMatrix_SoMFMatrix, SoSFMatrix, SoMFMatrix);
SOCONVERTALL_MULTI2SINGLE(SoMFMatrix_SoSFMatrix, SoMFMatrix, SoSFMatrix);
SOCONVERTALL_SINGLE2MULTI(SoSFName_SoMFName, SoSFName, SoMFName);
SOCONVERTALL_MULTI2SINGLE(SoMFName_SoSFName, SoMFName, SoSFName);
SOCONVERTALL_SINGLE2MULTI(SoSFNode_SoMFNode, SoSFNode, SoMFNode);
SOCONVERTALL_MULTI2SINGLE(SoMFNode_SoSFNode, SoMFNode, SoSFNode);
SOCONVERTALL_SINGLE2MULTI(SoSFPath_SoMFPath, SoSFPath, SoMFPath);
SOCONVERTALL_MULTI2SINGLE(SoMFPath_SoSFPath, SoMFPath, SoSFPath);
SOCONVERTALL_SINGLE2MULTI(SoSFPlane_SoMFPlane, SoSFPlane, SoMFPlane);
SOCONVERTALL_MULTI2SINGLE(SoMFPlane_SoSFPlane, SoMFPlane, SoSFPlane);
SOCONVERTALL_SINGLE2MULTI(SoSFRotation_SoMFRotation, SoSFRotation, SoMFRotation);
SOCONVERTALL_MULTI2SINGLE(SoMFRotation_SoSFRotation, SoMFRotation, SoSFRotation);
SOCONVERTALL_SINGLE2MULTI(SoSFShort_SoMFShort, SoSFShort, SoMFShort);
SOCONVERTALL_MULTI2SINGLE(SoMFShort_SoSFShort, SoMFShort, SoSFShort);
SOCONVERTALL_SINGLE2MULTI(SoSFString_SoMFString, SoSFString, SoMFString);
SOCONVERTALL_MULTI2SINGLE(SoMFString_SoSFString, SoMFString, SoSFString);
SOCONVERTALL_SINGLE2MULTI(SoSFTime_SoMFTime, SoSFTime, SoMFTime);
SOCONVERTALL_MULTI2SINGLE(SoMFTime_SoSFTime, SoMFTime, SoSFTime);
SOCONVERTALL_SINGLE2MULTI(SoSFUInt32_SoMFUInt32, SoSFUInt32, SoMFUInt32);
SOCONVERTALL_MULTI2SINGLE(SoMFUInt32_SoSFUInt32, SoMFUInt32, SoSFUInt32);
SOCONVERTALL_SINGLE2MULTI(SoSFUShort_SoMFUShort, SoSFUShort, SoMFUShort);
SOCONVERTALL_MULTI2SINGLE(SoMFUShort_SoSFUShort, SoMFUShort, SoSFUShort);
SOCONVERTALL_SINGLE2MULTI(SoSFVec2f_SoMFVec2f, SoSFVec2f, SoMFVec2f);
SOCONVERTALL_MULTI2SINGLE(SoMFVec2f_SoSFVec2f, SoMFVec2f, SoSFVec2f);
SOCONVERTALL_SINGLE2MULTI(SoSFVec3f_SoMFVec3f, SoSFVec3f, SoMFVec3f);
SOCONVERTALL_MULTI2SINGLE(SoMFVec3f_SoSFVec3f, SoMFVec3f, SoSFVec3f);
SOCONVERTALL_SINGLE2MULTI(SoSFVec4f_SoMFVec4f, SoSFVec4f, SoMFVec4f);
SOCONVERTALL_MULTI2SINGLE(SoMFVec4f_SoSFVec4f, SoMFVec4f, SoSFVec4f);

#undef SOCONVERTALL_SINGLE2MULTI
#undef SOCONVERTALL_MULTI2SINGLE


// Defines function for converting SoField -> SoSFString.
static void field_to_sfstring(SoField * from, SoField * to)
{
  SbString s;
  from->get(s);
  ((SoSFString *)to)->setValue(s);
}

// Defines function for converting SoSFString -> SoField.
static void sfstring_to_field(SoField * from, SoField * to)
{
  to->set(((SoSFString *)from)->getValue().getString());
}

// Defines function for converting SoSField -> SoMFString.
static void sfield_to_mfstring(SoField * from, SoField * to)
{
  SbString s;
  ((SoSField *)from)->get(s);
  ((SoMFString *)to)->setValue(s);
}

// Defines function for converting SoMFString -> SoSField.
static void mfstring_to_sfield(SoField * from, SoField * to)
{
  if (((SoMFString *)from)->getNum() > 0) ((SoSField *)to)->set((*((SoMFString *)from))[0].getString());
}

// Defines function for converting SoMField -> SoMFString.
static void mfield_to_mfstring(SoField * from, SoField * to)
{
  SbString s;
  for (int i=0; i < ((SoMField *)from)->getNum(); i++) {
    ((SoMField *)from)->get1(i, s);
    ((SoMFString *)to)->set1Value(i, s);
  }
}

// Defines function for converting SoMFString -> SoMField.
static void mfstring_to_mfield(SoField * from, SoField * to)
{
  for (int i=0; i < ((SoMFString *)from)->getNum(); i++)
    ((SoMField *)to)->set1(i, (*((SoMFString *)from))[i].getString());
}


void
SoConvertAll::register_converter(converter_func * f, SoType from, SoType to)
{
  SoDB::addConverter(from, to, SoConvertAll::getClassTypeId());
  uint32_t val = (((uint32_t)from.getKey()) << 16) + to.getKey();
  SbBool nonexist = SoConvertAll::converter_dict->enter(val, (void *)f);
  assert(nonexist);
}

void
SoConvertAll::initClass(void)
{
  // FIXME: deallocate at final exit(). 20000311 mortene.
  SoConvertAll::converter_dict = new SbDict;

  SO_ENGINE_INTERNAL_INIT_ABSTRACT_CLASS(SoConvertAll);

#define SOCONVERTALL_ADDCONVERTER(_fromto_, _from_, _to_) \
  SoConvertAll::register_converter(_fromto_, _from_::getClassTypeId(), _to_::getClassTypeId())

  SOCONVERTALL_ADDCONVERTER(SoSFBitMask_SoMFBitMask, SoSFBitMask, SoMFBitMask);
  SOCONVERTALL_ADDCONVERTER(SoMFBitMask_SoSFBitMask, SoMFBitMask, SoSFBitMask);
  SOCONVERTALL_ADDCONVERTER(SoSFBool_SoMFBool, SoSFBool, SoMFBool);
  SOCONVERTALL_ADDCONVERTER(SoMFBool_SoSFBool, SoMFBool, SoSFBool);
  SOCONVERTALL_ADDCONVERTER(SoSFColor_SoMFColor, SoSFColor, SoMFColor);
  SOCONVERTALL_ADDCONVERTER(SoMFColor_SoSFColor, SoMFColor, SoSFColor);
  SOCONVERTALL_ADDCONVERTER(SoSFEnum_SoMFEnum, SoSFEnum, SoMFEnum);
  SOCONVERTALL_ADDCONVERTER(SoMFEnum_SoSFEnum, SoMFEnum, SoSFEnum);
  SOCONVERTALL_ADDCONVERTER(SoSFFloat_SoMFFloat, SoSFFloat, SoMFFloat);
  SOCONVERTALL_ADDCONVERTER(SoMFFloat_SoSFFloat, SoMFFloat, SoSFFloat);
  SOCONVERTALL_ADDCONVERTER(SoSFInt32_SoMFInt32, SoSFInt32, SoMFInt32);
  SOCONVERTALL_ADDCONVERTER(SoMFInt32_SoSFInt32, SoMFInt32, SoSFInt32);
  SOCONVERTALL_ADDCONVERTER(SoSFMatrix_SoMFMatrix, SoSFMatrix, SoMFMatrix);
  SOCONVERTALL_ADDCONVERTER(SoMFMatrix_SoSFMatrix, SoMFMatrix, SoSFMatrix);
  SOCONVERTALL_ADDCONVERTER(SoSFName_SoMFName, SoSFName, SoMFName);
  SOCONVERTALL_ADDCONVERTER(SoMFName_SoSFName, SoMFName, SoSFName);
  SOCONVERTALL_ADDCONVERTER(SoSFNode_SoMFNode, SoSFNode, SoMFNode);
  SOCONVERTALL_ADDCONVERTER(SoMFNode_SoSFNode, SoMFNode, SoSFNode);
  SOCONVERTALL_ADDCONVERTER(SoSFPath_SoMFPath, SoSFPath, SoMFPath);
  SOCONVERTALL_ADDCONVERTER(SoMFPath_SoSFPath, SoMFPath, SoSFPath);
  SOCONVERTALL_ADDCONVERTER(SoSFPlane_SoMFPlane, SoSFPlane, SoMFPlane);
  SOCONVERTALL_ADDCONVERTER(SoMFPlane_SoSFPlane, SoMFPlane, SoSFPlane);
  SOCONVERTALL_ADDCONVERTER(SoSFRotation_SoMFRotation, SoSFRotation, SoMFRotation);
  SOCONVERTALL_ADDCONVERTER(SoMFRotation_SoSFRotation, SoMFRotation, SoSFRotation);
  SOCONVERTALL_ADDCONVERTER(SoSFShort_SoMFShort, SoSFShort, SoMFShort);
  SOCONVERTALL_ADDCONVERTER(SoMFShort_SoSFShort, SoMFShort, SoSFShort);
  SOCONVERTALL_ADDCONVERTER(SoSFString_SoMFString, SoSFString, SoMFString);
  SOCONVERTALL_ADDCONVERTER(SoMFString_SoSFString, SoMFString, SoSFString);
  SOCONVERTALL_ADDCONVERTER(SoSFTime_SoMFTime, SoSFTime, SoMFTime);
  SOCONVERTALL_ADDCONVERTER(SoMFTime_SoSFTime, SoMFTime, SoSFTime);
  SOCONVERTALL_ADDCONVERTER(SoSFUInt32_SoMFUInt32, SoSFUInt32, SoMFUInt32);
  SOCONVERTALL_ADDCONVERTER(SoMFUInt32_SoSFUInt32, SoMFUInt32, SoSFUInt32);
  SOCONVERTALL_ADDCONVERTER(SoSFUShort_SoMFUShort, SoSFUShort, SoMFUShort);
  SOCONVERTALL_ADDCONVERTER(SoMFUShort_SoSFUShort, SoMFUShort, SoSFUShort);
  SOCONVERTALL_ADDCONVERTER(SoSFVec2f_SoMFVec2f, SoSFVec2f, SoMFVec2f);
  SOCONVERTALL_ADDCONVERTER(SoMFVec2f_SoSFVec2f, SoMFVec2f, SoSFVec2f);
  SOCONVERTALL_ADDCONVERTER(SoSFVec3f_SoMFVec3f, SoSFVec3f, SoMFVec3f);
  SOCONVERTALL_ADDCONVERTER(SoMFVec3f_SoSFVec3f, SoMFVec3f, SoSFVec3f);
  SOCONVERTALL_ADDCONVERTER(SoSFVec4f_SoMFVec4f, SoSFVec4f, SoMFVec4f);
  SOCONVERTALL_ADDCONVERTER(SoMFVec4f_SoSFVec4f, SoMFVec4f, SoSFVec4f);

  SOCONVERTALL_ADDCONVERTER(field_to_sfstring, SoMFBitMask, SoSFString);
  SOCONVERTALL_ADDCONVERTER(field_to_sfstring, SoMFBool, SoSFString);
  SOCONVERTALL_ADDCONVERTER(field_to_sfstring, SoMFColor, SoSFString);
  SOCONVERTALL_ADDCONVERTER(field_to_sfstring, SoMFEnum, SoSFString);
  SOCONVERTALL_ADDCONVERTER(field_to_sfstring, SoMFFloat, SoSFString);
  SOCONVERTALL_ADDCONVERTER(field_to_sfstring, SoMFInt32, SoSFString);
  SOCONVERTALL_ADDCONVERTER(field_to_sfstring, SoMFMatrix, SoSFString);
  SOCONVERTALL_ADDCONVERTER(field_to_sfstring, SoMFName, SoSFString);
  SOCONVERTALL_ADDCONVERTER(field_to_sfstring, SoMFNode, SoSFString);
  SOCONVERTALL_ADDCONVERTER(field_to_sfstring, SoMFPath, SoSFString);
  SOCONVERTALL_ADDCONVERTER(field_to_sfstring, SoMFPlane, SoSFString);
  SOCONVERTALL_ADDCONVERTER(field_to_sfstring, SoMFRotation, SoSFString);
  SOCONVERTALL_ADDCONVERTER(field_to_sfstring, SoMFShort, SoSFString);
  SOCONVERTALL_ADDCONVERTER(field_to_sfstring, SoMFTime, SoSFString);
  SOCONVERTALL_ADDCONVERTER(field_to_sfstring, SoMFUInt32, SoSFString);
  SOCONVERTALL_ADDCONVERTER(field_to_sfstring, SoMFUShort, SoSFString);
  SOCONVERTALL_ADDCONVERTER(field_to_sfstring, SoMFVec2f, SoSFString);
  SOCONVERTALL_ADDCONVERTER(field_to_sfstring, SoMFVec3f, SoSFString);
  SOCONVERTALL_ADDCONVERTER(field_to_sfstring, SoMFVec4f, SoSFString);
  SOCONVERTALL_ADDCONVERTER(field_to_sfstring, SoSFBitMask, SoSFString);
  SOCONVERTALL_ADDCONVERTER(field_to_sfstring, SoSFBool, SoSFString);
  SOCONVERTALL_ADDCONVERTER(field_to_sfstring, SoSFColor, SoSFString);
  SOCONVERTALL_ADDCONVERTER(field_to_sfstring, SoSFEnum, SoSFString);
  SOCONVERTALL_ADDCONVERTER(field_to_sfstring, SoSFFloat, SoSFString);
  SOCONVERTALL_ADDCONVERTER(field_to_sfstring, SoSFInt32, SoSFString);
  SOCONVERTALL_ADDCONVERTER(field_to_sfstring, SoSFMatrix, SoSFString);
  SOCONVERTALL_ADDCONVERTER(field_to_sfstring, SoSFName, SoSFString);
  SOCONVERTALL_ADDCONVERTER(field_to_sfstring, SoSFNode, SoSFString);
  SOCONVERTALL_ADDCONVERTER(field_to_sfstring, SoSFPath, SoSFString);
  SOCONVERTALL_ADDCONVERTER(field_to_sfstring, SoSFPlane, SoSFString);
  SOCONVERTALL_ADDCONVERTER(field_to_sfstring, SoSFRotation, SoSFString);
  SOCONVERTALL_ADDCONVERTER(field_to_sfstring, SoSFShort, SoSFString);
  SOCONVERTALL_ADDCONVERTER(field_to_sfstring, SoSFTime, SoSFString);
  SOCONVERTALL_ADDCONVERTER(field_to_sfstring, SoSFUInt32, SoSFString);
  SOCONVERTALL_ADDCONVERTER(field_to_sfstring, SoSFUShort, SoSFString);
  SOCONVERTALL_ADDCONVERTER(field_to_sfstring, SoSFVec2f, SoSFString);
  SOCONVERTALL_ADDCONVERTER(field_to_sfstring, SoSFVec3f, SoSFString);
  SOCONVERTALL_ADDCONVERTER(field_to_sfstring, SoSFVec4f, SoSFString);
  SOCONVERTALL_ADDCONVERTER(sfield_to_mfstring, SoSFBitMask, SoMFString);
  SOCONVERTALL_ADDCONVERTER(sfield_to_mfstring, SoSFBool, SoMFString);
  SOCONVERTALL_ADDCONVERTER(sfield_to_mfstring, SoSFColor, SoMFString);
  SOCONVERTALL_ADDCONVERTER(sfield_to_mfstring, SoSFEnum, SoMFString);
  SOCONVERTALL_ADDCONVERTER(sfield_to_mfstring, SoSFFloat, SoMFString);
  SOCONVERTALL_ADDCONVERTER(sfield_to_mfstring, SoSFInt32, SoMFString);
  SOCONVERTALL_ADDCONVERTER(sfield_to_mfstring, SoSFMatrix, SoMFString);
  SOCONVERTALL_ADDCONVERTER(sfield_to_mfstring, SoSFName, SoMFString);
  SOCONVERTALL_ADDCONVERTER(sfield_to_mfstring, SoSFNode, SoMFString);
  SOCONVERTALL_ADDCONVERTER(sfield_to_mfstring, SoSFPath, SoMFString);
  SOCONVERTALL_ADDCONVERTER(sfield_to_mfstring, SoSFPlane, SoMFString);
  SOCONVERTALL_ADDCONVERTER(sfield_to_mfstring, SoSFRotation, SoMFString);
  SOCONVERTALL_ADDCONVERTER(sfield_to_mfstring, SoSFShort, SoMFString);
  SOCONVERTALL_ADDCONVERTER(sfield_to_mfstring, SoSFTime, SoMFString);
  SOCONVERTALL_ADDCONVERTER(sfield_to_mfstring, SoSFUInt32, SoMFString);
  SOCONVERTALL_ADDCONVERTER(sfield_to_mfstring, SoSFUShort, SoMFString);
  SOCONVERTALL_ADDCONVERTER(sfield_to_mfstring, SoSFVec2f, SoMFString);
  SOCONVERTALL_ADDCONVERTER(sfield_to_mfstring, SoSFVec3f, SoMFString);
  SOCONVERTALL_ADDCONVERTER(sfield_to_mfstring, SoSFVec4f, SoMFString);
  SOCONVERTALL_ADDCONVERTER(mfield_to_mfstring, SoMFBitMask, SoMFString);
  SOCONVERTALL_ADDCONVERTER(mfield_to_mfstring, SoMFBool, SoMFString);
  SOCONVERTALL_ADDCONVERTER(mfield_to_mfstring, SoMFColor, SoMFString);
  SOCONVERTALL_ADDCONVERTER(mfield_to_mfstring, SoMFEnum, SoMFString);
  SOCONVERTALL_ADDCONVERTER(mfield_to_mfstring, SoMFFloat, SoMFString);
  SOCONVERTALL_ADDCONVERTER(mfield_to_mfstring, SoMFInt32, SoMFString);
  SOCONVERTALL_ADDCONVERTER(mfield_to_mfstring, SoMFMatrix, SoMFString);
  SOCONVERTALL_ADDCONVERTER(mfield_to_mfstring, SoMFName, SoMFString);
  SOCONVERTALL_ADDCONVERTER(mfield_to_mfstring, SoMFNode, SoMFString);
  SOCONVERTALL_ADDCONVERTER(mfield_to_mfstring, SoMFPath, SoMFString);
  SOCONVERTALL_ADDCONVERTER(mfield_to_mfstring, SoMFPlane, SoMFString);
  SOCONVERTALL_ADDCONVERTER(mfield_to_mfstring, SoMFRotation, SoMFString);
  SOCONVERTALL_ADDCONVERTER(mfield_to_mfstring, SoMFShort, SoMFString);
  SOCONVERTALL_ADDCONVERTER(mfield_to_mfstring, SoMFTime, SoMFString);
  SOCONVERTALL_ADDCONVERTER(mfield_to_mfstring, SoMFUInt32, SoMFString);
  SOCONVERTALL_ADDCONVERTER(mfield_to_mfstring, SoMFUShort, SoMFString);
  SOCONVERTALL_ADDCONVERTER(mfield_to_mfstring, SoMFVec2f, SoMFString);
  SOCONVERTALL_ADDCONVERTER(mfield_to_mfstring, SoMFVec3f, SoMFString);
  SOCONVERTALL_ADDCONVERTER(mfield_to_mfstring, SoMFVec4f, SoMFString);

  SOCONVERTALL_ADDCONVERTER(sfstring_to_field, SoSFString, SoSFBitMask);
  SOCONVERTALL_ADDCONVERTER(sfstring_to_field, SoSFString, SoSFBool);
  SOCONVERTALL_ADDCONVERTER(sfstring_to_field, SoSFString, SoSFColor);
  SOCONVERTALL_ADDCONVERTER(sfstring_to_field, SoSFString, SoSFEnum);
  SOCONVERTALL_ADDCONVERTER(sfstring_to_field, SoSFString, SoSFFloat);
  SOCONVERTALL_ADDCONVERTER(sfstring_to_field, SoSFString, SoSFInt32);
  SOCONVERTALL_ADDCONVERTER(sfstring_to_field, SoSFString, SoSFMatrix);
  SOCONVERTALL_ADDCONVERTER(sfstring_to_field, SoSFString, SoSFName);
  SOCONVERTALL_ADDCONVERTER(sfstring_to_field, SoSFString, SoSFNode);
  SOCONVERTALL_ADDCONVERTER(sfstring_to_field, SoSFString, SoSFPath);
  SOCONVERTALL_ADDCONVERTER(sfstring_to_field, SoSFString, SoSFPlane);
  SOCONVERTALL_ADDCONVERTER(sfstring_to_field, SoSFString, SoSFRotation);
  SOCONVERTALL_ADDCONVERTER(sfstring_to_field, SoSFString, SoSFShort);
  SOCONVERTALL_ADDCONVERTER(sfstring_to_field, SoSFString, SoSFTime);
  SOCONVERTALL_ADDCONVERTER(sfstring_to_field, SoSFString, SoSFUInt32);
  SOCONVERTALL_ADDCONVERTER(sfstring_to_field, SoSFString, SoSFUShort);
  SOCONVERTALL_ADDCONVERTER(sfstring_to_field, SoSFString, SoSFVec2f);
  SOCONVERTALL_ADDCONVERTER(sfstring_to_field, SoSFString, SoSFVec3f);
  SOCONVERTALL_ADDCONVERTER(sfstring_to_field, SoSFString, SoSFVec4f);
  SOCONVERTALL_ADDCONVERTER(sfstring_to_field, SoSFString, SoMFBitMask);
  SOCONVERTALL_ADDCONVERTER(sfstring_to_field, SoSFString, SoMFBool);
  SOCONVERTALL_ADDCONVERTER(sfstring_to_field, SoSFString, SoMFColor);
  SOCONVERTALL_ADDCONVERTER(sfstring_to_field, SoSFString, SoMFEnum);
  SOCONVERTALL_ADDCONVERTER(sfstring_to_field, SoSFString, SoMFFloat);
  SOCONVERTALL_ADDCONVERTER(sfstring_to_field, SoSFString, SoMFInt32);
  SOCONVERTALL_ADDCONVERTER(sfstring_to_field, SoSFString, SoMFMatrix);
  SOCONVERTALL_ADDCONVERTER(sfstring_to_field, SoSFString, SoMFName);
  SOCONVERTALL_ADDCONVERTER(sfstring_to_field, SoSFString, SoMFNode);
  SOCONVERTALL_ADDCONVERTER(sfstring_to_field, SoSFString, SoMFPath);
  SOCONVERTALL_ADDCONVERTER(sfstring_to_field, SoSFString, SoMFPlane);
  SOCONVERTALL_ADDCONVERTER(sfstring_to_field, SoSFString, SoMFRotation);
  SOCONVERTALL_ADDCONVERTER(sfstring_to_field, SoSFString, SoMFShort);
  SOCONVERTALL_ADDCONVERTER(sfstring_to_field, SoSFString, SoMFTime);
  SOCONVERTALL_ADDCONVERTER(sfstring_to_field, SoSFString, SoMFUInt32);
  SOCONVERTALL_ADDCONVERTER(sfstring_to_field, SoSFString, SoMFUShort);
  SOCONVERTALL_ADDCONVERTER(sfstring_to_field, SoSFString, SoMFVec2f);
  SOCONVERTALL_ADDCONVERTER(sfstring_to_field, SoSFString, SoMFVec3f);
  SOCONVERTALL_ADDCONVERTER(sfstring_to_field, SoSFString, SoMFVec4f);

  SOCONVERTALL_ADDCONVERTER(mfstring_to_sfield, SoMFString, SoSFBitMask);
  SOCONVERTALL_ADDCONVERTER(mfstring_to_sfield, SoMFString, SoSFBool);
  SOCONVERTALL_ADDCONVERTER(mfstring_to_sfield, SoMFString, SoSFColor);
  SOCONVERTALL_ADDCONVERTER(mfstring_to_sfield, SoMFString, SoSFEnum);
  SOCONVERTALL_ADDCONVERTER(mfstring_to_sfield, SoMFString, SoSFFloat);
  SOCONVERTALL_ADDCONVERTER(mfstring_to_sfield, SoMFString, SoSFInt32);
  SOCONVERTALL_ADDCONVERTER(mfstring_to_sfield, SoMFString, SoSFMatrix);
  SOCONVERTALL_ADDCONVERTER(mfstring_to_sfield, SoMFString, SoSFName);
  SOCONVERTALL_ADDCONVERTER(mfstring_to_sfield, SoMFString, SoSFNode);
  SOCONVERTALL_ADDCONVERTER(mfstring_to_sfield, SoMFString, SoSFPath);
  SOCONVERTALL_ADDCONVERTER(mfstring_to_sfield, SoMFString, SoSFPlane);
  SOCONVERTALL_ADDCONVERTER(mfstring_to_sfield, SoMFString, SoSFRotation);
  SOCONVERTALL_ADDCONVERTER(mfstring_to_sfield, SoMFString, SoSFShort);
  SOCONVERTALL_ADDCONVERTER(mfstring_to_sfield, SoMFString, SoSFTime);
  SOCONVERTALL_ADDCONVERTER(mfstring_to_sfield, SoMFString, SoSFUInt32);
  SOCONVERTALL_ADDCONVERTER(mfstring_to_sfield, SoMFString, SoSFUShort);
  SOCONVERTALL_ADDCONVERTER(mfstring_to_sfield, SoMFString, SoSFVec2f);
  SOCONVERTALL_ADDCONVERTER(mfstring_to_sfield, SoMFString, SoSFVec3f);
  SOCONVERTALL_ADDCONVERTER(mfstring_to_sfield, SoMFString, SoSFVec4f);
  SOCONVERTALL_ADDCONVERTER(mfstring_to_mfield, SoMFString, SoMFBitMask);
  SOCONVERTALL_ADDCONVERTER(mfstring_to_mfield, SoMFString, SoMFBool);
  SOCONVERTALL_ADDCONVERTER(mfstring_to_mfield, SoMFString, SoMFColor);
  SOCONVERTALL_ADDCONVERTER(mfstring_to_mfield, SoMFString, SoMFEnum);
  SOCONVERTALL_ADDCONVERTER(mfstring_to_mfield, SoMFString, SoMFFloat);
  SOCONVERTALL_ADDCONVERTER(mfstring_to_mfield, SoMFString, SoMFInt32);
  SOCONVERTALL_ADDCONVERTER(mfstring_to_mfield, SoMFString, SoMFMatrix);
  SOCONVERTALL_ADDCONVERTER(mfstring_to_mfield, SoMFString, SoMFName);
  SOCONVERTALL_ADDCONVERTER(mfstring_to_mfield, SoMFString, SoMFNode);
  SOCONVERTALL_ADDCONVERTER(mfstring_to_mfield, SoMFString, SoMFPath);
  SOCONVERTALL_ADDCONVERTER(mfstring_to_mfield, SoMFString, SoMFPlane);
  SOCONVERTALL_ADDCONVERTER(mfstring_to_mfield, SoMFString, SoMFRotation);
  SOCONVERTALL_ADDCONVERTER(mfstring_to_mfield, SoMFString, SoMFShort);
  SOCONVERTALL_ADDCONVERTER(mfstring_to_mfield, SoMFString, SoMFTime);
  SOCONVERTALL_ADDCONVERTER(mfstring_to_mfield, SoMFString, SoMFUInt32);
  SOCONVERTALL_ADDCONVERTER(mfstring_to_mfield, SoMFString, SoMFUShort);
  SOCONVERTALL_ADDCONVERTER(mfstring_to_mfield, SoMFString, SoMFVec2f);
  SOCONVERTALL_ADDCONVERTER(mfstring_to_mfield, SoMFString, SoMFVec3f);
  SOCONVERTALL_ADDCONVERTER(mfstring_to_mfield, SoMFString, SoMFVec4f);

#undef SOCONVERTALL_ADDCONVERTER
}

SoConvertAll::SoConvertAll(const SoType from, const SoType to)
{
  SO_ENGINE_CONSTRUCTOR(SoConvertAll);

#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoConvertAll::SoConvertAll",
                         "from: %s, to: %s",
                         from.getName().getString(),
                         to.getName().getString());
#endif // debug

  this->input = (SoField *)from.createInstance();

  this->input->setContainer(this);
  this->output.setContainer(this);
  SoConvertAll::outputdata->addOutput(this, "output", &this->output, to);

  uint32_t val = (((uint32_t)from.getKey()) << 16) + to.getKey();
  void * ptr;
  if (!SoConvertAll::converter_dict->find(val, ptr)) assert(FALSE);
  this->convertvalue = (converter_func *)ptr;
}

SoConvertAll::~SoConvertAll()
{
  delete this->input;
}

SoField *
SoConvertAll::getInput(SoType /* type */)
{
  return this->input;
}

SoEngineOutput *
SoConvertAll::getOutput(SoType /* type */)
{
  return &this->output;
}

void
SoConvertAll::evaluate(void)
{
  for (int i = 0 ; i < this->output.getNumConnections(); i++)
    this->convertvalue(this->input, this->output[i]);
}
