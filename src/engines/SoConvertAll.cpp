/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

// SoConvertAll is an internal class used for converting values
// between all built-in field types.
//
// Note that this class is not supposed to be used by the application
// programmer -- so the API class definition header file is not
// installed.

#include <Inventor/engines/SoConvertAll.h>
#include <Inventor/SoDB.h>
#include <Inventor/engines/SoSubEngineP.h>
#include <Inventor/fields/SoFields.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/lists/SoFieldList.h>
#include <Inventor/lists/SoTypeList.h>
#include <assert.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

// FIXME: should perhaps use SbTime::parseDate() for So[SM]FString ->
// So[SM]FTime conversion? 20000331 mortene.


SbDict * SoConvertAll::converter_dict = NULL;


// SoConvertAll uses a dynamic list for each instance with information
// about input fields and engine outputs, not like the other engines
// (which have a single static list for each class).
//
// Because of this, we can't use the SO_ENGINE_ABSTRACT_SOURCE macro.

PRIVATE_ENGINE_TYPESYSTEM_SOURCE(SoConvertAll);
unsigned int SoConvertAll::classinstances = 0;
const SoFieldData ** SoConvertAll::parentinputdata = NULL;
const SoEngineOutputData ** SoConvertAll::parentoutputdata = NULL;

const SoFieldData *
SoConvertAll::getFieldData(void) const
{
  return this->inputdata_instance;
}

const SoEngineOutputData *
SoConvertAll::getOutputData(void) const
{
  return this->outputdata_instance;
}

// These are unused, but we list them here since they are part of the
// SO_ENGINE_ABSTRACT_HEADER macro, which we are using for
// convenience.
SoFieldData * SoConvertAll::inputdata = (SoFieldData *)0x1;
SoEngineOutputData * SoConvertAll::outputdata = (SoEngineOutputData *)0x1;
const SoFieldData ** SoConvertAll::getInputDataPtr(void) { return NULL; }
const SoEngineOutputData ** SoConvertAll::getOutputDataPtr(void) { return NULL; }






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


// Function for converting SoField -> SoSFString.
static void field_to_sfstring(SoField * from, SoField * to)
{
  SbString s;
  from->get(s);
  ((SoSFString *)to)->setValue(s);
}

// Function for converting SoSFString -> SoField.
static void sfstring_to_field(SoField * from, SoField * to)
{
  to->set(((SoSFString *)from)->getValue().getString());
}

// Function for converting SoSField -> SoMFString.
static void sfield_to_mfstring(SoField * from, SoField * to)
{
  SbString s;
  ((SoSField *)from)->get(s);
  ((SoMFString *)to)->setValue(s);
}

// Function for converting SoMFString -> SoSField.
static void mfstring_to_sfield(SoField * from, SoField * to)
{
  if (((SoMFString *)from)->getNum() > 0)
    ((SoSField *)to)->set((*((SoMFString *)from))[0].getString());
}

// Function for converting SoMField -> SoMFString.
static void mfield_to_mfstring(SoField * from, SoField * to)
{
  unsigned int numvals = ((SoMField *)from)->getNum();
  ((SoMField *)to)->setNum(numvals);
  SbString s;
  for (unsigned int i=0; i < numvals; i++) {
    ((SoMField *)from)->get1(i, s);
    ((SoMFString *)to)->set1Value(i, s);
  }
}

// Function for converting SoMFString -> SoMField.
static void mfstring_to_mfield(SoField * from, SoField * to)
{
  unsigned int numvals = ((SoMField *)from)->getNum();
  ((SoMField *)to)->setNum(numvals);

  for (unsigned int i=0; i < numvals; i++)
    ((SoMField *)to)->set1(i, (*((SoMFString *)from))[i].getString());
}

// Defines function for converting SoSField -> SoField, where
// conversion is just a typecast.
#define SOCONVERTALL_CAST_SFIELD2FIELD(_fromto_, _from_, _to_, _tobase_) \
static void _fromto_(SoField * from, SoField * to) \
{ \
  ((_to_ *)to)->setValue((_tobase_)((_from_ *)from)->getValue()); \
}

