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
  \class SoPickStyle SoPickStyle.h Inventor/nodes/SoPickStyle.h
  \brief The SoPickStyle class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoPickStyle.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>

/*!
  \enum SoPickStyle::Style
  FIXME: write documentation for enum
*/
/*!
  \var SoPickStyle::Style SoPickStyle::SHAPE
  FIXME: write documentation for enum definition
*/
/*!
  \var SoPickStyle::Style SoPickStyle::BOUNDING_BOX
  FIXME: write documentation for enum definition
*/
/*!
  \var SoPickStyle::Style SoPickStyle::UNPICKABLE
  FIXME: write documentation for enum definition
*/


/*!
  \var SoSFEnum SoPickStyle::style
  FIXME: write documentation for field
*/

// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoPickStyle)
SoType SoPickStyle::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoPickStyle node class.
*/
void *
SoPickStyle::createInstance(void)
{
  return new SoPickStyle;
}

/*!
  Returns the unique type identifier for the SoPickStyle class.
*/
SoType
SoPickStyle::getClassTypeId(void)
{
  return SoPickStyle::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoPickStyle::getTypeId(void) const
{
  return SoPickStyle::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoPickStyle::SoPickStyle()
{
//$ BEGIN TEMPLATE NodeConstructor(SoPickStyle)
  // Make sure the class has been initialized.
  assert(SoPickStyle::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor
  
  SO_NODE_ADD_FIELD(style, (SoPickStyle::SHAPE));

  SO_NODE_DEFINE_ENUM_VALUE(Style, SHAPE);
  SO_NODE_DEFINE_ENUM_VALUE(Style, BOUNDING_BOX);
  SO_NODE_DEFINE_ENUM_VALUE(Style, UNPICKABLE);
  SO_NODE_SET_SF_ENUM_TYPE(style, Style);
}

/*!
  Destructor.
*/
SoPickStyle::~SoPickStyle()
{
}

/*!
  Does initialization common for all objects of the
  SoPickStyle class. This includes setting up the
  type system, among other things.
*/
void
SoPickStyle::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource(PickStyle)
  // Make sure we only initialize once.
  assert(SoPickStyle::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoPickStyle::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "PickStyle",
                       &SoPickStyle::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoPickStyle::cleanClass(void)
{
}


#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
void
SoPickStyle::doAction(SoAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
 */
void
SoPickStyle::callback(SoCallbackAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
/*!
  FIXME: write doc
 */
void
SoPickStyle::pick(SoPickAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOPICKACTION
