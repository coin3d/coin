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
  \class SoPendulum SoPendulum.h Inventor/nodes/SoPendulum.h
  \brief The SoPendulum class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoPendulum.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>
#include <Inventor/SbVec3f.h>

/*!
  \var SoSFRotation SoPendulum::rotation0
  FIXME: write documentation for field
*/
/*!
  \var SoSFRotation SoPendulum::rotation1
  FIXME: write documentation for field
*/
/*!
  \var SoSFFloat SoPendulum::speed
  FIXME: write documentation for field
*/
/*!
  \var SoSFBool SoPendulum::on
  FIXME: write documentation for field
*/

// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoPendulum)
SoType SoPendulum::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoPendulum node class.
*/
void *
SoPendulum::createInstance(void)
{
  return new SoPendulum;
}

/*!
  Returns the unique type identifier for the SoPendulum class.
*/
SoType
SoPendulum::getClassTypeId(void)
{
  return SoPendulum::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoPendulum::getTypeId(void) const
{
  return SoPendulum::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoPendulum::SoPendulum()
{
//$ BEGIN TEMPLATE NodeConstructor(SoPendulum )
  // Make sure the class has been initialized.
  assert(SoPendulum::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

  SO_NODE_ADD_FIELD(rotation0, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_NODE_ADD_FIELD(rotation1, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_NODE_ADD_FIELD(speed, (1.0f));
  SO_NODE_ADD_FIELD(on, (TRUE));
}

/*!
  Destructor.
*/
SoPendulum::~SoPendulum()
{
}

/*!
  Does initialization common for all objects of the
  SoPendulum class. This includes setting up the
  type system, among other things.
*/
void
SoPendulum::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource(Pendulum )
  // Make sure we only initialize once.
  assert(SoPendulum::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoPendulum::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "Pendulum",
                       &SoPendulum::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoPendulum::cleanClass(void)
{
}