SOCONVERTALL_CAST_SFIELD2FIELD(SoSFBool_SoSFFloat, SoSFBool, SoSFFloat, float);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFBool_SoMFFloat, SoSFBool, SoMFFloat, float);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFFloat_SoSFBool, SoSFFloat, SoSFBool, SbBool);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFFloat_SoMFBool, SoSFFloat, SoMFBool, SbBool);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFBool_SoSFInt32, SoSFBool, SoSFInt32, int32_t);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFBool_SoMFInt32, SoSFBool, SoMFInt32, int32_t);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFInt32_SoSFBool, SoSFInt32, SoSFBool, SbBool);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFInt32_SoMFBool, SoSFInt32, SoMFBool, SbBool);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFBool_SoSFShort, SoSFBool, SoSFShort, short);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFBool_SoMFShort, SoSFBool, SoMFShort, short);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFShort_SoSFBool, SoSFShort, SoSFBool, SbBool);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFShort_SoMFBool, SoSFShort, SoMFBool, SbBool);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFBool_SoSFUInt32, SoSFBool, SoSFUInt32, unsigned short);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFBool_SoMFUInt32, SoSFBool, SoMFUInt32, unsigned short);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFUInt32_SoSFBool, SoSFUInt32, SoSFBool, SbBool);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFUInt32_SoMFBool, SoSFUInt32, SoMFBool, SbBool);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFBool_SoSFUShort, SoSFBool, SoSFUShort, unsigned short);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFBool_SoMFUShort, SoSFBool, SoMFUShort, unsigned short);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFUShort_SoSFBool, SoSFUShort, SoSFBool, SbBool);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFUShort_SoMFBool, SoSFUShort, SoMFBool, SbBool);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFColor_SoSFVec3f, SoSFColor, SoSFVec3f, SbVec3f);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFColor_SoMFVec3f, SoSFColor, SoMFVec3f, SbVec3f);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFVec3f_SoSFColor, SoSFVec3f, SoSFColor, SbColor);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFVec3f_SoMFColor, SoSFVec3f, SoMFColor, SbColor);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFFloat_SoSFInt32, SoSFFloat, SoSFInt32, int32_t);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFFloat_SoMFInt32, SoSFFloat, SoMFInt32, int32_t);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFInt32_SoSFFloat, SoSFInt32, SoSFFloat, float);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFInt32_SoMFFloat, SoSFInt32, SoMFFloat, float);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFFloat_SoSFShort, SoSFFloat, SoSFShort, short);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFFloat_SoMFShort, SoSFFloat, SoMFShort, short);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFShort_SoSFFloat, SoSFShort, SoSFFloat, float);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFShort_SoMFFloat, SoSFShort, SoMFFloat, float);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFFloat_SoSFUInt32, SoSFFloat, SoSFUInt32, uint32_t);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFFloat_SoMFUInt32, SoSFFloat, SoMFUInt32, uint32_t);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFUInt32_SoSFFloat, SoSFUInt32, SoSFFloat, float);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFUInt32_SoMFFloat, SoSFUInt32, SoMFFloat, float);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFFloat_SoSFUShort, SoSFFloat, SoSFUShort, unsigned short);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFFloat_SoMFUShort, SoSFFloat, SoMFUShort, unsigned short);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFUShort_SoSFFloat, SoSFUShort, SoSFFloat, float);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFUShort_SoMFFloat, SoSFUShort, SoMFFloat, float);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFInt32_SoSFShort, SoSFInt32, SoSFShort, short);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFInt32_SoMFShort, SoSFInt32, SoMFShort, short);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFShort_SoSFInt32, SoSFShort, SoSFInt32, int32_t);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFShort_SoMFInt32, SoSFShort, SoMFInt32, int32_t);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFInt32_SoSFUInt32, SoSFInt32, SoSFUInt32, uint32_t);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFInt32_SoMFUInt32, SoSFInt32, SoMFUInt32, uint32_t);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFUInt32_SoSFInt32, SoSFUInt32, SoSFInt32, int32_t);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFUInt32_SoMFInt32, SoSFUInt32, SoMFInt32, int32_t);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFInt32_SoSFUShort, SoSFInt32, SoSFUShort, unsigned short);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFInt32_SoMFUShort, SoSFInt32, SoMFUShort, unsigned short);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFUShort_SoSFInt32, SoSFUShort, SoSFInt32, int32_t);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFUShort_SoMFInt32, SoSFUShort, SoMFInt32, int32_t);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFShort_SoSFUInt32, SoSFShort, SoSFUInt32, uint32_t);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFShort_SoMFUInt32, SoSFShort, SoMFUInt32, uint32_t);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFUInt32_SoSFShort, SoSFUInt32, SoSFShort, short);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFUInt32_SoMFShort, SoSFUInt32, SoMFShort, short);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFShort_SoSFUShort, SoSFShort, SoSFUShort, unsigned short);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFShort_SoMFUShort, SoSFShort, SoMFUShort, unsigned short);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFUShort_SoSFShort, SoSFUShort, SoSFShort, short);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFUShort_SoMFShort, SoSFUShort, SoMFShort, short);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFUInt32_SoSFUShort, SoSFUInt32, SoSFUShort, unsigned short);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFUInt32_SoMFUShort, SoSFUInt32, SoMFUShort, unsigned short);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFUShort_SoSFUInt32, SoSFUShort, SoSFUInt32, uint32_t);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFUShort_SoMFUInt32, SoSFUShort, SoMFUInt32, uint32_t);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFFloat_SoSFTime, SoSFFloat, SoSFTime, SbTime);
SOCONVERTALL_CAST_SFIELD2FIELD(SoSFFloat_SoMFTime, SoSFFloat, SoMFTime, SbTime);

// Defines function for converting SoMField -> SoSField, where
// conversion is just a typecast.
#define SOCONVERTALL_CAST_MFIELD2SFIELD(_fromto_, _from_, _to_, _tobase_) \
static void _fromto_(SoField * from, SoField * to) \
{ \
  if (((_from_ *)from)->getNum() > 0) \
    ((_to_ *)to)->setValue((_tobase_)((*((_from_ *)from))[0])); \
}


SOCONVERTALL_CAST_MFIELD2SFIELD(SoMFBool_SoSFFloat, SoMFBool, SoSFFloat, float);
SOCONVERTALL_CAST_MFIELD2SFIELD(SoMFFloat_SoSFBool, SoMFFloat, SoSFBool, SbBool);
SOCONVERTALL_CAST_MFIELD2SFIELD(SoMFBool_SoSFInt32, SoMFBool, SoSFInt32, int32_t);
SOCONVERTALL_CAST_MFIELD2SFIELD(SoMFInt32_SoSFBool, SoMFInt32, SoSFBool, SbBool);
SOCONVERTALL_CAST_MFIELD2SFIELD(SoMFBool_SoSFShort, SoMFBool, SoSFShort, short);
SOCONVERTALL_CAST_MFIELD2SFIELD(SoMFShort_SoSFBool, SoMFShort, SoSFBool, SbBool);
SOCONVERTALL_CAST_MFIELD2SFIELD(SoMFBool_SoSFUInt32, SoMFBool, SoSFUInt32, uint32_t);
SOCONVERTALL_CAST_MFIELD2SFIELD(SoMFUInt32_SoSFBool, SoMFUInt32, SoSFBool, SbBool);
SOCONVERTALL_CAST_MFIELD2SFIELD(SoMFBool_SoSFUShort, SoMFBool, SoSFUShort, unsigned short);
SOCONVERTALL_CAST_MFIELD2SFIELD(SoMFUShort_SoSFBool, SoMFUShort, SoSFBool, SbBool);
SOCONVERTALL_CAST_MFIELD2SFIELD(SoMFColor_SoSFVec3f, SoMFColor, SoSFVec3f, SbVec3f);
SOCONVERTALL_CAST_MFIELD2SFIELD(SoMFVec3f_SoSFColor, SoMFVec3f, SoSFColor, SbColor);
SOCONVERTALL_CAST_MFIELD2SFIELD(SoMFFloat_SoSFInt32, SoMFFloat, SoSFInt32, int32_t);
SOCONVERTALL_CAST_MFIELD2SFIELD(SoMFInt32_SoSFFloat, SoMFInt32, SoSFFloat, float);
SOCONVERTALL_CAST_MFIELD2SFIELD(SoMFFloat_SoSFShort, SoMFFloat, SoSFShort, short);
SOCONVERTALL_CAST_MFIELD2SFIELD(SoMFShort_SoSFFloat, SoMFShort, SoSFFloat, float);
SOCONVERTALL_CAST_MFIELD2SFIELD(SoMFFloat_SoSFUInt32, SoMFFloat, SoSFUInt32, uint32_t);
SOCONVERTALL_CAST_MFIELD2SFIELD(SoMFUInt32_SoSFFloat, SoMFUInt32, SoSFFloat, float);
SOCONVERTALL_CAST_MFIELD2SFIELD(SoMFFloat_SoSFUShort, SoMFFloat, SoSFUShort, unsigned short);
SOCONVERTALL_CAST_MFIELD2SFIELD(SoMFUShort_SoSFFloat, SoMFUShort, SoSFFloat, float);
SOCONVERTALL_CAST_MFIELD2SFIELD(SoMFInt32_SoSFShort, SoMFInt32, SoSFShort, short);
SOCONVERTALL_CAST_MFIELD2SFIELD(SoMFShort_SoSFInt32, SoMFShort, SoSFInt32, int32_t);
SOCONVERTALL_CAST_MFIELD2SFIELD(SoMFInt32_SoSFUInt32, SoMFInt32, SoSFUInt32, uint32_t);
SOCONVERTALL_CAST_MFIELD2SFIELD(SoMFUInt32_SoSFInt32, SoMFUInt32, SoSFInt32, int32_t);
SOCONVERTALL_CAST_MFIELD2SFIELD(SoMFInt32_SoSFUShort, SoMFInt32, SoSFUShort, unsigned short);
SOCONVERTALL_CAST_MFIELD2SFIELD(SoMFUShort_SoSFInt32, SoMFUShort, SoSFInt32, int32_t);
SOCONVERTALL_CAST_MFIELD2SFIELD(SoMFShort_SoSFUInt32, SoMFShort, SoSFUInt32, uint32_t);
SOCONVERTALL_CAST_MFIELD2SFIELD(SoMFUInt32_SoSFShort, SoMFUInt32, SoSFShort, short);
SOCONVERTALL_CAST_MFIELD2SFIELD(SoMFShort_SoSFUShort, SoMFShort, SoSFUShort, unsigned short);
SOCONVERTALL_CAST_MFIELD2SFIELD(SoMFUShort_SoSFShort, SoMFUShort, SoSFShort, short);
SOCONVERTALL_CAST_MFIELD2SFIELD(SoMFUInt32_SoSFUShort, SoMFUInt32, SoSFUShort, unsigned short);
SOCONVERTALL_CAST_MFIELD2SFIELD(SoMFUShort_SoSFUInt32, SoMFUShort, SoSFUInt32, uint32_t);
SOCONVERTALL_CAST_MFIELD2SFIELD(SoMFFloat_SoSFTime, SoMFFloat, SoSFTime, SbTime);

