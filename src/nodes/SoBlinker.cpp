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
  \class SoBlinker SoBlinker.h Inventor/nodes/SoBlinker.h
  \brief The SoBlinker class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/SbName.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoBlinker.h>

/*!
  \var SoSFFloat SoBlinker::speed
  FIXME: write documentation for field
*/
/*!
  \var SoSFBool SoBlinker::on
  FIXME: write documentation for field
*/


// *************************************************************************

//$ BEGIN TEMPLATE NodeSource( SoBlinker )
SoType SoBlinker::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoBlinker node class.
*/
void *
SoBlinker::createInstance(void)
{
  return new SoBlinker;
}

/*!
  Returns the unique type identifier for the SoBlinker class.
*/
SoType
SoBlinker::getClassTypeId(void)
{
  return SoBlinker::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoBlinker::getTypeId(void) const
{
  return SoBlinker::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoBlinker::SoBlinker()
{
//$ BEGIN TEMPLATE NodeConstructor( SoBlinker )
  // Make sure the class has been initialized.
  assert(SoBlinker::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

  SO_NODE_ADD_FIELD(speed, (1));
  SO_NODE_ADD_FIELD(on, (TRUE));
}

/*!
  Destructor.
*/
SoBlinker::~SoBlinker()
{
}

/*!
  Does initialization common for all objects of the
  SoBlinker class. This includes setting up the
  type system, among other things.
*/
void
SoBlinker::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource( Blinker )
  // Make sure we only initialize once.
  assert(SoBlinker::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoBlinker::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "Blinker",
                       &SoBlinker::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoBlinker::cleanClass(void)
{
}

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write doc
*/
void
SoBlinker::getBoundingBox(SoGetBoundingBoxAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOWRITEACTION)
/*!
  FIXME: write doc
*/
void
SoBlinker::write(SoWriteAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOWRITEACTION
