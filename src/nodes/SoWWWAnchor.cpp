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
  \class SoWWWAnchor SoWWWAnchor.h Inventor/nodes/SoWWWAnchor.h
  \brief The SoWWWAnchor class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/SbName.h>
#include <Inventor/nodes/SoWWWAnchor.h>
#include <Inventor/nodes/SoSubNode.h>

/*!
  \enum SoWWWAnchor::Mapping
  FIXME: write documentation for enum
*/
/*!
  \var SoWWWAnchor::Mapping SoWWWAnchor::NONE
  FIXME: write documentation for enum definition
*/
/*!
  \var SoWWWAnchor::Mapping SoWWWAnchor::POINT
  FIXME: write documentation for enum definition
*/


/*!
  \var SoSFString SoWWWAnchor::name
  FIXME: write documentation for field
*/
/*!
  \var SoSFString SoWWWAnchor::description
  FIXME: write documentation for field
*/
/*!
  \var SoSFEnum SoWWWAnchor::map
  FIXME: write documentation for field
*/

// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoWWWAnchor)
SoType SoWWWAnchor::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoWWWAnchor node class.
*/
void *
SoWWWAnchor::createInstance(void)
{
  return new SoWWWAnchor;
}

/*!
  Returns the unique type identifier for the SoWWWAnchor class.
*/
SoType
SoWWWAnchor::getClassTypeId(void)
{
  return SoWWWAnchor::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoWWWAnchor::getTypeId(void) const
{
  return SoWWWAnchor::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoWWWAnchor::SoWWWAnchor()
{
//$ BEGIN TEMPLATE NodeConstructor(SoWWWAnchor)
  // Make sure the class has been initialized.
  assert(SoWWWAnchor::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

  SO_NODE_ADD_FIELD(name, ("<Undefined URL>"));
  SO_NODE_ADD_FIELD(description, (""));
  SO_NODE_ADD_FIELD(map, (NONE));

  SO_NODE_DEFINE_ENUM_VALUE(Map, NONE);
  SO_NODE_DEFINE_ENUM_VALUE(Map, POINT);
  SO_NODE_SET_SF_ENUM_TYPE(map, Map);
}

/*!
  Destructor.
*/
SoWWWAnchor::~SoWWWAnchor()
{
}

/*!
  Does initialization common for all objects of the
  SoWWWAnchor class. This includes setting up the
  type system, among other things.
*/
void
SoWWWAnchor::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource(WWWAnchor)
  // Make sure we only initialize once.
  assert(SoWWWAnchor::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoWWWAnchor::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "WWWAnchor",
                       &SoWWWAnchor::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoWWWAnchor::cleanClass(void)
{
}


/*!
  FIXME: write doc
 */
void
SoWWWAnchor::setFullURLName(const SbString & /* url */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
const SbString &
SoWWWAnchor::getFullURLName(void)
{
  assert(0 && "FIXME: not implemented");
  static SbString s;
  return s;
}

#if !defined(COIN_EXCLUDE_SOHANDLEEVENTACTION)
/*!
  FIXME: write doc
 */
void
SoWWWAnchor::handleEvent(SoHandleEventAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOHANDLEEVENTACTION

/*!
  FIXME: write doc
 */
void
SoWWWAnchor::setFetchURLCallBack(SoWWWAnchorCB * /* f */, void * /* userData */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
void
SoWWWAnchor::setHighlightURLCallBack(SoWWWAnchorCB * /* f */, void * /* userData */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
void
SoWWWAnchor::redrawHighlighted(SoAction * /* act */, SbBool /* isNowHighlighting */)
{
  assert(0 && "FIXME: not implemented");
}