// Defines functions for converting SoMField -> SoMField, where
// conversion is just a typecast.
#define SOCONVERTALL_CAST_MFIELD2MFIELD(_fromto_, _from_, _to_, _tobase_) \
static void _fromto_(SoField * from, SoField * to) \
{ \
  unsigned int numvals = ((SoMField *)from)->getNum(); \
  ((SoMField *)to)->setNum(numvals); \
  for (unsigned int i=0; i < numvals; i++) \
    ((_to_ *)to)->set1Value(i, (_tobase_)((*((_from_ *)from))[i])); \
}

SOCONVERTALL_CAST_MFIELD2MFIELD(SoMFBool_SoMFFloat, SoMFBool, SoMFFloat, float);
SOCONVERTALL_CAST_MFIELD2MFIELD(SoMFFloat_SoMFBool, SoMFFloat, SoMFBool, SbBool);
SOCONVERTALL_CAST_MFIELD2MFIELD(SoMFBool_SoMFInt32, SoMFBool, SoMFInt32, int32_t);
SOCONVERTALL_CAST_MFIELD2MFIELD(SoMFInt32_SoMFBool, SoMFInt32, SoMFBool, SbBool);
SOCONVERTALL_CAST_MFIELD2MFIELD(SoMFBool_SoMFShort, SoMFBool, SoMFShort, short);
SOCONVERTALL_CAST_MFIELD2MFIELD(SoMFShort_SoMFBool, SoMFShort, SoMFBool, SbBool);
SOCONVERTALL_CAST_MFIELD2MFIELD(SoMFBool_SoMFUInt32, SoMFBool, SoMFUInt32, uint32_t);
SOCONVERTALL_CAST_MFIELD2MFIELD(SoMFUInt32_SoMFBool, SoMFUInt32, SoMFBool, SbBool);
SOCONVERTALL_CAST_MFIELD2MFIELD(SoMFBool_SoMFUShort, SoMFBool, SoMFUShort, unsigned short);
SOCONVERTALL_CAST_MFIELD2MFIELD(SoMFUShort_SoMFBool, SoMFUShort, SoMFBool, SbBool);
SOCONVERTALL_CAST_MFIELD2MFIELD(SoMFColor_SoMFVec3f, SoMFColor, SoMFVec3f, SbVec3f);
SOCONVERTALL_CAST_MFIELD2MFIELD(SoMFVec3f_SoMFColor, SoMFVec3f, SoMFColor, SbColor);
SOCONVERTALL_CAST_MFIELD2MFIELD(SoMFFloat_SoMFInt32, SoMFFloat, SoMFInt32, int32_t);
SOCONVERTALL_CAST_MFIELD2MFIELD(SoMFInt32_SoMFFloat, SoMFInt32, SoMFFloat, float);
SOCONVERTALL_CAST_MFIELD2MFIELD(SoMFFloat_SoMFShort, SoMFFloat, SoMFShort, short);
SOCONVERTALL_CAST_MFIELD2MFIELD(SoMFShort_SoMFFloat, SoMFShort, SoMFFloat, float);
SOCONVERTALL_CAST_MFIELD2MFIELD(SoMFFloat_SoMFUInt32, SoMFFloat, SoMFUInt32, uint32_t);
SOCONVERTALL_CAST_MFIELD2MFIELD(SoMFUInt32_SoMFFloat, SoMFUInt32, SoMFFloat, float);
SOCONVERTALL_CAST_MFIELD2MFIELD(SoMFFloat_SoMFUShort, SoMFFloat, SoMFUShort, unsigned short);
SOCONVERTALL_CAST_MFIELD2MFIELD(SoMFUShort_SoMFFloat, SoMFUShort, SoMFFloat, float);
SOCONVERTALL_CAST_MFIELD2MFIELD(SoMFInt32_SoMFShort, SoMFInt32, SoMFShort, short);
SOCONVERTALL_CAST_MFIELD2MFIELD(SoMFShort_SoMFInt32, SoMFShort, SoMFInt32, int32_t);
SOCONVERTALL_CAST_MFIELD2MFIELD(SoMFInt32_SoMFUInt32, SoMFInt32, SoMFUInt32, uint32_t);
SOCONVERTALL_CAST_MFIELD2MFIELD(SoMFUInt32_SoMFInt32, SoMFUInt32, SoMFInt32, int32_t);
SOCONVERTALL_CAST_MFIELD2MFIELD(SoMFInt32_SoMFUShort, SoMFInt32, SoMFUShort, unsigned short);
SOCONVERTALL_CAST_MFIELD2MFIELD(SoMFUShort_SoMFInt32, SoMFUShort, SoMFInt32, int32_t);
SOCONVERTALL_CAST_MFIELD2MFIELD(SoMFShort_SoMFUInt32, SoMFShort, SoMFUInt32, uint32_t);
SOCONVERTALL_CAST_MFIELD2MFIELD(SoMFUInt32_SoMFShort, SoMFUInt32, SoMFShort, short);
SOCONVERTALL_CAST_MFIELD2MFIELD(SoMFShort_SoMFUShort, SoMFShort, SoMFUShort, unsigned short);
SOCONVERTALL_CAST_MFIELD2MFIELD(SoMFUShort_SoMFShort, SoMFUShort, SoMFShort, short);
SOCONVERTALL_CAST_MFIELD2MFIELD(SoMFUInt32_SoMFUShort, SoMFUInt32, SoMFUShort, unsigned short);
SOCONVERTALL_CAST_MFIELD2MFIELD(SoMFUShort_SoMFUInt32, SoMFUShort, SoMFUInt32, uint32_t);
SOCONVERTALL_CAST_MFIELD2MFIELD(SoMFFloat_SoMFTime, SoMFFloat, SoMFTime, SbTime);


