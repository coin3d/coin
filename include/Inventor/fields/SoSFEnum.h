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

#ifndef COIN_SOSFENUM_H
#define COIN_SOSFENUM_H

#include <Inventor/fields/SoSField.h>
#include <Inventor/fields/SoSubField.h>
#include <Inventor/SbString.h>


class COIN_DLL_EXPORT SoSFEnum : public SoSField {
  typedef SoSField inherited;

  SO_SFIELD_HEADER(SoSFEnum, int, int);

public:
  static void initClass(void);

  void setValue(const SbName name);
  void setEnums(const int num, const int * vals, const SbName * names);

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
