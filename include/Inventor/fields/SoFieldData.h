#ifndef COIN_SOFIELDDATA_H
#define COIN_SOFIELDDATA_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

#include <Inventor/SbBasic.h>
#include <Inventor/lists/SbList.h>

class SbName;
class SoField;
class SoFieldContainer;
class SoInput;
class SoOutput;

class SoEnumEntry;
class SoFieldEntry;

class COIN_DLL_API SoFieldData {
public:
  SoFieldData(void);
  SoFieldData(const SoFieldData & fd);
  SoFieldData(const SoFieldData * fd);
  SoFieldData(int numfields);
  ~SoFieldData();

  void addField(SoFieldContainer * base, const char * name,
                const SoField * field);

  void overlay(SoFieldContainer * to, const SoFieldContainer * from,
               SbBool copyconnections) const;

  int getNumFields(void) const;
  const SbName & getFieldName(int index) const;
  SoField * getField(const SoFieldContainer * object, int index) const;
  int getIndex(const SoFieldContainer * fc, const SoField * field) const;

  void addEnumValue(const char * enumname, const char * valuename, int value);
  void getEnumData(const char * enumname,
                   int & num, const int *& values, const SbName *& names);

  SbBool read(SoInput * in, SoFieldContainer * object,
              SbBool erroronunknownfield, SbBool & notbuiltin) const;
  SbBool read(SoInput * in, SoFieldContainer * object,
              const SbName & fieldname, SbBool & foundname) const;
  void write(SoOutput * out, const SoFieldContainer * object) const;
  void copy(const SoFieldData * src);
  SbBool isSame(const SoFieldContainer * c1,
                const SoFieldContainer * c2) const;

  SbBool readFieldDescriptions(SoInput * in, SoFieldContainer * object,
                               int numdescriptionsexpected,
                               const SbBool readfieldvalues = TRUE) const;
  void writeFieldDescriptions(SoOutput * out,
                              const SoFieldContainer * object) const;

private:
  // Bitflags for control word in the file format.
  enum ControlWord {
    NOTBUILTIN = 0x40
  };

  int operator==(const SoFieldData * fd) const;
  int operator!=(const SoFieldData * fd) const { return ! operator==(fd); }
  int operator==(const SoFieldData & fd) const { return operator==(&fd); }
  int operator!=(const SoFieldData & fd) const { return ! operator==(&fd); }

  void freeResources(void);

  SbList<SoFieldEntry *> fields;
  SbList<SoEnumEntry *> enums;
};

#endif // !COIN_SOFIELDDATA_H