// Defines function for converting SoSFTime -> So[SM]FFloat.
#define SOCONVERTALL_CAST_SFTIME2SFFLOAT(_fromto_, _to_) \
static void _fromto_(SoField * from, SoField * to) \
{ \
  ((_to_ *)to)->setValue((float)((SoSFTime *)from)->getValue().getValue()); \
}

SOCONVERTALL_CAST_SFTIME2SFFLOAT(SoSFTime_SoSFFloat, SoSFFloat);
SOCONVERTALL_CAST_SFTIME2SFFLOAT(SoSFTime_SoMFFloat, SoMFFloat);

// Function for converting SoMFTime -> SoSFFloat.
static void SoMFTime_SoSFFloat(SoField * from, SoField * to)
{
  if (((SoMField *)from)->getNum() > 0)
    ((SoSFFloat *)to)->setValue((float)((*((SoMFTime *)from))[0]).getValue());
}

// Function for converting SoMFTime -> SoMFFloat.
static void SoMFTime_SoMFFloat(SoField * from, SoField * to)
{
  unsigned int numvals = ((SoMField *)from)->getNum();
  ((SoMField *)to)->setNum(numvals);
  for (unsigned int i=0; i < numvals; i++)
    ((SoMFFloat *)to)->set1Value(i, (float)((*((SoMFTime *)from))[i]).getValue());
}



// Defines function for converting SoSFMatrix -> So[SM]FRotation.
#define SOCONVERTALL_SFMATRIX2ROTATION(_fromto_, _to_) \
static void _fromto_(SoField * from, SoField * to) \
{ \
  ((_to_ *)to)->setValue(SbRotation(((SoSFMatrix *)from)->getValue())); \
}

SOCONVERTALL_SFMATRIX2ROTATION(SoSFMatrix_SoSFRotation, SoSFRotation);
SOCONVERTALL_SFMATRIX2ROTATION(SoSFMatrix_SoMFRotation, SoMFRotation);

// Function for converting SoMFMatrix -> SoSFRotation.
static void SoMFMatrix_SoSFRotation(SoField * from, SoField * to)
{
  if (((SoMField *)from)->getNum() > 0)
    ((SoSFRotation *)to)->setValue(SbRotation(((*((SoMFMatrix *)from))[0])));
}

// Function for converting SoMFMatrix -> SoMFRotation.
static void SoMFMatrix_SoMFRotation(SoField * from, SoField * to)
{
  for (int i=0; i < ((SoMField *)from)->getNum(); i++)
    ((SoMFRotation *)to)->set1Value(i, SbRotation(((*((SoMFMatrix *)from))[i])));
}


// Defines function for converting SoSFRotation -> So[SM]FMatrix.
#define SOCONVERTALL_SFROTATION2MATRIX(_fromto_, _to_) \
static void _fromto_(SoField * from, SoField * to) \
{ \
  SbMatrix mat; \
  mat.setRotate(((SoSFRotation *)from)->getValue()); \
  ((_to_ *)to)->setValue(mat); \
}

SOCONVERTALL_SFROTATION2MATRIX(SoSFRotation_SoSFMatrix, SoSFMatrix);
SOCONVERTALL_SFROTATION2MATRIX(SoSFRotation_SoMFMatrix, SoMFMatrix);

// Function for converting SoMFRotation -> SoSFMatrix.
static void SoMFRotation_SoSFMatrix(SoField * from, SoField * to)
{
  if (((SoMField *)from)->getNum() > 0) {
    SbMatrix mat;
    mat.setRotate((*((SoMFRotation *)from))[0]);
    ((SoSFMatrix *)to)->setValue(mat);
  }
}

// Function for converting SoMFRotation -> SoMFMatrix.
static void SoMFRotation_SoMFMatrix(SoField * from, SoField * to)
{
  unsigned int numvals = ((SoMField *)from)->getNum();
  ((SoMField *)to)->setNum(numvals);
  for (unsigned int i=0; i < numvals; i++) {
    SbMatrix mat;
    mat.setRotate((*((SoMFRotation *)from))[i]);
    ((SoMFMatrix *)to)->set1Value(i, mat);
  }
}


// Helper function for the So[SM]FTime -> So[SM]FString converters
// below.
static void time2string(const SbTime & t, SbString & s)
{
  // Value is less than a year, assume we're counting seconds. Use
  // resolution at millisecond accuracy.
  if (t.getValue() < (60.0*60.0*24.0*365.0)) s.sprintf("%.3f", t.getValue());
  // Value is more than a year, assume we're interested in the date
  // and time.
#if 0 // Don't default to ISO 8601 conformant string, ...
  // Note: if this is ever enabled, remember that the format string
  // need to be different on MSWindows systems.
  else s = t.formatDate("%A %Y-%m-%d %H:%M:%S");
#else // .. follow Open Inventor instead.
  else s = t.formatDate();
#endif
}

// Function for converting SoSFTime -> SoSFString.
static void sftime_to_sfstring(SoField * from, SoField * to)
{
  SbString s;
  time2string(((SoSFTime *)from)->getValue(), s);
  ((SoSFString *)to)->setValue(s);
}

