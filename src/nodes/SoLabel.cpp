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
  \class SoLabel SoLabel.h Inventor/nodes/SoLabel.h
  \brief The SoLabel class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoLabel.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>

/*!
  \var SoSFName SoLabel::label
  FIXME: write documentation for field
*/

// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoLabel)
SoType SoLabel::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoLabel node class.
*/
void *
SoLabel::createInstance(void)
{
  return new SoLabel;
}

/*!
  Returns the unique type identifier for the SoLabel class.
*/
SoType
SoLabel::getClassTypeId(void)
{
  return SoLabel::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoLabel::getTypeId(void) const
{
  return SoLabel::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoLabel::SoLabel()
{
//$ BEGIN TEMPLATE NodeConstructor(SoLabel)
  // Make sure the class has been initialized.
  assert(SoLabel::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor
  
  SO_NODE_ADD_FIELD(label, ("<Undefined label>"));
}

/*!
  Destructor.
*/
SoLabel::~SoLabel()
{
}

/*!
  Does initialization common for all objects of the
  SoLabel class. This includes setting up the
  type system, among other things.
*/
void
SoLabel::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource(Label)
  // Make sure we only initialize once.
  assert(SoLabel::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoLabel::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "Label",
                       &SoLabel::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoLabel::cleanClass(void)
{
}
