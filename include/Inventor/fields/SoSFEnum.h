#ifndef COIN_SOSFENUM_H
#define COIN_SOSFENUM_H

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

#include <Inventor/fields/SoSField.h>
#include <Inventor/fields/SoSubField.h>
#include <Inventor/SbString.h>

class COIN_DLL_API SoSFEnum : public SoSField {
  typedef SoSField inherited;

  SO_SFIELD_HEADER(SoSFEnum, int, int);

public:
  static void initClass(void);

  void setValue(const SbName name);
  void setEnums(const int num, const int * vals, const SbName * names);

  int getNumEnums(void) const;
  int getEnum(const int idx, SbName & name) const;

protected:
  virtual SbBool findEnumValue(const SbName & name, int & val);
  virtual SbBool findEnumName(int value, const SbName * & name) const;

  int numEnums;
  int * enumValues;
  SbName * enumNames;
  SbBool legalValuesSet;
};

// These macros are called from both node and engine classes which
// uses SoSFEnum instances, so we put them here instead of in
// SoSub[Node|Engine].h.

#define PRIVATE_SOENUM_SET_TYPE(_fieldname_, _enumtype_, _fieldstorage_) \
  do { \
    /* Don't need to worry about name clashes, as we're inside our own scope. */ \
    int num; \
    const int * values; \
    const SbName * names; \
    _fieldstorage_->getEnumData(SO__QUOTE(_enumtype_), num, values, names); \
    this->_fieldname_.setEnums(num, values, names); \
  } while (FALSE)


#define SO_ENGINE_SET_SF_ENUM_TYPE(_fieldname_, _enumtype_) \
  PRIVATE_SOENUM_SET_TYPE(_fieldname_, _enumtype_, inputdata)

#define SO_NODE_SET_SF_ENUM_TYPE(_fieldname_, _enumtype_) \
  PRIVATE_SOENUM_SET_TYPE(_fieldname_, _enumtype_, fieldData)

#endif // !COIN_SOSFENUM_H