// Function for converting SoSFTime -> SoMFString.
static void sftime_to_mfstring(SoField * from, SoField * to)
{
  SbString s;
  time2string(((SoSFTime *)from)->getValue(), s);
  ((SoMFString *)to)->setValue(s);
}

// Function for converting SoMFTime -> SoSFString.
static void mftime_to_sfstring(SoField * from, SoField * to)
{
  SoMFTime * ff = (SoMFTime *)from;
  if (ff->getNum() > 0) {
    SbString s;
    time2string((*ff)[0], s);
    ((SoSFString *)to)->setValue(s);
  }
}

// Function for converting SoMFTime -> SoMFString.
static void mftime_to_mfstring(SoField * from, SoField * to)
{
  SoMFTime * ff = (SoMFTime *)from;
  unsigned int numvals = ff->getNum();
  ((SoMField *)to)->setNum(numvals);
  SbString s;
  for (unsigned int i=0; i < numvals; i++) {
    time2string((*ff)[i], s);
    ((SoMFString *)to)->set1Value(i, s);
  }
}

// Function for "converting" SoField -> SoSFTrigger _and_
// SoSFTrigger -> SoField.
static void to_and_from_sftrigger(SoField * from, SoField * to)
{
  to->setDirty(FALSE);
}

void
SoConvertAll::register_converter(converter_func * f, SoType from, SoType to)
{
  SoDB::addConverter(from, to, SoConvertAll::getClassTypeId());
  uint32_t val = (((uint32_t)from.getKey()) << 16) + to.getKey();
  SbBool nonexist = SoConvertAll::converter_dict->enter(val, (void *)f);
  assert(nonexist);
}

