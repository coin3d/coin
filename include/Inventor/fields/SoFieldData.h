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

#ifndef __SOFIELDDATA_H__
#define __SOFIELDDATA_H__

#include <Inventor/SbBasic.h>
#include <Inventor/lists/SbList.h>

class SbName;
class SoField;
class SoFieldContainer;
class SoInput;
class SoOutput;

class SoEnumEntry;
struct SoFieldEntry;

class SoFieldData {
public:
  SoFieldData(void);
  SoFieldData(const SoFieldData & fd);
  SoFieldData(const SoFieldData * fd);
  SoFieldData(int numfields);
  ~SoFieldData();

  void addField(SoFieldContainer * base, const char * name,
                const SoField * field);

  void overlay(SoFieldContainer * to, const SoFieldContainer * from,
               SbBool copyConnections) const;

  int getNumFields(void) const;
  const SbName & getFieldName(int index) const;
  SoField * getField(const SoFieldContainer * object, int index) const;
  int getIndex(const SoFieldContainer * fc, const SoField * field) const;

  void addEnumValue(const char * typeName, const char * valName, int val);
  void getEnumData(const char * typeName, int & num,
                   const int *& vals, const SbName *& names);

  SbBool read(SoInput * in, SoFieldContainer * object,
              SbBool errorOnUnknownField, SbBool & notBuiltIn) const;
  SbBool read(SoInput * in, SoFieldContainer * object,
              const SbName & fieldName, SbBool & foundName) const;
  void write(SoOutput * out, const SoFieldContainer * object) const;
  void copy(const SoFieldData * src);
  SbBool isSame(const SoFieldContainer * c1,
                const SoFieldContainer * c2) const;

  SbBool readFieldDescriptions(SoInput * in, SoFieldContainer * object,
                               int numDescriptionsExpected) const;
  void writeFieldDescriptions(SoOutput * out,
                              const SoFieldContainer * object) const;

  // -- FIXME: move to readFieldDescriptions()?

  SbBool readFieldTypes(SoInput * in, SoFieldContainer * object);


private:
  static SbName stripWhite(const char * name);

  SbList<SoFieldEntry *> fields;
  SbList<SoEnumEntry *> enums;
};

#endif // !__SOFIELDDATA_H__
