#ifndef COIN_SOTYPE_H
#define COIN_SOTYPE_H

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

#include <Inventor/SbBasic.h>
#include <Inventor/SbDict.h>
#include <stdlib.h> // For NULL definition.

class SbName;
class SoTypedObject;
class SoTypeList;
struct SoTypeData;
template <class Type> class SbList;

class COIN_DLL_API SoType {
public:
  typedef void * (*instantiationMethod)(void);

  static SoType fromName(const SbName name);
  SbName getName(void) const;
  const SoType getParent(void) const;
  SbBool isDerivedFrom(const SoType type) const;

  static int getAllDerivedFrom(const SoType type, SoTypeList & list);

  SbBool canCreateInstance(void) const;
  void * createInstance(void) const;

  uint16_t getData(void) const;
  int16_t getKey(void) const;

  SbBool operator == (const SoType type) const;
  SbBool operator != (const SoType type) const;

  SbBool operator <  (const SoType type) const;
  SbBool operator <= (const SoType type) const;
  SbBool operator >= (const SoType type) const;
  SbBool operator >  (const SoType type) const;

  static const SoType createType(const SoType parent, const SbName name,
                                 const instantiationMethod method = (instantiationMethod) NULL,
                                 const uint16_t data = 0);

  static const SoType overrideType(const SoType originalType,
                                   const instantiationMethod method = (instantiationMethod) NULL);

  static void init(void);

  static SoType fromKey(uint16_t key);
  static SoType badType(void);
  SbBool isBad(void) const;

  void makeInternal(void);
  SbBool isInternal(void) const;

  static int getNumTypes(void);

  instantiationMethod getInstantiationMethod(void) const;

private:
  static void clean(void);

  int16_t index;

  static SoTypeList * typelist;
  static SbList<SoTypeData *> * typedatalist;
  static SbDict * typedict;
};

/* inline methods ************************************************/

inline int16_t
SoType::getKey(void) const
{
  return this->index;
}

inline SbBool
SoType::operator != (const SoType type) const
{
  return (this->getKey() != type.getKey());
}

inline SbBool
SoType::operator == (const SoType type) const
{
  return (this->getKey() == type.getKey());
}

inline SbBool
SoType::operator <  (const SoType type) const
{
  return (this->getKey() < type.getKey());
}

inline SbBool
SoType::operator <= (const SoType type) const
{
  return (this->getKey() <= type.getKey());
}

inline SbBool
SoType::operator >= (const SoType type) const
{
  return (this->getKey() >= type.getKey());
}

inline SbBool
SoType::operator >  (const SoType type) const
{
  return (this->getKey() > type.getKey());
}

inline SbBool
SoType::isBad(void) const
{
  return (this->index == 0);
}

#endif // !COIN_SOTYPE_H