// doc in super
void
SoConvertAll::initClass(void)
{
  // FIXME: deallocate at final exit(). 20000311 mortene.
  SoConvertAll::converter_dict = new SbDict;

  // SoConvertAll doesn't have a createInstance() method (because it
  // doesn't have a default constructor), so use the ABSTRACT macros.
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

  SOCONVERTALL_ADDCONVERTER(SoSFBool_SoSFFloat, SoSFBool, SoSFFloat);
  SOCONVERTALL_ADDCONVERTER(SoSFBool_SoMFFloat, SoSFBool, SoMFFloat);
  SOCONVERTALL_ADDCONVERTER(SoSFFloat_SoSFBool, SoSFFloat, SoSFBool);
  SOCONVERTALL_ADDCONVERTER(SoSFFloat_SoMFBool, SoSFFloat, SoMFBool);
  SOCONVERTALL_ADDCONVERTER(SoSFBool_SoSFInt32, SoSFBool, SoSFInt32);
  SOCONVERTALL_ADDCONVERTER(SoSFBool_SoMFInt32, SoSFBool, SoMFInt32);
  SOCONVERTALL_ADDCONVERTER(SoSFInt32_SoSFBool, SoSFInt32, SoSFBool);
  SOCONVERTALL_ADDCONVERTER(SoSFInt32_SoMFBool, SoSFInt32, SoMFBool);
  SOCONVERTALL_ADDCONVERTER(SoSFBool_SoSFShort, SoSFBool, SoSFShort);
  SOCONVERTALL_ADDCONVERTER(SoSFBool_SoMFShort, SoSFBool, SoMFShort);
  SOCONVERTALL_ADDCONVERTER(SoSFShort_SoSFBool, SoSFShort, SoSFBool);
  SOCONVERTALL_ADDCONVERTER(SoSFShort_SoMFBool, SoSFShort, SoMFBool);
  SOCONVERTALL_ADDCONVERTER(SoSFBool_SoSFUInt32, SoSFBool, SoSFUInt32);
  SOCONVERTALL_ADDCONVERTER(SoSFBool_SoMFUInt32, SoSFBool, SoMFUInt32);
  SOCONVERTALL_ADDCONVERTER(SoSFUInt32_SoSFBool, SoSFUInt32, SoSFBool);
  SOCONVERTALL_ADDCONVERTER(SoSFUInt32_SoMFBool, SoSFUInt32, SoMFBool);
  SOCONVERTALL_ADDCONVERTER(SoSFBool_SoSFUShort, SoSFBool, SoSFUShort);
  SOCONVERTALL_ADDCONVERTER(SoSFBool_SoMFUShort, SoSFBool, SoMFUShort);
  SOCONVERTALL_ADDCONVERTER(SoSFUShort_SoSFBool, SoSFUShort, SoSFBool);
  SOCONVERTALL_ADDCONVERTER(SoSFUShort_SoMFBool, SoSFUShort, SoMFBool);
  SOCONVERTALL_ADDCONVERTER(SoSFColor_SoSFVec3f, SoSFColor, SoSFVec3f);
  SOCONVERTALL_ADDCONVERTER(SoSFColor_SoMFVec3f, SoSFColor, SoMFVec3f);
  SOCONVERTALL_ADDCONVERTER(SoSFVec3f_SoSFColor, SoSFVec3f, SoSFColor);
  SOCONVERTALL_ADDCONVERTER(SoSFVec3f_SoMFColor, SoSFVec3f, SoMFColor);
  SOCONVERTALL_ADDCONVERTER(SoSFFloat_SoSFInt32, SoSFFloat, SoSFInt32);
  SOCONVERTALL_ADDCONVERTER(SoSFFloat_SoMFInt32, SoSFFloat, SoMFInt32);
  SOCONVERTALL_ADDCONVERTER(SoSFInt32_SoSFFloat, SoSFInt32, SoSFFloat);
  SOCONVERTALL_ADDCONVERTER(SoSFInt32_SoMFFloat, SoSFInt32, SoMFFloat);
  SOCONVERTALL_ADDCONVERTER(SoSFFloat_SoSFShort, SoSFFloat, SoSFShort);
  SOCONVERTALL_ADDCONVERTER(SoSFFloat_SoMFShort, SoSFFloat, SoMFShort);
  SOCONVERTALL_ADDCONVERTER(SoSFShort_SoSFFloat, SoSFShort, SoSFFloat);
  SOCONVERTALL_ADDCONVERTER(SoSFShort_SoMFFloat, SoSFShort, SoMFFloat);
  SOCONVERTALL_ADDCONVERTER(SoSFFloat_SoSFUInt32, SoSFFloat, SoSFUInt32);
  SOCONVERTALL_ADDCONVERTER(SoSFFloat_SoMFUInt32, SoSFFloat, SoMFUInt32);
  SOCONVERTALL_ADDCONVERTER(SoSFUInt32_SoSFFloat, SoSFUInt32, SoSFFloat);
  SOCONVERTALL_ADDCONVERTER(SoSFUInt32_SoMFFloat, SoSFUInt32, SoMFFloat);
  SOCONVERTALL_ADDCONVERTER(SoSFFloat_SoSFUShort, SoSFFloat, SoSFUShort);
  SOCONVERTALL_ADDCONVERTER(SoSFFloat_SoMFUShort, SoSFFloat, SoMFUShort);
  SOCONVERTALL_ADDCONVERTER(SoSFUShort_SoSFFloat, SoSFUShort, SoSFFloat);
  SOCONVERTALL_ADDCONVERTER(SoSFUShort_SoMFFloat, SoSFUShort, SoMFFloat);
  SOCONVERTALL_ADDCONVERTER(SoSFInt32_SoSFShort, SoSFInt32, SoSFShort);
  SOCONVERTALL_ADDCONVERTER(SoSFInt32_SoMFShort, SoSFInt32, SoMFShort);
  SOCONVERTALL_ADDCONVERTER(SoSFShort_SoSFInt32, SoSFShort, SoSFInt32);
  SOCONVERTALL_ADDCONVERTER(SoSFShort_SoMFInt32, SoSFShort, SoMFInt32);
  SOCONVERTALL_ADDCONVERTER(SoSFInt32_SoSFUInt32, SoSFInt32, SoSFUInt32);
  SOCONVERTALL_ADDCONVERTER(SoSFInt32_SoMFUInt32, SoSFInt32, SoMFUInt32);
  SOCONVERTALL_ADDCONVERTER(SoSFUInt32_SoSFInt32, SoSFUInt32, SoSFInt32);
  SOCONVERTALL_ADDCONVERTER(SoSFUInt32_SoMFInt32, SoSFUInt32, SoMFInt32);
  SOCONVERTALL_ADDCONVERTER(SoSFInt32_SoSFUShort, SoSFInt32, SoSFUShort);
  SOCONVERTALL_ADDCONVERTER(SoSFInt32_SoMFUShort, SoSFInt32, SoMFUShort);
  SOCONVERTALL_ADDCONVERTER(SoSFUShort_SoSFInt32, SoSFUShort, SoSFInt32);
  SOCONVERTALL_ADDCONVERTER(SoSFUShort_SoMFInt32, SoSFUShort, SoMFInt32);
  SOCONVERTALL_ADDCONVERTER(SoSFShort_SoSFUInt32, SoSFShort, SoSFUInt32);
  SOCONVERTALL_ADDCONVERTER(SoSFShort_SoMFUInt32, SoSFShort, SoMFUInt32);
  SOCONVERTALL_ADDCONVERTER(SoSFUInt32_SoSFShort, SoSFUInt32, SoSFShort);
  SOCONVERTALL_ADDCONVERTER(SoSFUInt32_SoMFShort, SoSFUInt32, SoMFShort);
  SOCONVERTALL_ADDCONVERTER(SoSFShort_SoSFUShort, SoSFShort, SoSFUShort);
  SOCONVERTALL_ADDCONVERTER(SoSFShort_SoMFUShort, SoSFShort, SoMFUShort);
  SOCONVERTALL_ADDCONVERTER(SoSFUShort_SoSFShort, SoSFUShort, SoSFShort);
  SOCONVERTALL_ADDCONVERTER(SoSFUShort_SoMFShort, SoSFUShort, SoMFShort);
  SOCONVERTALL_ADDCONVERTER(SoSFUInt32_SoSFUShort, SoSFUInt32, SoSFUShort);
  SOCONVERTALL_ADDCONVERTER(SoSFUInt32_SoMFUShort, SoSFUInt32, SoMFUShort);
  SOCONVERTALL_ADDCONVERTER(SoSFUShort_SoSFUInt32, SoSFUShort, SoSFUInt32);
  SOCONVERTALL_ADDCONVERTER(SoSFUShort_SoMFUInt32, SoSFUShort, SoMFUInt32);
  SOCONVERTALL_ADDCONVERTER(SoSFFloat_SoSFTime, SoSFFloat, SoSFTime);
  SOCONVERTALL_ADDCONVERTER(SoSFFloat_SoMFTime, SoSFFloat, SoMFTime);
  SOCONVERTALL_ADDCONVERTER(SoSFTime_SoSFFloat, SoSFTime, SoSFFloat);
  SOCONVERTALL_ADDCONVERTER(SoSFTime_SoMFFloat, SoSFTime, SoMFFloat);

  SOCONVERTALL_ADDCONVERTER(SoMFBool_SoSFFloat, SoMFBool, SoSFFloat);
  SOCONVERTALL_ADDCONVERTER(SoMFFloat_SoSFBool, SoMFFloat, SoSFBool);
  SOCONVERTALL_ADDCONVERTER(SoMFBool_SoSFInt32, SoMFBool, SoSFInt32);
  SOCONVERTALL_ADDCONVERTER(SoMFInt32_SoSFBool, SoMFInt32, SoSFBool);
  SOCONVERTALL_ADDCONVERTER(SoMFBool_SoSFShort, SoMFBool, SoSFShort);
  SOCONVERTALL_ADDCONVERTER(SoMFShort_SoSFBool, SoMFShort, SoSFBool);
  SOCONVERTALL_ADDCONVERTER(SoMFBool_SoSFUInt32, SoMFBool, SoSFUInt32);
  SOCONVERTALL_ADDCONVERTER(SoMFUInt32_SoSFBool, SoMFUInt32, SoSFBool);
  SOCONVERTALL_ADDCONVERTER(SoMFBool_SoSFUShort, SoMFBool, SoSFUShort);
  SOCONVERTALL_ADDCONVERTER(SoMFUShort_SoSFBool, SoMFUShort, SoSFBool);
  SOCONVERTALL_ADDCONVERTER(SoMFColor_SoSFVec3f, SoMFColor, SoSFVec3f);
  SOCONVERTALL_ADDCONVERTER(SoMFVec3f_SoSFColor, SoMFVec3f, SoSFColor);
  SOCONVERTALL_ADDCONVERTER(SoMFFloat_SoSFInt32, SoMFFloat, SoSFInt32);
  SOCONVERTALL_ADDCONVERTER(SoMFInt32_SoSFFloat, SoMFInt32, SoSFFloat);
  SOCONVERTALL_ADDCONVERTER(SoMFFloat_SoSFShort, SoMFFloat, SoSFShort);
  SOCONVERTALL_ADDCONVERTER(SoMFShort_SoSFFloat, SoMFShort, SoSFFloat);
  SOCONVERTALL_ADDCONVERTER(SoMFFloat_SoSFUInt32, SoMFFloat, SoSFUInt32);
  SOCONVERTALL_ADDCONVERTER(SoMFUInt32_SoSFFloat, SoMFUInt32, SoSFFloat);
  SOCONVERTALL_ADDCONVERTER(SoMFFloat_SoSFUShort, SoMFFloat, SoSFUShort);
  SOCONVERTALL_ADDCONVERTER(SoMFUShort_SoSFFloat, SoMFUShort, SoSFFloat);
  SOCONVERTALL_ADDCONVERTER(SoMFInt32_SoSFShort, SoMFInt32, SoSFShort);
  SOCONVERTALL_ADDCONVERTER(SoMFShort_SoSFInt32, SoMFShort, SoSFInt32);
  SOCONVERTALL_ADDCONVERTER(SoMFInt32_SoSFUInt32, SoMFInt32, SoSFUInt32);
  SOCONVERTALL_ADDCONVERTER(SoMFUInt32_SoSFInt32, SoMFUInt32, SoSFInt32);
  SOCONVERTALL_ADDCONVERTER(SoMFInt32_SoSFUShort, SoMFInt32, SoSFUShort);
  SOCONVERTALL_ADDCONVERTER(SoMFUShort_SoSFInt32, SoMFUShort, SoSFInt32);
  SOCONVERTALL_ADDCONVERTER(SoMFShort_SoSFUInt32, SoMFShort, SoSFUInt32);
  SOCONVERTALL_ADDCONVERTER(SoMFUInt32_SoSFShort, SoMFUInt32, SoSFShort);
  SOCONVERTALL_ADDCONVERTER(SoMFShort_SoSFUShort, SoMFShort, SoSFUShort);
  SOCONVERTALL_ADDCONVERTER(SoMFUShort_SoSFShort, SoMFUShort, SoSFShort);
  SOCONVERTALL_ADDCONVERTER(SoMFUInt32_SoSFUShort, SoMFUInt32, SoSFUShort);
  SOCONVERTALL_ADDCONVERTER(SoMFUShort_SoSFUInt32, SoMFUShort, SoSFUInt32);
  SOCONVERTALL_ADDCONVERTER(SoMFFloat_SoSFTime, SoMFFloat, SoSFTime);
  SOCONVERTALL_ADDCONVERTER(SoMFTime_SoSFFloat, SoMFTime, SoSFFloat);

  SOCONVERTALL_ADDCONVERTER(SoMFBool_SoMFFloat, SoMFBool, SoMFFloat);
  SOCONVERTALL_ADDCONVERTER(SoMFFloat_SoMFBool, SoMFFloat, SoMFBool);
  SOCONVERTALL_ADDCONVERTER(SoMFBool_SoMFInt32, SoMFBool, SoMFInt32);
  SOCONVERTALL_ADDCONVERTER(SoMFInt32_SoMFBool, SoMFInt32, SoMFBool);
  SOCONVERTALL_ADDCONVERTER(SoMFBool_SoMFShort, SoMFBool, SoMFShort);
  SOCONVERTALL_ADDCONVERTER(SoMFShort_SoMFBool, SoMFShort, SoMFBool);
  SOCONVERTALL_ADDCONVERTER(SoMFBool_SoMFUInt32, SoMFBool, SoMFUInt32);
  SOCONVERTALL_ADDCONVERTER(SoMFUInt32_SoMFBool, SoMFUInt32, SoMFBool);
  SOCONVERTALL_ADDCONVERTER(SoMFBool_SoMFUShort, SoMFBool, SoMFUShort);
  SOCONVERTALL_ADDCONVERTER(SoMFUShort_SoMFBool, SoMFUShort, SoMFBool);
  SOCONVERTALL_ADDCONVERTER(SoMFColor_SoMFVec3f, SoMFColor, SoMFVec3f);
  SOCONVERTALL_ADDCONVERTER(SoMFVec3f_SoMFColor, SoMFVec3f, SoMFColor);
  SOCONVERTALL_ADDCONVERTER(SoMFFloat_SoMFInt32, SoMFFloat, SoMFInt32);
  SOCONVERTALL_ADDCONVERTER(SoMFInt32_SoMFFloat, SoMFInt32, SoMFFloat);
  SOCONVERTALL_ADDCONVERTER(SoMFFloat_SoMFShort, SoMFFloat, SoMFShort);
  SOCONVERTALL_ADDCONVERTER(SoMFShort_SoMFFloat, SoMFShort, SoMFFloat);
  SOCONVERTALL_ADDCONVERTER(SoMFFloat_SoMFUInt32, SoMFFloat, SoMFUInt32);
  SOCONVERTALL_ADDCONVERTER(SoMFUInt32_SoMFFloat, SoMFUInt32, SoMFFloat);
  SOCONVERTALL_ADDCONVERTER(SoMFFloat_SoMFUShort, SoMFFloat, SoMFUShort);
  SOCONVERTALL_ADDCONVERTER(SoMFUShort_SoMFFloat, SoMFUShort, SoMFFloat);
  SOCONVERTALL_ADDCONVERTER(SoMFInt32_SoMFShort, SoMFInt32, SoMFShort);
  SOCONVERTALL_ADDCONVERTER(SoMFShort_SoMFInt32, SoMFShort, SoMFInt32);
  SOCONVERTALL_ADDCONVERTER(SoMFInt32_SoMFUInt32, SoMFInt32, SoMFUInt32);
  SOCONVERTALL_ADDCONVERTER(SoMFUInt32_SoMFInt32, SoMFUInt32, SoMFInt32);
  SOCONVERTALL_ADDCONVERTER(SoMFInt32_SoMFUShort, SoMFInt32, SoMFUShort);
  SOCONVERTALL_ADDCONVERTER(SoMFUShort_SoMFInt32, SoMFUShort, SoMFInt32);
  SOCONVERTALL_ADDCONVERTER(SoMFShort_SoMFUInt32, SoMFShort, SoMFUInt32);
  SOCONVERTALL_ADDCONVERTER(SoMFUInt32_SoMFShort, SoMFUInt32, SoMFShort);
  SOCONVERTALL_ADDCONVERTER(SoMFShort_SoMFUShort, SoMFShort, SoMFUShort);
  SOCONVERTALL_ADDCONVERTER(SoMFUShort_SoMFShort, SoMFUShort, SoMFShort);
  SOCONVERTALL_ADDCONVERTER(SoMFUInt32_SoMFUShort, SoMFUInt32, SoMFUShort);
  SOCONVERTALL_ADDCONVERTER(SoMFUShort_SoMFUInt32, SoMFUShort, SoMFUInt32);
  SOCONVERTALL_ADDCONVERTER(SoMFFloat_SoMFTime, SoMFFloat, SoMFTime);
  SOCONVERTALL_ADDCONVERTER(SoMFTime_SoMFFloat, SoMFTime, SoMFFloat);

  SOCONVERTALL_ADDCONVERTER(SoSFMatrix_SoSFRotation, SoSFMatrix, SoSFRotation);
  SOCONVERTALL_ADDCONVERTER(SoMFMatrix_SoSFRotation, SoMFMatrix, SoSFRotation);
  SOCONVERTALL_ADDCONVERTER(SoSFMatrix_SoMFRotation, SoSFMatrix, SoMFRotation);
  SOCONVERTALL_ADDCONVERTER(SoMFMatrix_SoMFRotation, SoMFMatrix, SoMFRotation);
  SOCONVERTALL_ADDCONVERTER(SoSFRotation_SoSFMatrix, SoSFRotation, SoSFMatrix);
  SOCONVERTALL_ADDCONVERTER(SoMFRotation_SoSFMatrix, SoMFRotation, SoSFMatrix);
  SOCONVERTALL_ADDCONVERTER(SoSFRotation_SoMFMatrix, SoSFRotation, SoMFMatrix);
  SOCONVERTALL_ADDCONVERTER(SoMFRotation_SoMFMatrix, SoMFRotation, SoMFMatrix);

  SOCONVERTALL_ADDCONVERTER(sftime_to_sfstring, SoSFTime, SoSFString);
  SOCONVERTALL_ADDCONVERTER(sftime_to_mfstring, SoSFTime, SoMFString);
  SOCONVERTALL_ADDCONVERTER(mftime_to_sfstring, SoMFTime, SoSFString);
  SOCONVERTALL_ADDCONVERTER(mftime_to_mfstring, SoMFTime, SoMFString);

#undef SOCONVERTALL_ADDCONVERTER

  // Now add conversion to and from SoSFTrigger for all other
  // non-abstract field types (all conversions done by the same
  // function).

  SoTypeList allfieldtypes;
  int nrfieldtypes = SoType::getAllDerivedFrom(SoField::getClassTypeId(),
                                               allfieldtypes);
  for (int i=0; i < nrfieldtypes; i++) {
    if (allfieldtypes[i].canCreateInstance() &&
        allfieldtypes[i] != SoSFTrigger::getClassTypeId()) {
      SoConvertAll::register_converter(to_and_from_sftrigger,
                                       SoSFTrigger::getClassTypeId(),
                                       allfieldtypes[i]);
      SoConvertAll::register_converter(to_and_from_sftrigger,
                                       allfieldtypes[i],
                                       SoSFTrigger::getClassTypeId());
    }
  }
}

