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

#ifndef __SOTYPE_H__
#define __SOTYPE_H__

#include <Inventor/SbBasic.h>
#include <Inventor/SbDict.h>
#include <Inventor/system/inttypes.h>
#include <stdlib.h> // For NULL definition.

class SbName;
class SoTypedObject;
class SoTypeList;
struct SoTypeData;
template <class Type> class SbList;

class SoType {
public:
  typedef void * (*instantiationMethod)(void);

  static SoType fromName(const SbName name);
  SbName getName(void) const;
  const SoType getParent(void) const;
  SbBool isDerivedFrom(const SoType type) const;

  int getAllDerivedFrom(SoTypeList & list) const;
  static int getAllDerivedFrom(const SoType type, SoTypeList & list);

  int getAllSubTypes(SoTypeList & list) const;
  
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
				 const instantiationMethod method = NULL,
				 const uint16_t data = 0);

  static const SoType overrideType(const SoType originalType,
				   const instantiationMethod method = NULL);
  
  static void init(void);

  static SoType fromKey(uint16_t key);
  static SoType badType(void);
  SbBool isBad(void) const;

  void makeInternal(void);
  SbBool isInternal(void) const;
  
  static int getNumTypes(void);
  
private:    
  int16_t index;

  static SoTypeList typeList;
  static SbList<SoTypeData *> typeDataList;
  static SbDict typeDict;
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

#endif // !__SOTYPE_H__
