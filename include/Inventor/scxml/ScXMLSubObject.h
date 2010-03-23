#ifndef COIN_SCXMLSUBOBJECT_H
#define COIN_SCXMLSUBOBJECT_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/SbName.h>

/*
 * This file contains macros for setting up the classes in the ScXML
 * subsystem (Statechart XML) in Coin. They loosely follow the macros
 * design for nodes and nodekits and engines, etc.
 */

// *************************************************************************

#define SCXML_OBJECT_HEADER(classname)                          \
public:                                                         \
  static SoType getClassTypeId(void);                           \
  virtual SoType getTypeId(void) const;                         \
  static void * createInstance(void);                           \
private:                                                        \
  static SoType classTypeId;

// *************************************************************************

#define SCXML_OBJECT_ABSTRACT_HEADER(classname)                 \
public:                                                         \
  static SoType getClassTypeId(void);                           \
  virtual SoType getTypeId(void) const = 0;                     \
private:                                                        \
  static SoType classTypeId;

// *************************************************************************

#define SCXML_ELEMENT_ABSTRACT_HEADER(classname)                \
  SCXML_OBJECT_ABSTRACT_HEADER(classname)

// *************************************************************************

// FIXME: element-reader must be reenatrant/threadsafe
#define SCXML_ELEMENT_HEADER(classname)                         \
public:                                                         \
  static ScXMLEltReader * getElementReader(void);               \
private:                                                        \
  static ScXMLEltReader * elementReader;                        \
  SCXML_OBJECT_HEADER(classname)

// *************************************************************************

#define SCXML_OBJECT_SOURCE(classname)                          \
                                                                \
SoType classname::classTypeId = SoType::badType();              \
                                                                \
SoType                                                          \
classname::getClassTypeId(void)                                 \
{                                                               \
  return classname::classTypeId;                                \
}                                                               \
                                                                \
SoType                                                          \
classname::getTypeId(void) const                                \
{                                                               \
  return classname::classTypeId;                                \
}                                                               \
                                                                \
void *                                                          \
classname::createInstance(void)                                 \
{                                                               \
  return static_cast<void *>(new classname);                    \
}

// *************************************************************************

#define SCXML_OBJECT_ABSTRACT_SOURCE(classname)                 \
                                                                \
SoType classname::classTypeId = SoType::badType();              \
                                                                \
SoType                                                          \
classname::getClassTypeId(void)                                 \
{                                                               \
  return classname::classTypeId;                                \
}

#define SCXML_ELEMENT_ABSTRACT_SOURCE(classname)                \
  SCXML_OBJECT_ABSTRACT_SOURCE(classname)

#define SCXML_ELEMENT_SOURCE(classname)                         \
ScXMLEltReader * classname::elementReader = NULL;               \
                                                                \
ScXMLEltReader *                                                \
classname::getElementReader(void)                               \
{                                                               \
  return classname::elementReader;                              \
}                                                               \
                                                                \
  SCXML_OBJECT_SOURCE(classname)

// *************************************************************************

#define SCXML_OBJECT_INIT_CLASS(thisclass, parentclass, parentname)     \
  do {                                                                  \
    SoType parenttype = SoType::fromName(SO__QUOTE(parentclass));       \
    assert(parenttype != SoType::badType());                            \
    thisclass::classTypeId =                                            \
      SoType::createType(parenttype,                                    \
                         SbName(SO__QUOTE(thisclass)),                  \
                         thisclass::createInstance);                    \
 /* ScXMLObject::registerClassType(xmlns, xmlclass,                     \
                                   thisclass::classTypeId); */          \
  } while ( FALSE )


#define SCXML_ELEMENT_REGISTER_READER(thisclass, xmlelement, classname) \
  do {                                                                  \
    thisclass::elementReader = new classname;                           \
  } while ( FALSE )

#define SCXML_ELEMENT_UNREGISTER_READER(thisclass) \
  do {                                                                  \
    delete thisclass::elementReader;                                    \
    thisclass::elementReader = NULL;                                    \
  } while ( FALSE )

// *************************************************************************

#define SCXML_OBJECT_INIT_ABSTRACT_CLASS(thisclass, parentclass, parentname) \
  do {                                                                  \
    SoType parenttype = SoType::fromName(SO__QUOTE(parentclass));       \
    assert(parenttype != SoType::badType());                            \
    thisclass::classTypeId =                                            \
      SoType::createType(parenttype, SbName(SO__QUOTE(thisclass)));     \
  } while ( FALSE )

// *************************************************************************

#define SCXML_INVOKE_INIT_CLASS(thisclass, parentclass, xmlns, targettype, source) \
  do {                                                                  \
    SoType parenttype = SoType::fromName(SO__QUOTE(parentclass));       \
    assert(parenttype != SoType::badType());                            \
    thisclass::classTypeId =                                            \
      SoType::createType(parenttype,                                    \
                         SbName(SO__QUOTE(thisclass)),                  \
                         thisclass::createInstance);                    \
    ScXMLObject::registerInvokeClassType(xmlns, targettype, source,     \
                                         thisclass::classTypeId);       \
  } while ( FALSE )

// *************************************************************************


#endif // !COIN_SCXMLSUBOBJECT_H
