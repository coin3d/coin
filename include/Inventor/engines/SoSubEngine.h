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

#ifndef __SOSUBENGINE_H__
#define __SOSUBENGINE_H__

#include <Inventor/engines/SoEngine.h>
#include <Inventor/misc/SoBasic.h>
#include <Inventor/SbName.h>
#include <Inventor/SoType.h>
#include <Inventor/errors/SoDebugError.h>

#define SO_ENGINE_ABSTRACT_HEADER(className) \
private: \
  static SoType classTypeId; \
public: \
  static SoType getClassTypeId() {return className::classTypeId;} \
  virtual SoType getTypeId() const {return className::getClassTypeId();}

#define SO_ENGINE_HEADER(className) \
SO_ENGINE_ABSTRACT_HEADER(className) \
public: \
  static void *createInstance()

#define SO_ENGINE_ABSTRACT_SOURCE(className) \
SoType className::classTypeId;

#define SO_ENGINE_SOURCE(className) \
SO_ENGINE_ABSTRACT_SOURCE(className) \
void *className::createInstance() \
{ \
  return new className; \
} 

#define SO_ENGINE_CONSTRUCTOR(className)\
this->outputList=new SoEngineOutputList;

#define SO_ENGINE_ADD_INPUT(memberName,defaultValue)\
this->memberName.setValue defaultValue;\
this->memberName.setContainer(this);

#define SO_ENGINE_ADD_OUTPUT(memberName,outputType)\
this->memberName.setType(outputType::getClassTypeId());\
this->memberName.setContainer(this);\
this->outputList->append(&this->memberName);

#define SO_ENGINE_INIT_ABSTRACT_CLASS(className,parentClass,parentString)\
classTypeId=SoType::createType(parentClass::getClassTypeId(),SO__QUOTE(className));

#define SO_ENGINE_INIT_CLASS(className,parentClass,parentString)\
classTypeId=SoType::createType(parentClass::getClassTypeId(),SO__QUOTE(className),&className::createInstance)

#define SO_ENGINE_OUTPUT(outMember,outType,outValue)\
{ \
  if (outMember.isEnabled()) \
    for (int _i=0;_i<outMember.getNumConnections();_i++) \
      ((outType *)outMember[_i])->outValue; \
}
//FIXME: check read-only

#endif // !__SOSUBENGINE_H__
