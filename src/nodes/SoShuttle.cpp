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
  \class SoShuttle SoShuttle.h Inventor/nodes/SoShuttle.h
  \brief The SoShuttle class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoShuttle.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>

/*!
  \var SoSFVec3f SoShuttle::translation0
  FIXME: write documentation for field
*/
/*!
  \var SoSFVec3f SoShuttle::translation1
  FIXME: write documentation for field
*/
/*!
  \var SoSFFloat SoShuttle::speed
  FIXME: write documentation for field
*/
/*!
  \var SoSFBool SoShuttle::on
  FIXME: write documentation for field
*/

// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoShuttle)
SoType SoShuttle::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoShuttle node class.
*/
void *
SoShuttle::createInstance(void)
{
  return new SoShuttle;
}

/*!
  Returns the unique type identifier for the SoShuttle class.
*/
SoType
SoShuttle::getClassTypeId(void)
{
  return SoShuttle::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoShuttle::getTypeId(void) const
{
  return SoShuttle::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoShuttle::SoShuttle()
{
//$ BEGIN TEMPLATE NodeConstructor(SoShuttle)
  // Make sure the class has been initialized.
  assert(SoShuttle::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

  SO_NODE_ADD_FIELD(translation0, (SbVec3f(0.0f, 0.0f, 0.0f)));
  SO_NODE_ADD_FIELD(translation1, (SbVec3f(0.0f, 0.0f, 0.0f)));
  SO_NODE_ADD_FIELD(speed, (1.0f));
  SO_NODE_ADD_FIELD(on, (TRUE));
}

/*!
  Destructor.
*/
SoShuttle::~SoShuttle()
{
}

/*!
  Does initialization common for all objects of the
  SoShuttle class. This includes setting up the
  type system, among other things.
*/
void
SoShuttle::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource(Shuttle)
  // Make sure we only initialize once.
  assert(SoShuttle::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoShuttle::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "Shuttle",
                       &SoShuttle::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoShuttle::cleanClass(void)
{
}
