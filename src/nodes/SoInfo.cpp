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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

/*!
  \class SoInfo SoInfo.h Inventor/nodes/SoInfo.h
  \brief The SoInfo class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoInfo.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>

/*!
  \var SoSFString SoInfo::string
  FIXME: write documentation for field
*/

// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoInfo)
SoType SoInfo::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoInfo node class.
*/
void *
SoInfo::createInstance(void)
{
  return new SoInfo;
}

/*!
  Returns the unique type identifier for the SoInfo class.
*/
SoType
SoInfo::getClassTypeId(void)
{
  return SoInfo::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoInfo::getTypeId(void) const
{
  return SoInfo::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoInfo::SoInfo()
{
//$ BEGIN TEMPLATE NodeConstructor(SoInfo)
  // Make sure the class has been initialized.
  assert(SoInfo::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

  SO_NODE_ADD_FIELD(string, ("<Undefined info>"));
}

/*!
  Destructor.
*/
SoInfo::~SoInfo()
{
}

/*!
  Does initialization common for all objects of the
  SoInfo class. This includes setting up the
  type system, among other things.
*/
void
SoInfo::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource(Info)
  // Make sure we only initialize once.
  assert(SoInfo::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoInfo::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "Info",
                       &SoInfo::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoInfo::cleanClass(void)
{
}
