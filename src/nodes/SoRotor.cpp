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

/*!
  \class SoRotor SoRotor.h Inventor/nodes/SoRotor.h
  \brief The SoRotor class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoRotor.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>

/*!
  \var SoSFFloat SoRotor::speed
  FIXME: write documentation for field
*/
/*!
  \var SoSFBool SoRotor::on
  FIXME: write documentation for field
*/

// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoRotor)
SoType SoRotor::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoRotor node class.
*/
void *
SoRotor::createInstance(void)
{
  return new SoRotor;
}

/*!
  Returns the unique type identifier for the SoRotor class.
*/
SoType
SoRotor::getClassTypeId(void)
{
  return SoRotor::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoRotor::getTypeId(void) const
{
  return SoRotor::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoRotor::SoRotor()
{
//$ BEGIN TEMPLATE NodeConstructor(SoRotor)
  // Make sure the class has been initialized.
  assert(SoRotor::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

  SO_NODE_ADD_FIELD(speed, (1.0f));
  SO_NODE_ADD_FIELD(on, (TRUE));
}

/*!
  Destructor.
*/
SoRotor::~SoRotor()
{
}

/*!
  Does initialization common for all objects of the
  SoRotor class. This includes setting up the
  type system, among other things.
*/
void
SoRotor::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource(Rotor)
  // Make sure we only initialize once.
  assert(SoRotor::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoRotor::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "Rotor",
                       &SoRotor::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoRotor::cleanClass(void)
{
}
