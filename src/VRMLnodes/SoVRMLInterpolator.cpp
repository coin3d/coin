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
  \class SoVRMLInterpolator SoVRMLInterpolator.h Inventor/VRMLnodes/SoVRMLInterpolator.h
  \brief The SoVRMLInterpolator class ...
  \ingroup vrml2

  FIXME: write class doc
*/

#include <Inventor/VRMLnodes/SoVRMLInterpolator.h>
#include <Inventor/SbName.h>

//$ BEGIN TEMPLATE NodeAbstractSource( SoVRMLInterpolator )
SoType SoVRMLInterpolator::classTypeId = SoType::badType();

/*!
  Returns the unique type identifier for the SoVRMLInterpolator class.
*/
SoType
SoVRMLInterpolator::getClassTypeId(void)
{
  return SoVRMLInterpolator::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoVRMLInterpolator::getTypeId(void) const
{
  return SoVRMLInterpolator::classTypeId;
}
//$ END TEMPLATE NodeAbstractSource

/*!
  Constructor.
*/
SoVRMLInterpolator::SoVRMLInterpolator(void)
{
//$ BEGIN TEMPLATE NodeConstructor( SoVRMLInterpolator )
  // Make sure the class has been initialized.
  assert(SoVRMLInterpolator::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor
}

/*!
  Destructor.
*/
SoVRMLInterpolator::~SoVRMLInterpolator()
{
}

/*!
  Does initialization common for all objects of the
  SoVRMLInterpolator class. This includes setting up the
  type system, among other things.
*/
void
SoVRMLInterpolator::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeAbstractSource( VRMLInterpolator )
  // Make sure we only initialize once.
  assert(SoVRMLInterpolator::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoVRMLInterpolator::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "VRMLInterpolator",
                       NULL,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeAbstractSource
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoVRMLInterpolator::cleanClass(void)
{
}

/*!
  FIXME: write function documentation
*/
SbBool
SoVRMLInterpolator::getOutputName(const SoVRMLInterpOutput * /* output */,
				  SbName & /* outputName */) const
{
  assert(0 && "FIXME: not implemented yet");
  return FALSE;
}