SoConvertAll::SoConvertAll(const SoType from, const SoType to)
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoConvertAll::SoConvertAll",
                         "from: %s, to: %s",
                         from.getName().getString(),
                         to.getName().getString());
#endif // debug

  // This code is instead of SO_ENGINE_CONSTRUCTOR(), which we can't
  // use due to the fact that we need dynamic lists of input fields
  // and engine outputs.
  { // SO_ENGINE_CONSTRUCTOR replacement start
    SoConvertAll::classinstances++;
    /* Catch attempts to use an engine class which has not been initialized. */
    assert(SoConvertAll::classTypeId != SoType::badType());

    this->inputdata_instance =
      new SoFieldData(SoConvertAll::parentinputdata ?
                      *SoConvertAll::parentinputdata : NULL);

    this->outputdata_instance =
      new SoEngineOutputData(SoConvertAll::parentoutputdata ?
                             *SoConvertAll::parentoutputdata : NULL);

    /* SoConvertAll is not considered native (doesn't really matter
       one way or the other). */
    this->isBuiltIn = FALSE;
  } // SO_ENGINE_CONSTRUCTOR replacement end



  this->input = (SoField *)from.createInstance();

  this->input->setContainer(this);
  this->output.setContainer(this);
  this->outputdata_instance->addOutput(this, "output", &this->output, to);

  uint32_t val = (((uint32_t)from.getKey()) << 16) + to.getKey();
  void * ptr;
  if (!SoConvertAll::converter_dict->find(val, ptr)) assert(FALSE);
  this->convertvalue = (converter_func *)ptr;
}

