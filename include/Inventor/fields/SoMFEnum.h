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

#ifndef COIN_SOMFENUM_H
#define COIN_SOMFENUM_H

#include <Inventor/fields/SoMField.h>
#include <Inventor/fields/SoSubField.h>


class COIN_DLL_EXPORT SoMFEnum : public SoMField {
  typedef SoMField inherited;

  SO_MFIELD_HEADER(SoMFEnum, int, int);

public:
  static void initClass(void);

  void setValue(const SbName name);
  void set1Value(const int idx, const SbName name);
  void setEnums(const int num, const int * const values,
                const SbName * const names);

protected:
  virtual SbBool findEnumValue(const SbName & name, int & value);
  virtual SbBool findEnumName(int value, const SbName * & name) const;

  SbBool legalValuesSet;
  int numEnums;
  int * enumValues;
  SbName * enumNames;
};


// Macro definitions. See comment in SoSFEnum.h.

#include <Inventor/fields/SoSFEnum.h> // PRIVATE_SOENUM_SET_TYPE()

#define SO_ENGINE_SET_MF_ENUM_TYPE(_fieldname_, _enumtype_) \
  PRIVATE_SOENUM_SET_TYPE(_fieldname_, _enumtype_, inputdata)

#define SO_NODE_SET_MF_ENUM_TYPE(_fieldname_, _enumtype_) \
  PRIVATE_SOENUM_SET_TYPE(_fieldname_, _enumtype_, fieldData)


#endif // !COIN_SOMFENUM_H
