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
  \class SoTextureCoordinateFunction SoTextureCoordinateFunction.h Inventor/nodes/SoTextureCoordinateFunction.h
  \brief The SoTextureCoordinateFunction class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoTextureCoordinateFunction.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>

// *************************************************************************

//$ BEGIN TEMPLATE NodeAbstractSource(SoTextureCoordinateFunction)
SoType SoTextureCoordinateFunction::classTypeId = SoType::badType();

/*!
  Returns the unique type identifier for the SoTextureCoordinateFunction class.
*/
SoType
SoTextureCoordinateFunction::getClassTypeId(void)
{
  return SoTextureCoordinateFunction::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoTextureCoordinateFunction::getTypeId(void) const
{
  return SoTextureCoordinateFunction::classTypeId;
}
//$ END TEMPLATE NodeAbstractSource

/*!
  Constructor.
*/
SoTextureCoordinateFunction::SoTextureCoordinateFunction()
{
//$ BEGIN TEMPLATE NodeConstructor(SoTextureCoordinateFunction)
  // Make sure the class has been initialized.
  assert(SoTextureCoordinateFunction::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor
}

/*!
  Destructor.
*/
SoTextureCoordinateFunction::~SoTextureCoordinateFunction()
{
}

/*!
  Does initialization common for all objects of the
  SoTextureCoordinateFunction class. This includes setting up the
  type system, among other things.
*/
void
SoTextureCoordinateFunction::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeAbstractSource(TextureCoordinateFunction)
  // Make sure we only initialize once.
  assert(SoTextureCoordinateFunction::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoTextureCoordinateFunction::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "TextureCoordinateFunction",
                       NULL,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeAbstractSource
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoTextureCoordinateFunction::cleanClass(void)
{
}