SoConvertAll::~SoConvertAll()
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoConvertAll::~SoConvertAll", "%p", this);
#endif // debug
  delete this->input;
}

SoField *
SoConvertAll::getInput(SoType type)
{
#if COIN_DEBUG
  SoType inputtype = this->input->getTypeId();
  if (type != inputtype && type != SoType::badType()) {
    SoDebugError::postWarning("SoConvertAll::getInput",
                              "field is of type %s, not %s",
                              inputtype.getName().getString(),
                              type.getName().getString());
  }
#endif // COIN_DEBUG

  // ignore type, as we have only a single input field

  return this->input;
}

SoEngineOutput *
SoConvertAll::getOutput(SoType type)
{
#if COIN_DEBUG
  SoType outputtype = this->output.getConnectionType();
  if (type != outputtype && type != SoType::badType()) {
    SoDebugError::postWarning("SoConvertAll::getOutput",
                              "engineout is of type %s, not %s",
                              outputtype.getName().getString(),
                              type.getName().getString());
  }
#endif // COIN_DEBUG

  // ignore type, as we have only a single engineoutput

  return &this->output;
}

void
SoConvertAll::evaluate(void)
{
  // we cannot use the SO_ENGINE_OUTPUT macro, but this code should
  // do the same thing.
  if (this->output.isEnabled()) {
    for (int i = 0 ; i < this->output.getNumConnections(); i++) {
      SoField * f = this->output[i];
      if (!f->isReadOnly()) {
        // Convert directly from the "real" master field if possible,
        // to behave properly on enum fields (the this->input instance
        // doesn't contain the name<->value mappings in the case that
        // the master field is of type So[SM]FEnum or So[SM]FBitMask).
        SoField * masterfield = NULL;
        if (this->input->getConnectedField(masterfield))
          this->convertvalue(masterfield, f);
        // Couldn't get master field, this means we are connected to
        // an engine output (at least we _should_ be, could probably
        // do with an assert here).
        else
          this->convertvalue(this->input, f);
      }
    }
  }
}
